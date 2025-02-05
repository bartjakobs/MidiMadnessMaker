#include "midimadnessmaker.h"
/**
 * Initializes a new instance of the MidiMadnessMaker class.
 * This class is used to create a MIDI controller.
 */
MidiMadnessMaker::MidiMadnessMaker()
{
	onTouchChange = NULL;
	onTouchEnd = NULL;
	onTouchStart = NULL;

	for(int i = 0; i < NUM_PINS; i++){
		noteOnStates[i] = false;
		debounceTimes[i] = 0;
	}

}

/** 
 * Sets the callback function for when a touch is started.
 * @param callback The function to call when a touch is started.
 */
void MidiMadnessMaker::setOnTouchStart(void (*callback)(int index, int velocity))
{
	onTouchStart = callback;
}

/**
 * Sets the callback function for when a touch is ended.
 */
void MidiMadnessMaker::setOnTouchEnd(void (*callback)(int index))
{
	onTouchEnd = callback;
}

/**
 * Sets the callback function for when a touch is changed.
 */
void MidiMadnessMaker::setOnTouchChange(void (*callback)(int index, int velocity))
{
	onTouchChange = callback;
}

/**
 * Calibrates the touch pads.
 * This function reads the touch pads and sets the thresholds.
 */
void MidiMadnessMaker::calibrate()
{

// #ifdef AUTO_CALIBRATE
	for (int i = 0; i < NUM_PINS; i++)
	{
		thresholds[i] = touchRead(touchPins[i]) / 2;
		delay(50);
	}
	delay(200);
	for (int i = 0; i < NUM_PINS; i++)
	{
		thresholds[i] += touchRead(touchPins[i]) / 2;
		thresholds[i] += 10000;
		delay(50);
	}
// #endif
}


/**
 * This function is called when a pin value changes from pressed to not pressed or vice versa.
 */
void MidiMadnessMaker::pinChanged(int index, bool isPressed, int velocity = 127)
{
	if (isPressed)
	{
		if (onTouchStart != NULL)
		{
			onTouchStart(index, velocity);
		}
	}
	else
	{
		if (onTouchEnd != NULL)
		{
			onTouchEnd(index);
		}
	}
}

/**
 * Reads the value of a pin and checks if it has changed.
 */
bool MidiMadnessMaker::readAndCheckPin(int index){
	if (millis() - debounceTimes[index] < DEBOUNCE_TIME)
	{
		return 0;
	}
	uint32_t touchValue = touchRead(touchPins[index]);

#ifdef PRINT_READINGS
	Serial.print("TouchPin ");
	Serial.print(touchPins[index]);
	Serial.print(": ");
	Serial.println(((int)touchValue) - ((int)thresholds[index]));
#endif

	touchValue = constrain(touchValue, 0, max_values[index]);
	bool newState = touchValue > thresholds[index];
	if (newState != noteOnStates[index])
	{
		debounceTimes[index] = millis();
		noteOnStates[index] = newState;
		
		pinChanged(index, newState, map(touchValue, thresholds[index], max_values[index], 0, 127));
	}
	else
	{
		if (newState)
		{ // if already pressing, maybe send cc
			uint8_t ccValue = map(touchValue, thresholds[index], max_values[index], 0, 127);
			if (onTouchChange != NULL)
			{
				onTouchChange(index, ccValue);
			}
		}
	}

	if (readings[index] == touchValue)
	{ // if the value is the same as last time, return 1
		return 1;
	}
	readings[index] = touchValue;
	return 0;
}

/**
 * Loops through all the pins and checks if they have changed.
 */
void MidiMadnessMaker::loop(){
	int samePins = 0;
	for (int i = 0; i < NUM_PINS; i++)
	{
		samePins += readAndCheckPin(i);
	}
	if (samePins == NUM_PINS)
	{
		// all pins have the same value as last time. That's suspicious. Reset the touch pad
		touch_pad_fsm_start();
	}
}