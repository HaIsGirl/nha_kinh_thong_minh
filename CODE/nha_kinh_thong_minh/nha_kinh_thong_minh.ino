/*********************************************************
  CHƯƠNG TRÌNH ARDUINO UNO R3 - ESP8266 GIAO TIẾP UART   
            DỰ ÁN NHÀ KÍNH THÔNG MINH 
          CUỘC THI KHOA HỌC KĨ THUẬT 2023
              Created by Doan Ha 
    TRƯỜNG THPT NGUYỄN DU SÔNG HINH PHÚ YÊN
**********************************************************/
#include<Wire.h> //Thư viện giao tiếp I2C
#include <DHT.h> //Thư viện cảm biến nhiệt độ
#include <MQ135.h> //Thư viện cảm biến không khí
#include <Servo.h> //Thư viện điều khiển Servo
#include <LiquidCrystal_I2C.h> //Thư viện điều khiển LCD
#include<MFRC522.h> //Thư viện điều khiển RFID 522
#include<SPI.h> //Thư viện giao tiếp SPI
#include <SimpleKalmanFilter.h>
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 //define green LED pin
#define LED_R 4 //define red LED
#define BUZZER 6 
#define LED_Y 1
#define pinServo1 2
#define CB_DAD A0 //Cam bien do am dat
#define CB_KK  A1 // Cam bien khong khi
#define CB_AS 7//Cam bien anh sang 
#define CB_ND A2 // Cam bien nhiet do
#define CB_MUA 0 //Cảm biến mưa
//Định nghĩa các hàm trong thư viện DHT
#define doam readHumidity
#define nhietdo readTemperature
const int DHTTYPE = DHT11; //Khai bao cam bien kieu DHT11
DHT dht(CB_ND, DHTTYPE); //Khai báo thư viện chân cảm biến và kiểu cảm biến cho cảm biến DHT11



MQ135 KK = MQ135(CB_KK); //Khai báo thư viện chân cam biến cho 
LiquidCrystal_I2C lcd(0x27,16,2); //Khai bao thu vien cho LCD
MFRC522 mfrc522(SS_PIN, RST_PIN);  //kHAI BAO RFID
//Biến lưu lệnh gửi đi Serial
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
byte readCard[4];
String MasterTag = "9C8D9D";  // REPLACE this Tag ID with your Tag ID!!!
String tagID = "";
int Mua = 0; //biến lưu giá trị CB mưa
Servo servo1;
Servo servo2;
void setup(){
  Serial.begin(9600); //Khởi tạo giao tiếp Serial baund 9600
  //SETUP RFID
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522 
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(CB_DAD, INPUT); //PIN DAD INPUT
  pinMode(CB_AS, INPUT);
  pinMode(LED_Y, OUTPUT);
  digitalWrite(LED_G, LOW);
  lcd.init(); //Khởi tạo màn hình lcd
  lcd.backlight();
  dht.begin(); //Khởi tạo Cảm biến nhiệt độ 
  servo1.attach(pinServo1);
  servo1.write(90);
  lcd.setCursor(0,0);
  lcd.println("<<Scan Your Card>>");
}

void loop(){
  //lcd.clear(); //Xóa màn hình lcd
  LOCK_RFID();
  Do_am_dat();
  nhiet_do();
  Anh_sang();
  CBKK();
  CB_Mua();
}

boolean getID() 
{
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
  //readCard[i] = mfrc522.uid.uidByte[i];
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}

void LOCK_RFID(){
  bool Flag = false;
    while (getID()) 
  {    
    if (tagID == MasterTag) 
    {
      digitalWrite(LED_G, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.println("   WELCOME TO           ");
      lcd.setCursor(3,1);
      lcd.print("GREENHOUSE");
      Serial.println("UNLOCK");
      Flag = true;
    }
    else
    {
      tone(BUZZER, 300);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" Access Denied!");
      Flag = 1;
    }
      Serial.print(" ID : ");
      Serial.println(tagID);
    if (Flag == 1){
      delay(1000);
      noTone(BUZZER);
      lcd.setCursor(0,0);
      lcd.println(" Access Control ");
      lcd.setCursor(0, 1);
      lcd.println("<<Scan Your Card>>");
    }
  }

}
void Anh_sang(){
  AS = digitalRead(CB_AS);
  if (AS == 1){
    Serial.println("LEDON");
    digitalWrite(LED_Y, HIGH);
  }
  else if (AS==0){
    Serial.println("LEDOFF");
    digitalWrite(LED_Y, LOW);
  }
  delay(200);  
}

void Do_am_dat(){
  DAD = analogRead(CB_DAD);
  int phantram = map(DAD, 0,1023, 100,0);

  if (phantram <= 15 && Mua == 0){
    Serial.println("TUOICAY");
  }
  else {
    Serial.println("KHONGTUOI");
    digitalWrite(LED_Y,LOW);
  }
  adat = adat + "DAD " + phantram;
  Serial.println(adat);
  adat = "";
  delay(200); //Delay 200ms
}

void nhiet_do(){
  DA = dht.doam(); //Đọc độ ẩm không khí
  doC = dht.nhietdo(); //Đọc nhiệt độ C
  doF  = dht.nhietdo(true); //Đọc nhiệt độ F
  //Kiểm tra cảm biến
  if (isnan(DA) || isnan(doC) || isnan(doF))
    Serial.print("LOI DOC GIA TRI DHT11 !!!");
  else{
  if (doC > 35){
   Serial.println("PHUN");
   digitalWrite(LED_Y,HIGH); 
  }
  else{
    Serial.println("KHONGPHUN");
    digitalWrite(LED_Y,LOW);
    }
  }
  ndo = ndo + "ND " + doC;
  Serial.println(ndo);
  ndo = "";
  delay(200);
}

void CBKK(){
  ppm = KK.getPPM();
  if (ppm >= 100){
    tone(BUZZER, 300);
    Serial.println("FANON");
    digitalWrite(LED_Y,HIGH);
    noTone(BUZZER);
  }
  else{
    Serial.println("FANOFF");
    digitalWrite(LED_Y,LOW);
  }
  khidoc = khidoc + "KK " + ppm;
  Serial.println(khidoc);
  khidoc = "";
  delay(200); 
}

void CB_Mua(){
  Mua = digitalRead(CB_MUA);
  Serial.print("Mua: ");
  Serial.println(CB_MUA);
  if (Mua == 1 && DAD < 15){
    servo1.write(200);
    delay(1000);
    servo1.write(90);
  }
  else{
    servo1.write(50);
    delay(1000);
    servo1.write(90);
  }
}