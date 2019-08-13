int bPin = 3;
int gPin = 5;
int rPin = 6;

int bValue;
int gValue;
int rValue;
void setup() {
  // put your setup code here, to run once:
    rValue = 255;
  gValue = 0;
  bValue = 0;
}

void loop() {
  // put your main code here, to run repeatedly:


  for(int i = 0; i < 256; i++){ //fade out red, bring in green
    rValue--;
    gValue++;
    analogWrite(rPin,rValue);
    analogWrite(gPin,gValue);
    delay(10);
  }
  for(int i = 0; i < 256; i++){ //fade out green, bring in blue
    gValue--;
    bValue++;
    analogWrite(gPin,gValue);
    analogWrite(bPin,bValue);
    delay(10);
  }
  for(int i = 0; i < 256; i++){ //fade out blue, bring in red
    bValue--;
    rValue++;
    analogWrite(bPin,bValue);
    analogWrite(rPin,rValue);
    delay(10);
  }
  
}
