/**
 * MidiMadnessMaker.h - Library for capacitive touch MIDI controller, on esp32-s3
 * Library by Bart Jakobs, 2025
 * 
 * Define #PRINT_READINGS to print the readings of the touch pins.
 */

#pragma once

#include <Arduino.h>
#include "driver/touch_sensor.h"
#define NUM_PINS 8

/**
 * MidiMadnessMaker
 * This class is used to create a MIDI controller.
 * It uses the touchRead function to read the capacitive touch values of the pins.
 */
class MidiMadnessMaker
{
public:
	
	MidiMadnessMaker();
	void calibrate();

	void loop();


	void setOnTouchStart(void (*callback)(int index, int velocity));
	void setOnTouchEnd(void (*callback)(int index));
	void setOnTouchChange(void (*callback)(int index, int velocity));

private:
	void (*onTouchStart)(int index, int velocity);
	void (*onTouchEnd)(int index);
	void (*onTouchChange)(int index, int velocity);
	uint32_t readings[NUM_PINS];
	uint32_t thresholds[NUM_PINS] = {50000, 70000, 60000, 60000, 70000, 60000, 60000, 60000};
	uint32_t max_values[NUM_PINS] = {150000, 150000, 150000, 150000, 150000, 150000, 150000, 150000};
	bool noteOnStates[NUM_PINS];
	unsigned long debounceTimes[NUM_PINS + 1];
	int touchPins[NUM_PINS] = {T1, T2, T3, T4, T5, T6, T7, T8};
	int midiNotes[NUM_PINS];
	int DEBOUNCE_TIME = 50;
	void pinChanged(int index, bool isPressed, int velocity);
	bool readAndCheckPin(int index);


};
