#include <M5Stack.h>
#include <driver/dac.h>
#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

#include "Tunes.h"
Tunes tunes;

void setup() {
  M5.begin();
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(50, 0);
  M5.Lcd.print("8bit PSG SOUND");
  M5.Lcd.setCursor(160, 30);
  M5.Lcd.print("Synth");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(180, 60);
  M5.Lcd.print("by necobit");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(160, 70);
  M5.Lcd.print("Special Thanks");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(180, 80);
  M5.Lcd.print("@ina_ani");
  M5.Lcd.setCursor(180, 90);
  M5.Lcd.print("Lixie Labs");

  dacWrite(25, 0);
//  ledcDetachPin(SPEAKER_PIN);
//  pinMode(SPEAKER_PIN, INPUT);
  tunes.init();
  M5.Speaker.setVolume(1);

}

int p_output;

void loop() {
  tunes.resume();
  tunes.run();

  int ch, data1, data2;
  if (MIDI.read())
  {
    if (MIDI.getType() == midi::NoteOn)
    {
      ch = MIDI.getChannel();
      data1 = MIDI.getData1();
      data2 = MIDI.getData2();
      portENTER_CRITICAL(&Tunes::timerMux);
      tunes.noteon(ch, data1, data2);
      portEXIT_CRITICAL(&Tunes::timerMux);
    }
  }
  else if (MIDI.getType() == midi::NoteOff)
  {
    ch = MIDI.getChannel();
    data1 = MIDI.getData1();
    data2 = MIDI.getData2();
    portENTER_CRITICAL(&Tunes::timerMux);
    tunes.noteoff(ch, data1, data2);
    portEXIT_CRITICAL(&Tunes::timerMux);
  }
  M5.update();
}