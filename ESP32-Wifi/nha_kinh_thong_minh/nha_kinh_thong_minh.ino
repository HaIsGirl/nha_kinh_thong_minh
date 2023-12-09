#include<Wire.h> //Thư viện giao tiếp I2C
#include <DHT.h> //Thư viện cảm biến nhiệt độ
#include <MQ135.h> //Thư viện cảm biến không khí
#include <Servo.h> //Thư viện điều khiển Servo
#include <LiquidCrystal_I2C.h> //Thư viện điều khiển LCD
#define CB_DAD A1 //Cam bien do am dat
#define CB_KK  A3 // Cam bien khong khi
#define CB_AS 2 //Cam bien anh sang 
//#define relay 12 //Relay Role
#define CB_ND A2 // Cam bien nhiet do
#define CB_MUA 4
//const int LedPin = 7; //Biến chân đèn 
const int DHTTYPE = DHT11; //Khai bao cam bien kieu DHT11
DHT dht(CB_ND, DHTTYPE); //Khai báo thư viện chân cảm biến và kiểu cảm biến cho cảm biến DHT11
MQ135 KK = MQ135(CB_KK); //Khai báo thư viện chân cam biến cho 
LiquidCrystal_I2C lcd(0x27,16,2); //Khai bao thu vien cho LCD
const int Fan = 10;
Servo myservo1;
Servo myservo2;
//Định nghĩa các hàm trong thư viện DHT
#define doam readHumidity
#define nhietdo readTemperature
String adat = "";
String ndo = "";
String khidoc = "";

//Biến lưu giá trị cảm biến
float DAD;  //Bien luu gia tri do am dat
float doC;  //Biến lưu giá trị nhiệt độ (Độ C)
float doF;  //Biến lưu giá trị nhiệt độ (Độ F)
float DA;   //Biến lưu giá trị độ ẩm trong không khí
int AS;     //Biến lưu giá trị ánh sáng (0 là sáng, 1 là tối)
float ppm; //Biến lưu giá trị không khí
//float RZERO; //Biến lấy hệ số kk trung bình
void setup(){
  Serial.begin(9600); //Khởi tạo giao tiếp Serial baund 9600
  pinMode(CB_DAD, INPUT); //PIN DAD INPUT
  //pinMode(relay, OUTPUT); // RELAY OUTPUT
  //pinMode(Fan, OUTPUT); //Khởi tạo chân quạt gió xuất tín hiệu
  pinMode(CB_AS, INPUT);
//  pinMode(LedPin, OUTPUT);
  lcd.init(); //Khởi tạo màn hình lcd
  lcd.backlight(); //Bật đèn nền của lcd
  dht.begin(); //Khởi tạo Cảm biến nhiệt độ 
}


void loop(){
  //lcd.clear(); //Xóa màn hình lcd
  Do_am_dat();
  nhiet_do();
  Anh_sang();
  CBKK();
}


void Anh_sang(){
  AS = digitalRead(CB_AS);
  if (AS == 1){
    Serial.println("LEDON");
  }
  else if (AS==0){
    Serial.println("LEDOFF");
  }
  delay(500);  
}

void Do_am_dat(){
  DAD = analogRead(CB_DAD);
  
  int phantram = map(DAD, 0,1023, 100,0);

  if (phantram >= 30){
    //digitalWrite(relay, LOW); //Máy bơm OFF
    Serial.println("TUOICAYON");
  }
  else {
    Serial.println("TUOICAYOFF");
  }
  adat = adat + "DAD" + DAD;
  Serial.println(adat);
  adat = "";
  delay(500); //Delay 500ms
}

void nhiet_do(){
  DA = dht.doam(); //Đọc độ ẩm không khí
  doC  = dht.nhietdo(); //Đọc nhiệt độ C
  doF  = dht.nhietdo(true); //Đọc nhiệt độ F

  //Kiểm tra cảm biến
  if (isnan(DA) || isnan(doC) || isnan(doF))
    Serial.print("LOI DOC GIA TRI DHT11 !!!");
  else{
  if (doC > 35)
    Serial.println("PHUNSUONGON"); 
  else
    Serial.println("PHUNSUONGOFF");
  }
  ndo = ndo + "ND " + doC;
  Serial.println(ndo);
  ndo = "";
  delay(500);
}

void CBKK(){
  ppm = KK.getCorrectedRZero(doC, DA);
  if (ppm >= 150)
    Serial.println("FANON");
  else
    Serial.println("FANOFF");
  khidoc = khidoc + "KK " + ppm;
  Serial.println(khidoc);
  khidoc = "";
  delay(500); 
}