/*********************************************************
  CHƯƠNG TRÌNH ARDUINO UNO R3 - ESP8266 GIAO TIẾP UART   
            DỰ ÁN NHÀ KÍNH THỦY SINH
          CUỘC THI KHOA HỌC KĨ THUẬT 2023
              Created by Doan Ha 
**********************************************************/
#include<Wire.h> //Thư viện giao tiếp I2C
#include <DFRobot_DHT20.h>
#include <MQ135.h> //Thư viện cảm biến không khí
#include <Servo.h> //Thư viện điều khiển Servo
#include <LiquidCrystal_I2C.h> //Thư viện điều khiển LCD
#include<MFRC522.h> //Thư viện điều khiển RFID 522
#include<SPI.h> //Thư viện giao tiếp SPI
#include<SerialCommand.h>
//Chân digital đã sử dụng: 0,1,2,3,4,5,6,7,8,9,10,11,12,13
//Chân analog đã sử dụng: A0,A1,A2,A3
SerialCommand cmd;
//Công tắc hành trình
#define congtac1 5 
#define congtac2 7
//Hệ thống mái trượt
#define in1  2
#define in2  4
//Thiet bi dien tu
#define tuoicay 6
#define fan 8
#define led 11
#define phunsuong 12
#define lock 13
//Cảm biến 
#define CB_DAD A0 //Cam bien do am dat
#define CB_KK  A1 // Cam bien khong khi
#define CB_AS A3//Cam bien anh sang 
#define CB_MUA 3  //Cảm biến mưa

//Định nghĩa các hàm trong thư viện DHT
#define doam getHumidity
#define nhietdo getTemperature

DFRobot_DHT20 dht; //Khai báo biến DHT cho cảm biến DHt20

MQ135 KK = MQ135(CB_KK); //Khai báo thư viện chân cam biến cho 

LiquidCrystal_I2C lcd(33,16,2); //Khai bao thu vien cho LCD

//Biến lưu lệnh gửi đi Serial
String adat = "";
String ndo = "";
String khidoc = "";
String asang = "";
String doam = "";
//Biến lưu giá trị cảm biến
float DAD;  //Bien luu gia tri do am dat
float doC;  //Biến lưu giá trị nhiệt độ (Độ C)
float doF;  //Biến lưu giá trị nhiệt độ (Độ F)
float DA;   //Biến lưu giá trị độ ẩm trong không khí
int AS;     //Biến lưu giá trị ánh sáng (0 là sáng, 1 là tối)
float ppm; //Biến lưu giá trị không khí
int  MUA; //Biến lưu giá trị mưa
int phantram; //Biến lưu giá trị phầm trăm độ ẩm đất🐧
//Biến lưu giá trị hiệu chỉnh
float MDAD = 30;
float MND = 30;

//Cờ hiệu
bool flag_led = 0;
bool flag_fan = 0;
bool flag_tuoicay = 0;
bool flag_phunsuong = 0;
bool flag_automode = 1;
bool flag_nocnha =0 ;
//MFRC522 mfrc522(SS_PIN, RST_PIN);  //kHAI BAO RFID
byte readCard[4];
String MasterTag = "9C8D9D";  // REPLACE this Tag ID with your Tag ID!!!
String tagID = "";
int Mua = 0; //biến lưu giá trị CB mưa
void setup(){
  Serial.begin(9600); //Khởi tạo giao tiếp Serial baund 115200
  Wire.begin();
  pinMode(congtac1, INPUT_PULLUP);
  pinMode(congtac2, INPUT_PULLUP);
  pinMode(CB_MUA,OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(CB_DAD, INPUT);
  pinMode(CB_KK, INPUT);
  pinMode(CB_MUA, INPUT);
  pinMode(fan, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(tuoicay, OUTPUT);
  pinMode(phunsuong, OUTPUT);
  lcd.init(); //Khởi tạo màn hình lcd
  lcd.backlight();
  dht.begin(); //Khởi tạo Cảm biến nhiệt độ 
  cmd.addCommand("led", LED);
  cmd.addCommand("fan", FAN);      
  cmd.addCommand("phunsuong", PS);
  cmd.addCommand("tuoicay", TC);
  cmd.addCommand("automode", AUTOMODE);
  cmd.addCommand("nocnha", NOCNHA);
  cmd.addCommand("lock", LOCK);
}

void loop(){
  //lcd.clear(); //Xóa màn hình lcd
  lcd.setCursor(0,0);
  lcd.print("<<Scan Your Card>>");
  cmd.readSerial();
  Do_am_dat();
  nhiet_do();
  Anh_sang();
  CBKK();
}  

//Chức năng ERA
void LED(){
  char *arg;
  arg  = cmd.next();
  int Value = atoi(arg);
  digitalWrite(led, Value);
  Serial.print("BAT TAT DENNNNNNNNNNNN");
  Serial.println(Value);
  if (Value == 1)
    flag_led = 1;
  else 
    flag_led = 0;
}

void FAN(){
  char *arg;
  arg  = cmd.next();
  int Value = atoi(arg);
  digitalWrite(fan, Value);
  Serial.print("BAT TAT QUATTTTTTTTTTTTT ");
  Serial.println(Value);
  if (Value == 1)
    flag_fan = 1;
  else 
    flag_fan = 0;  
}

void PS(){
  char *arg;
  arg  = cmd.next();
  int Value = atoi(arg);
  digitalWrite(phunsuong, Value);
  Serial.print("BAT TAT PHUN SUONGGGGGGGGG");
  Serial.println(Value);
  if (Value == 1)
    flag_phunsuong = 1;
  else 
    flag_phunsuong = 0;
}

void TC(){
  char *arg;
  arg  = cmd.next();
  int Value = atoi(arg);
  digitalWrite(tuoicay, Value);
  Serial.print("BAT TAT TUOICAYYYYYYYYYYYYY");
  Serial.println(Value);
  if (Value == 1)
    flag_tuoicay = 1;
  else 
    flag_tuoicay = 0;
}

void AUTOMODE(){
  char *arg;
  arg  = cmd.next();
  int Value = atoi(arg);
  Serial.print("BAT TAT AUTOMODEEEEEEEEEEEEEE");
  Serial.println(Value);
  if (Value == 1)
    flag_automode = 1;
  else {
    MDAD = 30;
    MND = 30; 
    flag_automode = 0;
  }
}

void NOCNHA(){
  char *arg;
  arg = cmd.next();
  int Value = atoi(arg);
  if (Value == 1){
    flag_nocnha = 1;
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    if (congtac2 == 1){
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }   
  }
  if (Value == 0){
    flag_nocnha = 0; 
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    if (congtac1 == 1){
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }
  }
}


//Chức năng chính

void Anh_sang(){
  int value = analogRead(CB_AS); 
  AS = map(value, 0,1023, 0,100);
  asang = asang + "Anh sang: " + AS;
  Serial.println(asang);
  asang = "";
  if (flag_automode == 1){
    if (flag_led == 0){
    if (AS < 30){
      digitalWrite(led, HIGH);
    }
    else if (AS>30){
      digitalWrite(led, LOW);
     }
    }
  }
  delay(50);  
}           

void Do_am_dat(){
  DAD = analogRead(CB_DAD);
  phantram = map(DAD, 0,1024,0,100);
  adat = adat + "DAD " + phantram;
  Serial.println(adat);
  adat = "";
  if (flag_automode == 1){
    if (flag_tuoicay == 0){
     if (phantram <= 15 && Mua == 0){
      Serial.println("TUOICAY");
      digitalWrite(tuoicay,HIGH);
    }
    else{
      Serial.println("KHONGTUOI");
      digitalWrite(tuoicay, LOW);
    }
    }
  }
  delay(50); //Delay 50ms
}

void nhiet_do(){
  DA = dht.doam(); //Đọc độ ẩm không khí
  doC = dht.nhietdo(); //Đọc nhiệt độ C
  ndo = ndo + "ND " + doC;
  doam = doam + "doam " + DA;
  Serial.println(ndo);
  Serial.println(doam);
  ndo = "";
  doam = "";
  //Kiểm tra cảm biến
  if (isnan(DA) || isnan(doC))
    Serial.print("LOI DOC GIA TRI DHT20 !!!");
  else{
    if (flag_automode == 1){
      if (flag_phunsuong == 0){
       if (doC > 35){
        Serial.println("PHUN");
        digitalWrite(phunsuong, HIGH); 
       }
        else{
         Serial.println("KHONGPHUN");
         digitalWrite(phunsuong,LOW);
        }
      }
  }
  delay(50);
  }
}

void CBKK(){
  ppm = KK.getPPM();
  khidoc = khidoc + "KK " + ppm;
  Serial.println(khidoc);
  khidoc = "";
  if (flag_automode == 1){
    if (flag_fan == 0){
     if (ppm >= 150){
    //   tone(BUZZER, 300);
       Serial.println("FANON");
      digitalWrite(fan,HIGH);
    }
     else{
       Serial.println("FANOFF");
      digitalWrite(fan,LOW);
    }
    }
  }
  delay(50); 
}

void CBM(){
  MUA = digitalRead(CB_MUA);
  if (flag_automode == 1){
    if (flag_nocnha == 0){
      if (MUA == 0 && phantram < 30){
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        if (congtac2 == 1){
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
        }
      }
      else{
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        if (congtac1 == 1){
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
        }
      }
    }
  }
}


void LOCK(){
  char *arg;
  arg  = cmd.next();
  int Value = atoi(arg);
  Serial.print("BAT TAT KHOA");
  Serial.println(Value);
  if (Value == 1){
      digitalWrite(lock, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.println("   WELCOME TO           ");
      lcd.setCursor(3,1);
      lcd.print("GREENHOUSE");
      delay(3000);
      digitalwrite(lock, LOW);
  }
  else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" Access Denied!");
      delay(3000);
  }
}