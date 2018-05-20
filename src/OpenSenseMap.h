#ifndef OPENSENSEMAP_H
#define OPENSENSEMAP_H

#include <Arduino.h>
#include <String.h>
#include <EEPROM.h>

void loadOpenSenseMapSettings();

void saveOpenSenseMapSettings(String senseBoxId, String senseBoxTemperatureId, String senseBoxHumidityId, String senseBoxPressureId);

String getOpenSenseBoxId();
String getOpenSenseTemperatureId();
String getOpenSenseHumidityId();
String getOpenSensePressureId();

#endif
