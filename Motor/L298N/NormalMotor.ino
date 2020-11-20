void setup() {
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(5,OUTPUT); //PWM
}

void loop() {
  digitalWrite(7,HIGH);
  digitalWrite(6,LOW);
  analogWrite(5,200);
}
