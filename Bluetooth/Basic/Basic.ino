// https://blog.naver.com/PostView.nhn?blogId=eduino&logNo=221121406317&categoryNo=20&parentCategoryNo=0

#include <SoftwareSerial.h>
 
#define BT_RXD 8
#define BT_TXD 7
SoftwareSerial bluetooth(BT_RXD, BT_TXD);
 
void setup(){
  Serial.begin(9600);
  bluetooth.begin(9600);
}
 
void loop(){
  if (bluetooth.available()) {
    Serial.write(bluetooth.read());
  }
  if (Serial.available()) {
    bluetooth.write(Serial.read());
  }
}
