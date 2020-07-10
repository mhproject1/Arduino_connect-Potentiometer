
//////////////////// loop문의 알고리즘을 설명합니다. ////////////////////
// 마지막 글자가 \n이라는 뜻은 문장 끝에 줄바꿈 문자가 있음(엔터)을 의미하므로,
// test 시리얼 모니터에서 읽어온 글자가 '\n' 문자일 때까지
// string 변수인 RMsg에 저장합니다. (else문)
// 
// 마지막 문자가 '\n'문자이면 마지막 문자를 저장하고 
// 문자열의 끝을 나타내는 \0을 추가로 저장합니다.
// 저장된 문자열을 strncpy 함수를 사용해서 1차원 char배열 변수인 Msg에 저장합니다.
// 문자가 저장된 Msg에서 출력할 글자가 몇 글자인지 알기 위해서 \n의 인덱스를 찾습니다.    
// 예를 들어, 값이 6.10이라고 하면 Msg의 인덱스는 5입니다. >>> 6, ., 1, 0, \n
// 이대로 lcd에 Msg를 출력하면 \n문자까지 출력되기 때문에 \n를 제외하기 위해서 
// 인덱스-1 만큼만 Msg를 출력합니다.
// 
// 그런데, 시리얼 통신을 통해 얻는 test.read()의 값이 
// 26.86 > 108.68 > 3.17 > 32.11 > 471.91 > ... 라고 하고
// 이 값들을 lcd를 초기화 하지 않은 채로 lcd에 출력하면
// 26.86 > 108.68 > 3.1768 > 32.118 > 471.91 > ... 와 같이 출력됩니다.    
// lcd.clear()를 사용하면 전체 화면이 반복적으로 지워지기 때문에,
// 아래 코드를 적용해서 일부 화면만 지워지게 했습니다.
// 
// 현재 프로그램에서는 1번째 행에 dust:xx.xx 와 같이 미세먼지 수치를 출력하며,
// 출력되는 미세먼지의 값은 소수점 두번째 자리까지만 출력됩니다.
// 즉, setCursor(1,0)부터 dust:가 출력되고 미세먼지 수치를 출력할 때
// "."의 위치로부터 2칸 뒤의 lcd화면을 공백을 사용해서 초기화해줍니다.
// 이를 적용하면 다음과 같습니다.
// 26.86 > 108.68 > 3.17   > 32.11  > 471.91 > ... 와 같이 출력됩니다.
//
// lcd 화면을 출력하면 string 변수인 RMsg를 초기화해줍니다.
// 0,0 위치에 현재시간을 갱신하고 반복문을 종료합니다.
//////////////////////////////////////////////////////////////////////   


//////////////// include //////////////////
// 시리얼 통신 >>> 10번 11번 핀 사용합니다. //
#include <SoftwareSerial.h>
SoftwareSerial Team13(10,11);

// RTC 모듈 //

#include <DS1302.h>
DS1302 rtc(7,8,13);

// I2C_lcd //
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
//////////////////////////////////////////


// 전역 변수 //
String RMsg = ""; char Msg[10] = {0}; int acount = 0;  

void setup() {  
  // 초기 상태 //
  lcd.begin();  Serial.begin(9600);  Team13.begin(9600); 
  Serial.println("lnitialized");  
}

void loop() {
 // test와 시리얼 통신을 사용합니다. //
 Team13.listen();
 
 // Team13 시리얼 모니터에 뭔가 입력되면 아래 반복문을 수행합니다. //
 while(Team13.available())  
 {  // ch에 Team13 시리얼 모니터의 값을 저장합니다. //
    char ch = Team13.read();    
    
    // if ~ else 문... 위에서 자세히 설명 //
    if(ch == '\n')
    { RMsg += ch;  RMsg += '\0';
      strncpy(Msg, RMsg.c_str(), RMsg.length());                      

      // 인덱스 찾기 //
      for(int a = 0; a < RMsg.length(); a++) 
      { if(Msg[a] == '\n') { acount = a;   break; } }                                        

      // lcd 화면 초기화 //
      for(int a = 0; a<acount-1; a++)              
      {  if(Msg[a] == '.') 
         { if(a == 1) { lcd.setCursor(8,1); lcd.print("        "); }
           if(a == 2) { lcd.setCursor(9,1); lcd.print("        "); } } }
      
      // 미세먼지의 값 출력 //
      lcd.setCursor(0,1); lcd.print("dust:");           
      for(int a = 0; a<acount-1; a++)
      { lcd.print(Msg[a]); }

      // 문자열 초기화 //
      RMsg = ""; }          
    else
    { RMsg += ch; } }          

    // lcd 화면 0,0 위치에 시간 표시 //
    lcd.setCursor(0,0); lcd.print(rtc.getTimeStr());
    delay(100);  
}
