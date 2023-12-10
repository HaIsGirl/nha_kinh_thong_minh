#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6AXni-pgM"
#define BLYNK_TEMPLATE_NAME "Nha Kinh Thong Minh"
#define BLYNK_AUTH_TOKEN "Nbm_pqRyQzNyk_J7M2Ix6gbj-LMQcI7v"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>  
#include <SerialCommand.h> // Khai báo biến sử dụng thư viện Serial Command

SerialCommand sCmd;

#define led  D1
#define fan  D2
#define tuoicay D4
#define phunsuong D3

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ESP8266";
char pass[] = "kakaaka8lan";

void setup(){
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(tuoicay, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(phunsuong, OUTPUT);
  sCmd.addCommand("LEDON", led_on);
  sCmd.addCommand("LEDOFF", led_off);
  sCmd.addCommand("TUOICAYON", tuoicay_on);
  sCmd.addCommand("TUOICAYOFF", tuoicay_off);
  sCmd.addCommand("FANON", fan_on);
  sCmd.addCommand("FANOFF", fan_off);
  sCmd.addCommand("DAD", DAD);
  sCmd.addCommand("ND", ND);
  sCmd.addCommand("PHUNSUONGON", phunsuong_on);
  sCmd.addCommand("PHUNSUONGOFF", phunsuong_off);
  sCmd.addCommand("KK", KK);
}
BLYNK_WRITE(V2){
  int button = param.asInt();
  if (button == 1)
    digitalWrite(led, HIGH);
  else 
    digitalWrite(led, LOW);
}
BLYNK_WRITE(V4){
  int button = param.asInt();
  if (button == 1)
    digitalWrite(fan, HIGH);
  else 
    digitalWrite(fan, LOW);
}
void loop(){
  Blynk.run();
  sCmd.readSerial();
}
void led_on(){
  digitalWrite(led,HIGH);
}

void led_off(){
  digitalWrite(led, LOW);
}

void fan_on(){
  digitalWrite(fan, HIGH);
}

void fan_off(){
  digitalWrite(fan, LOW);
}

void tuoicay_on(){
  digitalWrite(tuoicay, HIGH);
}

void tuoicay_off(){
  digitalWrite(tuoicay, LOW);
}

void phunsuong_on(){
  digitalWrite(phunsuong, HIGH);
}

void phunsuong_off(){
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
