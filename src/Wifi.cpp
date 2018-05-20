#include "Wifi.h"


DNSServer dnsServer;
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

const char *softAP_ssid = "ESP_ap";
const char *myHostname = "esp8266";
const byte DNS_PORT = 53;

/* Don't set these wifi credentials. They are configured at runtime and stored on EEPROM */

char ssid[32] = "";
char password[32] = "";

void startSoftAp() {
  Serial.print("Configuring access point...");
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid);
  delay(500);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */
  //dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  //dnsServer.start(DNS_PORT, "*", apIP);

}

const char* getMyHostName() {
  return myHostname;
}

char* getSSID() {
  return ssid;
}

char* getPASSWD() {
  return password;
}

const char* getSoftAPSSID() {
  return softAP_ssid;
}

bool hasSSIDConfigured() {
  return strlen(ssid) > 0;
}



void connectWifi() {
  Serial.println("Connecting as wifi client...");
  WiFi.disconnect();
  WiFi.begin ( ssid, password );
  int connRes = WiFi.waitForConnectResult();
  Serial.print ( "connRes: " );
  Serial.println ( connRes );
}




void loadCredentials() {
  EEPROM.begin(512);
  EEPROM.get(0, ssid);
  EEPROM.get(0+sizeof(ssid), password);
  char ok[2+1];
  EEPROM.get(0+sizeof(ssid)+sizeof(password), ok);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    ssid[0] = 0;
    password[0] = 0;
  }
  Serial.println("Recovered credentials:");
  Serial.println(strlen(ssid)>0?ssid: "<no ssid>");
  Serial.println(strlen(password)>0?"********":"<no password>");
}

/** Store WLAN credentials to EEPROM */
void saveCredentials(String ssidStr, String passwordStr) {


  ssidStr.toCharArray(ssid, sizeof(ssid) - 1);
  passwordStr.toCharArray(password, sizeof(password) - 1);

  Serial.println("Saving credentials: ");
  Serial.println(strlen(ssid)>0?ssid: "<no ssid>");
  Serial.println(strlen(password)>0? password :"<no password>");


  EEPROM.begin(512);
  EEPROM.put(0, ssid);
  EEPROM.put(0+sizeof(ssid), password);
  char ok[2+1] = "OK";
  EEPROM.put(0+sizeof(ssid)+sizeof(password), ok);
  EEPROM.commit();
  EEPROM.end();
}

void wifiHandleLoop() {
  dnsServer.processNextRequest();
}
