/* Music Player with Switch - VDoring */

//Quest>
//STOP 기능 구현하기
//자동재생 기능 구현하기(BUSY 핀 이용)

#include <SoftwareSerial.h> //DF Player
#include <DFPlayer_Mini_Mp3.h> //DF Player

SoftwareSerial mySerial(10,11); //DF Player RX, TX Setting

//Button List (input)
int button_prev = 13; //이전 곡
int button_play_pause = 12; //재생 / 일시정지
int button_next = 9; //다음 곡
//int button_stop = ?; //정지
int button_volume_plus = 8; //볼륨 +
int button_volume_minus = 7; //볼륨 -

int volume = 2; //볼륨 값 범위: 0 ~ 30
#define VOLUME_SET_COOLTIME 150

void setup() {
    //DF Player SetUp
    Serial.begin(9600);
    mySerial.begin(9600);
    mp3_set_serial(mySerial);
    delay(1); // 볼륨값 적용을 위한 딜레이
    mp3_set_volume(volume);

    //Button pinMode SetUp
    pinMode(button_prev, INPUT_PULLUP);
    pinMode(button_play_pause, INPUT_PULLUP);
    pinMode(button_next, INPUT_PULLUP);
    pinMode(button_volume_plus, INPUT_PULLUP);
    pinMode(button_volume_minus, INPUT_PULLUP);
}

int all_music_number = 126; //총 음악 번호(총 음악 개수)
int now_music_number = 1; //현재 플레이중인 음악 번호
int mode_play = 0; //현재 재생 여부(기본값 0. 대기 상태)
int is_first_play = 0; //처음 재생 여부(기본값 0. 한번도 재생하지 않은 상태)

void loop() {
    /*
    //버튼 작동 테스트 코드
    if(digitalRead(button_prev) == LOW)
        Serial.println("button_prev 작동중");
    if(digitalRead(button_play_pause) == LOW)
        Serial.println("button_play_pause 작동중");
    if(digitalRead(button_next) == LOW)
        Serial.println("button_next 작동중");
    if(digitalRead(button_volume_plus) == LOW)
        Serial.println("button_volume_plus 작동중");
    if(digitalRead(button_volume_minus) == LOW)
        Serial.println("button_volume_minus 작동중");
    */
    
    //https://pkr7098.tistory.com/39 DF Plaer 함수종류
    

    //버튼 눌러짐 상태 체크
    int is_prev = digitalRead(button_prev);
    int is_play_pause = digitalRead(button_play_pause);
    int is_next = digitalRead(button_next);
    int is_volume_plus = digitalRead(button_volume_plus);
    int is_volume_minus = digitalRead(button_volume_minus);

    if(!is_prev) { //이전곡 재생 버튼이 눌려졌을시
        mp3_prev();
        Serial.print("Prev, ");
        Serial.print("Now Music Number : ");
        if(now_music_number == 1) { //제일 처음 곡일경우
            now_music_number = all_music_number;
            Serial.println(now_music_number);
        }
        else {
            now_music_number--;
            Serial.println(now_music_number);
        }
    }
    else if(!is_play_pause) { //일시정지/재생 버튼이 눌려졌을시
        if(!is_first_play) { //처음 재생시
            mode_play = 1;
            mp3_play(1);
            Serial.println("First Play");
            is_first_play++;
        }
        else if(!mode_play) { //현재 일시정지(또는 대기) 상태일떄
            mode_play = 1;
            mp3_play();
            Serial.println("Play");
        }
        else { //현재 재생 상태일떄
            mode_play = 0;
            mp3_pause();
            Serial.println("Pause");
        }
    }
    else if(!is_next) { //다음곡 재생 버튼이 눌려졌을시
        mp3_next();
        Serial.print("Next, ");
        Serial.print("Now Music Number : ");
        if(now_music_number == all_music_number) { //제일 마지막 곡일 경우
            now_music_number = 1;
            Serial.println(now_music_number);
        }
        else {
            now_music_number++;
            Serial.println(now_music_number);
        }
    }
    else if(!is_volume_plus) { //볼륨+ 버튼이 눌려졌을시 
        if(volume <= 29) {
            volume++;
            mp3_set_volume(volume);
            Serial.print("Volume : ");
            Serial.println(volume);
            
            delay(VOLUME_SET_COOLTIME); //다음 볼륨 설정까지의 딜레이(짧은 시간에 볼륨이 확 변하지 않게 함) 
        }   
    }
    else if(!is_volume_minus) { //볼륨- 버튼이 눌려졌을시
        if(volume >= 1) {
            volume--;
            mp3_set_volume(volume);
            Serial.print("Volume : ");
            Serial.println(volume);

            delay(VOLUME_SET_COOLTIME); //다음 볼륨 설정까지의 딜레이(짧은 시간에 볼륨이 확 변하지 않게 함) 
        }
    }

    while(!(is_prev && is_play_pause && is_next)) { //버튼을 계속 누르고 있음으로써 곡 상태가 계속 바뀌는 것을 방지
        is_prev = digitalRead(button_prev);
        is_play_pause = digitalRead(button_play_pause);
        is_next = digitalRead(button_next);
    }
}
