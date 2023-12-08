void setup() {
  pinMode(8,INPUT);
  pinMode(10,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int cb = digitalRead(8);
  Serial.println(cb);
  if (cb == 1){
    digitalWrite(10, HIGH);
  }
  else{
    digitalWrite(10, LOW);
  }
}
