#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_AUTH_TOKEN "-AgvxbeXcZR9DVYrxHy7sbKePEouooco"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WIFI NAME";
char pw[] = "WIFI PASSWORD";


BLYNK_WRITE(V0) {
  digitalWrite(D0, param.asInt());
}


void setup() {
  // put your setup code here, to run once:
  pinMode(D0, OUTPUT);
  Blynk.begin(auth, ssid, pw, "blynk.cloud", 80);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
}
