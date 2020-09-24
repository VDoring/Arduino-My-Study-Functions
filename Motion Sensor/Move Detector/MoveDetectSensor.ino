/* Move Detect (Motion)Sensor */
/* Check status using 2 LEDs. */

int ledPin1 = 9;      // 1번 LED 
int ledPin2 = 8;      // 2번 LED 
int inputPin = 7;     // 센서 신호핀
int pirState = LOW;   // 센서 초기상태는 움직임이 없음을 가정
int val = 0;          // 센서 신호의 판별을 위한 변수
 
void setup(){
    pinMode(ledPin1, OUTPUT);    // 1번 LED를 출력으로 설정
    pinMode(ledPin2, OUTPUT);    // 2번 LED를 출력으로 설정
    pinMode(inputPin, INPUT);    // 센서 Input 설정
    Serial.begin(9600);         // 시리얼 통신, 속도는 9600
}
 
void loop(){
  val = digitalRead(inputPin);         // 센서 신호값을 읽어와서 val에 저장
    
  if (val == HIGH) {                   // 센서 신호값이 HIGH면(인체 감지가 되면)    
    digitalWrite(ledPin1, HIGH);       // 1번 LED ON
    digitalWrite(ledPin2, LOW);        // 2번 LED OFF 
    if (pirState == LOW){                         
         Serial.println("주변 물체 감지됨!");    // 시리얼 모니터 출력
         pirState = HIGH;
    } 
   } 
   else {                             // 센서 신호값이 LOW면(인체감지가 없으면)
    digitalWrite(ledPin1, LOW);       // 1번 LED OFF
    digitalWrite(ledPin2, HIGH);      // 2번 LED ON
    if (pirState == HIGH){                
        Serial.println("감시 중..");   // 시리얼 모니터 출력
        pirState = LOW;
    }
  }
}
