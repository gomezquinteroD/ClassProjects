//LED setups
#include "FastLED.h"
#define FASTLED_INTERNAL
#define NUM_LEDS 50
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

//Clock setups
#include <DS3231.h>
DS3231 rtc(SDA,SCL);
String currentClock;
String currh;
String prevh;
int tmz; //time zone

void setup() {
  // put your setup code here, to run once:
  //setup LEDs
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

  //setup Clock
  rtc.begin();
  rtc.setDOW(TUESDAY);
  rtc.setTime(21,50,0);
  rtc.setDate(4,9,2019);
  currentClock = rtc.getTimeStr(FORMAT_SHORT);
  currh = currentClock.substring(0,2);
  prevh = currentClock.substring(0,2);
  tmz = currh.toInt() - 6; //this is the first time zone based off EST (6hrs behind EST)

  //setup Serial
  Serial.begin(9600);
}

void loop() {
  currentClock = rtc.getTimeStr(FORMAT_SHORT);
  Serial.print("Time: ");
  Serial.println(currentClock);
  currh = currentClock.substring(0,2);
  tmz = currh.toInt()-6;
  Serial.print("Hour: ");
  Serial.println(currh);
  Serial.print("TMZ: ");
  Serial.println(tmz);

  if(currh != prevh){
    prevh = currh;
    changeHour();
  }
  assignColor(tmz,leds);
//end loop
}

void changeHour(){
  
}

void assignColor(int zoneT, CRGB leds){
  Serial.println("Assigning colors...");
  int i = 0;
  int ix = 0;
  int kx = 0;

  for(ix = i; ix < NUM_LEDS; ix += 4){
    Serial.print("ix: ");
    Serial.println(ix);
    kx = ix;

    switch(zoneT){
      case 6 ... 8:
        Serial.println("RED ASSIGNED");
        for(kx = ix; kx < ix+4; kx++){
          Serial.print("kx: ");
          Serial.println(kx);
          leds[kx] = CRGB::Red;
          FastLED.show();
        }
        i += 4;
        break;
      case 9 ... 11:
        Serial.println("ORANGE ASSIGNED");
        for(kx = ix; kx < ix+4; kx++){
          Serial.print("kx: ");
          Serial.println(kx);
          leds[kx] = CRGB::Red;
          FastLED.show();
        }
        i += 4;
        break;
      case 12:
        Serial.println("YELLOW ASSIGNED");
        for(kx = ix; kx < ix+4; kx++){
          Serial.print("kx: ");
          Serial.println(kx);
          leds[kx] = CRGB::Red;
          FastLED.show();
        }
        i += 4;
        break;
      case 13 ... 15:
        Serial.println("ORANGE ASSIGNED");
        for(kx = ix; kx < ix+4; kx++){
          Serial.print("kx: ");
          Serial.println(kx);
          leds[kx] = CRGB::Red;
          FastLED.show();
        }
        i += 4;
        break;
      case 16 ... 19:
        Serial.println("RED ASSIGNED");
        for(kx = ix; kx < ix+4; kx++){
          Serial.print("kx: ");
          Serial.println(kx);
          leds[kx] = CRGB::Red;
          FastLED.show();
        }
        i += 4;
        break;
      case 20 ... 21:
        Serial.println("RED VIOLET ASSIGNED");
        for(kx = ix; kx < ix+4; kx++){
          Serial.print("kx: ");
          Serial.println(kx);
          leds[kx] = CRGB::Red;
          FastLED.show();
        }
        i += 4;
        break;
      case 22 ... 23:
        Serial.println("VIOLET ASSIGNED");
        for(kx = ix; kx < ix+4; kx++){
          Serial.print("kx: ");
          Serial.println(kx);
          leds[kx] = CRGB::Red;
          FastLED.show();
        }
        i += 4;
        break;
      case 24:
        Serial.println("BLUE ASSIGNED");
        for(kx = ix; kx < ix+4; kx++){
          Serial.print("kx: ");
          Serial.println(kx);
          leds[kx] = CRGB::Red;
          FastLED.show();
        }
        i += 4;
        break;
        
    } //end switch
    zoneT++;
  }//end outer for loop
} //end function
