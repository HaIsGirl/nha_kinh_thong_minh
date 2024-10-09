void setup(){
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(3, INPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  Serial.begin(9600);
  }

void loop(){
  int MUA = digitalRead(3);
  int ct1 = digitalRead(5);
  int ct2 = digitalRead(7);
  int DAD = analogRead(A0);
  if (MUA == 0 && DAD < 15){
    digitalWrite(2, HIGH);
    digitalWrite(4, LOW);
    if (ct1 == 0){
      digitalWrite(2, LOW);
      digitalWrite(4, LOW);
    }
  }
  else{
    digitalWrite(2, LOW);
    digitalWrite(4, HIGH);
    if (ct2 == 0){
      digitalWrite(2, LOW);
      digitalWrite(4, LOW);
    }
  }
}