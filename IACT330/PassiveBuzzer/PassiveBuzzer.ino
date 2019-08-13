#include "pitches.h"
int melody[] = { NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int duration = 500;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  /*for(int note = 0; note < 8; note++){
    tone(8, melody[note], duration);
    delay(1000);
  }
  delay(1000);*/
  tone(8, melody[random(0,8)], 500);
  delay(500);
}
