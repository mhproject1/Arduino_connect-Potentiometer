//***************프로그램 설명******************//
// 20154215 구명회
// 디지털 3번 핀에 피에조 스피커를 연결하고,
// 디지털 5,6,7번핀에 버튼을 풀다운으로 연결합니다.
// 버튼을 누르면 각 버튼 별로 도, 레, 미 음을 출력합니다.
//**************프로그램 설명 끝****************//

// 음계를 저장해놓은 헤더
#include <pitches.h>
// 음 길이 10을 매크로상수 DURATION으로 정의합니다.
#define DURATION 10

// buttons[]는 버튼이 각각 5,6,7번 핀에 연결되어 있으며,
// 이 핀들을 입력으로 사용합니다.
// pie는 피에조 스피커가 디지털 3번핀에 연결되어 있으며,
// 이 핀을 출력으로 사용합니다.
int buttons[] = {5,6,7};
int pie = 3;

int notes[] = {  // 도레미 음계
  NOTE_C4, NOTE_D4, NOTE_E4 };  

void setup() {
  pinMode(pie, OUTPUT);
  for(int i=0; i<3; i++)
    pinMode(buttons[i], INPUT);  
}

void loop() {
  // 5~7번 버튼이 눌렸는지의 상태를 button에 저장
  // 버튼이 눌렸으면 button값은 1
  // 눌리지 않았으면 button값은 0
  // 버튼이 눌리면 피에조 스피커가 그 버튼에 맞는 음계를 출력
  // ex) buttons[0] ~> 5번 버튼이 눌림
  // tone(pie, notes[0], DURATION) ~> NOTE_C4 즉 '도'음 출력
  for(int i = 0; i<3; i++){
    boolean button = digitalRead(buttons[i]);

    if(button)
      tone(pie, notes[i], DURATION);
  }
}
