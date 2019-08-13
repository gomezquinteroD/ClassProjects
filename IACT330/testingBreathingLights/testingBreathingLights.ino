#include "FastLED.h"
#define FASTLED_INTERNAL
#define NUM_LEDS 3
#define DATA_PIN 6
#define BRIGHTNESS 100
int bright = 100;

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness( bright );
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Blue;
    FastLED.show();
  }

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
