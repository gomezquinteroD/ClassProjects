//LED setups
#include <FastLED.h>
#define FASTLED_INTERNAL
#define NUM_STRIPS 2
#define NUM_LEDS_PER_STRIP 50
#define DATA_PINA 6
#define DATA_PINB 5
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

//Clock setups
#include <DS3231.h>
DS3231 rtc(SDA,SCL);
String currentClock;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, DATA_PINA, RGB>(leds[0], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2811, DATA_PINB, RGB>(leds[1], NUM_LEDS_PER_STRIP);

  //setup Clock
  rtc.begin();
  rtc.setDOW(THURSDAY);
  rtc.setTime(15,06,0);
  rtc.setDate(4,11,2019);

  //setup Serial
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(rtc.getTimeStr(FORMAT_SHORT));
  for(int x = 0; x < NUM_STRIPS; x++) {
    //Serial.print("X: ");
    //Serial.println(x);
    // This inner loop will go over each led in the current strip, one at a time
    for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      //Serial.print("i: ");
      //Serial.println(i);
      leds[x][i] = CRGB::DarkViolet;
      FastLED.show();
      delay(1000);
      leds[x][i] = CRGB::Black;
      delay(1000);
    }
  }
}
