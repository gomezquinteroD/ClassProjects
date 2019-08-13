#include "FastLED.h"
#define FASTLED_INTERNAL
#define NUM_LEDS 3
#define DATA_PIN 6
#define BRIGHTNESS 100

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Blue;
    FastLED.show();
  }

  for( int colorStep=0; colorStep <= 127; colorStep++ ) { //to purple

        int r = colorStep;
        int g = 0;
        int b = 255; 

        // Now loop though each of the LEDs and set each one to the current color
        for(int x = 0; x < NUM_LEDS; x++){
            leds[x] = CRGB(r,g,b);
        }

        // Display the colors we just set on the actual LEDs
        delay(10); 
        FastLED.show();
   }

   for( int colorStep=127; colorStep >= 0; colorStep-- ) { //to blue

        int r = colorStep;
        int g = 0;
        int b = 255; 

        // Now loop though each of the LEDs and set each one to the current color
        for(int x = 0; x < NUM_LEDS; x++){
            leds[x] = CRGB(r,g,b);
        }

        // Display the colors we just set on the actual LEDs
        delay(10); 
        FastLED.show();
   }
}
