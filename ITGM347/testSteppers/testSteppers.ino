#include <Stepper.h>

const int stepsPerRevolution = 300;  // change this to fit the number of steps per revolution
// for your motor
Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7); //green
Stepper myOtherStepper(stepsPerRevolution, 8, 9, 10, 11); //red



void setup() {
  // put your setup code here, to run once:
  myStepper.setSpeed(60);
  myOtherStepper.setSpeed(60);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 5; i++){
    myStepper.step(stepsPerRevolution);
    //myStepper.step(stepsPerRevolution);
    delay(500);
  }
  delay(1000);
  for(int i = 0; i < 5; i++){
    myOtherStepper.step(stepsPerRevolution);
    //myStepper.step(-stepsPerRevolution);
    delay(500);
  }
  
}
