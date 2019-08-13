int pinArray[] = {9,10,11,12,13};
int count = 0;
int timer = 50;
int randPin = 9;

void setup() {
  // put your setup code here, to run once:
  //declares all pins in pin array
  for (count = 0; count < 5; count++) {
    pinMode(pinArray[count], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //turns on LEDs in one direction. Static
  for(count = 0; count < 5; count++){
    digitalWrite(pinArray[count], HIGH);
    delay(timer);
    digitalWrite(pinArray[count+1], HIGH);
    delay(timer);
    digitalWrite(pinArray[count], LOW);
    delay(timer);
  }
  //turns on LEDs in opposite direction. Static
  for (count = 4; count >= 0; count--){
    digitalWrite(pinArray[count], HIGH);
    delay(timer);
    digitalWrite(pinArray[count-1], HIGH);
    delay(timer);
    digitalWrite(pinArray[count], LOW);
    delay(timer);
  }
  
}
