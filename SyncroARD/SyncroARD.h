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
        static void notifyGPIO();
        static void syncTask();
    private:
        static const byte _outPin = 2;
        static EventGroupHandle_t _xEventBits;
};


typedef void (*void_f)();
typedef void (*int_f)(int);

class SyncroS {
    public:
        static void begin(int_f i2cIsr, void_f gpioIsr);
        static void attachTask(void_f task);
        static void receiveI2C();
        static void receiveGPIO();
        static void doSyncTask();
    private:
        static const byte _inPin = 2;
        static volatile byte _flagI2C;
        static volatile byte _flagGPIO;
        static volatile byte _startupPulse;
        static void_f _task;
};

#endif