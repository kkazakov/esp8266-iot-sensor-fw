#ifndef THINGSPEAK_SENDER_H
#define THINGSPEAK_SENDER_H

#include <Arduino.h>
#include <String.h>
#include <WiFiClient.h>

void thingSpeakSend(float measurement, int digits, String writeAPIKey, String fieldName);
void thingSpeakSendAll(float measurement1, String field1, float measurement2, String field2, float measurement3, String field3, int digits, String writeAPIKey);

#endif
