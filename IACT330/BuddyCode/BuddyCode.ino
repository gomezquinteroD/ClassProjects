//control variables
boolean startBuddy = false;
char cmnd;
int startTime;
int currTime;
int timer;

//motion sensor variables
int led = 13;                // the pin that the LED is atteched to
int sensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)

//leds variables
#include "FastLED.h"
#define FASTLED_INTERNAL
#define NUM_LEDS 3
#define DATA_PIN 6
#define BRIGHTNESS 100
int bright = 100;
CRGB leds[NUM_LEDS];

//mp3 Player variables
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

//servo variables
#include <Servo.h>
Servo myservo;
int pos = 0;

void setup() {
  // put your setup code here, to run once:
  //motion sensor setup
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input

  //leds setup
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness( bright );

  //mp3 Player setup
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(10);  //Set volume value.
  //myDFPlayer.play(1);  //Play the first mp3

  //servo setup
  myservo.attach(9);
  myservo.write(90); //reset position

  //control setup
  startBuddy = false;
  startTime = int(millis()/1000);
  currTime = startTime;

  //Serial.println("Testing print interface");

} //end setup

void loop() {
  // put your main code here, to run repeatedly:
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
  //startingBuddy();
  
  //if (!startBuddy){
    //handleSerial();
    for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB::Blue;
      FastLED.show();
    }
    delay(5000);
    startingBuddy();
  //}
  /*else {
    Serial.end();
    mySoftwareSerial.begin(9600);
    currTime = millis()/1000;
    timer = currTime - startTime;
    Serial.print("time: ");
    Serial.println(timer);
    startingBuddy();
  }*/
  
  
} //end loop

void handleSerial(){
  if(Serial.available() > 0){
    char incomingCharacter = Serial.read();
    switch(incomingCharacter){
      case 's':
        startBuddy = true;
        Serial.println("starting Buddy...."); 
        break;

      case 'b':
        Serial.println("I now have a B");
        break;
    }
  }
}

void startingBuddy(){
  //Serial.println("Starting!");
  myDFPlayer.play(1);  //Play the first mp3
  delay(16000);
  actions();
}
void resetTimer(){
  timer = 0;
}

void actions(){
  timer = currTime - startTime;
  if (timer > 16 && timer < 20){
     wiggle();
  }
  timer = currTime - startTime;
  if (timer > 52 && timer < 63){
    breath();
  }
  
}//end actions

//servo functions
void wiggle(){
  for (pos = 0; pos <= 10; pos += 1) { // goes from 0 degrees to 10 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(45);                       // waits 45ms for the servo to reach the position
  }
  for (pos = 10; pos >= 0; pos -= 1) { // goes from 10 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(45);                       // waits 45ms for the servo to reach the position
  }

}

//led function
void standbyCol(){
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Blue;
    FastLED.show();
  }
}
void breath(){
  for (int i = 100; i > 10; i--){
    bright = i;
    FastLED.setBrightness( bright );
    FastLED.show();
    delay(15);
  }
  for (int i = 10; i < 100; i++){
    bright = i;
    FastLED.setBrightness( bright );
    FastLED.show();
    delay(15);
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}
