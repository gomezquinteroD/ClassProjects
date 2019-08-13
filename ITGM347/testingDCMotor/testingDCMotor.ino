const int controlPin1 = 2; // connected to pin 7 on the H-bridge
const int controlPin2 = 3; // connected to pin 2 on the H-bridge
const int enablePin = 9;   // connected to pin 1 on the H-bridge

void setup() {
  // put your setup code here, to run once:
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(controlPin1,HIGH);
  digitalWrite(controlPin2,LOW);
  analogWrite(enablePin,100);
  delay(2000);
  digitalWrite(controlPin1,LOW);
  digitalWrite(controlPin2,LOW);
  delay(5000);
  
}
