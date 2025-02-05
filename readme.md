# Stukkie code voor de Midi Madness Maker

## Code:

```cpp
void setup()
{
	// Initialize serial for debugging
	Serial.begin(115200);
	MIDI.begin(MIDI_CHANNEL_OMNI);

	midiMadnessMaker = MidiMadnessMaker();
	midiMadnessMaker.calibrate();

	midiMadnessMaker.setOnTouchStart([](int index, int velocity)
									 { 
										Serial.print("Touch start on index: ");
										Serial.println(index);

									 });

	midiMadnessMaker.setOnTouchEnd([](int index)
								   { 
									
									Serial.print("Touch end on index: ");
									Serial.println(index);
								   
								   });

	midiMadnessMaker.setOnTouchChange([](int index, int velocity)
									  { 
										
										Serial.print("Touch change on index: ");
										Serial.println(index);

									});
}

void loop()
{
	midiMadnessMaker.loop();
}
```