/*
    Slave.ino: synchronization template for the 
    slave card.
    Created by Álvaro Fernández G., March 2020.
    Released under GPLv3 license.
*/

// FreeRTOS is optional in Slave
#include <SyncroARD.h>     // Include SyncroARD Library


void setup() {
  // Init library. Use SyncroS class for slave.
  // It receives the functions that manage the interruption 
  // of I2C and GPIO respectively. If you don't want to use 
  // any of them, pass NULL as parameter
  SyncroS::begin(I2C, GPIO);
  // Tell the library the function of your task. It must 
  // return void and not take arguments
  SyncroS::attachTask(Task);
  /*
      * Other initialization stuff
      * ...
      * ...
  */
}


// Keep the main loop like this. This will call your linked task 
// at the right time
void loop() {
  SyncroS::doSyncTask();
}


// This code block carries out communication via I2C. 
// If you don't want to use it, you can delete it.
// Keep both functions (I2C, GPIO) if you want to have redundancy
void I2C(int howMany) {
  SyncroS::receiveI2C();
}


// This code block carries out communication via GPIO. 
// If you don't want to use it, you can delete it.
// Keep both functions (I2C, GPIO) if you want to have redundancy
void GPIO() {
  SyncroS::receiveGPIO();
}


// Task must return void and not take arguments
void Task() {
  /*
        * YOUR TASK CODE GOES HERE
        * WITHOUT INFINITE LOOPS
        * ...
    */
}
