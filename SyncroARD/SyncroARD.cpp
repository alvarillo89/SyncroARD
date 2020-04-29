/*
    SyncroARD.h - Library for task synchronization 
    on distributed cards.
    Created by Álvaro Fernández G., March 2020.
    Released under GPLv3 license.
*/

#include "Arduino.h"
#include "SyncroARD.h"

/////////////////////////////////////////////////////////////////////////////////////////
// MASTER
/////////////////////////////////////////////////////////////////////////////////////////

// Sync Macros definition:
#define TASK_0_BIT  ( 1 << 0 )
#define TASK_1_BIT  ( 1 << 1 )
#define TASK_2_BIT  ( 1 << 2 )
#define ALL_SYNC_BITS ( TASK_0_BIT | TASK_1_BIT | TASK_2_BIT )


// Master static variables definitions:
EventGroupHandle_t SyncroM::_xEventBits;
const byte SyncroM::_outPin;


void SyncroM::begin() {
    _xEventBits = xEventGroupCreate();
    Wire.begin();
    pinMode(_outPin, OUTPUT);
    digitalWrite(_outPin, LOW);
}


void SyncroM::notifyI2C() {
    xEventGroupSync( _xEventBits, TASK_0_BIT, ALL_SYNC_BITS, portMAX_DELAY );
    Wire.beginTransmission(4);
    Wire.write(1);
    Wire.endTransmission();
}


void SyncroM::notifyGPIO() {
    xEventGroupSync( _xEventBits, TASK_2_BIT, ALL_SYNC_BITS, portMAX_DELAY );
    digitalWrite(_outPin, HIGH);
    delay(10);
    digitalWrite(_outPin, LOW);
}


void SyncroM::syncTask() {
    xEventGroupSync( _xEventBits, TASK_1_BIT, ALL_SYNC_BITS, portMAX_DELAY );
}


/////////////////////////////////////////////////////////////////////////////////////////
// SLAVE
/////////////////////////////////////////////////////////////////////////////////////////

// Slave static variables definitions:
const byte SyncroS::_inPin;
volatile byte SyncroS::_flagI2C;
volatile byte SyncroS::_flagGPIO;
volatile byte SyncroS::_startupPulse;
void_f SyncroS::_task;


void SyncroS::begin(int_f i2cIsr, void_f gpioIsr) {
    _flagI2C = 0;
    _flagGPIO = 0;
    _startupPulse = 0;
    Wire.begin(4);
    Wire.onReceive(i2cIsr);
    pinMode(_inPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_inPin), gpioIsr, RISING);
}


void SyncroS::attachTask(void_f task) {
    _task = task;
}


void SyncroS::receiveI2C() {
    _flagI2C = 1;
    Wire.read();
}


void SyncroS::receiveGPIO() {
    if(_startupPulse) {
        _flagGPIO = 1;
    } else {
        _startupPulse = 1;
    }
}


void SyncroS::doSyncTask() {
    if(_flagI2C || _flagGPIO) {
        _task();
        _flagI2C = 0;
        _flagGPIO = 0;
    }
}