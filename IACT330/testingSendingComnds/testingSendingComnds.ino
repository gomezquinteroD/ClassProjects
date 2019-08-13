
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  handleSerial();
}

void handleSerial(){
  while(Serial.available() > 0){
    char incomingCharacter = Serial.read();
    switch(incomingCharacter){
      case 'a':
        Serial.println("I received an A");
        break;

      case 'b':
        Serial.println("I now have a B");
        break;
    }
  }
}
