#include "FastLED.h"
#define FASTLED_INTERNAL
#define NUM_LEDS 50
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
  // put your main code here, to run repeatedly:
  int i = 0;
  int k = 4;
   for(i = 0; i< k; i++){
    leds[i] = CRGB::Yellow;
    FastLED.show();
   }
   delay(500);
   k += 4;
   for(i = 4; i< k; i++){
    leds[i] = CRGB::Orange;
    FastLED.show();
   }
   delay(500);
   k += 4;
   for(i = 8; i< k; i++){
    leds[i] = CRGB::Red;
    FastLED.show();
   }
   delay(500);
   k += 4;
   for(i = 12; i< k; i++){
    leds[i] = CRGB::MediumVioletRed;
    FastLED.show();
   }
   delay(500);
   k += 4;
   for(i = 16; i< k; i++){
    leds[i] = CRGB::DarkViolet;
    FastLED.show();
   }
   delay(500);
   k += 4;
   for(i = 20; i< k; i++){
    leds[i] = CRGB::Blue;
    FastLED.show();
   }
   delay(500);
   k += 4;
   for(i = 24; i< k; i++){
    leds[i] = CRGB::DarkViolet;
    FastLED.show();
   }
   delay(500);
   k += 4;
   for(i = 28; i< k; i++){
    leds[i] = CRGB::MediumVioletRed;
    FastLED.show();
   }
   delay(500);
   k += 4;
   for(i = 32; i< k; i++){
    leds[i] = CRGB::Red;
    FastLED.show();
   }
   delay(500);
   k += 4;
   for(i = 36; i< k; i++){
    leds[i] = CRGB::Orange;
    FastLED.show();
   }
   delay(500);
   k += 4;
   for(i = 40; i< k; i++){
    leds[i] = CRGB::Yellow;
    FastLED.show();
   }
   delay(500);
   k += 4;
   for(i = 44; i< k; i++){
    leds[i] = CRGB::Orange;
    FastLED.show();
   }
   delay(500);
   k += 4;
   for(i = 48; i< k; i++){
    leds[i] = CRGB::Yellow;
    FastLED.show();
   }
   delay(500);
   k += 4;
}
