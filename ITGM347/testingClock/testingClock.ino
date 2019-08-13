#include <LiquidCrystal.h>
#include <DS3231.h>
DS3231 rtc(SDA,SCL);
LiquidCrystal lcd(7,8,9,10,11,12);

void setup() {
  // put your setup code here, to run once:
  rtc.begin();
  rtc.setDOW(THURSDAY);
  rtc.setTime(18,43,0);
  rtc.setDate(4,4,2019);
  
  lcd.begin(16,2);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,0);
  lcd.print("Time: ");
  lcd.print(rtc.getTimeStr());

  lcd.setCursor(0,1);
  lcd.print("Date: ");
  lcd.print(rtc.getDateStr());

  Serial.print(rtc.getDOWStr());
  Serial.print(" ");

  Serial.print(rtc.getDateStr());
  Serial.print(" ");
}
