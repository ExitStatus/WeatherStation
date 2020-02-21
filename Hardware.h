#ifndef _HARDWARE_H
#define _HARDWARE_H

// Which ESP32 pins are the I2C bus connected to
#define PIN_SDA     25
#define PIN_SCL     26

// What speed to set the I2C bus to
#define I2C_SPEED   1000000

// The I2C Address for the OLED display
#define I2C_OLED_ADDRESS    0x3c

// The I2C Address for the AHT10 temperature and humidity sensor
#define I2C_AHT10_ADDRESS   0x38

// The I2C Address for the BMP280 pressure sensor
#define I2C_BMP280_ADDRESS  0x76

// Which ESP32 pins are the 3 control buttons connected to
#define PIN_LEFT    12
#define PIN_MIDDLE  14
#define PIN_RIGHT   27

// Which ESP32 pin is the SDCARD Module SPI CS connected to
#define SPI_CS_SDCARD   17

#endif