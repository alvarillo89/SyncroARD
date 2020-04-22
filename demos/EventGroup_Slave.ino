#include <Wire.h>

int doTask;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  doTask = 0;
}


void loop() {
  if(doTask) {
    BlinkLed();
    doTask = 0;
  }
}


void receiveEvent(int howMany) {
  doTask = 1;
  int x = Wire.read();
}


void BlinkLed(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}
