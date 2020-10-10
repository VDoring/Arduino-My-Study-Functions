#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // WIFI 모듈의 TX, RX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);// 9600 115200
                       //시리얼모니터에서도 AT+UART_DEF=9600,8,1,0,0 입력
}

void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}

/*
void setup() {
    
}

void loop() {
    
}
*/
