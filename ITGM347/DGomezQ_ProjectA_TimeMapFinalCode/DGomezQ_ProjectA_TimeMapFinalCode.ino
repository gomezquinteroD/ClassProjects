//LED setups
#include <FastLED.h>
#define FASTLED_INTERNAL
#define NUM_STRIPS 1
#define NUM_LEDS 100
#define DATA_PINA 6
#define BRIGHTNESS  64

CRGB leds[NUM_LEDS];
CRGBPalette16 myPalette; //colors being used
CRGB aColor;
  CRGB Red = CHSV(HUE_RED, 0, 0);
  CRGB MediumVioletRed = CHSV(204,0,102);
  CRGB DarkViolet = CHSV(102,0,204);
  CRGB Blue = CHSV(0,0,HUE_BLUE);
  CRGB Orange = CHSV(255,128,0);
  CRGB Yellow = CHSV(255,255,0);
  CRGB Black = CHSV(0,0,0);
int currled = 0; //the current LED in the loop

//Clock setups
#include <DS3231.h>
DS3231 rtc(SDA,SCL);
String currentClock; //display current time
String currh; //display current hour
String prevh; //keep track of the previous hour
int tmz; //time zone
int hrTmz; //the hour of the time zone

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, DATA_PINA, RGB>(leds, NUM_LEDS); //LED strip
  FastLED.setBrightness( BRIGHTNESS); //Lower the brightness

  //setup Serial
  Serial.begin(9600); //Print to check on values

  //setup Clock
  rtc.begin();
  rtc.setDOW(FRIDAY); //set the time for the clock
  rtc.setTime(15,54,0);
  rtc.setDate(4,18,2019);
  currentClock = rtc.getTimeStr(FORMAT_SHORT); //this format only returns HR and MIN
  currh = currentClock.substring(0,2); //extract the first hour
  prevh = currentClock.substring(0,2); //extract first hour for reference
  tmz = currh.toInt() - 6; //this is the first time zone based off EST (6hrs behind EST)


  
}

void loop() {
  // put your main code here, to run repeatedly:
  currentClock = rtc.getTimeStr(FORMAT_SHORT); //check the time
  Serial.print("--------Time: "); //print the time
  Serial.println(currentClock);
  currh = currentClock.substring(0,2);
  tmz = currh.toInt()-6; //set the time zone according to current time
  Serial.print("Hour: "); //print the current hour
  Serial.println(currh);
  //Serial.print("TMZ: "); //print the first time zone's time
  //Serial.println(tmz);
  hrTmz = tmz; //time zone cycle hour
  currled = 0;
  
  for(int k = 0; k < 24; k++){ //for each time zone, determine the hour, light LEDs with appropriate color
    //switch case based on hour
    //at each case call assignColor with the corresponding color
    Serial.print("hrTmz: ");
    Serial.println(hrTmz);
    Serial.print("currled: ");
    Serial.println(currled);
    switch(hrTmz){
      case 6 ... 8: //6-8am
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::Red;
          FastLED.show();
          delay(300);
        }
        break;
      case 9 ... 11: //9-11am
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::Orange;
          FastLED.show();
          delay(300);
        }
        break;
      case 12: //noon
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::Yellow;
          FastLED.show();
          delay(300);
        }
        break;
      case 13 ... 15: //1-3pm
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::Orange;
          FastLED.show();
          delay(300);
        }
        break;
      case 16 ... 19: //4-7pm
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::Red;
          FastLED.show();
          delay(300);
        }
        break;
      case 20 ... 21: //8-9pm
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::MediumVioletRed;
          FastLED.show();
          delay(300);
        }
        break;
      case 22 ... 23: //10-11pm
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::DarkViolet;
          FastLED.show();
          delay(300);
        }
        break;
      case 24: //midnight
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::Blue;
          FastLED.show();
          delay(300);
        }
        break;
      case 1 ... 2: //1-2am
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::DarkViolet;
          FastLED.show();
          delay(300);
        }
        break;
      case 3 ... 5: //3-5am
        for(int i = currled; i < currled+4; i++){ //cycle through the leds in the zones
          leds[i] = CRGB::MediumVioletRed;
          FastLED.show();
          delay(300);
        }
        break;
      default:
        break;
    }
    currled +=4; //move the current led + 4 since each time zone is 4 LEDs
    hrTmz++; //move to the next time zone
    if(hrTmz > 24){ //if it is midnight, but 24 time zones have not been set, reset to 1am
      hrTmz = 1;
    }
    if (currled > 100){ //once it has gone through all the LEDs in the strip, reset
      currled = 0;
    }
  }
  
}//end loop
