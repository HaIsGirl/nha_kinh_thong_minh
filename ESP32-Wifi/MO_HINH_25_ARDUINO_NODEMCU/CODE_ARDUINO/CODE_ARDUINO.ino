#include <SoftwareSerial.h>
 
SoftwareSerial nodemcu(2,3);//tx-2, rx-3
 
int sensor1 = A0; // biến trở
int sensor2 = A1; //cảm biến hồng ngoại
int sensor3 = A2; 

int sdata1 = 0; 
int sdata2 = 0; 
int sdata3 = 0; 
 
String cdata; 
 
void setup()
{
Serial.begin(9600); 
nodemcu.begin(9600);
 
pinMode(sensor1, INPUT);
pinMode(sensor2, INPUT);
pinMode(sensor3, INPUT);
pinMode(4, OUTPUT); // bat led
digitalWrite(4, LOW); 
 
}
 
void loop()
{
  
    sdata1 = analogRead(sensor1);
    sdata2 = analogRead(sensor2);
    sdata3 = analogRead(sensor3);
     if (sdata1 > 500) {digitalWrite(4, HIGH);}
 else {digitalWrite(4, LOW);}
   cdata = cdata + sdata1+","+sdata2+","+sdata3; // comma will be used a delimeter
   Serial.println(cdata); 
   nodemcu.println(cdata);
   delay(2000); // 100 milli seconds
   cdata = ""; 
}
