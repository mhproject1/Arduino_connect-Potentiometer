//////////////i2c lcd//////////////
// I2C주소를 0x3F로 지정합니다.
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
//////////////i2c lcd//////////////

//////////온습도 센서 변수//////////
// 2번 핀에 DHT11 온습도 센서 연결합니다.
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
#define AVERAGE 10

DHT dht(DHTPIN, DHTTYPE);
//////////온습도 센서 변수//////////

///////////조도 센서 변수///////////
// A2번 핀에 조도 센서 연결합니다.
int light = A2;
///////////조도 센서 변수///////////

//////////사용자 정의 문자//////////
// good > '_;
// soso > 'ㅠ'
// bad  > '-'
byte status_good[8] = { B00000, B00000, B11011, B11011, B00000, B10001, B01110, B00000 };
byte status_soso[8] = { B00000, B00000, B11011, B11011, B00000, B11111, B01010, B01010 };
byte status_bad[8]  = { B00000, B00000, B11011, B11011, B00000, B01110, B10001, B00000 };
//////////사용자 정의 문자//////////

void setup() {
  lcd.begin();  
  lcd.createChar(0, status_good);
  lcd.createChar(1, status_soso);
  lcd.createChar(2, status_bad);
}

void loop() {   
  delay(1000);
  
  ////////////////온도 센서////////////////////    
  // 온도 센서의 값을 lcd모니터 0,0위치부터 출력합니다.
  // 제 방의 온도보다 값이 3도정도 높게 출력되었기 때문에
  // 실습 편의상 3을 뺀 값을 출력했습니다.
  int TEMP = dht.readTemperature();    
  TEMP = TEMP-3;
  lcd.setCursor(0,0);      
  lcd.print("Temp.:");
  lcd.print(TEMP);        
  ////////////////온도 센서////////////////////
  
  ////////////////조도 센서////////////////////
  // 조도 센서의 값을 lcd모니터 0,1위치부터 출력합니다.
  int light_val = analogRead(light);
  
  lcd.setCursor(0,1);  
  lcd.print("Light:");  
  lcd.print(light_val);   
  ////////////////조도 센서////////////////////
  
  ///////////////정의 문자 출력 ////////////////
  // 문자는 모니터 14,0위치에 출력됩니다.
  // 온도가 27도 이하이고 밝기가 600 이하일 때
  // 온도가 29도보다 높을 때 bad
  if((light_val <= 600 && TEMP <= 27) || (TEMP > 29)){        
    lcd.setCursor(14,0);  lcd.write(byte(2));   
  }
  
  // 온도가 27도 이하이고 밝기가 600보다 높을 때
  // 온도가 27 ~ 29도 사이이고 밝기가 600이하일 때 soso
  if((light_val >= 600 && TEMP <= 27) ||
    (light_val <= 600 && TEMP > 27 && TEMP <= 29)){
      lcd.setCursor(14,0);  lcd.write(byte(1));                                        
  }
  
  // 온도 27 ~ 29도 사이이고 밝기가 600보다 높을 때 good
  if((TEMP >= 27 && TEMP <= 29 && light_val > 600)){
    lcd.setCursor(14,0);  lcd.write(byte(0));  
  }  
  ///////////////정의 문자 출력 ////////////////  
}
