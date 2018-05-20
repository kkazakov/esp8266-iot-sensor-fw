#ifndef OPENSENSEMAP_SENDER_H
#define OPENSENSEMAP_SENDER_H

#include <Arduino.h>
#include <String.h>
#include <WiFiClient.h>

void openSenseMapSend(float measurement, int digits, String senseBoxId, String sensorId);
void waitForServerResponse();

#endif
