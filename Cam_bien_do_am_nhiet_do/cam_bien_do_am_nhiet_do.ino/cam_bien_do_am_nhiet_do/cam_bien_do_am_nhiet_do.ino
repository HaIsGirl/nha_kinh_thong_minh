#include<DHT.h>
#define doam readHumidity
#define nhietdo readTemperature
const int DHTPIN = A2; //Digital 5

const int DHTTYPE = DHT11; //Khai báo cảm biến kiểu DHT11

DHT dht(DHTPIN, DHTTYPE); //Khai báo thư viện chân cảm biến và kiểu cảm biến

void setup(){
  Serial.begin(9600);
  dht.begin();
  
}

void loop(){
  float doam = dht.doam(); //Đọc độ ẩm;
  float doC = dht.nhietdo(); //Đọc nhiệt độ C
  float doF = dht.nhietdo(true); //Đọc nhiệt độ F

  if (isnan(doam) || isnan(doC) || isnan(doF)){
    Serial.println("LOI DOC GIA TRI TU CAM BIEN!!!");
    return;
  }
  else{
     Serial.print("Do am ");
     Serial.println(doam);
     Serial.print("Nhiet do: ");
     Serial.print(doC);
     Serial.print("°C | ");
     Serial.print(doF);
     Serial.println("°F");
  }
  delay(1000);
}