#include <Servo.h>
Servo myServo;
int servoP = 9;
int potP = A0;
int potVal;
int angle;
void setup() {
  // put your setup code here, to run once:
  myServo.attach(9);
  pinMode(potP, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  potVal = analogRead(potP);
  angle = map(potVal, 0, 1023, 0 , 179);
  Serial.print("angle: ");
  Serial.println(angle);
  myServo.write(angle);
  delay(15);
}
