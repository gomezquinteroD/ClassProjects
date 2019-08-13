int ledpin = 9;
void setup() {
  // put your setup code here, to run once:
  //for analogWrite, you dont need to use pinMode(#,OUTPUT)
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 256; i++){
    analogWrite(ledpin,i);
    delay(40);
  }
  for (int k = 255; k > 0; k--){
    analogWrite(ledpin,k);
    delay(40);
  }
}
