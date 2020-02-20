#ifndef _CARDLOGGER_H
#define _CARDLOGGER_H

#include <SPI.h>
#include <SD.h>
#include <RtcDS3231.h>

#include "Hardware.h"

class CardLogger
{
    private:
        bool _enabled = false;
        uint8_t _cardType;
        uint64_t _cardSize;

    public: 
        CardLogger();

        void Record(RtcDateTime dateStamp, float temperature, float humidity, float pressure);
};

#endif