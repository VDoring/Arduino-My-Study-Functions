//참고한 자료 : http://blog.naver.com/PostView.nhn?blogId=roboholic84&logNo=221187867841&categoryNo=7&parentCategoryNo=0&viewDate=&currentPage=1&postListTopCurrentPage=1&from=section
//user_interface.h?(단 현재 이 프로젝트는 이 해더파일을 넣지않음)
// https://github.com/esp8266/Arduino/blob/master/tools/sdk/include/user_interface.h
// https://m.blog.naver.com/PostView.nhn?blogId=roboholic84&logNo=220937218314&proxyReferer=https:%2F%2Fwww.google.com%2F


#include "user_interface.h"
#include <ESP8266WiFi.h>
//#include <arduino.h>

const char* ssid = "SSID"; // 네트워크 이름
const char* password = "PW"; // 네트워크 비밀번호

WiFiServer server(80);

// 변수 지정
//int LED_pin_R = 5; // D1
//int LED_pin_G = 4; // D2
//int LED_pin_B = 0; // D3
int sensor_CdS_1; //조도센서 1

// Anode
//int turn_on = 0;
//int turn_off = 1;

// Cathode
//int turn_on = 1;
//int turn_off = 0;

// deepsleep mode
//int dsmode_on = 1;
//int dsmode_off = 0;

void setup() {
  Serial.begin(9600);
  delay(10);

  wifi_set_sleep_type(NONE_SLEEP_T);
  
  //pinMode(LED_pin_R, OUTPUT);
  //pinMode(LED_pin_G, OUTPUT);
  //pinMode(LED_pin_B, OUTPUT);
  //digitalWrite(LED_pin_R, turn_off);
  //digitalWrite(LED_pin_G, turn_off);
  //digitalWrite(LED_pin_B, turn_off);

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  
  Serial.println("Connected!");


  // Start the server
  server.begin();
  Serial.println("Server started!");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

 

void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while(!client.available()) {
    delay(1);
  }
  Serial.println("New Client >>>");

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request
  //int deep_sleep = dsmode_off;
  //int value_R = turn_off;
  //int value_G = turn_off;
  //int value_B = turn_off;

  if (request.indexOf("/DeepSleep1Day=ON") != -1)  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    
    client.print("<h1>NodeMCU Living Room : Server Lv.1</h1>\r\n");          
    client.print("<h2>Deep Sleep Mode Now!");
    client.print("</h2>\r");
    client.print("<h2>1 Days(86400 sec)");
    client.print("</h2>\r");
      
    client.println("<meta http-equiv='Content-Type' content='text/html' charset='utf-8'/>");
    client.println("<br><br>");
    client.println("</html>");

    delay(5000);
    Serial.println("DeepSleep Mode ON : 1 Days(86400 sec)");
    delay(100);
    ESP.deepSleep(86400e6); //1일동안 Deep-Sleep 시행 후 리셋
  }
  if (request.indexOf("/DeepSleepReset=ON") != -1) {
    Serial.println("Reboot");
    delay(10);
    ESP.deepSleep(1e6);
  }
  /*
  if (request.indexOf("/LED_R=ON") != -1)  {
    digitalWrite(LED_pin_R, turn_on);
    value_R = turn_on;
  }
  if (request.indexOf("/LED_R=OFF") != -1)  {
    digitalWrite(LED_pin_R, turn_off);
    value_R = turn_off;
  }
  if (request.indexOf("/LED_G=ON") != -1)  {
    digitalWrite(LED_pin_G, turn_on);
    value_G = turn_on;
  }
  if (request.indexOf("/LED_G=OFF") != -1)  {
    digitalWrite(LED_pin_G, turn_off);
    value_G = turn_off;
  }
  if (request.indexOf("/LED_B=ON") != -1)  {
    digitalWrite(LED_pin_B, turn_on);
    value_B = turn_on;
  }
  if (request.indexOf("/LED_B=OFF") != -1)  {
    digitalWrite(LED_pin_B, turn_off);
    value_B = turn_off;
  }
  */

  sensor_CdS_1 = analogRead(A0); //조도센서 1 값 읽기
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("<h1>NodeMCU Living Room : Server Lv.1</h1>\r\n"); //제목(title) 출력
          
  client.print("<h2>CdS 1(A0): "); //조도센서 1 값 출력
  client.print(sensor_CdS_1);
  client.print("</h2>\r");
  
  client.println("<meta http-equiv='Content-Type' content='text/html' charset='utf-8'/>");
  client.println("<br><br>");

  client.println("<a href=\"/DeepSleepReset=ON\"\"><button> Reboot </button></a>");
  client.println("<a href=\"/DeepSleep1Day=ON\"\"><button> DeepSleep : 1 Days </button></a>");
  
  /*
  client.println("<a href=\"/LED_R=ON\"\"><button> 빨간색 LED ON </button></a>");
  client.println("<a href=\"/LED_R=OFF\"\"><button> 빨간색 LED OFF </button></a><br/>");
  client.println("<a href=\"/LED_G=ON\"\"><button> 녹색 LED ON </button></a>");
  client.println("<a href=\"/LED_G=OFF\"\"><button> 녹색 LED OFF </button></a><br/>");
  client.println("<a href=\"/LED_B=ON\"\"><button> 파랑색 LED ON </button></a>");
  client.println("<a href=\"/LED_B=OFF\"\"><button> 파랑색 LED OFF </button></a><br/>");
  */

  client.println("</html>");

  delay(1);
  Serial.println("Client Disonnected");
  Serial.println("---------------------");
}
