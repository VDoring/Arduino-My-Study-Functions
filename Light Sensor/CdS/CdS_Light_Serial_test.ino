void setup() {
    Serial.begin(9600);
}

void loop() {
    int CdS = analogRead(A0);
    Serial.println(CdS);
}
