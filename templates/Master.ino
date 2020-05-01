/*
    Master.ino: synchronization template for the 
    master card.
    Created by Álvaro Fernández G., March 2020.
    Released under GPLv3 license.
*/

#include <Arduino_FreeRTOS.h>   // FreeRTOS required on master
#include <SyncroARD.h>          // Include SyncroARD Library


// FreeRTOS tasks definitions:
void I2C( void *pvParameters );
void GPIO( void *pvParameters );
void Task( void *pvParameters );


void setup() {
  // Init library. Use SyncroM class for master
  SyncroM::begin();
  // Create the FreeRTOS tasks. They must all have the same priority.
  xTaskCreate(I2C, "I2C", 128, NULL, 1, NULL);    // I2C communication
  xTaskCreate(GPIO, "GPIO", 128, NULL, 1, NULL);  // GPIO communication
  xTaskCreate(Task, "Task", 128, NULL, 1, NULL);  // Your task
  /*
      * Other initialization stuff
      * ...
      * ...
  */
}


// Empty main loop
void loop() { delay(10000); }


// This code block carries out communication via I2C. 
// If you don't want to use it, you can delete it.
// Keep both functions (I2C, GPIO) if you want to have redundancy
void I2C( void *pvParameters ) {
  (void) pvParameters;
  for(;;) { SyncroM::notifyI2C(); }
}


// This code block carries out communication via GPIO. 
// If you don't want to use it, you can delete it.
// Keep both functions (I2C, GPIO) if you want to have redundancy
void GPIO( void *pvParameters ) {
  (void) pvParameters;
  for(;;) { SyncroM::notifyGPIO(); }
}


// Your task
void Task( void *pvParameters ) {
  (void) pvParameters;

  for(;;) {
    SyncroM::syncTask();
    /*
        * YOUR TASK CODE GOES HERE
        * ...
        * ...
    */
  }  
}