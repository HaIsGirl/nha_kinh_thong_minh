//CHUYỂN GIÁ TRỊ ANALOG TỪ CẢM BIẾN ĐO ĐỘ ẨM ĐẤT SANG GIÁ TRỊ %

const int DoAmPin = A1; // Nối chân cảm biến vào chân A1 Analog
#define relay = 8 //Nối chân role vào chân 8 Digital

void setup(){
  Serial.begin(9600);
  pinMode(DoAmPin, INPUT);
  pinMode(relay, OUTPUT);
  digitalwrite(relay, LOW);
}
float DAD;
void loop(){
  DAD = analogRead(DoAmPin);
  Serial.println(DAD); //Cảm biến độ ẩm nhận giá trị từ 0 - 1023
  int phantramao = map(DAD, 0,1023, 0,100);
  int phantram = 100 - phantramao; 

  if (phantram >= 30){
    digitalWrite(relay, LOW);
    Serial.println("Cay du H20");
  }
  else {
    digitalWrite(relay,HIGH);
    Serial.println("Tuoi H20 cho cay");
  }
  delay(1000);
}