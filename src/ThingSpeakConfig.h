#ifndef THINGSPEAK_CONFIG_H
#define THINGSPEAK_CONFIG_H

#include <Arduino.h>
#include <String.h>
#include <EEPROM.h>

void loadThingSpeakSettings();

void saveThingSpeakSettings(String writeAPIKey, String temperatureField, String humidityField, String pressureField);

String getThingSpeakAPIKey();
String getThingSpeakTemperatureField();
String getThingSpeakHumidityField();
String getThingSpeakPressureField();

#endif
