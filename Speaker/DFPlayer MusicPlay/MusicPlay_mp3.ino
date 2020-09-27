/* DFPlayer Music Player */
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

SoftwareSerial mySerial(10,11); //RX, TX

void setup () {
    Serial.begin (9600);
    mySerial.begin (9600);
    mp3_set_serial (mySerial);     
    delay(1);                     // 볼륨값 적용을 위한 delay
    mp3_set_volume (1);          // 볼륨조절 값 0~30
}
 
void loop () {        
    mp3_play (1);    //0001 파일 플레이
    delay(10000000);
}


/* 장치 상태 테스트용 코드
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// 시리얼 통신용 Rx, Tx 설정
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX

// DFPlayer 라이브러리 
DFRobotDFPlayerMini myDFPlayer;

// 상태 정보를 위한 함수 
// 함수는 제일 아래 작성되어 있다.
void printDetail(uint8_t type, int value);

void setup()
{
  //DFPlayer 시리얼 설정 9600
  mySoftwareSerial.begin(9600);

  //모니터용 시리얼 설정 115200
  Serial.begin(115200);
  
  //DFPlayer 시리얼을 DFPlayer 클래스에 연결시킨 다음
  // 시작!
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    //SDCARD가 없거나
    //통신이 제대로 연결 안되면 
    //에러 메시지 출력 후 정지
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  
  // 시작을 알림
  Serial.println(F("DFPlayer Mini online."));
  
  //소리 크기 설정 ( 0 ~ 30 )
  myDFPlayer.volume(15);  //Set volume value. From 0 to 30
 
  //첫 mp3파일 재생
  // mp3파일을 001.mp3, 002.mp3, 003.mp3 식으로 저장하며 된다. 
  myDFPlayer.play(1);  //Play the first mp3
}

void loop()
{
   //DFPlayer에서 상태 변경이 되었다면
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
*/
