#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Wifi.h"
#include "FS.h"

void webServerHandleLoop();

void startWebServer();

void sendHeader();

void handleRoot();
void handleWifi();

void handleReboot();

void handleWifiSave();
void handleWifiSaved();
void handleNotFound();

void handleOSM();
void handleOSMSave();
void handleOSMSaved();

//void handleCSS();
//void handleJS();

bool isIp(String str);
String toStringIp(IPAddress ip);

#endif
