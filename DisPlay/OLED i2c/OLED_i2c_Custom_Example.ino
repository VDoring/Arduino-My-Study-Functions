//OLED i2c Display Module Test Code for Arduino Project
//Original : https://samstory.coolschool.co.kr/zone/story/codingarray/streams/45621
//Remake : VDoring

//핀 꼽는방법)
//아두이노 5V, GND = OLED VCC, GND 연결
//아두이노 A4, A5 = OLED SDA, SCL 연결

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED 가로 길이
#define SCREEN_HEIGHT 32 // OLED 세로 길이

#define OLED_RESET -1 // 리셋핀 #(또는 -1 아두이노와 리셋핀을 연결하는 경우)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = 내부 3.3V 차지 펌프 회로를 켜둔다.
    // OLED 주소 0x3C 지정
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 Not Connected"));
        while(1) {} // SSD1306에 주소할당이 되지 않으면 무한루프
}


// 버퍼 지우기(화면 초기화)
display.clearDisplay();

 
// 텍스트 출력
display.setTextSize(1); //글꼴 크기 설정
display.setTextColor(WHITE); //글꼴 색상 설정
display.setCursor(0,0); //커서 위치 설정. (X,Y)이다.
display.println("Hello World!"); //문자 출력
display.display(); //SSD1306 컨트롤러의 내부 메모리에 화면에 ATmega328P 버퍼로부터 벌크 전송을 수행하는 함수
                   //(라이브러리가 화면 버퍼 (초당 100 프레임 이상)에서 매우 빠른 연산을 수행하기 위해
                   //인쇄 기능을 호출해도 화면 버퍼의 내용이 SSD1306 컨트롤러로 즉시 전송되지 않는다.)
delay(2000); //화면 유지
display.clearDisplay(); //화면 초기화

 
// 텍스트 반전 출력
display.setTextColor(BLACK, WHITE); // 반전된 텍스트를 출력하기 위해 두개의 매개 변수 전달. (이 코드의 결과 = 흰색 배경에 검정 텍스트가 나타난다.)
                                    //(함수 오버로딩으로 인해 가능하다. 함수 오버로딩은 동일한 이름이지만 매개 변수 집합이 다른 여러 함수를 만드는 기능이다.)
                                    //(오버로드 된 함수를 호출하면 전달 된 매개 변수에 따라 해당 함수의 특정 기능 구현이 실행된다.
display.setCursor(0,0);
display.println("Reverse Hello World!");
display.display();
delay(2000);
display.clearDisplay();

 
// 글꼴 크기 조정
display.setTextColor(WHITE); 
display.setCursor(0,0);
display.setTextSize(3); //글꼴 크기 설정
                        //문자는 7:10의 비율로 렌더링된다. setTextSize(2)를 한다면 문자당 14x20픽셀의 텍스트가 렌더링된다.
                        //(여담으로 Adafruit_GFX 라이브러리는 글꼴 렌더링을 담당한다. 기본적으로 모노 간격 글꼴이 제공되나, 최신 버전의 Adafruit_GFX 라이브러리는 대체 글꼴을 사용할 수 있는 기능을 제공한다.
                        //여러 대체 글꼴이 라이브러리와 함께 제공되며, 새 글꼴을 추가 할 수도 있다.)
display.println("BIGGER");
display.display();
delay(2000);
display.clearDisplay();

 
// 숫자 표시
display.setTextSize(1);
display.setCursor(0,0);
display.println(123456789); //숫자 출력. 
display.display();
delay(2000);
display.clearDisplay();

 
// 숫자에 대한 포멧 지정.
// 최종 출력 : 0xFF(HEX) = 255(DEC)
//BIN(2진수), OCT(8진수), DEC(10진수), HEX((16진수) 사용 가능.
display.setCursor(0,0);
display.print("0x"); 
display.print(0xFF, HEX);
display.print("(HEX) = ");
display.print(0xFF, DEC);
display.println("(DEC)");
display.display();
delay(2000);
display.clearDisplay();

 
// ASCII 기호표시
display.setCursor(0,0);
display.setTextSize(3);
display.write(3); // 하트(3)모양 기호 출력
                  //(ASCII 기호 표시 가능)
display.display();
delay(2000);
display.clearDisplay();

 
// 전체화면 스크롤링
//스크롤 관련 함수(stopscroll() 제외)들은 두 개의 매개 변수 즉, 시작 페이지 및 중지 페이지. 0 에서 7 까지 디스플레이에 총 8 페이지가 있기 때문에
//모든 페이지를 스크롤(예 : 매개 변수 0x00 및 0x07 전달)하여 전체 화면을 스크롤 할 수 있다.
display.setCursor(0,0);
display.setTextSize(1);
display.println("Full");
display.println("Screen");
display.println("Scrolling!");
display.display();
display.startscrollright(0x00, 0x07); //오른쪽으로 스크롤
delay(2000);
display.stopscroll(); //스크롤 멈춤
delay(1000);
display.startscrollleft(0x00, 0x07); //왼쪽으로 스크롤
delay(2000);
display.stopscroll(); //스크롤 멈춤
delay(1000);
display.startscrolldiagright(0x00, 0x07); //대각선 오른쪽으로 전체 스크롤
delay(2000);
display.startscrolldiagleft(0x00, 0x07); //대각선 왼쪽으로 전체 스크롤
delay(2000);
display.stopscroll(); //스크롤 멈춤
display.clearDisplay();

 
// 특정부분 스크롤링
display.setCursor(0,0);
display.setTextSize(1);
display.println("Scroll");
display.println("Some part");
display.println("Of the screen.");
display.display();
display.startscrollright(0x00, 0x00); //(0x00, 0x00)은 "Scroll"만 스크롤되고,
                                      //(0x00, 0x01)하면 "Scroll"과 "Some part"가 동시에 스크롤된다.
}
 
void loop() {}
