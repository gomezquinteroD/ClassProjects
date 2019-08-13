int ledP = 5;
int bA = 9;
int bB = 8;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledP,OUTPUT);
  pinMode(bA, INPUT_PULLUP);
  pinMode(bB, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(bA) == LOW){
    digitalWrite(ledP, HIGH);
  }
  else{
    digitalWrite(ledP, LOW);
  }
  /*if(digitalRead(bB) == LOW){
    digitalWrite(ledP, LOW);
  }*/
}
