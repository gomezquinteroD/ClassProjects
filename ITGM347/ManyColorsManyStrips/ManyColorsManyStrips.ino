//LED setups
#include <FastLED.h>
#define FASTLED_INTERNAL
#define NUM_STRIPS 2
#define NUM_LEDS_PER_STRIP 50
#define TOTAL_LEDS 100
#define DATA_PINA 6
#define DATA_PINB 5
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, DATA_PINA, RGB>(leds[0], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2811, DATA_PINB, RGB>(leds[1], NUM_LEDS_PER_STRIP);

  //setup Serial
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int x = 0; x < NUM_STRIPS; x++){
    Serial.print("x: ");
    Serial.println(x);
    for(int i = 0; i < NUM_LEDS_PER_STRIP; i++){
      Serial.print("i: ");
      Serial.println(i);
      leds[x][i] = CRGB::Green;
      FastLED.show();
      delay(500);
    }
  }
  for(int x = 0; x < NUM_STRIPS; x++){ //turn them off
    Serial.print("x: ");
    Serial.println(x);
    for(int i = 0; i < NUM_LEDS_PER_STRIP; i++){
      Serial.print("i: ");
      Serial.println(i);
      leds[x][i] = CRGB::Black;
      FastLED.show();
      delay(500);
    }
  }
}
