#include <Arduino_FreeRTOS.h>
#include <SyncroARD.h>


// FreeRTOS tasks:
void I2C( void *pvParameters );
void GPIO( void *pvParameters );
void BlinkLed( void *pvParameters );


void setup() {
  SyncroM::begin();
  pinMode(LED_BUILTIN, OUTPUT);
  xTaskCreate(I2C, "I2C", 128, NULL, 1, NULL);
  xTaskCreate(GPIO, "GPIO", 128, NULL, 1, NULL);
  xTaskCreate(BlinkLed, "BlinkLed", 128, NULL, 1, NULL);
}


void loop() { delay(10000); }


void I2C( void *pvParameters ) {
  (void) pvParameters;
  for(;;) { SyncroM::notifyI2C(); }
}


void GPIO( void *pvParameters ) {
  (void) pvParameters;
  for(;;) { SyncroM::notifyGPIO(); }
}


void BlinkLed( void *pvParameters ) {
  (void) pvParameters;

  for(;;) {
    SyncroM::syncTask();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }  
}
