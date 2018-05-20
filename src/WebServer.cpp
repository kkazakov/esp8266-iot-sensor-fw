#include "WebServer.h"
#include "ESPTemplateProcessor.h"
#include "Settings.h"

#include "Sensor.h"

ESP8266WebServer webServer(80);

void webServerHandleLoop() {
  webServer.handleClient();
}

void startWebServer() {
    webServer.on("/", handleRoot);
    webServer.on("/wifi", handleWifi);
    webServer.on("/reboot", handleReboot);
    webServer.on("/wifisave", handleWifiSave);
    webServer.on("/wifisaved", handleWifiSaved);

    webServer.on("/osm", handleOSM);
    webServer.on("/osmsave", handleOSMSave);
    webServer.on("/osmsaved", handleOSMSaved);

    webServer.serveStatic("/beauter.min.css", SPIFFS, "/beauter.min.css");
    webServer.serveStatic("/beauter.min.js", SPIFFS, "/beauter.min.js");
    webServer.serveStatic("/custom.js", SPIFFS, "/custom.js");

    webServer.onNotFound ( handleNotFound );
    webServer.begin();
    Serial.println("HTTP server started");
}

void sendHeader() {
  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.sendHeader("Expires", "-1");
  webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer.send(200, "text/html", "");
}

String getTopNav(const String& page) {
  String s = "<ul class=\"topnav\" id=\"myTopnav2\">\n\t\t\t<li><a href=\"/\" class=\"brand\">ESP Sensor</a></li>\n\t\t\t<li><a href=\"/\" ";

  if (page == "home") {
    s = s + String(" class=\"active\"");
  }
  s = s + String(">Home</a></li>\n\t\t\t<li><a href=\"/wifi\"");
  if (page == "wifi") {
    s = s + String(" class=\"active\"");
  }
  s = s + String(">Wi-Fi</a></li>\n\t\t\t<li><a href=\"/osm\"");
  if (page == "osm") {
    s = s + String(" class=\"active\"");
  }
  s = s + String(">OpenSenseMap</a></li>\n\t\t\t<li style=\"float:right;\"><a href=\"javascript:askRebootFirmware();\" >Restart</a></li>\n\t\t\t<li class=\"-icon\">\n\t\t\t\t<a href=\"javascript:void(0);\" onclick=\"topnav('myTopnav2')\">☰</a>\n\t\t\t</li>\n\n\t\t</ul>");
  return s;
}

String getTitle(const String& page) {

  if (page == "wifi") {
    return "Wi-Fi settings";
  } else if (page == "osm") {
    return "OpenSenseMap settings";
  }

  return String(FW_VERSION);
}

String defaultProcessor(const String& key, const String& page) {

  if (key == "NAV") return getTopNav(page);
  if (key == "TITLE") return getTitle(page);
  return "";
}

String indexProcessor(const String& key) {

  String output = defaultProcessor(key, "home");

  if (output != "") {
    return output;
  }

  // custom keys here
  if (key == "WIFI_SSID") {
    if (getSSID[0] == 0) {
      return getSSID();
    } else {
      return getSoftAPSSID();
    }
  }

  if (key == "PASSWD") {
    return getPASSWD();
  }

  if (key == "IP") {
    return webServer.client().remoteIP().toString();
  }

  if (key == "READINGS") {

    String s = "<p>&nbsp;</p>";



    s = s + "<p>Temperature: <strong>" + String(getTemperature()) + " *C</strong></p>";
    s = s + "<p>Pressure: <strong>" + String(getPressure()) + " Pa</strong></p>";
    s = s + "<p>Humidity: <strong>" + String(getHumidity()) + " %</strong></p>";
    s = s + "<p>Altitude: <strong>" + String(getAltitude()) + " m</strong></p>";

    s = s + "<p>&nbsp;</p>";

    return s;
  }


  return key;
}



void handleRoot() {
  if (!ESPTemplateProcessor(webServer).send(String("/index.html"), indexProcessor)) {
    webServer.send(200, "text/plain", "page not found.");
  }
}

bool passwdSaved = false;

String wifiProcessor(const String& key) {

  String output = defaultProcessor(key, "wifi");

  if (output != "") {
    return output;
  }

  if (key == "WIFI_SAVED") {
    if (passwdSaved) {
      passwdSaved = false;
      return "<div class='alert _success'>Network configuration saved. Please, restart to connect.</div>";
    } else {
      return "&nbsp;";
    }
  }

/*
  if (key == "PASSWD") {
    return getPASSWD();
  }
*/
  if (key == "WIFI_LIST") {

    String s = "";

    Serial.println("scan start");
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n > 0) {
      for (int i = 0; i < n; i++) {

        s = s + String("<option value='");
        s = s + WiFi.SSID(i);

        if (String(getSSID()) == WiFi.SSID(i)) {
          s = s + "' selected>";
        } else {
          s = s + String("'>");
        }

        s = s + WiFi.SSID(i);
        if (WiFi.encryptionType(i) != ENC_TYPE_NONE) {
          s = s + " *";
        }
        s = s + " (" + WiFi.RSSI(i) + ")";
        s = s + String("</option>");

//        webServer.sendContent(String() + "\r\n<tr><td>SSID " + WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":" *") + " (" + WiFi.RSSI(i) + ")</td></tr>");
      }

      return s;
    } else {
      return "<option value=''>No SSID found, please reload.</option>";
    }

  }

  //if (key == "BODY") return "Wi-Fi PAGE";

  return key;
}

void handleWifi() {
  if (!ESPTemplateProcessor(webServer).send(String("/wifi.html"), wifiProcessor)) {
    webServer.send(200, "text/plain", "page not found.");
  }
}

bool configSaved = false;


String osmProcessor(const String& key) {

  String output = defaultProcessor(key, "osm");

  if (output != "") {
    return output;
  }

  if (key == "CONFIG_SAVED") {
    if (configSaved) {
      configSaved = false;
      return "<div class='alert _success'>OpenSenseMap configuration saved. Please, restart to apply.</div>";
    } else {
      return "&nbsp;";
    }
  }

  if (key == "BOX_ID") {
    return getOpenSenseBoxId();
  }
  if (key == "TEMPERATURE") {
    return getOpenSenseTemperatureId();
  }
  if (key == "HUMIDITY") {
    return getOpenSenseHumidityId();
  }
  if (key == "PRESSURE") {
    return getOpenSensePressureId();
  }

  return key;

}

void handleOSM() {
  if (!ESPTemplateProcessor(webServer).send(String("/osm.html"), osmProcessor)) {
    webServer.send(200, "text/plain", "page not found.");
  }
}


void handleOSMSave() {
  Serial.println("Client accessed /osmsave");

  saveOpenSenseMapSettings(webServer.arg("b"), webServer.arg("t"), webServer.arg("h"), webServer.arg("p"));

  webServer.sendHeader("Location", "/osmsaved", true);
  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer.sendHeader("Expires", "-1");
  webServer.send ( 302, "text/plain", "");
  webServer.client().stop();
}

void handleOSMSaved() {
  configSaved = true;
  if (!ESPTemplateProcessor(webServer).send(String("/osm.html"), osmProcessor)) {
    webServer.send(200, "text/plain", "page not found.");
  }
}

void handleReboot() {
  ESP.reset();
}

/*

void handleWifi() {
  sendHeader();

  Serial.println("Client accessed /wifi");

  webServer.sendContent(
    "<html><head></head><body>"
    "<h1>Wifi config</h1>"
  );
  webServer.sendContent(String("<p>You are connected through the soft AP: ") + getSoftAPSSID() + "</p>");

  webServer.sendContent(
    "\r\n<br />"
    "<table><tr><th align='left'>SoftAP config</th></tr>"
  );
  webServer.sendContent(String() + "<tr><td>SSID " + String(getSoftAPSSID()) + "</td></tr>");
  webServer.sendContent(String() + "<tr><td>IP " + toStringIp(WiFi.softAPIP()) + "</td></tr>");
  webServer.sendContent(
    "</table>"
    "\r\n<br />"
    "<table><tr><th align='left'>WLAN config</th></tr>"
  );
  webServer.sendContent(String() + "<tr><td>SSID " + String(getSSID()) + "</td></tr>");
  webServer.sendContent(String() + "<tr><td>IP " + toStringIp(WiFi.localIP()) + "</td></tr>");
  webServer.sendContent(
    "</table>"
    "\r\n<br />"
    "<table><tr><th align='left'>WLAN list (refresh if any missing)</th></tr>"
  );
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      webServer.sendContent(String() + "\r\n<tr><td>SSID " + WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":" *") + " (" + WiFi.RSSI(i) + ")</td></tr>");
    }
  } else {
    webServer.sendContent(String() + "<tr><td>No WLAN found</td></tr>");
  }
  webServer.sendContent(
    "</table>"
    "\r\n<br /><form method='POST' action='wifisave'><h4>Connect to network:</h4>"
    "<input type='text' placeholder='network' name='n'/>"
    "<br /><input type='password' placeholder='password' name='p'/>"
    "<br /><input type='submit' value='Connect/Disconnect'/></form>"
    "<p>You may want to <a href='/'>return to the home page</a>.</p>"
    "</body></html>"
  );
  webServer.client().stop(); // Stop is needed because we sent no content length
}
*/

void handleWifiSave() {
  Serial.println("Client accessed /wifisave");

  saveCredentials(webServer.arg("n"), webServer.arg("p"));

  webServer.sendHeader("Location", "/wifisaved", true);
  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer.sendHeader("Expires", "-1");
  webServer.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  webServer.client().stop(); // Stop is needed because we sent no content length
}


void handleWifiSaved() {
  passwdSaved = true;
  if (!ESPTemplateProcessor(webServer).send(String("/wifi.html"), wifiProcessor)) {
    webServer.send(200, "text/plain", "page not found.");
  }
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webServer.uri();
  message += "\nMethod: ";
  message += ( webServer.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webServer.args();
  message += "\n";

  for ( uint8_t i = 0; i < webServer.args(); i++ ) {
    message += " " + webServer.argName ( i ) + ": " + webServer.arg ( i ) + "\n";
  }
  sendHeader();
  webServer.send ( 404, "text/plain", message );
}

// Tools

bool isIp(String str) {
  for (int i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}
