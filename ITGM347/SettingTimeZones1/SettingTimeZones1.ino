//LED setups
#include <FastLED.h>
#define FASTLED_INTERNAL
#define NUM_STRIPS 1
#define NUM_LEDS 100
#define TOTAL_LEDS 100
#define DATA_PINA 6
#define BRIGHTNESS  64

CRGB leds[NUM_LEDS];
CRGBPalette16 myPalette;
CRGB aColor;
  CRGB Red = CHSV(HUE_RED, 0, 0);
  CRGB MediumVioletRed = CHSV(204,0,102);
  CRGB DarkViolet = CHSV(102,0,204);
  CRGB Blue = CHSV(0,0,HUE_BLUE);
  CRGB Orange = CHSV(255,128,0);
  CRGB Yellow = CHSV(255,255,0);
  CRGB Black = CHSV(0,0,0);
int currled = 0;
int currstrip = 0;

//Clock setups
#include <DS3231.h>
DS3231 rtc(SDA,SCL);
String currentClock;
String currh;
String prevh;
int tmz;
int hrTmz;
CRGB timeZones[24][4];

void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, DATA_PINA, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS);

  //setup Serial
  Serial.begin(9600);

  //setup Clock
  rtc.begin();
  rtc.setDOW(FRIDAY);
  rtc.setTime(21,48,0);
  rtc.setDate(4,16,2019);
  currentClock = rtc.getTimeStr(FORMAT_SHORT);
  currh = currentClock.substring(0,2);
  prevh = currentClock.substring(0,2);
  tmz = currh.toInt() - 6; //this is the first time zone based off EST (6hrs behind EST)


  
}

void loop() {
  // put your main code here, to run repeatedly:
  currentClock = rtc.getTimeStr(FORMAT_SHORT); //check the time
  Serial.print("--------Time: "); //print the time
  Serial.println(currentClock);
  currh = currentClock.substring(0,2);
  tmz = currh.toInt()-6;
  Serial.print("Hour: "); //print the current hour
  Serial.println(currh);
  //Serial.print("TMZ: "); //print the first time zone's time
  //Serial.println(tmz);
  hrTmz = tmz; //time zone cycle hour
  currled = 0;
  
  for(int k = 0; k < 24; k++){
    //switch case based on hour
    //at each case call assignColor with the corresponding color
    Serial.print("hrTmz: ");
    Serial.println(hrTmz);
    Serial.print("currled: ");
    Serial.println(currled);
    switch(hrTmz){
      case 6 ... 8:
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::Red;
          FastLED.show();
          delay(300);
          /*if(i == 53){
            currstrip = 1;
            currled = 0;
          }*/
        }
        //currled +=4;
        break;
      case 9 ... 11:
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::Orange;
          FastLED.show();
          delay(300);
          /*if(i == 53){
            currstrip = 1;
            currled = 0;
          }*/
        }
        //currled +=4;
        break;
      case 12:
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::Yellow;
          FastLED.show();
          delay(300);
          /*if(i == 53){
            currstrip = 1;
            currled = 0;
          }*/
        }
        //currled +=4;
        break;
      case 13 ... 15:
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::Orange;
          FastLED.show();
          delay(300);
          /*if(i == 53){
            currstrip = 1;
            currled = 0;
          }*/
        }
        //currled +=4;
        break;
      case 16 ... 19:
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::Red;
          FastLED.show();
          delay(300);
          /*if(i == 53){
            currstrip = 1;
            currled = 0;
          }*/
        }
        //currled +=4;
        break;
      case 20 ... 21:
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::MediumVioletRed;
          FastLED.show();
          delay(300);
          /*if(i == 53){
            currstrip = 1;
            currled = 0;
          }*/
        }
        //currled +=4;
        break;
      case 22 ... 23:
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::DarkViolet;
          FastLED.show();
          delay(300);
          /*if(i == 53){
            currstrip = 1;
            currled = 0;
          }*/
        }
        //currled +=4;
        break;
      case 24:
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::Blue;
          FastLED.show();
          delay(300);
          /*if(i == 53){
            currstrip = 1;
            currled = 0;
          }*/
        }
        //currled +=4;
        break;
      case 1 ... 2:
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::DarkViolet;
          FastLED.show();
          delay(300);
          /*if(i == 53){
            currstrip = 1;
            currled = 0;
          }*/
        }
        //currled +=4;
        break;
      case 3 ... 5:
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::MediumVioletRed;
          FastLED.show();
          delay(300);
          /*if(i == 53){
            currstrip = 1;
            currled = 0;
          }*/
        }
        //currled +=4;
        break;
      default:
        break;
    }
    currled +=4;
    hrTmz++;
    if(hrTmz > 24){
      hrTmz = 1;
    }
    if (currled > 100){
      currled = 0;
    }
  }
    
  
}//end loop
