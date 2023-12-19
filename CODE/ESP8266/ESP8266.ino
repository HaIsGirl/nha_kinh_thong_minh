/*
                                                           CHƯƠNG TRÌNH ARDUINO UNO R3 - ESP8266 GIAO TIẾP UART 
                                                                      DỰ ÁN NHÀ KÍNH THÔNG MINH 
                                                                   CUỘC THI KHOA HỌC KĨ THUẬT 2023
                                                                        Created by Doan Ha 
                                                              TRƯỜNG THPT NGUYỄN DU SÔNG HINH PHÚ YÊN
*/

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>  
#include <SerialCommand.h> // Khai báo biến sử dụng thư viện Serial Command
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6AXni-pgM"
#define BLYNK_TEMPLATE_NAME "Nha Kinh Thong Minh"
#define BLYNK_AUTH_TOKEN "Nbm_pqRyQzNyk_J7M2Ix6gbj-LMQcI7v"

SerialCommand sCmd;
#define led  D1
#define fan  D2
#define tuoicay D4
#define phunsuong D3
#define lock D5
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ESP8266";
char pass[] = "kakaaka8lan";
bool flag_fan = 0;
bool flag_light = 0;
void setup(){
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(tuoicay, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(phunsuong, OUTPUT);
  pinMode(lock, OUTPUT);
  sCmd.addCommand("LEDON", led_on);
  sCmd.addCommand("LEDOFF", led_off);
  sCmd.addCommand("TUOICAY", tuoicay_on);
  sCmd.addCommand("KHONGTUOI", tuoicay_off);
  sCmd.addCommand("FANON", fan_on);
  sCmd.addCommand("FANOFF", fan_off);
  sCmd.addCommand("DAD", DAD);
  sCmd.addCommand("ND", ND);
  sCmd.addCommand("PHUN", phunsuong_on);
  sCmd.addCommand("KHONGPHUN", phunsuong_off);
  sCmd.addCommand("KK", KK);
  sCmd.addCommand("UNLOCK", unlock);
}
BLYNK_WRITE(V2){
  int button = param.asInt();
  if (button == 1){
    flag_light = 1;
    digitalWrite(led, HIGH);
  }
  else{
    digitalWrite(led, LOW);
    flag_light = 0;
  }
}
BLYNK_WRITE(V4){
  int button = param.asInt();
  if (button == 1){
    flag_fan = 1;
    digitalWrite(fan, HIGH);
  }
  else{ 
    digitalWrite(fan, LOW);
    flag_fan = 0;
  }
}
void loop(){
  Blynk.run();
  sCmd.readSerial();
}
void led_on(){
    Serial.println("LED ON");
    digitalWrite(led,HIGH);
}

void led_off(){
  if (flag_light == 0)
    digitalWrite(led, LOW);
  Serial.println("LED OFF");
}

void fan_on(){
  Serial.println("FAN ON");
    digitalWrite(fan, HIGH);
}

void fan_off(){
  if (flag_fan == 0)
    digitalWrite(fan, LOW);
  Serial.println("FAN OFF");
}

void tuoicay_on(){
  Serial.println("TUOI CAY");
  digitalWrite(tuoicay, HIGH);
}

void tuoicay_off(){
  Serial.println("Cay du nuoc");
  digitalWrite(tuoicay, LOW);
}

void phunsuong_on(){
  Serial.println("Phun suong");
  digitalWrite(phunsuong, HIGH);
}

void phunsuong_off(){
  Serial.println("Tat phun suong");
  digitalWrite(phunsuong, LOW);
}

void DAD(){
  char *arg;
  arg  = sCmd.next();
  int Value = atoi(arg);
  Serial.print("DAD: ");
  Serial.println(Value);
  Blynk.virtualWrite(V1, Value);
}

void ND(){
  char *arg;
  arg  = sCmd.next();
  int Value = atoi(arg);
  Serial.print("ND: ");
  Serial.println(Value);
  Blynk.virtualWrite(V0, Value);
}

void KK(){
  char *arg;
  arg  = sCmd.next();
  int Value = atoi(arg);
  Blynk.virtualWrite(V3, Value);
}

void unlock(){
  digitalWrite(lock, HIGH);
  delay(500);
  digitalWrite(lock, LOW);
}