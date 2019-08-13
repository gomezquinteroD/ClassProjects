#define FASTLED_INTERNAL
#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 50

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
      // Uncomment/edit one of the following lines for your leds arrangement.
       FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() { 
  // Turn the LED on, then pause
  /*for(int i = 1; i < NUM_LEDS; i++){
  leds[i] = CRGB::Red;
  FastLED.show();
  delay(100);
  leds[i-1] = CRGB::Green;
  FastLED.show();
  delay(100);
  // Now turn the LED off, then pause
  leds[i] = CRGB::Black;
  FastLED.show();
  delay(100);
  leds[i-1] = CRGB::Black;
  FastLED.show();
  delay(100);
  }*/
  int k = 0;
  int m = 0;
  int k2 = 4;
  int m2 = 4;
  for(int i = 0; i < NUM_LEDS -3; i+=4){
    for (int k1 = k; k1 < k2; k1++){
      leds[k1] = CRGB::Green;
      FastLED.show();
    }
    delay(1000);
    for (int m1 = m; m1 < m2; m1++){
      leds[m1] = CRGB::Black;
      FastLED.show();
    }
    delay(1000);
    k += 4;
    m += 4;
    k2 += 4;
    m2 += 4;
  }
}
