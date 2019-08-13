int pin = 1;
int spin = 2;
//dots are 500ms long
//dashes are 1500ms long
//red light is the morse
//green signals change of letter, double green is end of word/text
void setup() {
  // put your setup code here, to run once:
pinMode(pin, OUTPUT);
pinMode(spin,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(spin, LOW); //green
  delay(1000);
  
  digitalWrite(pin, HIGH); //dash D
  delay(1500);
  digitalWrite(pin, LOW);
  delay(500);
  
  digitalWrite(pin, HIGH); //dot
  delay(500);
  digitalWrite(pin, LOW);
  delay(500);

  digitalWrite(pin, HIGH); //dot
  delay(500);
  digitalWrite(pin, LOW);
  delay(500);

  digitalWrite(spin, HIGH); //green --------
  delay(1000);
  digitalWrite(spin, LOW);
  delay(1000);

  digitalWrite(pin, HIGH); //dot A
  delay(500);
  digitalWrite(pin, LOW);
  delay(500);
  digitalWrite(pin, HIGH); //dash
  delay(1500);
  digitalWrite(pin, LOW);
  delay(500);

  digitalWrite(spin, HIGH); //green --------
  delay(1000);
  digitalWrite(spin, LOW);
  delay(1000);

  digitalWrite(pin, HIGH); //dash N
  delay(1500);
  digitalWrite(pin, LOW);
  delay(500);
  digitalWrite(pin, HIGH); //dot 
  delay(500);
  digitalWrite(pin, LOW);
  delay(500);

  digitalWrite(spin, HIGH); //green --------
  delay(1000);
  digitalWrite(spin, LOW);
  delay(1000);

  digitalWrite(pin, HIGH); //dot I
  delay(500);
  digitalWrite(pin, LOW);
  delay(500);
  digitalWrite(pin, HIGH); //dot 
  delay(500);
  digitalWrite(pin, LOW);
  delay(500);

  digitalWrite(spin, HIGH); //green ----------
  delay(2000);
  digitalWrite(spin, LOW);
  delay(1000);
  digitalWrite(spin, HIGH); //green
  delay(1000);
  digitalWrite(spin, LOW);
  delay(1000);
  
}
