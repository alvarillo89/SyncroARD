#include <Wire.h>
#include <Arduino_FreeRTOS.h>
#include <event_groups.h>


// Sync Stuff:
EventGroupHandle_t xEventBits;
#define TASK_0_BIT  ( 1 << 0 )
#define TASK_1_BIT  ( 1 << 1 )
#define ALL_SYNC_BITS ( TASK_0_BIT | TASK_1_BIT )


// RTOS tasks:
void Notify( void *pvParameters );
void BlinkLed( void *pvParameters );


void setup() {
  Wire.begin();
  xEventBits = xEventGroupCreate();

  if( xEventBits != NULL ) {
    xTaskCreate(Notify, "Notify", 128, NULL, 1, NULL);
    xTaskCreate(BlinkLed, "BlinkLed", 128, NULL, 1, NULL);
  }
}


void loop(){ delay(10000); }


void Notify( void *pvParameters ) {
  (void) pvParameters;

  for(;;) {
    xEventGroupSync( xEventBits, TASK_0_BIT, ALL_SYNC_BITS, portMAX_DELAY );
    Wire.beginTransmission(4);
    Wire.write(1);
    Wire.endTransmission();
  }
}


void BlinkLed( void *pvParameters ) {
  (void) pvParameters;
  pinMode(LED_BUILTIN, OUTPUT);

  for(;;) {
    xEventGroupSync( xEventBits, TASK_1_BIT, ALL_SYNC_BITS, portMAX_DELAY );
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }  
}
