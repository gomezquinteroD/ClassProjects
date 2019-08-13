#include <Stepper.h>

const int stepsPerRevolution = 300;  // change this to fit the number of steps per revolution
// for your motor
const int negSteps = -400;
Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7);

const int controlPin1 = 2; // connected to pin 7 on the H-bridge
const int controlPin2 = 3; // connected to pin 2 on the H-bridge
const int enablePin = 9;   // connected to pin 1 on the H-bridge

void setup() {
  // put your setup code here, to run once:
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);

  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);
  
  digitalWrite(controlPin1,HIGH);
  digitalWrite(controlPin2,LOW);
  analogWrite(enablePin,100);
  delay(2000);
  digitalWrite(controlPin1,LOW);
  digitalWrite(controlPin2,LOW);
  delay(5000);
}
