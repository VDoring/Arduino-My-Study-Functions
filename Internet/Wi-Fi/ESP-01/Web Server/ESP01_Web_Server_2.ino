//이 코드는 작동하지 않습니다.
//Original Code : File - Examples - WiFiEsp - WebServer
//Time : https://postpop.tistory.com/84
//Remake : VDoring

#include <SoftwareSerial.h>
#include "WiFiEsp.h"

//https://postpop.tistory.com/84
#include <WiFi.h>
#include <TimeLib.h>

SoftwareSerial mySerial(2, 3); // RX, TX

char ssid[] = "SSID"; // 와이파이 SSID
char pass[] = "PW"; // 와이파이 비밀번호
int status = WL_IDLE_STATUS; // the Wifi radio's status
int reqCount = 0; // number of requests received

WiFiEspServer server(80);

const char* ntpServer = "pool.ntp.org"; //NTP 서버
uint8_t timeZone = 9; //한국 타임존 설정
uint8_t summerTime = 0; //3600  //썸머타임 시간

int s_hh = 12; // 시간 설정용 시 변수 및 초기값, < 0 조건 위해 자료형 int
int s_mm = 59; // 시간 설정용 분 변수 및 초기값
uint8_t s_ss = 45; // 시간 설정용 초 변수 및 초기값
uint16_t s_yy = 2017; // 시간 설정용 년 변수 및 초기값
uint8_t s_MM = 11; // 시간 설정용 월 변수 및 초기값
uint8_t s_dd = 19; // 시간 설정용 일 변수 및 초기값

time_t noww; //현재 시간 변수
time_t prevEpoch; //이전 UTC 시간 변수

typedef struct structTM
  {
    int  tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
    #ifdef __TM_GMTOFF
      long  __TM_GMTOFF;
    #endif
    #ifdef __TM_ZONE
      const char *__TM_ZONE;
    #endif
  } tm;
  tm timeinfo;

struct tm timeinfo; //로컬 시간 반영용 구조체 변수 선언

/* 개인적인 변수 선언 */
int sensor_CdS_1; //조도센서 1
int pin_PIR_1 = 8; //움직임 감지 센서 1의 핀 (inputpin)
int PIR_state = LOW; //움직임 감지 센서의 초기상태는 움직임이 없음을 가정 (pirstate)
int is_PIR_move = 0; //움직임 감지 센서 신호의 판별을 위한 변수 (val)

//움직임이 감지된 시간을 저장
int dd;
int MM;
int yy;
int ss;
int mm;
int hh;
int week;

void setup()
{
  pinMode(pin_PIR_1, INPUT); //움직임 감지 센서 INPUT 설정
    
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  mySerial.begin(9600);
  // initialize ESP module
  WiFi.init(&mySerial);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("[경고] WiFi 실드가 없습니다.");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("네트워크(WPA SSID)에 연결하고 있습니다... : ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("네트워크에 연결되었습니다!");
  printWifiStatus();

  //get_NTP(), set_time()
  configTime(3600 * timeZone, 3600 * summerTime, ntpServer); // NTP 서버 접속 및 동기화 함수
  while(!getLocalTime(&timeinfo)){ // 로컬 시간 반영함수이 리턴 값이 false이면, 시간값 없으면
      Serial.println("Failed to obtain time"); // 메시지 표시

      struct tm tm; // 사용자 시간 설정용 구조체 변수 선언
      tm.tm_year = s_yy - 1900;
      tm.tm_mon = s_MM - 1;
      tm.tm_mday = s_dd;
      tm.tm_hour = s_hh;
      tm.tm_min = s_mm;
      tm.tm_sec = s_ss;
      time_t t = mktime(&tm); // UTC 값으로 변환
      printf("Setting time: %s", asctime(&tm)); // 설정 시간을 문자열로 출력
      struct timeval noww = { .tv_sec = t }; // 설정시간을 현재 시간에 반영
      settimeofday(&noww, NULL);

      return;
  }
  
  // start the web server on port 80
  server.begin();

}

void loop()
{
  is_PIR_move = digitalRead(pin_PIR_1); //움직임 감지 센서의 신호값을 읽어와 is_PIR_move에 저장

  if(is_PIR_move == HIGH) { //움직임 감지 시
      if(PIR_state == LOW) {
          if (time(&noww) != prevEpoch) { // 현재 UTC 시간 값과 이전 UTC 시간 값이 다르면 
              Serial.println(time(&noww));  // 현재 UTC 시간 값 출력 
              getLocalTime(&timeinfo);     // 로컬 변경 함수 이용 UTC 시간 값 변경 
              dd = timeinfo.tm_mday;       // 구조체 내 해당 값 가져오기 
              MM = timeinfo.tm_mon + 1; 
              yy = timeinfo.tm_year + 1900; 
              ss = timeinfo.tm_sec; 
              mm = timeinfo.tm_min; 
              hh = timeinfo.tm_hour; 
              week = timeinfo.tm_wday;

              Serial.println("움직임 감지!");
              Serial.print(week); Serial.print(". "); 
              Serial.print(yy); Serial.print(". "); 
              Serial.print(MM); Serial.print(". "); 
              Serial.print(dd); Serial.print(" "); 
              Serial.print(hh); Serial.print(": "); 
              Serial.print(mm); Serial.print(": "); 
              Serial.println(ss);
    
              PIR_state = HIGH;
              prevEpoch = time(&noww); //현재 UTC 시간 값을 저장하여 1초마다 실행되도록 함
           }
       }
   }
   else {
        if(PIR_state == HIGH) {
            PIR_state == LOW;
      }
  }
  
  // listen for incoming clients
  WiFiEspClient client = server.available();
  if (client) {
    Serial.println("새로운 클라이언트:");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
            sensor_CdS_1 = analogRead(A0); //조도센서 1 값 읽기
            
          Serial.println("Sending response:");
          
          // send a standard http response header
          // use \r\n instead of many println statements to speedup data send
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
            "Refresh: 31536000\r\n"        // 자동 새로고침 시간 설정 (저는 1년으로 설정)
            "\r\n");
          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");
          
          client.print("<h1>Living Room : Server 0</h1>\r\n"); //제목(title) 출력
          
          client.print("<h2>CdS 1(A0): "); //조도센서 1 값 출력
          client.print(sensor_CdS_1);
          client.print("</h2>\r\n");

          client.print("<h2>PIR(D8) last check time: "); //움직임 감지 센서 1이 감지했을떄 시간 출력
          client.print(week); client.print(". "); 
          client.print(yy); client.print(". "); 
          client.print(MM); client.print(". "); 
          client.print(dd); client.print(" "); 
          client.print(hh); client.print(": "); 
          client.print(mm); client.print(": "); 
          client.print(ss);
          client.print("</h2>\r\n");
          
          client.print("<br>\r\n");
          
          client.print("<h4>Requests received: "); //새로고침 휫수? 값 출력
          client.print(++reqCount);
          client.print("</h4>\r");
          
          client.print("<br>\r\n");
          
          client.print("</html>\r\n");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
    Serial.println("클라이언트 연결 끊김");
    Serial.println("---------------------");
    Serial.println("");
    Serial.println("");
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print where to go in the browser
  Serial.println();
  Serial.print("이제 서버를 볼 수 있습니다. http://");
  Serial.println(ip);
  Serial.println();
}

/* 원본 코드
//ESP01_Web_Server_2 file
//Original Code : File - Examples - WiFiEsp - WebServer
//Remake : VDoring

#include <SoftwareSerial.h>
#include "WiFiEsp.h"

SoftwareSerial mySerial(2, 3); // RX, TX

char ssid[] = "U+Net340F"; // 와이파이 SSID
char pass[] = "018A015635"; // 와이파이 비밀번호
int status = WL_IDLE_STATUS; // the Wifi radio's status
int reqCount = 0; // number of requests received

WiFiEspServer server(80);


void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  mySerial.begin(9600);
  // initialize ESP module
  WiFi.init(&mySerial);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("[경고] WiFi 실드가 없습니다.");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("네트워크(WPA SSID)에 연결하고 있습니다... : ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("네트워크에 연결되었습니다!");
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
}

// 개인적인 변수 선언
int sensor_CdS_1; //조도센서 1

void loop()
{
  
  // listen for incoming clients
  WiFiEspClient client = server.available();
  if (client) {
    Serial.println("새로운 클라이언트:");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
            sensor_CdS_1 = analogRead(A0); //조도센서 1 값 읽기
            
          Serial.println("Sending response:");
          
          // send a standard http response header
          // use \r\n instead of many println statements to speedup data send
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
            "Refresh: 31536000\r\n"        // 자동 새로고침 시간 설정 (저는 1년으로 설정)
            "\r\n");
          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");
          
          client.print("<h1>My Room OutSee : Server 0</h1>\r\n"); //제목(title) 출력
          
          client.print("<h2>CdS 1(A0): "); //조도센서 1 값 출력
          client.print(sensor_CdS_1);
          client.print("</h2>\r\n");
          
          client.print("<br>\r\n");
          
          client.print("<h4>Requests received: "); //새로고침 휫수? 값 출력
          client.print(++reqCount);
          client.print("</h4>\r");
          
          client.print("<br>\r\n");
          
          client.print("</html>\r\n");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
    Serial.println("클라이언트 연결 끊김");
    Serial.println("---------------------");
    Serial.println("");
    Serial.println("");
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print where to go in the browser
  Serial.println();
  Serial.print("이제 서버를 볼 수 있습니다. http://");
  Serial.println(ip);
  Serial.println();
}
*/
