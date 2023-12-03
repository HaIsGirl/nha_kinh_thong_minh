#include<Wire.h> //Thư viện giao tiếp I2C
#include <DHT.h> //Thư viện cảm biến nhiệt độ
#include <MQ135.h> //Thư viện cảm biến không khí
#include <Servo.h> //Thư viện điều khiển Servo
#include <LiquidCrystal_I2C.h> //Thư viện điều khiển LCD
#define CB_DAD A1 //Cam bien do am dat
#define CB_KK A3 // Cam bien khong khi
#define CB_AS 4 //Cam bien anh sang
#define relay 12 //Relay Role
#define CB_ND A2 // Cam bien nhiet do
const int DHTTYPE = DHT11; //Khai bao cam bien kieu DHT11
DHT dht(CB_ND, DHTTYPE); //Khai báo thư viện chân cảm biến và kiểu cảm biến cho cảm biến DHT11
MQ135 KK = MQ135(CB_KK); //Khai báo thư viện chân cam biến cho 
LiquidCrystal_I2C lcd(0x27,16,2); //Khai bao thu vien cho LCD
const int Fan = 10;
//Định nghĩa các hàm trong thư viện DHT
#define doam readHumidity
#define nhietdo readTemperature


//Biến lưu giá trị cảm biến
float DAD;  //Bien luu gia tri do am dat
float doC;  //Biến lưu giá trị nhiệt độ (Độ C)
float doF;  //Biến lưu giá trị nhiệt độ (Độ F)
float DA;   //Biến lưu giá trị độ ẩm trong không khí
void setup(){
  Serial.begin(9600); //Khởi tạo giao tiếp Serial baund 9600
  pinMode(CB_DAD, INPUT); //PIN DAD INPUT
  pinMode(relay, OUTPUT); // RELAY OUTPUT
  pinMode(Fan, OUTPUT); //Khởi tạo chân quạt gió xuất tín hiệu
  lcd.init(); //Khởi tạo màn hình lcd
  lcd.backlight(); //Bật đèn nền của lcd
  dht.begin(); //Khởi tạo Cảm biến nhiệt độ 
}


void loop(){
  lcd.clear(); //Xóa màn hình lcd
  Do_am_dat();
}

void Do_am_dat(){
  DAD = analogRead(CB_DAD);
  Serial.println(DAD); //Cảm biến độ ẩm nhận giá trị từ 0 - 1023
  int phantram = map(DAD, 0,1023, 100,0);

  if (phantram >= 30){
    digitalWrite(relay, LOW); //Máy bơm OFF
    Serial.println("Cay du H20");
  }
  else {
    digitalWrite(relay,HIGH); //Máy bơm ON
    Serial.println("Tuoi H20 cho cay");
  }
  lcd.setCursor(0,0); //Đặt vị trí con trỏ ở hàng 1 cột 1
  lcd.print("Do am: "); //In ra lcd 
 // lcd.setCursor(7,0); //Đặt vị trí con trỏ ở hàng 8 cột 1
  lcd.print(phantram); //In ra giá trị phần trăm của độ ẩm
  lcd.print("%");
  delay(100); //Delay 100ms
}

void nhiet_do(){
  DA = dht.doam(); //Đọc độ ẩm không khí
  doC  = dht.nhietdo(); //Đọc nhiệt độ C
  doF  = dht.nhietdo(true); //Đọc nhiệt độ F

  //Kiểm tra cảm biến
  if (isnan(DA) || isnan(doC) || isnan(doF)){
    Serial.print("LOI DOC GIA TRI DHT11 !!!");
  }
  else{
    if (doC> 35){
      digitalWrite(Fan, LOW); // FAN ON
    }
    else{
      digitalWrite(Fan, HIGH); // FAN OFF
    }
    lcd.setCursor(0,1); //Đặt vị trí con trỏ ở hàng 1 cột 2
    lcd.print("Nhiet do: ");
 //   lcd.setCursor(10,1);
    lcd.print(doC);
    lcd.write(0xDF);          
  }
}