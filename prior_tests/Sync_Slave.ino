#include <Wire.h>

int doTask;
int flagGPIO;
byte startUpPulse;
const byte interruptPin = 2;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  doTask = 0;
  flagGPIO = 0;
  startUpPulse = 0;
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), isr, RISING);
}


void loop() {
  if(doTask || flagGPIO) {
    BlinkLed();
    doTask = 0;
    flagGPIO = 0;
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


void isr(){
  if (startUpPulse) {
    flagGPIO = 1;
  } else {
    startUpPulse = 1;
  }
}
