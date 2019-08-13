#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Stepper.h>
#include <Servo.h>

//pre-setup definitions
//motor shield variables
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //motor shield
uint8_t servoNum = 0;

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096) MIGHT NEED TWEAKING
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
uint8_t servonum = 0;

//stepper motor
const int stepsPerRevolution = 300; //might need to change this for my motor
Stepper myGreenStepper(stepsPerRevolution, 4,5,6,7);
Stepper myRedStepper(stepsPerRevolution, 8, 9, 10, 11);

//there are 4 checkpts, 2 per player, and 1 finish line checkpt
boolean gameOn = false;
boolean gameOver = false;

//player buttons
int redButton = 2;
int redState = 0;
int greenButton = 3;
int greenState = 0;

//player scores
int redScore = 0;
int greenScore = 0;

//checkpoint points & statuses
int check1R = 15;
int check1G = 15;
int check2R = 45;
int check2G = 45;
int finish = 70;
boolean point1R = false; //all checkpoints down
boolean point2R = false;
boolean point1G = false;
boolean point2G = false;
boolean lifted1R = false; //if they have been lifted or not
boolean lifted2R = false;
boolean lifted1G = false;
boolean lifted2G = false;
int winner = 2;
int redClicks = 0;
int greenClicks = 0;

void setup() {// -----------------setup
  //motor shield setup
  pwm.begin();
  pwm.setPWMFreq(60); //test run!
  delay(10);

  //stepper setup
  myRedStepper.setSpeed(60);
  myGreenStepper.setSpeed(60);
  Serial.begin(9600);
  
  gameOn = true;
  resetGame();

  delay(1000);
  //wave starting flag
  waveFlag();
  

}

void loop() {
  
  //have a start function

  //while in game, check for input from buttons, update servos each time each button is clicked
  if(gameOn){
    redState = digitalRead(redButton);
    greenState = digitalRead(greenButton);
    //Serial.print("redState: ");
    //Serial.println(redState);

    if(redState == 1){
      Serial.print("Red Forward! Current Points: ");
      myRedStepper.step(-stepsPerRevolution);
      redScore += 5;
      redState = 0;
      Serial.println(redScore);
      delay(500);
    }
    if(greenState == 1){
      Serial.print("Green Forward! Current Points: ");
      myGreenStepper.step(stepsPerRevolution);
      greenScore += 5;
      greenState = 1;
      Serial.println(greenScore);
      delay(500);
    }
  }
  
  //if P1 > point of 1st checkpt, activate first checkpt
  // same for every other checkpt
  if (redScore > check1R){
    if (!lifted1R){
      for (uint16_t pulselen = 400; pulselen > 200; pulselen--) {
        pwm.setPWM(1, 0, pulselen);
      }
      delay(15);
      point1R = true;
      lifted1R = true;
    }
  }
  if (redScore > check2R){
    if (!lifted2R){
      for (uint16_t pulselen = 400; pulselen > 200; pulselen--) {
        pwm.setPWM(3, 0, pulselen);
      }
      delay(15);
      point2R = true;
      lifted2R = true;
    }
  }
  if (greenScore > check1G){
    if (!lifted1G){
      for (uint16_t pulselen = 400; pulselen > 200; pulselen--) {
        pwm.setPWM(0, 0, pulselen);
      }
      delay(15);
      point1G = true;
      lifted1G = true;
    }
  }
  if (greenScore > check2G){
    if (!lifted2G){
      for (uint16_t pulselen = 400; pulselen > 200; pulselen--) {
        pwm.setPWM(2, 0, pulselen);
      }
      delay(15);
      point2G = true;
      lifted2G = true;
    }
  }

  //if P1 || P2 > final point - Winner function activated
  //delay for a bit
  if (redScore > finish){
    FinishLineWin(1);
    winner = 0;
    gameOver = true;
    gameOn = false;
  }
  if (greenScore > finish){
    FinishLineWin(2);
    winner = 1;
    gameOn = false;
    gameOver = true;
  }
  
  //reset game -> put down all check pts
  if (gameOver){
    delay(15000); //delay 15sec before reset
    resetGame();
  }
}

void FinishLineWin(int winner){
  if(winner == 1){ //P1 Wins the game
    Serial.println("Red Cat Wins!");
    for (uint16_t pulselen = 400; pulselen > 300; pulselen--) { //from 90deg lean right
      pwm.setPWM(4, 0, pulselen);
    }
    winner = 2;
  }
  else{ //P2 Wins the game
    Serial.println("Green Dog Wins!");
    for (uint16_t pulselen = 400; pulselen < 500; pulselen++) { //from 90deg lean left
      pwm.setPWM(4, 0, pulselen);
    }
    winner = 2;
  }
  delay(5000);
  //resetGame();
}

void resetGame(){
  Serial.println("Resetting game...");
  servonum = 0; //reset checkpoints
  while(servonum < 4){
    for (uint16_t pulselen = 200; pulselen < 400; pulselen++) {
      pwm.setPWM(servonum, 0, pulselen);
    }
    servonum++;
    Serial.println(servonum);
  }
  
    
  waveFlag();
  //set finish line back up to default, not max or min
  if (winner == 0){ //if red won, lean left to 90deg
    for (uint16_t pulselen = 300; pulselen < 400; pulselen++) { //from 90deg lean left
      pwm.setPWM(4, 0, pulselen);
    }
  }
  else if(winner == 1){ //if green won, lean right to 90deg
    for (uint16_t pulselen = 500; pulselen > 400; pulselen--) { //from 90deg lean right
      pwm.setPWM(4, 0, pulselen);
    }
  }
  //reset scores
  redScore = 0;
  greenScore = 0;
  winner = 2;

  //reset chk pts
  point1R = false;
  lifted1R = false;
  point2R = false;
  lifted2R = false;
  point1G = false;
  lifted1G = false;
  point2G = false;
  lifted2G = false;
  
  gameOn = true;
  gameOver = false;
  delay(500);
  Serial.println("Game reset!");
  waveFlag();
}

void waveFlag(){
  for (uint16_t pulselen = 400; pulselen > 300; pulselen--) { //from 90deg lean right
      pwm.setPWM(4, 0, pulselen);
  }
  delay(200);
  for (uint16_t pulselen = 300; pulselen < 500; pulselen++) { //from 90deg lean left
      pwm.setPWM(4, 0, pulselen);
  }
  delay(200);
  for (uint16_t pulselen = 500; pulselen > 400; pulselen--) { //from 90deg lean right
      pwm.setPWM(4, 0, pulselen);
  }
  delay(200);
}
