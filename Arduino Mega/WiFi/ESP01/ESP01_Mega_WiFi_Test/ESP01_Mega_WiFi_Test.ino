void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);// 9600 115200
                       //시리얼모니터에서도 AT+UART_DEF=9600,8,1,0,0 입력
                       
  //Serial1은 Arduino Mega의 Rx1, Tx1에 해당
  //Serial2은 Arduino Mega의 Rx2, Tx2에 해당
  //Serial3은 Arduino Mega의 Rx3, Tx3에 해당
}

void loop() {
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
}
