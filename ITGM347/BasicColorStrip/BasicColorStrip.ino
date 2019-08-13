//LED setups
#include "FastLED.h"
#define FASTLED_INTERNAL
#define NUM_LEDS 50
#define DATA_PIN 6

CRGB leds[NUM_LEDS];
//CRGB colors[] = {"Yellow", "Orange", "Red", "RedViolet", "Violet", "Blue"};

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int m = 4;
  int i = 0;
  for(m = 4; m < NUM_LEDS; m+=4){
    for(i = 0; i < m; i++){
      leds[i] = CRGB::Red;
      FastLED.show();
    }
    delay(1000);
  }

  
}
