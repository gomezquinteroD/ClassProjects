#include <Adafruit_NeoPixel.h>
#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"
#include <SPI.h>
#include "Adafruit_GFX.h" //graphics library
#include "Adafruit_HX8357.h" //screens 
#include <Adafruit_SPIFlash.h>    // SPI / QSPI flash library
#include <Adafruit_ImageReader.h> // Image-reading functions
#include <Servo.h>
///////////////////////////////
///////// VARIABLES ///////////
///////////////////////////////

int gemini = 22;        //port assignments for the audio port inputs.
int cancer = 23;
int leo = 24;
int virgo = 25;
int libra = 26;
int scorpio = 27;
int sagittarius = 28;
int capricorn = 29;
int aquarius = 30;
int pisces = 31;
int aries = 32;
int taurus = 33;
int ServPin = 47;

int startGemini = 0;            //start is the first light in the lightstrip that is part of the constellation.
int starlengthGemini = 14;      //starlength is the amount of lights in that constellation.
int startCancer = 14;
int starlengthCancer = 5;
int startLeo = 19;
int starlengthLeo = 9;
int startVirgo = 28;
int starlengthVirgo = 10;
int startLibra = 38;
int starlengthLibra = 5;
int startScorpio = 43;
int starlengthScorpio = 9;
int startSagittarius = 52;
int starlengthSagittarius = 9;
int startCapricorn = 61;
int starlengthCapricorn = 8;
int startAquarius = 69;
int starlengthAquarius = 9;
int startPisces = 78;
int starlengthPisces = 9;
int startAries = 87;
int starlengthAries = 6;
int startTaurus = 93;
int starlengthTaurus = 7;

int ledcount = 100;       // the amount of LEDs on the lightstrip.
int stripport = 2;        // the port that the data wire of the lightstrip is plugged into. 
int ledr = 8;             
int ledg = 12;            //ledr, g, and b are values used to control the additional lights that are not part of the lightstrip.
int ledb = 13;
int switchPort = 3;       //the port that the switch used to change modes is connected to.
int pluggedlightcolorR = 0; //these variables control the color of a constellation when they are plugged in. 
int pluggedlightcolorG = 0;
int pluggedlightcolorB = 255;
int otherlightcolorR = 0;  //these variables control the color of all other lights when a constellation is plugged in. 
int otherlightcolorG = 0;
int otherlightcolorB = 0;
int inputDelay = 5000;    //this variable is the delay after you input a constellation. This is set to be longer in astrology mode
                           //The longer delay is useful in co-op to allow more time for a second user to plug into another port.
boolean comboPlayed;      //This boolean is used to prevent duplicate audio after a co-op audio file is played.
boolean astrologyMode;    //These determine which mode is currently active. 
boolean mythologyMode;
boolean attachState;
boolean screensRefreshedMyth;   //these booleans refresh the screen display after the mode is changed, and prevents double text appearing.
boolean screensRefreshedAstro;

Adafruit_NeoPixel lightstrip(ledcount, stripport, NEO_GRB + NEO_KHZ800); //NeoPixel Light Strip object.

//variables for the MP3 player
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
///////////////////

//variables for screen 1
#define TFT_CS  7
#define TFT_DC  6
#define TFT_RST 8
#define SD_CS   5
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

//variables for screen 2
#define TFT_CS2   38
#define TFT_DC2   40
#define TFT_RST2  7
Adafruit_HX8357 tft2 = Adafruit_HX8357(TFT_CS2, TFT_DC2, TFT_RST2);
//////////////////////////////////////////////
//////////////////////////////////////////

//variables for setting up SD card
SdFat                SD;         // SD card filesystem
Adafruit_ImageReader reader(SD); // Image-reader object, pass in SD filesys
ImageReturnCode stat; // Status from image-reading functions

//////////////////////////////////////////////////
//servo variables//
Servo myservo;  // create servo object to control a servo
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to start up again (milliseconds)
int servoState = 0;
//end of servo stuff/////////////////////////////

//////////////////////////////////////
///////// VOID SETUP ////////////////
//////////////////////////////////////

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  initializePins();       


  lightstrip.begin();     //begin initializes the lightstrip and ensures that all lights are turned off.
  lightstrip.show();      //show forces  the brightness of all lights on the lightstrip.
  lightstrip.setBrightness(250);

  //error check initialization of MP3 player
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3. Begins the MP3 player.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  //once MP3 player is online
  myDFPlayer.volume(25);  //Set volume value. From 0 to 30

  //start the screens
  tft.begin();
  tft2.begin();
  //set rotation of the screens.
  tft.setRotation(4);
  tft2.setRotation(2);

  //  //start the SD cards
  //  if (!SD.begin(SD_CS, SD_SCK_MHZ(25))) { // ESP32 requires 25 MHz limit
  //    Serial.println(F("SD begin() failed"));
  //    //for (;;); // Fatal error, do not continue
  //  }
  /* if(!SD.begin(SD_CS2, SD_SCK_MHZ(25))) { // ESP32 requires 25 MHz limit
     Serial.println(F("SD begin() failed"));
     for(;;); // Fatal error, do not continue
    }*/
  Serial.println("Both SD cards are live!");
  myservo.attach(ServPin);
  //Sets the screens to show instructions for mythology mode on startup. Will change to astrology if it needs to.
  idleInstructionsMyth();
}

//////////////////////////////////////////////////////////////
//////////////////END OF VOID SETUP///////////////////////////
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
//////////////// VOID LOOP /////////////////////////////////
////////////////////////////////////////////////////////////

void loop() {

  checkMode();          //This function checks the position of the switch, and updates two booleans, mythologyMode and astrologyMode, to either true or false.
                        // These variables are referenced in many different parts of the program and affect behavior depending on which mode is active.
                        
  checkPlugsMythology(); //This function checks the states of all audio ports, and handles events when they are plugged in accordingly. This is an incredibly dense function.
                          //When an audio jack is plugged in, it will pause the idle animation, and proceed to play the corresponding audio file.
                          //The program will delay until the audio file is played in its entirety to prevent overlap or stuttering.
                          //In astrology mode, there is a 5 second delay after a plug is inserted into the table.
                          //If another plug is inserted during this delay, a co-op file will be triggered.
                          
  idleAnimation();        //checks the state of all ports, and if none are plugged in, then initiates an idle animation.
                          //this idle animation can break if a port is inserted.
                   
  comboPlayed = false;    //This resets the combo boolean, allowing another co-op audio file to play after a combo has played.
  rotateServo();           //This controls the servo motor at the end of the loop to allow automatic rotation.
}

/////////////////////////////////////////////////////////////////
///////////////END OF VOID LOOP//////////////////////////
/////////////////////////////////////////////



//This function constantly checks the state of the switch, and adjusts the mode of the program depending on the switch position.
//This also has certain information regarding modes, such as the text displayed on the screens, the volume levels of each modes,
//And other information like the input delay.

void checkMode() {                              
  if (digitalRead(switchPort) == HIGH) {
    if (!screensRefreshedMyth) {
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    //myDFPlayer.volume(26);  //Set volume value. From 0 to 30
    screensRefreshedMyth = true;
    screensRefreshedAstro = false;
    mythologyMode = true;
    astrologyMode = false;
    inputDelay = 0;
    Serial.print("Switch is on, Kupo!");
  }
  if (digitalRead(switchPort) == LOW) {
    if (!screensRefreshedAstro) {
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    //myDFPlayer.volume(30);  //Set volume value. From 0 to 30
    screensRefreshedMyth = false;
    screensRefreshedAstro = true;
    mythologyMode = false;
    astrologyMode = true;
    inputDelay = 5000;
  }
}
/////////////////////////////////////////////
/////////////////////////////////////////////

//flash is used to control any section of the lightstrip. It is just a for loop that turns on a specified number of lights, in a specific place, in a specific color. 
//this also forces the lightstrip to update, to make sure that the lights are visible.

void flash(int start, int starlength, int r, int g, int b) {    //Uses a loop to turn on a specified range of lights in a specified color.
  //start is the location of the first star in the constellation, and starlength is the amount of stars in the constellation.
  for (int i = start; i < start + starlength; i++) {
    lightstrip.setPixelColor(i, r, g, b);
  }
  lightstrip.show();
  setLeds(0, 0, 0);

}
//////////////////////////////////////////
//////////////////////////////////////////

//off turns off all pixes in a specific area at a specific distance.

void off(int start, int starlength) { //turns off all pixels.
  for (int i = start; i < (start + starlength); i++) {
    lightstrip.setPixelColor(i, 0, 0, 0);
  }
}
/////////////////////////////////////////
/////////////////////////////////////////

//initializePins helps clean up the setup, and makes all pins initialize correctly.

void initializePins() {               //sets all pins to input pullup, which enables pin resistors, reversing the typical "HIGH / LOW" reading, allowing for more stable inputs.
  pinMode(gemini, INPUT_PULLUP);
  pinMode(cancer, INPUT_PULLUP);
  pinMode(leo, INPUT_PULLUP);
  pinMode(virgo, INPUT_PULLUP);
  pinMode(libra, INPUT_PULLUP);
  pinMode(scorpio, INPUT_PULLUP);
  pinMode(sagittarius, INPUT_PULLUP);
  pinMode(capricorn, INPUT_PULLUP);
  pinMode(aquarius, INPUT_PULLUP);
  pinMode(pisces, INPUT_PULLUP);
  pinMode(aries, INPUT_PULLUP);
  pinMode(taurus, INPUT_PULLUP);
  pinMode(switchPort, INPUT_PULLUP);
}

////////////////////////////////////
/////////////////////////////////////

//This monster of a function checks if the plugs are plugged in, and then plays the appropriate audio file.
//Every port has its own section in the function. This is because every port plays a different audio file, and controls the lights differently.
//there is a series of "combo" functions that point to the correct co-op audio file when two ports are plugged in.

void checkPlugsMythology() {                     //Check if the plugs are plugged in.
  //////////////////////////////////
  if (digitalRead(gemini) == LOW) {
    delay(inputDelay);
    off(0, 100);                                               //clears the idle animation.
    delay(5);
    flash(0, 100, otherlightcolorR, otherlightcolorG, otherlightcolorB);                           //sets the color of the rest of the lights.
    flash(startGemini, starlengthGemini, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);        //sets the color of the constellation that is plugged in.
    cancerCombo(29, 15000);
    taurusCombo(65, 14000);
    ariesCombo(7, 16000);
    piscesCombo(33, 13000);
    aquariusCombo(28, 11000);
    capricornCombo(30, 17000);
    sagittariusCombo(34, 12000);
    scorpioCombo(35, 15000);
    libraCombo(32, 13000);
    virgoCombo(36, 16000);
    leoCombo(31, 13000);
    
    if (mythologyMode) {           //if the table is in mythology mode, it will play the file for the mythological story of Gemini.
      geminiMyth();
      myDFPlayer.play(83);           //plays the appropriate MP3 file, stored in the SD card at a certain index.
      delay(28000);                  //the length of the audio file. If set improperly, audio file will be stuck on repeat.
      tft.fillScreen(HX8357_BLACK);   //resets the screen backgrounds after the audio file plays. 
      tft2.fillScreen(HX8357_BLUE);
    }

    if (astrologyMode && !comboPlayed) {
      myDFPlayer.play(27);
      delay(14000);
    }

    stat = reader.drawBMP("/SignGemini.bmp", tft, 35, 100);
    stat = reader.drawBMP("/SignGemini.bmp", tft2, 35, 100);
    Serial.print("gemini is plugged in, kupo!");
    delay(3000);
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (digitalRead(cancer) == LOW) {
    off(0, 100);                                               //clears the idle animation.
    delay(inputDelay);
    flash(0, 100, otherlightcolorR, otherlightcolorG, otherlightcolorB);                           //sets the color of the rest of the lights.
    flash(startCancer, starlengthCancer, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);        //sets the color of the constellation that is plugged in.

    geminiCombo(29, 15000);
    taurusCombo(63, 15000);
    ariesCombo(5, 17000);
    piscesCombo(20, 18000);
    aquariusCombo(16, 15000);
    capricornCombo(17, 16000);
    sagittariusCombo(21, 16000);
    scorpioCombo(22, 13000);
    libraCombo(19, 17000);
    virgoCombo(23, 17000);
    leoCombo(18, 16000);


    if (mythologyMode) {
      cancerMyth();
      myDFPlayer.play(81);                                    //plays the appropriate MP3 file.
      delay(21000);                                           //the length of the audio file. If set improperly, audio file will be stuck on repeat.
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    if (astrologyMode && !comboPlayed) {
      myDFPlayer.play(15);
      delay(14000);
    }
    stat = reader.drawBMP("/SignGemini.bmp", tft, 35, 100);
    stat = reader.drawBMP("/SignGemini.bmp", tft2, 35, 100);
    Serial.print("cancer is plugged in, kupo!");
    delay(3000);
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (digitalRead(leo) == LOW) {
    off(0, 100);                                               //clears the idle animation.
    delay(inputDelay);
    flash(0, 100, otherlightcolorR, otherlightcolorG, otherlightcolorB);                           //sets the color of the rest of the lights.
    flash(startLeo, starlengthLeo, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);        //sets the color of the constellation that is plugged in.

    geminiCombo(31, 13000);
    cancerCombo(18, 16000);
    taurusCombo(66, 17000);
    ariesCombo(8, 15000);
    piscesCombo(41, 18000);
    aquariusCombo(38, 12000);
    capricornCombo(39, 13000);
    sagittariusCombo(42, 20000);
    scorpioCombo(43, 16000);
    libraCombo(40, 14000);
    virgoCombo(44, 18000);

    if (mythologyMode) {
      leoMyth();
      myDFPlayer.play(84);                                    //plays the appropriate MP3 file.
      delay(28000);                                           //the length of the audio file. If set improperly, audio file will be stuck on repeat.
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    if (astrologyMode && !comboPlayed) {
      myDFPlayer.play(37);
      delay(13000);
    }
    stat = reader.drawBMP("/SignGemini.bmp", tft, 35, 100);
    stat = reader.drawBMP("/SignGemini.bmp", tft2, 35, 100);
    Serial.print("leo is plugged in, kupo!");
  }
////////////////////////////////////////////////////////////////////////////////////////////////
  if (digitalRead(virgo) == LOW) {
    off(0, 100);                                               //clears the idle animation.
    delay(inputDelay);
    flash(0, 100, otherlightcolorR, otherlightcolorG, otherlightcolorB);                           //sets the color of the rest of the lights.
    flash(startVirgo, starlengthVirgo, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);        //sets the color of the constellation that is plugged in.

    ariesCombo(14, 15000);
    aquariusCombo(73, 19000);
    cancerCombo(23, 17000);
    capricornCombo(74, 20000);
    geminiCombo(36, 16000);
    leoCombo(44, 18000);
    piscesCombo(76, 17000);
    sagittariusCombo(77, 21000);
    scorpioCombo(78, 17000);
    taurusCombo(71, 17000);

    if (mythologyMode) {
      virgoMyth();
      myDFPlayer.play(90);                                    //plays the appropriate MP3 file.
      delay(24000);                                           //the length of the audio file. If set improperly, audio file will be stuck on repeat.
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    if (astrologyMode && !comboPlayed) {
      myDFPlayer.play(72);
      delay(14000);
    }
    stat = reader.drawBMP("/SignGemini.bmp", tft, 35, 100);
    stat = reader.drawBMP("/SignGemini.bmp", tft2, 35, 100);
    Serial.print("virgo is plugged in, kupo!");
  }
////////////////////////////////////////////////////////////////////////////////////////////////
  if (digitalRead(libra) == LOW) {
    off(0, 100);                                               //clears the idle animation.
    delay(inputDelay);
    flash(0, 100, otherlightcolorR, otherlightcolorG, otherlightcolorB);                           //sets the color of the rest of the lights.
    flash(startLibra, starlengthLibra, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);        //sets the color of the constellation that is plugged in.

    ariesCombo(8, 15000);
    aquariusCombo(46, 16000);
    cancerCombo(19, 17000);
    capricornCombo(47, 16000);
    geminiCombo(32, 13000);
    leoCombo(36, 16000);
    piscesCombo(48, 15000);
    sagittariusCombo(49, 18000);
    scorpioCombo(50, 16000);
    taurusCombo(66, 17000);
    virgoCombo(30, 15000);

    if (mythologyMode) {
      libraMyth();
      myDFPlayer.play(85);                                    //plays the appropriate MP3 file.
      delay(23000);                                           //the length of the audio file. If set improperly, audio file will be stuck on repeat.
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    if (astrologyMode && !comboPlayed) {
      myDFPlayer.play(45);
      delay(16000);
    }
    stat = reader.drawBMP("/SignGemini.bmp", tft, 35, 100);
    stat = reader.drawBMP("/SignGemini.bmp", tft2, 35, 100);
    Serial.print("libra is plugged in, kupo!");
  }
////////////////////////////////////////////////////////////////////////////////////////////////
  if (digitalRead(scorpio) == LOW) {
    off(0, 100);                                               //clears the idle animation.
    delay(inputDelay);
    flash(0, 100, otherlightcolorR, otherlightcolorG, otherlightcolorB);                           //sets the color of the rest of the lights.
    flash(startScorpio, starlengthScorpio, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);        //sets the color of the constellation that is plugged in.

    ariesCombo(12, 16000);
    aquariusCombo(57, 16000);
    cancerCombo(22, 13000);
    capricornCombo(58, 16000);
    geminiCombo(35, 15000);
    leoCombo(43, 16000);
    libraCombo(50, 16000);
    piscesCombo(59, 16000);
    sagittariusCombo(60, 15000);
    taurusCombo(70, 15000);
    virgoCombo(78, 17000);


    if (mythologyMode) {
      scorpioMyth();
      myDFPlayer.play(88);                                    //plays the appropriate MP3 file.
      delay(24000);                                           //the length of the audio file. If set improperly, audio file will be stuck on repeat.
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    if (astrologyMode && !comboPlayed) {
      myDFPlayer.play(56);
      delay(14000);
    }
    stat = reader.drawBMP("/SignGemini.bmp", tft, 35, 100);
    stat = reader.drawBMP("/SignGemini.bmp", tft2, 35, 100);
    Serial.print("scorpio is plugged in, kupo!");
  }
////////////////////////////////////////////////////////////////////////////////////////////////
  if (digitalRead(sagittarius) == LOW) {
    off(0, 100);                                               //clears the idle animation.
    delay(inputDelay);
    flash(0, 100, otherlightcolorR, otherlightcolorG, otherlightcolorB);                           //sets the color of the rest of the lights.
    flash(startSagittarius, starlengthSagittarius, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);        //sets the color of the constellation that is plugged in.

    ariesCombo(11, 13000);
    aquariusCombo(57, 16000);
    cancerCombo(21, 16000);
    capricornCombo(54, 10000);
    geminiCombo(34, 12000);
    leoCombo(42, 20000);
    libraCombo(49, 18000);
    piscesCombo(55, 13000);
    scorpioCombo(60, 15000);
    taurusCombo(69, 17000);
    virgoCombo(77, 21000);


    if (mythologyMode) {
      sagiMyth();
      myDFPlayer.play(87);                                    //plays the appropriate MP3 file.
      delay(24000);                                           //the length of the audio file. If set improperly, audio file will be stuck on repeat.
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    if (astrologyMode && !comboPlayed) {
      myDFPlayer.play(52);
      delay(16000);
    }
    stat = reader.drawBMP("/SignGemini.bmp", tft, 35, 100);
    stat = reader.drawBMP("/SignGemini.bmp", tft2, 35, 100);
    Serial.print("sagittarius is plugged in, kupo!");
  }
////////////////////////////////////////////////////////////////////////////////////////////////
  if (digitalRead(capricorn) == LOW) {
    off(0, 100);                                               //clears the idle animation.
    delay(inputDelay);
    flash(0, 100, otherlightcolorR, otherlightcolorG, otherlightcolorB);                           //sets the color of the rest of the lights.
    flash(startCapricorn, starlengthCapricorn, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);        //sets the color of the constellation that is plugged in.

    ariesCombo(6, 14000);
    aquariusCombo(25, 18000);
    cancerCombo(17, 16000);
    geminiCombo(30, 17000);
    leoCombo(39, 13000); //
    libraCombo(47, 16000); //
    piscesCombo(26, 16000); //
    sagittariusCombo(54, 10000); //
    scorpioCombo(58, 16000);
    taurusCombo(64, 17000);
    virgoCombo(74, 20000);


    if (mythologyMode) {
      capriMyth();
      myDFPlayer.play(82);                                    //plays the appropriate MP3 file.
      delay(24000);                                           //the length of the audio file. If set improperly, audio file will be stuck on repeat.
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    if (astrologyMode && !comboPlayed) {
      myDFPlayer.play(24);
      delay(16000);

    }
    stat = reader.drawBMP("/SignGemini.bmp", tft, 35, 100);
    stat = reader.drawBMP("/SignGemini.bmp", tft2, 35, 100);
    Serial.print("capricorn is plugged in, kupo!");
  }
////////////////////////////////////////////////////////////////////////////////////////////////
  if (digitalRead(aquarius) == LOW) {
    off(0, 100);                                               //clears the idle animation.
    delay(inputDelay);
    flash(0, 100, otherlightcolorR, otherlightcolorG, otherlightcolorB);                           //sets the color of the rest of the lights.
    flash(startAquarius, starlengthAquarius, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);        //sets the color of the constellation that is plugged in.

    ariesCombo(4, 12000);
    cancerCombo(16, 15000);
    capricornCombo(25, 18000);
    geminiCombo(28, 11000);
    leoCombo(38, 12000);
    libraCombo(46, 16000);
    piscesCombo(2, 14000);
    sagittariusCombo(53, 16000);
    scorpioCombo(57, 16000);
    taurusCombo(62, 16000);
    virgoCombo(73, 19000);

    if (mythologyMode) {
      aquaMyth();
      myDFPlayer.play(79);                                    //plays the appropriate MP3 file.
      delay(24000);                                           //the length of the audio file. If set improperly, audio file will be stuck on repeat.
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    if (astrologyMode && !comboPlayed) {
      myDFPlayer.play(1);
      delay(14000);
    }
    stat = reader.drawBMP("/SignGemini.bmp", tft, 35, 100);
    stat = reader.drawBMP("/SignGemini.bmp", tft2, 35, 100);
    Serial.print("aquarius is plugged in, kupo!");
  }
////////////////////////////////////////////////////////////////////////////////////////////////
  if (digitalRead(pisces) == LOW) {
    off(0, 100);                                               //clears the idle animation.
    delay(inputDelay);
    flash(0, 100, otherlightcolorR, otherlightcolorG, otherlightcolorB);                           //sets the color of the rest of the lights.
    flash(startPisces, starlengthPisces, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);        //sets the color of the constellation that is plugged in.

    ariesCombo(10, 15000);
    aquariusCombo(2, 14000);
    cancerCombo(20, 18000);
    capricornCombo(26, 16000);
    geminiCombo(33, 13000);
    leoCombo(41, 18000);
    libraCombo(48, 15000);
    sagittariusCombo(55, 13000);
    scorpioCombo(59, 16000);
    taurusCombo(68, 12000);
    virgoCombo(76, 17000);


    if (mythologyMode) {
      piscesMyth();
      myDFPlayer.play(86);                                    //plays the appropriate MP3 file.
      delay(27000);                                           //the length of the audio file. If set improperly, audio file will be stuck on repeat.
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    if (astrologyMode && !comboPlayed) {
      myDFPlayer.play(51);
      delay(13000);
    }
    stat = reader.drawBMP("/SignGemini.bmp", tft, 35, 100);
    stat = reader.drawBMP("/SignGemini.bmp", tft2, 35, 100);
    Serial.print("pisces is plugged in, kupo!");
  }
////////////////////////////////////////////////////////////////////////////////////////////////
  if (digitalRead(aries) == LOW) {
    off(0, 100);                                               //clears the idle animation.
    delay(inputDelay);
    flash(0, 100, otherlightcolorR, otherlightcolorG, otherlightcolorB);                           //sets the color of the rest of the lights.
    flash(startAries, starlengthAries, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);        //sets the color of the constellation that is plugged in.

    aquariusCombo(4, 12000);
    cancerCombo(5, 17000);
    capricornCombo(6, 14000);
    geminiCombo(7, 16000);
    leoCombo(8, 15000);
    libraCombo(9, 19000);
    piscesCombo(10, 15000);
    sagittariusCombo(11, 13000);
    scorpioCombo(12, 16000);
    taurusCombo(13, 10000);
    virgoCombo(14, 15000);

    if (mythologyMode) {
      ariesMyth();
      myDFPlayer.play(80);                                    //plays the appropriate MP3 file.
      delay(21000);                                           //the length of the audio file. If set improperly, audio file will be stuck on repeat.
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    if (astrologyMode && !comboPlayed) {
      myDFPlayer.play(3);
      delay(16000);
    }
    stat = reader.drawBMP("/SignGemini.bmp", tft, 35, 100);
    stat = reader.drawBMP("/SignGemini.bmp", tft2, 35, 100);
    Serial.print("aries is plugged in, kupo!");
  }
////////////////////////////////////////////////////////////////////////////////////////////////
  if (digitalRead(taurus) == LOW) {
    off(0, 100);
    delay(inputDelay);
    flash(0, 100, otherlightcolorR, otherlightcolorG, otherlightcolorB);
    flash(startTaurus, starlengthTaurus, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    delay(10);
    analogWrite(ledr, pluggedlightcolorR);
    analogWrite(ledg, pluggedlightcolorB);
    analogWrite(ledb, pluggedlightcolorG);

    ariesCombo(13, 10000);
    aquariusCombo(62, 16000);
    cancerCombo(63, 15000);
    capricornCombo(64, 17000);
    geminiCombo(65, 14000);
    leoCombo(66, 17000);
    libraCombo(67, 19000);
    piscesCombo(68, 12000);
    sagittariusCombo(69, 17000);
    scorpioCombo(70, 15000);
    virgoCombo(71, 17000);


    if (mythologyMode) {
      taurusMyth();
      myDFPlayer.play(89);                                    //plays the appropriate MP3 file.
      delay(23000);                                           //the length of the audio file. If set improperly, audio file will be stuck on repeat.
      tft.fillScreen(HX8357_BLACK);
      tft2.fillScreen(HX8357_BLUE);
    }
    if (astrologyMode && !comboPlayed) {
      myDFPlayer.play(61);
      delay(15000);
    }
    stat = reader.drawBMP("/SignTaurus.bmp", tft, 35, 100);
    stat = reader.drawBMP("/SignTaurus.bmp", tft2, 35, 100);
    Serial.print("taurus is plugged in, kupo!");

  }
  ////////////////////////////////////////////////////////////////////////////////////////////////

}
/////////////////////////////////////////////
/////////////////////////////////////////////

//rainbow is a function used for the idle animation in astrology mode. It wraps a band of color around the light strip, and animates it to cycle through.
void rainbow(int wait) {
  int red;
  int blue;
  int green;
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < lightstrip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / lightstrip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      lightstrip.setPixelColor(i, lightstrip.gamma32(lightstrip.ColorHSV(pixelHue)));
      //setLeds(map(int(firstPixelHue), 0, 65536,  0, 100), map(int(firstPixelHue), 0, 65536,  0, 255), map(int(firstPixelHue), 0, 65536,  0, 255));
    }
    //    if( firstPixelHue <= 65535){
    //      setLeds(255,0,0);
    //    }
    lightstrip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
    checkMode();
    if (digitalRead(gemini) == LOW || digitalRead(taurus) == LOW || digitalRead(aries) == LOW || digitalRead(pisces) == LOW || digitalRead(aquarius) == LOW || digitalRead(capricorn) == LOW || digitalRead(sagittarius) == LOW || digitalRead(scorpio) == LOW || digitalRead(libra) == LOW || digitalRead(virgo) == LOW || digitalRead(leo) == LOW || digitalRead(cancer) == LOW || mythologyMode) {
      break;
    }
  }
}
///////////////////////////
///////////////////////////
//colorwipe is the function used to animate the idle animation for mythology mode. It makes the LEDS blink through a random color in sequence.

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < lightstrip.numPixels(); i++) { // For each pixel in strip...
    lightstrip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    lightstrip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
    checkMode();
    if (digitalRead(gemini) == LOW || digitalRead(taurus) == LOW || digitalRead(aries) == LOW || digitalRead(pisces) == LOW || digitalRead(aquarius) == LOW || digitalRead(capricorn) == LOW || digitalRead(sagittarius) == LOW || digitalRead(scorpio) == LOW || digitalRead(libra) == LOW || digitalRead(virgo) == LOW || digitalRead(leo) == LOW || digitalRead(cancer) == LOW || astrologyMode) {
      break;
    }
  }
}
/////////////////////////////////
////////////////////////////////
//rotateServo is used to rotate the servo as part of the idle animation.

void rotateServo() {
  //this if checks if anything is plugged in.
  if (digitalRead(gemini) == LOW || digitalRead(taurus) == LOW || digitalRead(aries) == LOW || digitalRead(pisces) == LOW || digitalRead(aquarius) == LOW || digitalRead(capricorn) == LOW || digitalRead(sagittarius) == LOW || digitalRead(scorpio) == LOW || digitalRead(libra) == LOW || digitalRead(virgo) == LOW || digitalRead(leo) == LOW || digitalRead(cancer) == LOW) {
  } else {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      // save the last time servo was on
      previousMillis = currentMillis;

      // if the servo is going left turn it right and vice-versa:
      if (servoState == 0) {
        servoState = 180;
      } else {
        servoState = 0;
      }
      //      if (attachState == true) {
      //        if (servoState  == 0) {
      //          servoState = 180;
      //          myservo.write(servoState);
      //        } else {
      //          servoState = 0;
      //          myservo.write(servoState);
      //        }
      //        attachState = false;
      //        myservo.detach();
      //        Serial.println("detached!");
      //      } else {
      //        attachState = true;
      //        myservo.attach(ServPin);
      //        Serial.println("re-attached!");
      //      }
      myservo.write(servoState);
    }

  }
}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//idleanimation is used to trigger the idle animations when there is no input on the table, and the correct mode is activated.

void idleAnimation() {
  // if (digitalRead(gemini) == HIGH && digitalRead(taurus) == HIGH && digitalRead(aries) == HIGH && digitalRead(pisces) == HIGH && digitalRead(aquarius) == HIGH && digitalRead(capricorn) == HIGH && digitalRead(sagittarius) == HIGH && digitalRead(scorpio) == HIGH && digitalRead(libra) == HIGH && digitalRead(virgo) == HIGH && digitalRead(leo) == HIGH && digitalRead(cancer) == HIGH) {
  if (astrologyMode) {
    idleInstructionsAstrology();
    setLeds(random(5), random(5), random(5));
    idleInstructionsAstrology();
    rainbow(1);
  }
  if (mythologyMode) {
    idleInstructionsMyth();
    colorWipe(lightstrip.Color(random(255), random(255), random(255)), 10);
    setLeds(random(25), random(25), random(25));
  }
}
/////////////////////////////////////////////
////////////////////////////////////////////
//These Combo functions point to a specified co-op audio file. These were used to save space in checkplugs function. 
//It also turns on the co-op constellation lights.

void cancerCombo(int num, int del) {
  if (astrologyMode && digitalRead(cancer) == LOW) {
    comboPlayed = true;
    flash(startCancer, starlengthCancer, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    myDFPlayer.play(num);
    delay(del);
  }
}
///////////////////
void geminiCombo(int num, int del) {
  if (astrologyMode && digitalRead(gemini) == LOW) {
    comboPlayed = true;
    flash(startGemini, starlengthGemini, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    myDFPlayer.play(num);
    delay(del);
  }
}
///////////////////////////////////
void taurusCombo(int num, int del) {
  if (astrologyMode && digitalRead(taurus) == LOW) {
    comboPlayed = true;
    flash(startTaurus, starlengthTaurus, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    analogWrite(ledr, pluggedlightcolorR);
    analogWrite(ledg, pluggedlightcolorB);
    analogWrite(ledb, pluggedlightcolorG);
    myDFPlayer.play(num);
    delay(del);
  }
}
////////////////////////////////
void ariesCombo(int num, int del) {
  if (astrologyMode && digitalRead(aries) == LOW) {
    comboPlayed = true;
    flash(startAries, starlengthAries, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    myDFPlayer.play(num);
    delay(del);
  }
}
////////////////////////////////
void piscesCombo(int num, int del) {
  if (astrologyMode && digitalRead(pisces) == LOW) {
    comboPlayed = true;
    flash(startPisces, starlengthPisces, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    myDFPlayer.play(num);
    delay(del);
  }
}
////////////////////////////////
void aquariusCombo(int num, int del) {
  if (astrologyMode && digitalRead(aquarius) == LOW) {
    comboPlayed = true;
    flash(startAquarius, starlengthAquarius, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    myDFPlayer.play(num);
    delay(del);
  }
}
//////////////////////////////
void capricornCombo(int num, int del) {
  if (astrologyMode && digitalRead(capricorn) == LOW) {
    comboPlayed = true;
    flash(startCapricorn, starlengthCapricorn, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    myDFPlayer.play(num);
    delay(del);
  }
}
//////////////////////////////
void sagittariusCombo(int num, int del) {
  if (astrologyMode && digitalRead(sagittarius) == LOW) {
    comboPlayed = true;
    flash(startSagittarius, starlengthSagittarius, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    myDFPlayer.play(num);
    delay(del);
  }
}
//////////////////////////////
void scorpioCombo(int num, int del) {
  if (astrologyMode && digitalRead(scorpio) == LOW) {
    comboPlayed = true;
    flash(startScorpio, starlengthScorpio, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    myDFPlayer.play(num);
    delay(del);
  }
}
//////////////////////////////
void libraCombo(int num, int del) {
  if (astrologyMode && digitalRead(libra) == LOW) {
    comboPlayed = true;
    flash(startLibra, starlengthLibra, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    myDFPlayer.play(num);
    delay(del);
  }
}
//////////////////////////////
void virgoCombo(int num, int del) {
  if (astrologyMode && digitalRead(virgo) == LOW) {
    comboPlayed = true;
    flash(startVirgo, starlengthVirgo, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    myDFPlayer.play(num);
    delay(del);
  }
}
//////////////////////////////
void leoCombo(int num, int del) {
  if (astrologyMode && digitalRead(leo) == LOW) {
    comboPlayed = true;
    flash(startLeo, starlengthLeo, pluggedlightcolorR, pluggedlightcolorG, pluggedlightcolorB);
    myDFPlayer.play(num);
    delay(del);
  }
}
//////////////////////////////
////////////////////////////////
//setLeds is a secondary way to control the color of the additional LEDs on Taurus.
void setLeds(int r, int g, int b) {
  analogWrite(ledr, r);
  analogWrite(ledg, g);
  analogWrite(ledb, b);
}

//////////////////////////////////////
//////////////////////////////////////
//the following functions are used to control the text that appears on the screens.

void geminiMyth() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(5);
  tft.println("Gemini");
  tft.println("Myth");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Gemini is represented by the two twins, Castor and Pollux. Castor was born a mortal, while Pollux was born immortal. After a tragic twist of events, Castor passed away, leaving Pollux all alone. Zeus, taking pity on the twins, decided to put both of them in the stars together forever, leaving behind the constellation Gemini.");

  tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(5);
  tft2.println("Gemini");
  tft2.println("Myth");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Gemini is represented by the two twins, Castor and Pollux. Castor was born a mortal, while Pollux was born immortal. After a tragic twist of events, Castor passed away, leaving Pollux all alone. Zeus, taking pity on the twins, decided to put both of them in the stars together forever, leaving behind the constellation Gemini.");
}
//////////////////////////////
void cancerMyth() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(5);
  tft.println("Cancer");
  tft.println("Myth");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Cancer is represented by a crab. The story of Cancer is closely tied to that of Hercules. Sent by Hera to test the hero, it provided little struggle for him. Annoyed, Hercules kicked the crab into the stars, where it eventually landed in the heavens itself, becoming the constellation Cancer.");

  tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(5);
  tft2.println("Cancer");
  tft2.println("Myth");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Cancer is represented by a crab. The story of Cancer is closely tied to that of Hercules. Sent by Hera to test the hero, it provided little struggle for him. Annoyed, Hercules kicked the crab into the stars, where it eventually landed in the heavens itself, becoming the constellation Cancer.");
}
//////////////////////////////
void leoMyth() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(5);
  tft.println("Leo");
  tft.println("Myth");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Leo is represented by a ferocious lion! The hero Hercules was tasked with fighting this near-invincible lion, and this was one battle that the hero had some difficulty with. It was a ferocious battle, but in the end, the hero emerged victorious. Impressed with the display of strength, Zeus put the lion in the heavens as the constellation Leo,  as a trophy and reminder of the strength of Hercules.");

  tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(5);
  tft2.println("Leo");
  tft2.println("Myth");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Leo is represented by a ferocious lion! The hero Hercules was tasked with fighting this near-invincible lion, and this was one battle that the hero had some difficulty with. It was a ferocious battle, but in the end, the hero emerged victorious. Impressed with the display of strength, Zeus put the lion in the heavens as the constellation Leo,  as a trophy and reminder of the strength of Hercules.");

}
//////////////////////////////
void virgoMyth() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(5);
  tft.println("Virgo");
  tft.println("Myth");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Virgo represents the god Demeter. Demeter is responsible for ensuring a good harvest every year to guarantee that humans have enough food to eat. After Demeter’s daughter was captured by the nefarious Hades, Demeter fell into despair and only helped the humans harvest their food in the season of Spring, where she looks over the fields at night as the constellation of Virgo.");

  tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(5);
  tft2.println("Virgo");
  tft2.println("Myth");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Virgo represents the god Demeter. Demeter is responsible for ensuring a good harvest every year to guarantee that humans have enough food to eat. After Demeter’s daughter was captured by the nefarious Hades, Demeter fell into despair and only helped the humans harvest their food in the season of Spring, where she looks over the fields at night as the constellation of Virgo.");

}
//////////////////////////////
void libraMyth() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(5);
  tft.println("Libra");
  tft.println("Myth");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Libra is represented by the scales of justice, thought to have been used by Themis, the deity of law and order. Themis made sure that the humans were fair and safe when discussing matters of law, as well as debating. Her scales were placed in the heavens as a constant reminder that she is always watching over us. ");

  tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(5);
  tft2.println("Libra");
  tft2.println("Myth");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Libra is represented by the scales of justice, thought to have been used by Themis, the deity of law and order. Themis made sure that the humans were fair and safe when discussing matters of law, as well as debating. Her scales were placed in the heavens as a constant reminder that she is always watching over us. ");

}
//////////////////////////////
void scorpioMyth() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(5);
  tft.println("Scorpio");
  tft.println("Myth");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Scorpio is represented by a scorpion. This scorpion was sent by the Gods to punish an arrogant hunter named Orion. After angering Athena, the goddess of the hunt, she decides to send the scorpion to fight him. After teaching Orion a lesson, Athena placed the scorpion in the sky as a constellation to reward it for helping the gods.");

  tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(5);
  tft2.println("Scorpio");
  tft2.println("Myth");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Scorpio is represented by a scorpion. This scorpion was sent by the Gods to punish an arrogant hunter named Orion. After angering Athena, the goddess of the hunt, she decides to send the scorpion to fight him. After teaching Orion a lesson, Athena placed the scorpion in the sky as a constellation to reward it for helping the gods.");

}
//////////////////////////////
void sagiMyth() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(4);
  tft.println("Sagittarius");
  tft.println("Myth");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Sagittarius is represented by the great archer Crotonic. Crotonic was a satyr that was so good with a bow and arrow, he attracted a great number of followers. These followers pleaded to the Gods to put Crotonic in the sky, so that he could demonstrate his incredible archery skills to the entire world. The Gods listened, and placed Crotonic among the stars as the constellation Sagittarius, so he can show his archery to everyone, forever.");

  tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(4);
  tft2.println("Sagittarius");
  tft2.println("Myth");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Sagittarius is represented by the great archer Crotonic. Crotonic was a satyr that was so good with a bow and arrow, he attracted a great number of followers. These followers pleaded to the Gods to put Crotonic in the sky, so that he could demonstrate his incredible archery skills to the entire world. The Gods listened, and placed Crotonic among the stars as the constellation Sagittarius, so he can show his archery to everyone, forever.");

}
//////////////////////////////
void capriMyth() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(5);
  tft.println("Capricorn");
  tft.println("Myth");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Capricorn is represented by the Greek deity, Pan, a half goat half man. Pan was known for liking to party, but his parties caught the eye of a ferocious monster known as Typhon. Pan fought against Typhon, and after turning him into a fish, he was rewarded by Zeus, who put Pan in the sky as the constellation Capricorn.");

  tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(5);
  tft2.println("Capricorn");
  tft2.println("Myth");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Capricorn is represented by the Greek deity, Pan, a half goat half man. Pan was known for liking to party, but his parties caught the eye of a ferocious monster known as Typhon. Pan fought against Typhon, and after turning him into a fish, he was rewarded by Zeus, who put Pan in the sky as the constellation Capricorn.");

}
//////////////////////////////
void aquaMyth() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(5);
  tft.println("Aquarius");
  tft.println("Myth");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Aquarius is represented by Ganymede, a mortal prince who ascended to the heavens alongside Zeus. Ganymede wanted to make sure that the mortals had enough water to survive a bad drought, so he asked Zeus to help. Zeus made Ganymede into a deity with the power to control water, and was immortalized in the stars as the Constellation, Aquarius. ");

  tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(5);
  tft2.println("Aquarius");
  tft2.println("Myth");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Aquarius is represented by Ganymede, a mortal prince who ascended to the heavens alongside Zeus. Ganymede wanted to make sure that the mortals had enough water to survive a bad drought, so he asked Zeus to help. Zeus made Ganymede into a deity with the power to control water, and was immortalized in the stars as the Constellation, Aquarius. ");

}
//////////////////////////////
void piscesMyth() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(5);
  tft.println("Pisces");
  tft.println("Myth");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Pisces is represented by two fish who stand for the god Venus, and her son Cupid. A monstrous storm began to rock the ocean, and fearing that they would be separated, Venus transformed herself and her son into fish. They then tied their tails together so that they wouldn’t be separated in the storm. After the storm had passed, the two ascended into the stars as the constellation, Pisces.");

  tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(5);
  tft2.println("Pisces");
  tft2.println("Myth");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Pisces is represented by two fish who stand for the god Venus, and her son Cupid. A monstrous storm began to rock the ocean, and fearing that they would be separated, Venus transformed herself and her son into fish. They then tied their tails together so that they wouldn’t be separated in the storm. After the storm had passed, the two ascended into the stars as the constellation, Pisces.");

}
//////////////////////////////
void ariesMyth() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(5);
  tft.println("Aries");
  tft.println("Myth");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Aries is represented by the Golden Ram. This ram was said to have fleece made of pure gold, created by Poseidon. After being hunted and sacrificed for its fleece, the gods took pity on the creature and brought it to the stars as the constellation we know and observe today.");

  tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(5);
  tft2.println("Aries");
  tft2.println("Myth");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Aries is represented by the Golden Ram. This ram was said to have fleece made of pure gold, created by Poseidon. After being hunted and sacrificed for its fleece, the gods took pity on the creature and brought it to the stars as the constellation we know and observe today.");

}
//////////////////////////////
void taurusMyth() {
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(5);
  tft.println("Taurus");
  tft.println("Myth");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Taurus is represented by a bull, often thought to represent the god Zeus, after he transformed himself into a bull to be able to interact with mortal women. It is also possible that this constellation represents the monstrous Minotaur, a half-man, half bull, who was locked away in a labyrinth to prevent him from eating people.");

  tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(5);
  tft2.println("Taurus");
  tft2.println("Myth");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Taurus is represented by a bull, often thought to represent the god Zeus, after he transformed himself into a bull to be able to interact with mortal women. It is also possible that this constellation represents the monstrous Minotaur, a half-man, half bull, who was locked away in a labyrinth to prevent him from eating people.");

}
//////////////////////////////
//////////////////////////////
//This function is used to write the instructions on the idle animations.

unsigned long idleInstructionsMyth() {
  //tft.fillScreen(HX8357_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(1);
  tft.println("Instructions");
  tft.println("");
  tft.setTextSize(5);
  tft.println("Star Chart");
  tft.println("");
  tft.setTextSize(4);
  tft.println("Mythology");
  tft.println("Mode");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Plug into any one");
  tft.println("constellation to learn");
  tft.println("about how that");
  tft.println("constellation came to be");
  tft.println("according to Greek");
  tft.println("mythology!");

  //tft2.fillScreen(HX8357_BLUE);
  //unsigned long start = micros();
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(1);
  tft2.println("Instructions");
  tft2.println("");
  tft2.setTextSize(5);
  tft2.println("Star Chart");
  tft2.println("");
  tft2.setTextSize(4);
  tft2.println("Mythology");
  tft2.println("Mode");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Plug into any one");
  tft2.println("constellation to learn");
  tft2.println("about how that");
  tft2.println("constellation came to be");
  tft2.println("according to Greek");
  tft2.println("mythology!");

  return micros() - start;
}
void idleInstructionsAstrology() {
  //tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(1);
  tft.println("Instructions");
  tft.println("");
  tft.setTextSize(5);
  tft.println("Star Chart");
  tft.println("");
  tft.setTextSize(4);
  tft.println("Astrology");
  tft.println("Mode");
  tft.println("");
  tft.setTextSize(2);
  tft.println("Plug into any ONE");
  tft.println("constellation to get");
  tft.println("your horoscope!");
  tft.println("");
  tft.println("Plug into any TWO");
  tft.println("constellations to get");
  tft.println("your compatibility!");

  //tft2.fillScreen(HX8357_BLUE);
  tft2.setCursor(0, 0);
  tft2.setTextColor(HX8357_WHITE);
  tft2.setTextSize(1);
  tft2.println("Instructions");
  tft2.println("");
  tft2.setTextSize(5);
  tft2.println("Star Chart");
  tft2.println("");
  tft2.setTextSize(4);
  tft2.println("Astrology");
  tft2.println("Mode");
  tft2.println("");
  tft2.setTextSize(2);
  tft2.println("Plug into any ONE");
  tft2.println("constellation to get");
  tft2.println("your horoscope!");
  tft2.println("");
  tft2.println("Plug into any TWO");
  tft2.println("constellations to get");
  tft2.println("your compatibility!");
}
