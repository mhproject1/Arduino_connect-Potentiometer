
////////////////////// 미세먼지 센서를 설명합니다. ////////////////////
// 미세먼지 센서는 적외선 발광 다이오드와 포토 다이오드가 대각선으로 배치되어 있습니다.
// 이렇게 배치된 센서들은 공기 중에 먼지의 반사광을 감지해서 값을 출력합니다.
//
// 미세먼지 센서의 작동 원리는 다음과 같습니다.
// 1. 적외선 led가 켜진 상태에서 0.28ms를 기다립니다.
// 2. analogRead를 사용해서 적외선 수신기의 값을 읽어오는데, 이 때 읽어온 값이 미세먼지의 값입니다.
// 3. 0.4ms를 기다린 후 적외선 led를 끕니다.
// 4. 0.968ms뒤에 이 과정을 반복합니다. 
// 이 led를 켜고 끄는 과정을 펄스라고 합니다.
// 
// 적외선 수신기에서 읽어낸 미세먼지의 값은 전압을 0~1023으로 표현한 값입니다. (analog에서 읽은 값)
// 이 값을 이용해서 원래의 전압을 구하기 위해 V = 값 x 5.0 / 1024.0를 해줘야 합니다.
//
// 위의 과정을 통해 얻어낸 전압값으로 진짜 미세먼지 센서의 값을 구하기 위해 
// Sensor_V = (전압-(0.44))/0.005를 해줘야 합니다.
//
// 0.005를 나눈 이유는 미세먼지 센서의 단위가 ug를 사용하기 때문입니다.
// 주의할 점은 원래의 전압값에서 뺀 전압값... 즉 0.44부분입니다. 
// 미세먼지 센서에서 미세먼지가 있는지 없는지 측정을 하기 위해서 어떠한 기준을 잡아야 하는데,
// 이 어떠한 기준을 잡기 위해서는 먼지가 없을 때의 전압값을 찾아야 합니다.
// 전압값이 중요한 이유는 먼지가 없을 때의 초기값에 따라 센서의 값이 크게 변하기 때문입니다.
// 센서 기재사항에 따르면, 먼지가 없을 때의 전압값은 최소값이 0.1 이고 최대값이 1.1이며,
// 보통 0.6라고 표기되어 있지만 실제로 0.6보다 적게 나오는 경우가 많아서 저는 0.44를 사용했습니다.
//
// 하지만 아무리 위의 과정을 통해서 최적화된 전압값을 업로드한 미세먼지 센서로 미세먼지를 측정한다고 해도
// 미세먼지의 움직임이 크게 불규칙하기 때문에 값이 튈 수 밖에 없어 센서의 값이 신뢰도가 떨어집니다.
// 이 현상은 구간 평균을 사용하면 개선할 수 있으며, 저는 50번의 구간을 나눠서 평균을 냈습니다.
///////////////////////////////////////////

///////////////// include /////////////////
// 블루투스 통신 >>> 2번, 3번 핀 사용합니다.
// 시리얼 통신 >>> 10번, 11번 핀 사용합니다.
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2,3); 
SoftwareSerial Team13(10,11);
///////////////////////////////////////////

//////////////// 전역 변수 /////////////////
// 미세먼지 적외선 수신기와 미세먼지 적외선 led핀을 각각 A0, 4로 설정합니다.
int measurePin = A0;   int ledPower = 4;   

// 미세먼지 센서의 펄스값을 저장할 변수입니다.
int samplingTime = 280;  int deltaTime = 40;  int sleepTime = 9680; 

// 미세먼지의 값, 미세먼지의 전압값, 최적화된 미세먼지의 값을 저장할 변수입니다.
float voMeasured = 0;  float calcVoltage = 0;  float dustDensity = 0;  

// 구간 평균을 위한 변수입니다.
float dust_sum = 0;  float dust_average = 0; int dust_average_Number = 0;

// state를 false로 설정합니다.
int state = false;

// 3색 LED 핀을 설정합니다. 7 : green, 8 : blue, 13 : red
int color[] ={7, 8, 13};
///////////////////////////////////////////

void RDC ( ) 
{ // RGB가 각각 켜고꺼질지 무작위로 선택 //
  // 켜진경우 2-1 = 1 = HIGH 꺼진경우 1-1 = 0 = LOW //
  int R = random(2); int G = random(2); int B = random(2); 
  digitalWrite(color[0], G-1); digitalWrite(color[1], B-1); digitalWrite(color[2], R-1); 
}

void LEDreset(){  
  delay(300);
  for (int RGB_three = 0; RGB_three < 3; RGB_three++) 
  { digitalWrite(color[RGB_three], 0); } 
}

void setup() {
  // 초기 상태 //  
  // 팬1, 팬2, ledPower, 3색 led 핀 RGB를 출력 단자로 설정합니다. //
  Serial.begin(9600);  Team13.begin(9600);  BTSerial.begin(9600);    
  pinMode(6,OUTPUT);  pinMode(9,OUTPUT);  pinMode(ledPower,OUTPUT);     
  
  for (int RGB_three = 0; RGB_three < 3; RGB_three++) 
  { pinMode(color[RGB_three], OUTPUT); }
}

void loop() {  
  // 펄스 구하기 //
  digitalWrite(ledPower,LOW);  delayMicroseconds(samplingTime);          
  voMeasured = analogRead(measurePin);          
  delayMicroseconds(deltaTime);  digitalWrite(ledPower,HIGH);  delayMicroseconds(sleepTime);  
  
  // 전압 값 구하기 //
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = (calcVoltage - 0.64)/0.005;  

  // 구간 평균 //
  if (dustDensity > 0)
  { Serial.print("dustDensity : ");  Serial.println(dustDensity);
    dust_sum = dust_sum + dustDensity;
      
    if(dust_average_Number == 50)
    { dust_average = dust_sum/dust_average_Number;      
      Team13.println(dust_average);    
      dust_average_Number = 0;  dust_sum = 0; }         
    
    
    if(dust_average > 0 && dust_average < 30)       // 초록
    {  digitalWrite(color[0], 1);   LEDreset();  }    
    
    else if(dust_average > 31 && dust_average < 60) // 파랑
    {   digitalWrite(color[1], 1);  LEDreset();  }   
    
    else if(dust_average > 61 && dust_average < 90) // 빨강
    {  digitalWrite(color[2], 1);   LEDreset();  }  
    
    else if(dust_average >=91 )
    { // RGB가 각각 켜고꺼질지 무작위로 선택합니다. //
      // led가 켜진경우 2-1 = 1 = HIGH, 꺼진경우 1-1 = 0 = LOW //
       RDC ( );  LEDreset(); } }
    
  dust_average_Number++;  dust_average = 0;  delay(100);
    
  // 블루투스와 시리얼 통신을 합니다. //
  BTSerial.listen();
 
  // BTSerial 시리얼 모니터에 뭔가 입력되면 아래 if문을 수행합니다. //
  if(BTSerial.available())
  { char ch = BTSerial.read(); // ch에 BTSerial 시리얼 모니터의 값을 저장합니다. //
    
    Serial.write(ch); // o, x, i, u 각각의 값에 따라 팬의 동작을 제어합니다. //
         if( ch =='o')  { digitalWrite(6,0);  digitalWrite(9,0); }
    else if( ch == 'x') { digitalWrite(6,1);  digitalWrite(9,1); }
    else if(ch == 'i')  { state = !state;  digitalWrite(6,state); }
    else if(ch == 'u')  { state = !state;  digitalWrite(9,state); } }
  
  if(Serial.available()) 
  { char ch = Serial.read();
    Serial.write(ch);  BTSerial.write(ch); } 
}   
