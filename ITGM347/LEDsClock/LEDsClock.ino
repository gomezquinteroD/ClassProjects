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

  //setup Lighting
  
}

void loop() {
  // print time for checking
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
  for(int i = 0; i < 24; i++){
    assignColor(tmz,leds);
    tmz++;
  }
  
}

void changeHour(){
  
}
void assignColor(int zoneT, CRGB leds){ //zoneT is the zone's hour, zoneN is the zone number
  Serial.println("Assigning colors...");
  int i = 0;
  int k = 0;
  int kx = 0;
  for(int ix = i; ix < NUM_LEDS; ix+=4){
    Serial.print("ix: ");
    Serial.println(ix);
    switch (zoneT){
      case 6 ... 8: //Red
        Serial.println("RED ASSIGNED");
        for(kx = k; kx < k + 4; kx++){
           Serial.print("kx: ");
          Serial.println(kx);
          leds[kx] = CRGB::Red;
          FastLED.show();
        }
        i += 4;
        k += 4;
        break;
      case 9 ... 11: //Orange
         Serial.println("ORANGE ASSIGNED");
        for(kx = k; kx < k + 4; kx++){
          leds[kx] = CRGB::Orange;
          FastLED.show();
        }
        i += 4;
        k += 4;
        break;
      case 12: //Yellow
       Serial.println("YELLOW ASSIGNED");
        for(kx = k; kx < k + 4; kx++){
          leds[kx] = CRGB::Yellow;
          FastLED.show();
        }
        i += 4;
        k += 4;
        break;
      case 13 ... 14: //Orange
       Serial.println("ORANGE ASSIGNED");
        for(kx = k; kx < k + 4; kx++){
          leds[kx] = CRGB::Orange;
          FastLED.show();
        }
        i += 4;
        k += 4;
        break;
      case 15: //Orange
       Serial.println("ORANGE ASSIGNED");
        for(kx = k; kx < k + 4; kx++){
          Serial.print("kx: ");
          Serial.println(kx);
          leds[kx] = CRGB::Orange;
          FastLED.show();
        }
        i += 4;
        k += 4;
        break;
      case 16 ... 19: //Red
       Serial.println("RED ASSIGNED");
        for(kx = k; kx < k + 4; kx++){
          leds[kx] = CRGB::Red;
          FastLED.show();
        }
        i += 4;
        k += 4;
        break;
      case 20 ... 21: //Red Violet
       Serial.println("RED VIOLET ASSIGNED");
        for(kx = k; kx < k + 4; kx++){
          leds[kx] = CRGB::MediumVioletRed;
          FastLED.show();
        }
        i += 4;
        k += 4;
        break;
      case 22 ... 23: //Violet
       Serial.println("VIOLET ASSIGNED");
       for(kx = k; kx < k + 4; kx++){
          leds[kx] = CRGB::DarkViolet;
          FastLED.show();
        }
        i += 4;
        k += 4;
        break;
      case 24: //Blue
       Serial.println("BLUE ASSIGNED");
        for(kx = k; kx < k + 4; kx++){
          leds[kx] = CRGB::Blue;
          FastLED.show();
        }
        break;
      case 1 ... 2: //Violet
       Serial.println("VIOLET ASSIGNED");
        for(kx = k; kx < k + 4; kx++){
          leds[kx] = CRGB::DarkViolet;
          FastLED.show();
        }
        i += 4;
        k += 4;
        break;
      case 3 ... 5: //Red Violet
       Serial.println("RED VIOLET ASSIGNED");
        for(kx = k; kx < k + 4; kx++){
          leds[kx] = CRGB::MediumVioletRed;
          FastLED.show();
        }
        i += 4;
        k += 4;
        break;
      default:
        break;
    }
    delay(1000);
  }
}
