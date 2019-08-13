#include <Servo.h>
Servo myServo;
int servoP = 9;
int potP = A0;
int potVal;
int angle;
int gPin = 5;
int rPin = 4;
void setup() {
  // put your setup code here, to run once:
  myServo.attach(9);
  pinMode(potP, INPUT_PULLUP);
  pinMode(gPin, OUTPUT);
  pinMode(rPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  potVal = analogRead(potP);
  angle = map(potVal, 0, 1023, 0 , 179);
  Serial.print("angle: ");
  Serial.println(angle);
  myServo.write(angle);

  if(angle > 170){
    digitalWrite(gPin, HIGH);
  }
  else{
    digitalWrite(gPin, LOW);
  }

  if(angle < 10){
    digitalWrite(rPin, HIGH);
  }
  else{
    digitalWrite(rPin, LOW);
  }
  delay(15);
}
