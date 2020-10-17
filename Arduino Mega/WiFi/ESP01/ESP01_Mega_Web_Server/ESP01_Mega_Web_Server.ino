//Original Code : File - Examples - WiFiEsp - WebServer
//Remake : VDoring

//WiFi
#include "WiFiEsp.h"

//OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


char ssid[] = "U+Net340F"; // 와이파이 SSID
char pass[] = "018A015635"; // 와이파이 비밀번호
int status = WL_IDLE_STATUS; // the Wifi radio's status
int reqCount = 0; // number of requests received

Adafruit_SSD1306 display(128, 32, &Wire, -1); //OLED 가로 길이, OLED 세로 길이, ?, 리셋핀 #(또는 -1 아두이노와 리셋핀을 연결하는 경우)

WiFiEspServer server(80);

void setup()
{
  //OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306(OLED) Not Connected"));
    while(1) {} // SSD1306에 주소할당이 되지 않으면 무한루프
  }
  display.clearDisplay(); //OLED 화면 클리어
  OLEDNormalSet(); //OLED 기본 셋팅
    
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("[경고] WiFi 실드가 없습니다.");
    display.println("[No WiFi Shield]"); //OLED 출력
    display.display(); //OLED 화면 출력
    // don't continue
    while (true);
  }
  display.clearDisplay();
  OLEDNormalSet();

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("네트워크(WPA SSID)에 연결하고 있습니다... : ");
    Serial.println(ssid);
    display.println("Connecting...");
    display.display();
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("네트워크에 연결되었습니다!");
  display.println("Connected!");
  display.display();
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
}

/* 개인적인 변수 선언 */

void loop()
{ 
  // listen for incoming clients
  WiFiEspClient client = server.available();
  if (client) {
    display.clearDisplay();
    OLEDNormalSet();
    
    Serial.println("새로운 클라이언트:");
    display.println("New Client >");
    display.display();
    
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
          
          client.print("<h1>Arduino Mega : Server 1</h1>\r\n"); //제목(title) 출력
          
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
    Serial.println("---------------------\n\n");
    display.print("ReqCount: ");
    display.println(reqCount-1);
    display.println("Client Disconnect.");
    display.print("---------------------");
    display.display();
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  display.print("SSID: ");
  display.println(WiFi.SSID());
  display.display();

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  display.print("IP: ");
  display.println(ip);
  display.display();
  
  // print where to go in the browser
  Serial.println();
  Serial.print("이제 서버를 볼 수 있습니다. http://");
  Serial.println(ip);
  Serial.println();
}

//OLED 기본 셋팅
void OLEDNormalSet() {
    display.setTextSize(1); //글꼴 크기 설정 (기본값 : 1)
    display.setTextColor(WHITE); //글꼴 색상 설정 (기본값 : WHITE)
    display.setCursor(0,0); //커서 위치 설정. (기본값 : 0,0)
}
