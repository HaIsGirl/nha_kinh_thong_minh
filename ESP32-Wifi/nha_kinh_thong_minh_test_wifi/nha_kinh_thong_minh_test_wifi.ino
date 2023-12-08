#define BLYNK_TEMPLATE_ID "TMPL6toP4AcNm"
#define BLYNK_TEMPLATE_NAME "Nha kinh"
#define BLYNK_AUTH_TOKEN "yJPJmSY_6vpxLpkPVrRV8ffydwnn5kAD"

//Khai báo thư viện ESP32 và Wifi
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include<Wire.h> //Thư viện giao tiếp I2C
#include <DHT.h> //Thư viện cảm biến nhiệt độ
#include <MQ135.h> //Thư viện cảm biến không khí
#include <Servo.h> //Thư viện điều khiển Servo
#include <LiquidCrystal_I2C.h> //Thư viện điều khiển LCD
#define CB_DAD A1 //Cam bien do am dat
#define CB_KK A3 // Cam bien khong khi
#define CB_AS 2 //Cam bien anh sang 
#define relay 12 //Relay Role
#define CB_ND A2 // Cam bien nhiet do
#define CB_MUA 4
const int LedPin = 7; //Biến chân đèn 
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
char auth[] = BLYNK_AUTH_TOKEN;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "HIEN HA";
char pass[] = "123456789";


//Biến lưu giá trị cảm biến
float DAD;  //Bien luu gia tri do am dat
float doC;  //Biến lưu giá trị nhiệt độ (Độ C)
float doF;  //Biến lưu giá trị nhiệt độ (Độ F)
float DA;   //Biến lưu giá trị độ ẩm trong không khí
int AS;     //Biến lưu giá trị ánh sáng (0 là sáng, 1 là tối)
void setup(){
  Serial.begin(9600); //Khởi tạo giao tiếp Serial baund 9600
  pinMode(CB_DAD, INPUT); //PIN DAD INPUT
  pinMode(relay, OUTPUT); // RELAY OUTPUT
  pinMode(Fan, OUTPUT); //Khởi tạo chân quạt gió xuất tín hiệu
  pinMode(CB_AS, INPUT);
  pinMode(LedPin, OUTPUT);
  lcd.init(); //Khởi tạo màn hình lcd
  lcd.backlight(); //Bật đèn nền của lcd
  dht.begin(); //Khởi tạo Cảm biến nhiệt độ
  blynk.begin(auth,ssid,pass) 
}
