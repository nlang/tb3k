#ifndef SOUND_EFFECTS_H
#define SOUND_EFFECTS_H

#include "Arduino.h"
#include "Tone.h"
#include "TonePitches.h"

class SoundEffects {

public:
  static void playErrorTone(int speakerPin);
  static void playAlarmSignal(int speakerPin);
  static void playHappyBirthday(int speakerPin);
  static void playMelody(int speakerPin, Tone * melody, float speed, int noteCount);
};

#endif
