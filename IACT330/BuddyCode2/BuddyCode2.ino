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
  myDFPlayer.volume(3 0);  //Set volume value.

  //servo setup
  myservo.attach(9);
  myservo.write(0); //reset position
}

void loop() {
  // put your main code here, to run repeatedly:
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
  runBreathExer();

}//end loop ------------------------------------------------------------------------------------

void runBreathExer(){
  standbyCol();
  myDFPlayer.play(13); //sound effect
  delay(5000);
  myDFPlayer.play(1); //alright...
  delay(15000); //15sec
  wiggle();
  delay(10);
  myDFPlayer.play(2); //during the...
  delay(11000); //11sec
  dim();
  delay(10);
  myDFPlayer.play(3); //now notice...
  delay(14000); //14sec
  myDFPlayer.play(4); //once you're...
  delay(11000); //11sec
  brighten();
  myDFPlayer.play(5); //hold it ***
  delay(7000); //7sec
  dim();
  delay(300);
  brighten();
  myDFPlayer.play(6); //breath in...
  delay(3000); //3sec
  myDFPlayer.play(5); //hold it ***
  delay(7000); //7sec
  dim();
  delay(2000);
  brighten();
  checkMov();
  myDFPlayer.play(11); //as you take these...
  delay(6000); //6sec
  dim();
  delay(2000);
  brighten();
  myDFPlayer.play(12); //as you take these pt 2...
  delay(4000); //4sec
  dim();
  delay(2000);
  myDFPlayer.play(8); //one more time...
  delay(4000); //4sec
  brighten();
  delay(1000);
  myDFPlayer.play(5); //hold it ***
  delay(7000); //7sec
  dim();
  delay(100);
  brighten();
  celebrate();
  delay(100);
  checkMovEnd();
  //myDFPlayer.play(9); //Very nice... other is 10
  //delay(15000);//15sec
  
}

//motion sensor functions
void checkMovEnd(){
  val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    myDFPlayer.play(10); //nice... other is 9
    delay(15000);//15sec
  } 
  else {
    myDFPlayer.play(9); //Very nice... other is 10
    delay(15000);//15sec
  }
}

void checkMov(){
  val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    //digitalWrite(led, HIGH);   // turn LED ON
    wiggle();
    delay(100);                // delay 100 milliseconds 
  } 
  else {
  }
}

//servo functions
void wiggle(){
  for (pos = 0; pos <= 30; pos += 1) { // goes from 0 degrees to 10 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 45ms for the servo to reach the position
  }
  for (pos = 30; pos >= 0; pos -= 1) { // goes from 10 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 45ms for the servo to reach the position
  }

}

//led functions
void standbyCol(){
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Blue;
    FastLED.show();
  }
}
void dim(){
  for (int i = 100; i > 10; i--){
    bright = i;
    FastLED.setBrightness( bright );
    FastLED.show();
    delay(15);
  }
}
void brighten(){
  for (int i = 10; i < 100; i++){
    bright = i;
    FastLED.setBrightness( bright );
    FastLED.show();
    delay(15);
  }
}
void celebrate(){
  leds[0] = CRGB::Purple;
  FastLED.show();
  delay(15);
  leds[1] = CRGB::Blue;
  FastLED.show();
  delay(15);
  leds[2] = CRGB::Green;
  FastLED.show();
  delay(15);
}

//mp3 Player function
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
