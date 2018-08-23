#include "SoundEffects.h"

void SoundEffects::playErrorTone(int speakerPin) {
  tone(speakerPin, 130, 140);
  delay(110);
  tone(speakerPin, 70, 270);
  delay(270);
  digitalWrite(speakerPin, HIGH);
}

void SoundEffects::playAlarmSignal(int speakerPin) {
  tone(speakerPin, 580, 60);
}

void SoundEffects::playHappyBirthday(int speakerPin) {
  Tone melody[] = {
    Tone( NOTE_B5, 4),
    Tone(NOTE_B5, 4),
    Tone(NOTE_CS6, 2),
    Tone(NOTE_B5, 2),
    Tone(NOTE_E6, 2),
    Tone(NOTE_DS6, 1),

    Tone(0, 16),

    Tone(NOTE_B5, 4),
    Tone(NOTE_B5, 4),
    Tone(NOTE_CS6, 2),
    Tone(NOTE_B5, 2),
    Tone(NOTE_FS6, 2),
    Tone(NOTE_E6, 1),

    Tone(0, 16),

    Tone(NOTE_B5, 4),
    Tone(NOTE_B5, 4),
    Tone(NOTE_B6, 2),
    Tone(NOTE_GS6, 2),
    Tone(NOTE_E6, 2),
    Tone(NOTE_DS6, 2),
    Tone(NOTE_CS6, 1),

    Tone(0, 16),

    Tone(NOTE_A6, 4),
    Tone(NOTE_A6, 4),
    Tone(NOTE_GS6, 2),
    Tone(NOTE_E6, 2),
    Tone(NOTE_FS6, 2),
    Tone(NOTE_E6, 1)

  };
  playMelody(speakerPin, melody, 1.2f, sizeof(melody) / sizeof(melody[0]));
}

void SoundEffects::playMelody(int speakerPin, Tone *melody, float speed, int noteCount) {
  for (int i = 0; i < noteCount; i++) {
    Tone *note = melody + i;
    int noteDuration = (1000 / speed) / note->duration;
    tone(speakerPin, note->tone, noteDuration);
    int pause = noteDuration * 1.30;
    delay(pause);
    noTone(0);
  }
}
