#ifndef SENSOR_H
#define SENSOR_H

#define HAS_BME280 true // set to false for BMP180

#include <Arduino.h>
#include <String.h>
#include <Wire.h>

#if HAS_BME280

  #include <Adafruit_Sensor.h>
  #include <Adafruit_BME280.h>

  #define BME280_ADDRESS (0x76)

#else

  #include <Adafruit_BMP085.h>

#endif

#include "Settings.h"
#include "OpenSenseMap/Sender.h"
#include "WebServer.h"
#include "Wifi.h"
#include "OpenSenseMap.h"

void measure();
void measureAndSend();
void sleep(unsigned long ms);
void deepsleep(unsigned long micros);

float getTemperature();
float getHumidity();
float getPressure();
float getAltitude();

#endif
