#include <Wire.h>
#include <Arduino_FreeRTOS.h>

int shouldStart;

// FreeRTOS tasks:
void BlinkLed( void *pvParameters );


void setup() {
  shouldStart = 0;
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  xTaskCreate(BlinkLed, "BlinkLed", 128, NULL, 1, NULL);
}


void loop() { delay(10000); }


void receiveEvent(int howMany) {
  int x = Wire.read();
  shouldStart = x;
}


void BlinkLed( void *pvParameters ) {
  (void) pvParameters;
  pinMode(LED_BUILTIN, OUTPUT);

  while(!shouldStart) { delay(100); }
  
  for(;;) {
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
}
