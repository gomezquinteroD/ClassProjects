int buzz = 12;
int i;

void setup() {
  // put your setup code here, to run once:
  pinMode(buzz, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(i = 0; i < 80; i++){
    digitalWrite(buzz, HIGH);
    delay(1);
    digitalWrite(buzz, LOW);
    delay(1);
  }

  for(i = 0; i < 100; i++){
    digitalWrite(buzz, HIGH);
    delay(2);
    digitalWrite(buzz, LOW);
  }
}
