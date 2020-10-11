//Original Code : File - Examples - WiFiEsp - WebServer
//Remake : VDoring


#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3); // RX, TX
#endif

char ssid[] = "WiFi Name"; // 와이파이 SSID
char pass[] = "WiFi Password"; // 와이파이 비밀번호
int status = WL_IDLE_STATUS; // the Wifi radio's status
int reqCount = 0; // number of requests received

WiFiEspServer server(80);


void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

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

/* 개인적인 변수 선언 */
int sensor_CdS_1; //조도센서 1
int sensor_CdS_2; //조도센서 2

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
            sensor_CdS_1 = analogRead(A0); //조도센서 1
            sensor_CdS_2 = analogRead(A1); //조도센서 2
            
          Serial.println("Sending response:");
          
          // send a standard http response header
          // use \r\n instead of many println statements to speedup data send
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
            "Refresh: 31536000\r\n"        // 자동 새로고침 시간 설정 (저는 1년으로 설정함)
            "\r\n");
          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");
          
          client.print("<h1>Room : Server 1</h1>\r\n"); //제목(title) 출력
          
          client.print("<h2>CdS 1(A0): "); //조도센서 1 값 출력
          client.print(sensor_CdS_1);
          client.print("</h2>\r\n");
          
          client.print("<h2>CdS 2(A1): "); //조도센서 2 값 출력
          client.print(sensor_CdS_2);
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
