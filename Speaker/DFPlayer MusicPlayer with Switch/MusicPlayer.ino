/* Music Player with Switch - VDoring */

//Quest>
//자동재생 기능 구현하기(BUSY 핀 이용)

#include <SoftwareSerial.h> //DF Player
#include <DFPlayer_Mini_Mp3.h> //DF Player

SoftwareSerial mySerial(10,11); //DF Player RX, TX Setting

//Button List (input)
int button_prev = 13; //이전 곡
int button_play_pause = 12; //재생 / 일시정지
int button_next = 9; //다음 곡
int button_stop = 8; //정지
int button_volume_plus = 7; //볼륨 +
int button_volume_minus = 6; //볼륨 -
int button_autoplay_mode = 5; //자동 (다음 곡)재생 모드
int pin_busy = 4; //DF Player 'BUSY' 핀

int volume = 2; //볼륨 값 범위: 0 ~ 30
#define VOLUME_SET_COOLTIME 150 //볼륨 설정 후 다음 볼륨 설정까지의 대기시간
#define AUTOMODE_SET_COOLTIME 300 //자동재생모드 설정후 다음 자동재생모드 설정까지의 대기시간

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
    pinMode(button_stop, INPUT_PULLUP);
    pinMode(button_volume_plus, INPUT_PULLUP);
    pinMode(button_volume_minus, INPUT_PULLUP);
    pinMode(button_autoplay_mode, INPUT_PULLUP);
}


int all_music_number = 126; //총 음악 번호(총 음악 개수)
int now_music_number = 1; //현재 플레이중인 음악 번호
int is_mode_play = 0; //현재 재생 여부(기본값 0. 대기 상태)
int is_first_play = 0; //처음 재생 여부(기본값 0. 한번도 재생하지 않은 상태)
int is_now_music_number_reset = 0; //정지 버튼을 누른 상태에서 Prev, Next 버튼을 누를 경우 각 곡의 번호가 섞이는 문제가 있다.
                                   //이 변수는 정지 상태인지 확인하여 Prev, Next 버튼을 누를 경우 0001에서 Prev, Next하게 해준다.
int is_pause_log = 0; //일시정지를 한 기록이 있는지 저장
int is_stop_log = 0; //정지를 한 기록이 있는지 저장
int is_auto_sequential_mode = 0; //현재 자동재생모드인지에 관한 여부


//버튼 눌려짐 상태 확인용 변수
int is_prev;
int is_play_pause;
int is_next;
int is_stop;
int is_volume_plus;
int is_volume_minus;
int is_autoplay_mode;

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
    //https://pkr7098.tistory.com/39 DFPlayer 함수종류 정리 링크
    

    //버튼 눌러짐 상태 체크
    is_prev = digitalRead(button_prev);
    is_play_pause = digitalRead(button_play_pause);
    is_next = digitalRead(button_next);
    is_stop = digitalRead(button_stop);
    is_volume_plus = digitalRead(button_volume_plus);
    is_volume_minus = digitalRead(button_volume_minus);
    is_autoplay_mode = digitalRead(button_autoplay_mode);


    if(!is_autoplay_mode) { //자동 재생 버튼이 눌려졌을시
        if(is_auto_sequential_mode) { //현재 자동 재생모드일경우
            is_auto_sequential_mode = 0; //자동 재생모드 해제
            Serial.println("Auto Play Mode : Disable");
        }
        else {
            is_auto_sequential_mode = 1; //자동 재생모드로 설정
            Serial.println("Auto Play Mode : Enable");
        }
        delay(AUTOMODE_SET_COOLTIME); //여러번 모드를 전환하지 않기 위한 딜레이
    }

    if(!is_prev) { //이전곡 재생 버튼이 눌려졌을시
        is_mode_play = 1;
        is_stop_log = 0; //정지 상태가 아니게 됨으로써 정지상태 로그 초기화
        if(is_now_music_number_reset) { //만약 현재 정지버튼을 누른 후라면
            mp3_play(1); //곡 플레이리스트를 처음으로 초기화.(처음의 이전곡을 재생하기 위함)
            is_now_music_number_reset = 0; //기능 완수했으므로 변수 초기화
        }
        mp3_prev(); //이전 곡 재생
        Serial.print("Prev, ");
        Serial.print("Now Music Number : ");
        if(now_music_number == 1) { //제일 처음 곡일경우
            now_music_number = all_music_number;
            Serial.println(now_music_number);
        }
        else { //제일 처음 곡이 아닐경우
            now_music_number--;
            Serial.println(now_music_number);
        }
    }
    else if(!is_play_pause) { //일시정지/재생 버튼이 눌려졌을시
        is_stop_log = 0; //정지 상태가 아니게 됨으로써 정지상태 로그 초기화
        
        if(!is_first_play && !is_mode_play && !is_pause_log) { //처음 재생버튼을 눌렀고, 현재 곡 재생상태가 아니고, 일시정지 모드를 한번도 거치치 않았을시
            is_mode_play = 1; //재생 상태로 전환
            mp3_play(1); //1번 곡부터 재생
            Serial.println("First Play");
            is_first_play = 1; //재생 모드 전환
        }
        else if(!is_mode_play) { //현재 일시정지(또는 대기) 상태일떄
            is_mode_play = 1; //재생 상태로 전한
            is_pause_log = 0; //일시정지 기록 제거
            mp3_play(); //현재 곡 재생
            Serial.println("Play");
        }
        else { //현재 재생 상태일떄
            is_mode_play = 0; //일시정지 상태로 전환
            is_pause_log = 1; //일시정지 기록 생김
            mp3_pause(); //현재 곡 일시정지
            Serial.println("Pause");
        }
    }
    else if(!is_next) { //다음곡 재생 버튼이 눌려졌을시
        is_mode_play = 1;
        is_stop_log = 0; //정지 상태가 아니게 됨으로써 정지상태 로그 초기화
        if(is_now_music_number_reset) { //만약 현재 정지버튼을 누른 후라면
            mp3_play(1); //곡 플레이리스트를 처음으로 초기화.(처음의 이전곡을 재생하기 위함)
            is_now_music_number_reset = 0; //기능 완수했으므로 변수 초기화
        }
        mp3_next(); //다음 곡 재생
        Serial.print("Next, ");
        Serial.print("Now Music Number : ");
        if(now_music_number == all_music_number) { //제일 마지막 곡일 경우
            now_music_number = 1;
            Serial.println(now_music_number);
        }
        else { //제일 마지막 곡이 아닐 경우
            now_music_number++;
            Serial.println(now_music_number);
        }
    }
    else if(!is_stop) { //정지 버튼이 눌려졌을떄
        mp3_stop(); //곡 정지
        is_first_play = 0; //처음 곡 실행상태 아님
        is_mode_play = 0; //재생 상태 아님
        is_now_music_number_reset = 1; //곡이 섞이지 않게 하기 위해 초기화
        now_music_number = 1; //곡 번호 초기화
        is_pause_log = 0; //일시정지 기록 초기화
        is_stop_log = 1; //정지상태였던 기록 생김
        Serial.println("Stop");
        
    }
    else if(!is_volume_plus) { //볼륨+ 버튼이 눌려졌을시 
        if(volume <= 29) { //볼륨이 30보다 작을 떄
            volume++; //볼륨 값 높힘
            mp3_set_volume(volume); //볼륨 세팅
            Serial.print("Volume : ");
            Serial.println(volume);
            
            delay(VOLUME_SET_COOLTIME); //다음 볼륨 설정까지의 딜레이(짧은 시간에 볼륨이 확 변하지 않게 함) 
        }   
    }
    else if(!is_volume_minus) { //볼륨- 버튼이 눌려졌을시
        if(volume >= 1) { //볼륨이 0보다 클떄
            volume--; //볼륨 값 줄임
            mp3_set_volume(volume);
            Serial.print("Volume : ");
            Serial.println(volume);

            delay(VOLUME_SET_COOLTIME); //다음 볼륨 설정까지의 딜레이(짧은 시간에 볼륨이 확 변하지 않게 함) 
        }
    }

    while(!(is_prev && is_play_pause && is_next && is_stop && is_autoplay_mode)) { //버튼을 계속 누르고 있음으로써 곡 상태나 모드가 계속 바뀌는 것을 방지
        is_prev = digitalRead(button_prev);
        is_play_pause = digitalRead(button_play_pause);
        is_next = digitalRead(button_next);
        is_stop = digitalRead(button_stop);
        is_autoplay_mode = digitalRead(button_autoplay_mode);
    }

    //현재 작동 안됨
    if(is_auto_sequential_mode) { //자동 재생 모드일시
        if(digitalRead(pin_busy)) { //DF Player의 BUSY핀이 HIGH(1)일시.(DF Player 기준 곡이 정지된 상태일시)
                if(!is_mode_play && !is_pause_log && !is_stop_log) { //소프트웨어 기준 곡이 정지된 상태이고, 일시정지와 정지로 인한 곡 정지가 아닐 시
                    is_mode_play = 1;
                    if(is_now_music_number_reset) { //만약 현재 정지버튼을 누른 후라면
                        mp3_play(1); //곡 플레이리스트를 처음으로 초기화.(처음의 이전곡을 재생하기 위함)
                        is_now_music_number_reset = 0; //기능 완수했으므로 변수 초기화
                    }
                    mp3_next(); //다음 곡 재생
                    Serial.println("[Auto Mode]");
                    Serial.print("Next, ");
                    Serial.print("Now Music Number : ");
                    if(now_music_number == all_music_number) { //제일 마지막 곡일 경우
                        now_music_number = 1;
                        Serial.println(now_music_number);
                    }
                    else { //제일 마지막 곡이 아닐 경우
                        now_music_number++;
                        Serial.println(now_music_number);
                    }
                //delay(100); //한번에 여러 명령이 동작되지 않게 하기 위한 딜레이(임시)
            }
        }
    }
}
