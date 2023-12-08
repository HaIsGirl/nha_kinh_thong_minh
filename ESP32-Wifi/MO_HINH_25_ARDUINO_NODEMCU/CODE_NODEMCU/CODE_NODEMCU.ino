//KHAI BÁO THƯ VIỆN VÀ HẰNG
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>
 //KHAI BÁO TOKEN, SSID VÀ PASS WIFI
char auth[] = "TANQNFpxFNal5GGGBt846IRf1XLvB2ZB";
 
char ssid[] = "Trinh Quang Nam.";
char pass[] = "bubu123789";
 // Khai báo timer
SimpleTimer timer;

// Khai báo biến lưu trữ chuỗi dữ liệu nhận từ Arduino
String myString;

// Khai báo biến nhận từng ký tự dữ liệu từ Arduino
char rdata;

// Khai báo biến lưu trữ giá trị của 3 cảm biến
int firstVal, secondVal, thirdVal;

void myTimerEvent()
{

  Blynk.virtualWrite(V1, millis() / 1000);
  
}
 
 
 
void setup()
{
  // Debug console
  Serial.begin(9600);
 
  Blynk.begin(auth, ssid, pass);
 
    timer.setInterval(1000L,sensorvalue1); 
     timer.setInterval(1000L,sensorvalue2); 
 
}
 
void loop()
{
   if (Serial.available() == 0 ) //NẾU KHÔNG NHẬN DỮ LIỆU THÌ
   {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
   }
   
  if (Serial.available() > 0 )  //NẾU NHẬN DỮ LIỆU THÌ
  {
    rdata = Serial.read();  //ĐỌC MỘT KÍ TỰ DỮ LIỆU TỪ ARDUINO
    myString = myString+ rdata; 
   // Serial.print(rdata);
    if( rdata == '\n') //NẾU NHƯ KÍ TỰ ĐỌC LÀ XUỐNG DÒNG THÌ
    {

      String l = getValue(myString, ',', 0); //TRÍCH XUẤT GIÁ TRỊ THỨ NHẤT
      String m = getValue(myString, ',', 1); //TRÍCH XUẤT GIÁ TRỊ THỨ HAI
      String n = getValue(myString, ',', 2); //NHẤT XUẤT GIÁ TRỊ THỨ BA
      firstVal = l.toInt(); //CHUYỂN GIÁ TRỊ THÀNH SỐ NGUYÊN
      secondVal = m.toInt(); //CHUYỂN GIÁ TRỊ THÀNH SỐ NGUYÊN
      thirdVal = n.toInt(); //CHUYỂN GIÁ TRỊ THÀNH SỐ NGUYÊN
      myString = ""; //KHỞI TẠO LẠI CHUỖI STRING LÀ RỖNG
// end new code
    }
  }
 
}
 
void sensorvalue1()
{
int sdata = firstVal;

Blynk.virtualWrite(V2, sdata);
 
}

void sensorvalue2()
{
int sdata = secondVal;

  Blynk.virtualWrite(V3, sdata);
 }
 
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
