#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup() {
  // put your setup code here, to run once:
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  myDFPlayer.start();
  myDFPlayer.volume(29);  //Set volume value. From 0 to 30
  myDFPlayer.play(2);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //myDFPlayer.play(1);  //Play the first mp3
}
