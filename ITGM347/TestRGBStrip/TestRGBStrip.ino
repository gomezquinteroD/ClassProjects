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
int timesection = 1;

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
  
  //test code
  int k = 0;
  int m = 0;
  int k2 = 4;
  int m2 = 4;
  for(int i = 0; i < NUM_LEDS -3; i+=4){ //cycle through lights
    for (int k1 = k; k1 < k2; k1++){ //turn on first 4 lights
      leds[k1] = CRGB::Blue;
      FastLED.show();
    }
    delay(1000);
    /*
    for (int m1 = m; m1 < m2; m1++){ //turn off 4 lights
      leds[m1] = CRGB::Black;
      FastLED.show();
    }*/
    delay(1000);
    k += 4;
    m += 4;
    k2 += 4;
    m2 += 4;
  }
  
}
