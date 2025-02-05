#include <Arduino.h>
#include "midimadnessmaker.h"
#include <BLEMIDI_Transport.h>		//BLE MIDI Transport library
#include <hardware/BLEMIDI_ESP32.h> //BLE ESP32 library

#define DEBOUNCE_TIME 20
BLEMIDI_CREATE_INSTANCE("MidiMadnessMaker", MIDI); // make instance of BLEMIDI. Give this a recognizable name! Like your name? Alter ego?

MidiMadnessMaker midiMadnessMaker;
const uint8_t midiNotes[NUM_PINS] = {60, 61, 62, 63, 64, 65, 66, 67};

unsigned long ccDebounce = 0;
void sendAndDebounceControlChange(uint8_t cc, uint8_t value, uint8_t channel)
{
	if (millis() - ccDebounce < DEBOUNCE_TIME)
	{
		return;
	}
	ccDebounce = millis();
	MIDI.sendControlChange(cc, value, channel);
}

void setup()
{
	// Initialize serial for debugging
	Serial.begin(115200);
	MIDI.begin(MIDI_CHANNEL_OMNI);

	midiMadnessMaker = MidiMadnessMaker();
	midiMadnessMaker.calibrate();

	midiMadnessMaker.setOnTouchStart([](int index, int velocity)
									 { MIDI.sendNoteOn(midiNotes[index], velocity, 1); });

	midiMadnessMaker.setOnTouchEnd([](int index)
								   { MIDI.sendNoteOff(midiNotes[index], 0, 1); });

	midiMadnessMaker.setOnTouchChange([](int index, int velocity)
									  { sendAndDebounceControlChange(1, velocity, 1); });
}

void loop()
{
	midiMadnessMaker.loop();
}
