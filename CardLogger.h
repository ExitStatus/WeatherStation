#ifndef _CARDLOGGER_H
#define _CARDLOGGER_H

#include <SPI.h>
#include <SD.h>
#include <RtcDS3231.h>

#include "Hardware.h"
#include "ClockTime.h"
#include "SensorData.h"

class CardLogger
{
    private:
        bool _enabled = false;
        uint8_t _cardType;
        uint64_t _cardSize;

    public: 
        CardLogger();

        void Record(ClockTime *clockTime, float temperature, float humidity, float pressure);
        void WriteSensorData(uint8_t type, SensorData **allocData, int length);
        void ReadSensorData(uint8_t type, SensorData **data, int nItems);
};

#endif