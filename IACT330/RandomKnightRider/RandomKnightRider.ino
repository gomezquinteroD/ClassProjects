int pinArray[] = {9,10,11,12,13};
int count = 0;
int timer = 100;
int randPin = 9;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(0));
  //declares all pins in pin array
  for (count = 0; count < 5; count++) {
    pinMode(pinArray[count], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //choose the random pin
  randPin = random(9,14);
  Serial.println(randPin);
  //turn on a random LED
  digitalWrite(randPin, HIGH);
  delay(timer);
  digitalWrite(randPin, LOW);

}
