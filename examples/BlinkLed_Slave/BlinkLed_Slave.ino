#include <SyncroARD.h>


void setup() {
  SyncroS::begin(I2C, GPIO);
  SyncroS::attachTask(Task);
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
  SyncroS::doSyncTask();
}


void I2C(int howMany) {
  SyncroS::receiveI2C();
}


void GPIO() {
  SyncroS::receiveGPIO();
}


void Task() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}
