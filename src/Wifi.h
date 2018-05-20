#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <EEPROM.h>

void startSoftAp();
void loadCredentials();
void saveCredentials(String ssidStr, String passwordStr);


const char* getMyHostName();

char* getSSID();
char* getPASSWD();

const char* getSoftAPSSID();

// whether we have ssid and should connect to it
bool hasSSIDConfigured();


void connectWifi();

void wifiHandleLoop();

#endif
