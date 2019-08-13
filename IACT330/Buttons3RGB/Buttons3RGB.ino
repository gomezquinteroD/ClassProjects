int bPin = 6;
int gPin = 5;
int rPin = 4;

int bR = 10;
int bG = 9; 
int bB = 8;
void setup() {
  // put your setup code here, to run once:
  pinMode(bR, INPUT_PULLUP);
  pinMode(bG, INPUT_PULLUP);
  pinMode(bB, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(bR) == LOW){
    analogWrite(rPin, 255);
  }
  else {
    analogWrite(rPin, 0);
  }

  if(digitalRead(bG) == LOW){
    analogWrite(gPin, 255);
  }
  else {
    analogWrite(gPin, 0);
  }

  if(digitalRead(bB) == LOW){
    analogWrite(bPin, 255);
  }
  else {
    analogWrite(bPin, 0);
  }
}
