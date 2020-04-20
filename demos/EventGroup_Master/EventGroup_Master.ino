#include <Wire.h>
#include <Arduino_FreeRTOS.h>
#include <event_groups.h>


// Sync Stuff:
EventGroupHandle_t xEventBits;
#define TASK_0_BIT  ( 1 << 0 )
#define TASK_1_BIT  ( 1 << 1 )
#define ALL_SYNC_BITS ( TASK_0_BIT | TASK_1_BIT )

int tickStart;
int tickEnd;
int lastDeltaTick;

// RTOS tasks:
void Notify( void *pvParameters );
void BlinkLed( void *pvParameters );


void setup() {
  Serial.begin(9600);
  Wire.begin(5);
  xEventBits = xEventGroupCreate();

  if( xEventBits != NULL ) {
    xTaskCreate(Notify, "Notify", 128, NULL, 1, NULL);
    xTaskCreate(BlinkLed, "BlinkLed", 128, NULL, 1, NULL);
  }
}

void loop(){ delay(10000); }

void Notify( void *pvParameters ) {
  (void) pvParameters;

  xEventGroupSync( xEventBits, TASK_0_BIT, ALL_SYNC_BITS, portMAX_DELAY );
  Wire.beginTransmission(4);
  Wire.write(1);              
  Wire.endTransmission();
  vTaskSuspend( NULL );
}


void BlinkLed( void *pvParameters ) {
  (void) pvParameters;
  pinMode(LED_BUILTIN, OUTPUT);

  xEventGroupSync( xEventBits, TASK_1_BIT, ALL_SYNC_BITS, portMAX_DELAY );

  for(;;) {
    tickStart = xTaskGetTickCount();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    tickEnd = xTaskGetTickCount();
    lastDeltaTick = tickEnd - tickStart;

    Wire.requestFrom(4, 1);    // request 6 bytes from slave device #2
    int c = Wire.read(); // receive a byte as character
    Serial.println(c);
  }  
}
