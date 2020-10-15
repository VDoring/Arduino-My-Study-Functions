#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

const char* ssid = "SSID"; //SSID
const char* password = "PW"; //PW (PW가 없다면 공란으로 두기)
WiFiServer server(80);

void setup() {
    Serial.begin(9600);
    delay(100);
    
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.print("...");
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED) {} //와이파이 연결이 정상적일시 while 탈출
    
    Serial.println();
    Serial.println("Connected!");

    server.begin();
    Serial.println("Server ON");
    Serial.print("IP:");
    Serial.print(WiFi.localIP());
}

void loop() {

}
