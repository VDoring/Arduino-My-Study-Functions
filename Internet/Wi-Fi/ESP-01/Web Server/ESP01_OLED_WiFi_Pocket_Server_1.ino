// WiFi
#include <SoftwareSerial.h>
#include "WiFiEsp.h"

// OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 32, &Wire, -1); //OLED 가로 길이, OLED 세로 길이, ?, 리셋핀 #(또는 -1 아두이노와 리셋핀을 연결하는 경우)

SoftwareSerial mySerial(2, 3); // RX, TX

char ssid[] = "U+Net340F"; // 와이파이 SSID
char pass[] = "018A015635"; // 와이파이 비밀번호
int status = WL_IDLE_STATUS; // the Wifi radio's status
int reqCount = 0; // number of requests received

float ver = 0.1;

WiFiEspServer server(80);

void setup()
{    
  //OLED 파트
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 Not Connected"));
    while(1) {} // SSD1306에 주소할당이 되지 않으면 무한루프
  }
  display.clearDisplay();
  OLEDBeforePrint(1);

  //ESP8266 파트
  Serial.begin(9600); // initialize serial for debugging
  mySerial.begin(9600); // initialize serial for ESP module
  WiFi.init(&mySerial); // initialize ESP module

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println(F("WiFi 실드 없음"));
    while (true); // don't continue
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print(F("네트워크 연결중.. : "));
    Serial.println(ssid);
    OLEDAfterPrint("Connecting..");
    status = WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network
  }

  Serial.println(F("네트워크 연결완료!"));
  OLEDAfterPrint("Connected!");
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
}

/* 개인적인 변수 선언 */
//
//
//

void loop()
{
  // listen for incoming clients
  WiFiEspClient client = server.available();
  if (client) {
    display.clearDisplay();
    OLEDBeforePrint(1);
    Serial.println(F("새로운 클라이언트:"));
    OLEDAfterPrint("New Client>");
    
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
          Serial.println(F("Sending response:"));
          
          // send a standard http response header
          // use \r\n instead of many println statements to speedup data send
          client.print(F(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
            "Refresh: 31536000\r\n"        // 자동 새로고침 시간 설정 (현재 1년으로 설정)
            "\r\n"));
          client.print(F("<!DOCTYPE HTML>\r\n"));
          client.print(F("<html>\r\n"));
          
          client.print(F("<h1>HDR M-Server 0</h1>\r")); //제목(title) 출력
          client.print("<h1>Welcome to HDR Server!</h1>\r\n");
          
          client.print(F("<h4>Requests received: ")); //새로고침 휫수? 값 출력
          client.print(++reqCount);
          client.print(F("</h4>\r"));
          
          client.print(F("<br>\r\n"));
          
          client.print(F("<h5>Version:")); //새로고침 휫수? 값 출력
          client.print(ver);
          client.print(F("</h5>\r"));
          
          client.print(F("</html>\r\n"));
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true; //you're starting a new line
        }
        else if (c != '\r') {
          currentLineIsBlank = false; //you've gotten a character on the current line
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
    
    display.print("ReqCount:");
    display.println(reqCount-1);
    display.display();
    
    Serial.println(F("클라이언트 연결 끊김"));
    Serial.println(F("---------------------\n\n"));
    OLEDAfterPrint("Disconnected");
    OLEDAfterPrint("------------------");
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  display.print("SSID:");
  OLEDAfterPrint(WiFi.SSID());


  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP: ");
  Serial.println(ip);
  display.print("IP:");
  display.println(ip);
  display.display();

  
  // print where to go in the browser
  Serial.println();
  Serial.print(F("이제 서버를 볼 수 있습니다. http://"));
  Serial.println(ip);
  Serial.println();
}

//OLED 일반 출력
void OLEDBeforePrint(char text_size) { //글꼴 크기, 커서 위치(x,y), 문자 출력
    display.setTextSize(text_size); //글꼴 크기 설정 (기본값 : 1)
    display.setTextColor(WHITE); //글꼴 색상 설정 (기본값 : WHITE)
    display.setCursor(0,0); //커서 위치 설정. (기본값 : 0,0)
}

void OLEDAfterPrint(const char* text) {
    display.println(text); //문자 출력
    display.display();  //SSD1306 컨트롤러의 내부 메모리에 화면에 ATmega328P 버퍼로부터 벌크 전송을 수행하는 함수
                        //(라이브러리가 화면 버퍼 (초당 100 프레임 이상)에서 매우 빠른 연산을 수행하기 위해
                        //인쇄 기능을 호출해도 화면 버퍼의 내용이 SSD1306 컨트롤러로 즉시 전송되지 않는다.)
    delay(50); //화면 유지
}
