int outPin = 2;

void setup() {
  pinMode(outPin, OUTPUT);
  digitalWrite(outPin, LOW);
}

void loop() {
  delay(10000);  
  digitalWrite(outPin, HIGH);
  delay(10);
  digitalWrite(outPin, LOW);     
}
