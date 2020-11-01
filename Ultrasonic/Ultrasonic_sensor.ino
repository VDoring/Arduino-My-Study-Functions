void setup() {
  Serial.begin(9600);

  pinMode(2,OUTPUT); //trigger
  pinMode(3,INPUT); //echo
}

 

void loop() {
  float backtime;
  float cm;

  //초음파 발사
  digitalWrite(2,HIGH);
  delay(10);
  digitalWrite(2,LOW);
 
  //물체에 맞고 돌아온 시간 저장
  backtime = pulseIn(3,HIGH);

  //거리 환산
  cm = ((float)(backtime * 340) / 10000) / 2;

  Serial.print(cm);
  Serial.println("cm");
}
