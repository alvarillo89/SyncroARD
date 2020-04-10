/////////////////////////////////////////////////////
// References:
// https://www.freertos.org/xEventGroupWaitBits.html
// https://www.freertos.org/xEventGroupSetBits.html
// https://www.freertos.org/xEventGroupCreate.html
// https://www.freertos.org/xEventGroupSync.html
/////////////////////////////////////////////////////

#include <Arduino_FreeRTOS.h>
#include <event_groups.h>


// Sync Stuff:
EventGroupHandle_t xEventBits;
#define TASK_0_BIT  ( 1 << 0 )
#define TASK_1_BIT  ( 1 << 1 )
#define ALL_SYNC_BITS ( TASK_0_BIT | TASK_1_BIT )


// RTOS tasks:
void Task0( void *pvParameters );
void Task1( void *pvParameters );


void setup() {
  Serial.begin(9600);
  xEventBits = xEventGroupCreate();

  if( xEventBits == NULL ){
    Serial.println("An error occurred!");
  } else {
    // Create tasks:
    xTaskCreate(Task0, "Task0", 128, NULL, 1, NULL);
    xTaskCreate(Task1, "Task1", 128, NULL, 1, NULL);
  }
}

void loop(){ }

void Task0( void *pvParameters ) {
  (void) pvParameters;

  Serial.println("TASK0: I'm waiting for one!");
  xEventGroupSync( xEventBits, TASK_0_BIT, ALL_SYNC_BITS, portMAX_DELAY );

  for(;;) {
    Serial.println("TASK0: I'm on for loop!");
  }
}


void Task1( void *pvParameters ) {
  (void) pvParameters;

  // Wait 6 seconds:
  vTaskDelay( 6000 / portTICK_PERIOD_MS );
  xEventGroupSync( xEventBits, TASK_1_BIT, ALL_SYNC_BITS, portMAX_DELAY );

  for(;;) {
    Serial.println("TASK1: I'm ready and looping!");
  }
}
