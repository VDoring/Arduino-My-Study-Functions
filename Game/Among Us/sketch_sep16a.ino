//부품
const int touch_sensor_left = 2; //왼쪽 터치센서
const int touch_sensor_right = 3; //오른쪽 터치센서
const int count_down_led = 13; //빨강 LED
const int count_down_disable_check_led = 12; //초록 LED

//게임 시스템
#define SABOTAGE_DISABLE_TIME 10
#define GAMEOVER_TIME 30
int sabotage_disable_time = SABOTAGE_DISABLE_TIME; //사보타지가 비활성화 되어있는 시간
int is_sabotage_warning; //사보타지 활성화 여부 확인
int hand_left; //왼쪽 (사보타지) 해제 센서
int hand_right; //오른쪽 (사보타지) 해제 센서
int gameover = GAMEOVER_TIME; //제한시간

void setup() {
  pinMode(touch_sensor_left, INPUT);
  pinMode(touch_sensor_right, INPUT);
  pinMode(count_down_led, OUTPUT);
  pinMode(count_down_disable_check_led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
    is_sabotage_warning = sabotageAutoTrigger();

    switch(is_sabotage_warning) {
        case 1: //사보타지 활성화 상태일시
            sabotageOn();
            Reset();
            break;
        case 0: //사보타지 비활성화 상태일시
            sabotageOff();
            break;
    }
}

int sabotageAutoTrigger() { //시간 경과에 따른 사보타지 활성화
    delay(1000); //시간 단위 설정 (기본값 : 1초)
    sabotage_disable_time--;
    if(sabotage_disable_time <= 0) //사보타지 활성화 조건
        return 1;
    return 0;
}

void sabotageOn() { //사보타지 켜짐 상태
    digitalWrite(count_down_disable_check_led, LOW);
    
    while((hand_left + hand_right) != 2) { //사보타지 해제할떄까지 빨강 LED 점멸
        if(gameover % 2 == 0)
            digitalWrite(count_down_led, HIGH);
        else
            digitalWrite(count_down_led, LOW);
        delay(1000); //시간 단위 설정 (기본값 : 1초)
        handTouchCkeck();
        sabotageTimeoutCheck();
    }
    digitalWrite(count_down_led, LOW);
    handTouchReset();
}

void handTouchCkeck() { //(사보타지) 해제 센서 터치여부 체크
    hand_left = digitalRead(touch_sensor_left);
    hand_right = digitalRead(touch_sensor_right);

    if(hand_left)
        Serial.println("왼쪽 해제 센서 인식됨");
    if(hand_right)
        Serial.println("오른쪽 해제 센서 인식됨");
}

void handTouchReset() { //(사보타지 해제 센서 입력 값 초기화
    hand_left = 0;
    hand_right = 0;
}

void sabotageTimeoutCheck() { //사보타지 켜진 이후 제한시간 초과여부 확인 및 텍스트 출력
    gameover--;
    Serial.print("원자로 멜트다운까지 : ");
    Serial.print(gameover);
    Serial.println(" 초 남음.");

    if(gameover <= 0) { //만약 제시간안에 사보타지 해제를 하지 못했을시
        while(1) { //무한반복
            digitalWrite(count_down_led, HIGH);
            digitalWrite(count_down_disable_check_led, HIGH); 
            delay(275);
            digitalWrite(count_down_led, LOW);
            digitalWrite(count_down_disable_check_led, LOW);
            delay(275);
            Serial.println("< Fatal System Error >");
        }
    }
}

void Reset() { //사보타지 해제시 초기화할 값들
    sabotage_disable_time = SABOTAGE_DISABLE_TIME;
    gameover = GAMEOVER_TIME;
}

void sabotageOff() { //사보타지 꺼짐 상태
    digitalWrite(count_down_disable_check_led, HIGH);
    Serial.print("정상 작동중. 다음 사보타지까지의 시간 : ");
    Serial.print(sabotage_disable_time);
    Serial.println("초 남음.");
}
