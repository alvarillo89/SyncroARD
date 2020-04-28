/*
    SyncroARD.h - Library for task synchronization 
    on distributed cards.
    Created by Álvaro Fernández G., March 2020.
    Released under GPLv3 license.
*/

#include "Arduino.h"
#include "SyncroARD.h"


// Sync Macros definition:
#define TASK_0_BIT  ( 1 << 0 )
#define TASK_1_BIT  ( 1 << 1 )
//#define TASK_2_BIT  ( 1 << 2 )
#define ALL_SYNC_BITS ( TASK_0_BIT | TASK_1_BIT )


// Master static variables definitions:
EventGroupHandle_t SyncroM::_xEventBits;
const byte SyncroM::_outPin;


void SyncroM::begin() {
    _xEventBits = xEventGroupCreate();
    Wire.begin();
}


void SyncroM::notifyI2C() {
    xEventGroupSync( _xEventBits, TASK_0_BIT, ALL_SYNC_BITS, portMAX_DELAY );
    Wire.beginTransmission(4);
    Wire.write(1);
    Wire.endTransmission();
}


void SyncroM::syncTask() {
    xEventGroupSync( _xEventBits, TASK_1_BIT, ALL_SYNC_BITS, portMAX_DELAY );
}