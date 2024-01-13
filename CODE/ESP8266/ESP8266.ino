
/*************************************************************
  Download latest ERa library here:
    https://github.com/eoh-jsc/era-lib/releases/latest
    https://www.arduino.cc/reference/en/libraries/era
    https://registry.platformio.org/libraries/eoh-ltd/ERa/installation

    ERa website:                https://e-ra.io
    ERa blog:                   https://iotasia.org
    ERa forum:                  https://forum.eoh.io
    Follow us:                  https://www.fb.com/EoHPlatform
 *************************************************************/

// Enable debug console
// Set CORE_DEBUG_LEVEL = 3 first
//#define ERA_DEBUG

/*
RFID:  
• 3.3v = 3.3v 
• RST = D3  
• GND = GND 
• IRQ = Not Used 
• MISO = D6 
• MOSI = D7 
• SCK = D5 
• SDA = D4
*/ 
#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"

// You should get Auth Token in the ERa App or ERa Dashboard
#define ERA_AUTH_TOKEN "226afa54-cab0-4b3a-9eea-2e2ac5b0a027"

#include <SPI.h>
#include <MFRC522.h>
#include<SerialCommand.h>
#include <SoftwareSerial.h>  
#include <ESP8266WiFi.h>
#include <ERa.hpp>
#include <ERa/ERaTimer.hpp>
const char ssid[] = "ESP8266";
const char pass[] = "kakaaka8lan";

SerialCommand cmd;

ERaTimer timer;

/* This function print uptime every second */
String led = "";
String fan = "";
String tuoicay = "";
String phunsuong = "";
String automode = "";
String nocnha =  "";
String mucnd = "";
String mucdad = "";
String muckk = "";
String mucas = "";
#define SS_PIN D4
#define RST_PIN D3
#define LED_G D1 //define green LED pin
#define LED_R D2 //define red LED
#define BUZZER D8 //buzzer pin
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
void setup() {
    /* Setup debug console */
    Serial.begin(115200);
    SPI.begin();          // Initiate  SPI bus
    mfrc522.PCD_Init();   // Initiate MFRC522
    pinMode(LED_G, OUTPUT);
    pinMode(LED_R, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    noTone(BUZZER);
    ERa.begin(ssid, pass);
    ERa.virtualWrite(V4, 1);
    ERa.virtualWrite(V11, 15);
    ERa.virtualWrite(V12, 35);
    ERa.virtualWrite(V13, 150);
    ERa.virtualWrite(V14, 50);
    /* Setup timer called function every second */
    cmd.addCommand("DAD",doamdat);
    cmd.addCommand("ND", nhietdo);
    cmd.addCommand("KK", khongkhi);
    cmd.addCommand("doam", doam);
    cmd.addCommand("mo noc", OPENNOC);
    cmd.addCommand("dongnoc", CLOSENOC);
    cmd.addCommand("dung motor", STOPMOTOR);
}

ERA_WRITE(V0){
  int value = param.getInt();
  led = led + "led " + value;
  Serial.println(led);
  led = "";
  ERa.virtualWrite(V0, value);
}

ERA_WRITE(V1){
  int value = param.getInt();
  fan = fan + "fan " + value;
  Serial.println(fan);
  fan = "";
  ERa.virtualWrite(V1, value);
}

ERA_WRITE(V2){
  int value = param.getInt();
  tuoicay = tuoicay + "tuoicay " + value;
  Serial.println(tuoicay);
  tuoicay = "";
  ERa.virtualWrite(V2, value);
}

ERA_WRITE(V3){
  int value = param.getInt();
  phunsuong = phunsuong + "phunsuong " + value;
  Serial.println(phunsuong);
  phunsuong = "";
  ERa.virtualWrite(V3, value);
}

ERA_WRITE(V4){
    int value = param.getInt();
    automode = automode + "automode " + value;
    Serial.println(automode);
    automode = "";
    ERa.virtualWrite(V4, value);
}

ERA_WRITE(V5){
  int value = param.getInt();
  nocnha = nocnha + "nocnha " + value; 
  Serial.println(nocnha);
  nocnha = "";
  ERa.virtualWrite(V5, value);
}
ERA_WRITE(V11){
  int value = param.getInt();
  mucdad = mucdad + "MDAD " + value;
  Serial.println(mucdad);
  mucdad = "";
  ERa.virtualWrite(V11, value);  
}

ERA_WRITE(V12){
  int value = param.getInt();
  mucnd = mucnd + "MND " + value;
  mucnd = "";
  ERa.virtualWrite(V12, value);
}

ERA_WRITE(V13){
  int value = param.getInt();
  muckk = "";
  ERa.virtualWrite(V13, value);
}

ERA_WRITE(V14){
  int value = param.getInt();
  mucas = mucas +"MAS " + value;
  mucas = "";
  ERa.virtualWrite(V14, value);
}
void loop() {
    ERa.run();
    cmd.readSerial();
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
  Serial.print("UID tag :");
  String content= "";
  int letter;
  for (int i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  content.toUpperCase();
  if (content.substring(1) == "09 0C 8D 9D") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("lock 1");
    Serial.println("Authorized access");
    Serial.println();
    digitalWrite(LED_G, HIGH);
    delay(ACCESS_DELAY);
    digitalWrite(LED_G, LOW);
    
  }
 
 else   {
    Serial.println("lock 0");
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 1000);
    delay(DENIED_DELAY);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
}

void khongkhi(){
  char *arg;
  arg  = cmd.next();
  int Value = atoi(arg);
  ERa.virtualWrite(V9, Value);
}

void doamdat(){
  char *arg;
  arg  = cmd.next();
  int Value = atoi(arg);
  ERa.virtualWrite(V8, Value);
}

void doam(){
  char *arg;
  arg  = cmd.next();
  int Value = atoi(arg);
  ERa.virtualWrite(V7, Value);
}

void nhietdo(){
  char *arg;
  arg  = cmd.next();
  int Value = atoi(arg);
  ERa.virtualWrite(V6, Value);
}