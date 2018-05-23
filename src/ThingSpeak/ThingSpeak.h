#ifndef THINGSPEAK_SENDER_H
#define THINGSPEAK_SENDER_H

#include <Arduino.h>
#include <String.h>
#include <WiFiClient.h>

void thingSpeakSend(float measurement, int digits, String writeAPIKey, String fieldName);

#endif
