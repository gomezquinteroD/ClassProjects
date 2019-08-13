
int rb = 2;
int gb = 3;

int rs = 0;
int gs = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(rb, INPUT);
  pinMode(gb, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  rs = digitalRead(rb);
  gs = digitalRead(gb);

  if (rs == 1){
    Serial.println("Red pressed");
    delay(500);
  }
  else{
    //Serial.println("Red pressed");
  }

  if (gs == 1){
    Serial.println("Green pressed");
    delay(500);
  }
  else{
    //Serial.println("Red pressed");
  }
}
