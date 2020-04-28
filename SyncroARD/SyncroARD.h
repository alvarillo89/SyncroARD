/*
    SyncroARD.h - Library for task synchronization 
    on distributed cards.
    Created by Álvaro Fernández G., March 2020.
    Released under GPLv3 license.
*/

#ifndef __SyncroARD_h
#define __SyncroARD_h

#include <Wire.h>
#include <Arduino_FreeRTOS.h>
#include <event_groups.h>
#include "Arduino.h"


class SyncroM {
    public:
        static void begin();
        static void notifyI2C();
        //static void notifyGPIO();
        static void syncTask();
    private:
        static const byte _outPin = 2;
        static EventGroupHandle_t _xEventBits;
};


class SyncroS {
    public:
        static void begin(void(*I2CIrs)(int), void(*GPIOIsr)(void));
        static void attachTask(void (*task)(void));
        static void receiveI2C();
        //static void receiveGPIO();
        static void doSyncTask();
    private:
        const byte _inPin = 2;
        byte _flagI2C;
        //byte _flagGPIO;
        //byte _startupPulse;
};

#endif