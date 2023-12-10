#include<Wire.h> //Thư viện giao tiếp I2C
#include <DHT.h> //Thư viện cảm biến nhiệt độ
#include <MQ135.h> //Thư viện cảm biến không khí
#include <Servo.h> //Thư viện điều khiển Servo
#include <LiquidCrystal_I2C.h> //Thư viện điều khiển LCD
#include<MFRC522.h> //Thư viện điều khiển RFID 522
#include<SPI.h> //Thư viện giao tiếp SPI


#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 //define green LED pin
#define LED_R 4 //define red LED
#define LOCK 3 //relay pin
#define BUZZER 6 
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define CB_DAD A0 //Cam bien do am dat
#define CB_KK  A1 // Cam bien khong khi
#define CB_AS 7 //Cam bien anh sang 
//#define relay 12 //Relay Role
#define CB_ND A2 // Cam bien nhiet do
#define CB_MUA 8
//const int LedPin = 7; //Biến chân đèn 
const int DHTTYPE = DHT11; //Khai bao cam bien kieu DHT11
DHT dht(CB_ND, DHTTYPE); //Khai báo thư viện chân cảm biến và kiểu cảm biến cho cảm biến DHT11
MQ135 KK = MQ135(CB_KK); //Khai báo thư viện chân cam biến cho 
LiquidCrystal_I2C lcd(0x27,16,2); //Khai bao thu vien cho LCD
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
int UID[4],i; //Biến lưu UID thẻ quét
int ID1[4] = {9, 12, 141, 157}; //ID MASTER

void setup(){
  Serial.begin(9600); //Khởi tạo giao tiếp Serial baund 9600
  //SETUP RFID
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522 
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LOCK, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(LOCK, LOW);

  pinMode(CB_DAD, INPUT); //PIN DAD INPUT
  pinMode(CB_AS, INPUT);
  lcd.init(); //Khởi tạo màn hình lcd
  lcd.backlight(); //Bật đèn nền của lcd
  dht.begin(); //Khởi tạo Cảm biến nhiệt độ 
}


void loop(){
  //lcd.clear(); //Xóa màn hình lcd
  LOCK_RFID();
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
  adat = adat + "DAD " + phantram;
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
  ppm = KK.getCorrectedPPM(doC, DA);
  if (ppm >= 150)
    Serial.println("FANON");
  else
    Serial.println("FANOFF");
  khidoc = khidoc + "KK " + ppm;
  Serial.println(khidoc);
  khidoc = "";
  delay(500); 
}


void LOCK_RFID(){
    // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
   Serial.print("UID của thẻ: ");   
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  { 
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");   
    UID[i] = mfrc522.uid.uidByte[i];
    Serial.print(UID[i]);
  }

  Serial.println("   ");

  if (UID[i] == ID1[i])
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(LOCK, HIGH);
    digitalWrite(LED_G, HIGH);
    delay(ACCESS_DELAY);
    digitalWrite(LOCK, LOW);
    digitalWrite(LED_G, LOW);
  } 
 else   {
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(DENIED_DELAY);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
}