#include "Sensor.h"


#if HAS_BME280
  Adafruit_BME280 bme;
#else
  Adafruit_BMP085 bmp;
#endif

boolean connect;

boolean hasSensor = true;

// set this (via setting or here to sleep mode = true in order to deep sleep)
boolean sleepMode = true;

boolean shouldStartWebServer = false; // do not modify it, will be set in code

long lastConnectTry = 0;

int status = WL_IDLE_STATUS;

const unsigned int postingInterval = 300000; // 300000 is 5 minutes. Fine tune for deep sleep mode
// const unsigned int postingInterval = 10000;

#define CLR_BTN 5 // GPIO5

unsigned long passed_millis = 0;

float temperature;
float pressure;
float humidity;
float altitude;

void setup () {
  delay(1000);
  Serial.begin(9600);

  Serial.println();
  Serial.println(FW_VERSION);

  SPIFFS.begin();

  Wire.begin(2, 0);

  pinMode(CLR_BTN, INPUT);
  delay(100);


  if (digitalRead(CLR_BTN) == LOW) {
    Serial.println("CLR PRESSED!!!");

    sleepMode = false; // we do not want to sleep.

    delay(5000);
    // depressed within 5 seconds? start web server
    if (digitalRead(CLR_BTN) == HIGH) {
        shouldStartWebServer = true;
    } else {

    // still low after 5 seconds? reset wifi + check for more

      // reset wifi settings
      Serial.println("resetting wi-fi settings ...");

      saveCredentials("", "");

      // if it was released after 5 seconds, reset the chip
      delay(5000);
      if (digitalRead(CLR_BTN) == LOW) {
        Serial.println("restarting ...");
        ESP.restart();
        return;
      }

      delay(10000);
      if (digitalRead(CLR_BTN) == LOW) {

        // still low after 15 + 5 = 20+ seconds? reset fully.
        Serial.println("resetting all settings and restarting ...");
      }

      // we do ESP restart after 15 seconds
      ESP.restart();
    }

  }


  #if HAS_BME280
    if (!bme.begin(BME280_ADDRESS, &Wire)) {
      Serial.println("No BME280");
      hasSensor = false;
    }
  #else
    if (!bmp.begin()) {
      Serial.println("No BMP180 / BMP085");
      hasSensor = false;
    }
  #endif

  loadCredentials();

  if (!hasSSIDConfigured()) {
    startSoftAp();
    startWebServer();
  }
  connect = hasSSIDConfigured();

  loadOpenSenseMapSettings();
  loadThingSpeakSettings();

  if (shouldStartWebServer) {
    startWebServer();
  }


  passed_millis = millis();
}

void loop () {

  if (connect) {
    Serial.println ( "Connect requested" );
    connect = false;
    connectWifi();
    lastConnectTry = millis();
  }

  if (hasSSIDConfigured()) {

    int s = WiFi.status();
    if (s == WL_IDLE_STATUS && millis() > (lastConnectTry + 60000) ) {
      /* If WLAN disconnected and idle try to connect */
      /* Don't set retry time too low as retry interfere the softAP operation */
      connect = true;
    }
    if (status != s) { // WLAN status change
      Serial.print ( "Status: " );
      Serial.println ( s );

/*
WL_NO_SHIELD = 255,
WL_IDLE_STATUS = 0,
WL_NO_SSID_AVAIL = 1
WL_SCAN_COMPLETED = 2
WL_CONNECTED = 3
WL_CONNECT_FAILED = 4
WL_CONNECTION_LOST = 5
WL_DISCONNECTED = 6
*/

      status = s;
      if (s == WL_CONNECTED) {
        /* Just connected to WLAN */
        Serial.println ( "" );
        Serial.print ( "Connected to " );
        Serial.println ( getSSID() );
        Serial.print ( "IP address: " );
        Serial.println ( WiFi.localIP() );

        //TEMPORARY, DISABLE WHEN NOT NEEDED:

        if (!sleepMode) {
          Serial.println ("Starting web server.");
          startWebServer();
        }

/*
        // Setup MDNS responder
        if (!MDNS.begin(getMyHostName())) {
          Serial.println("Error setting up MDNS responder!");
        } else {
          Serial.println("mDNS responder started");
          // Add service to MDNS-SD
          MDNS.addService("http", "tcp", 80);
        }
*/
      } else if (s == WL_NO_SSID_AVAIL) {

        // if we cannot find the SSID, go to deep sleep.
        // perhaps the SSID is not available at the moment, but will become
        // available later. Prev code started the Soft AP, but we don't want
        // this, as there's no going out of this mode and drains the battery.
        deepsleep(postingInterval * 1000);

        /*
        WiFi.disconnect();
        connect = false;
        startSoftAp();
        startWebServer();
        */
      }
    }

    if (s == WL_CONNECTED) {

      if (sleepMode) {
        measureAndSend();
        deepsleep(postingInterval * 1000);
      } else {

        unsigned long now = millis();
        unsigned long elapsed = now - passed_millis;

        if (passed_millis == 0 || elapsed >= 15000) {
          passed_millis = now;
          measureAndSend();
        }

        wifiHandleLoop();
        webServerHandleLoop();
      }

    }

  } else {

    // Soft AP mode, handle
    wifiHandleLoop();
    webServerHandleLoop();
  }

}

void measure() {
  if (hasSensor) {

    #if HAS_BME280
      temperature = bme.readTemperature();
      pressure = bme.readPressure();
      humidity = bme.readHumidity();
      altitude = bme.readAltitude(103000);
      String t = "T=" + String(temperature) + " *C, ";
      String p = "P=" + String(pressure) + " Pa, ";
      String h = "H=" + String(humidity) + " %, ";
      String a = "A=" + String(altitude) + " m";

      Serial.print(t);
      Serial.print(p);
      Serial.print(h);
      Serial.print(a);
    #else
      temperature = bmp.readTemperature();
      pressure = bmp.readPressure();
      altitude = bmp.readAltitude(103000);
      String t = "T=" + String(temperature) + " *C, ";
      String p = "P=" + String(pressure) + " Pa, ";
      String a = "A=" + String(altitude) + " m";

      Serial.print(t);
      Serial.print(p);
      Serial.print(a);
    #endif

    Serial.println();


  } else {
    #if HAS_BME280
      Serial.println("No BME280 sensor found, doing nothing.");
    #else
      Serial.println("No BMP180 sensor found, doing nothing.");
    #endif
  }
}

void measureAndSend() {

  measure();

  if (hasSensor) {

    // OpenSenseMap
    String senseBoxId = getOpenSenseBoxId();

    if (senseBoxId.length() != 0) {
      #if HAS_BME280
        Serial.println("BME sensor reading, posting to OpenSenseMap ... ");
      #else
        Serial.println("BMP sensor reading, posting to OpenSenseMap ... ");
      #endif


      String tempSensorId = getOpenSenseTemperatureId();

      if (tempSensorId.length() != 0 && temperature > -200 && temperature < 200) {
        openSenseMapSend(temperature, 1, senseBoxId, tempSensorId);
      }

      String humiditySensorId = getOpenSenseHumidityId();
      if (humiditySensorId.length() != 0) {
        openSenseMapSend(humidity, 1, senseBoxId, humiditySensorId);
      }

      String pressureSensorId = getOpenSensePressureId();
      if (pressureSensorId.length() != 0) {
        openSenseMapSend(pressure, 1, senseBoxId, pressureSensorId);
      }

    } else {
      Serial.println("OpenSenseMap not configured, skipping ...");
    }

    String thingSpeakWriteKey = getThingSpeakAPIKey();

    if (thingSpeakWriteKey.length() != 0) {

      #if HAS_BME280
        Serial.println("BME sensor reading, posting to ThingSpeak ... ");
      #else
        Serial.println("BMP sensor reading, posting to ThingSpeak ... ");
      #endif

      String tempSensorField = getThingSpeakTemperatureField();
      String humiditySensorField = getThingSpeakHumidityField();
      String pressureSensorField = getThingSpeakPressureField();

      // do not report temperature if weird readings
      if (tempSensorField.length() == 0 || temperature < -200 || temperature > 200) {
        tempSensorField = "";
      }

      thingSpeakSendAll(temperature, tempSensorField, humidity, humiditySensorField, pressure, pressureSensorField, 1, thingSpeakWriteKey);

    } else {
      Serial.println("ThingSpeak not configured, skipping ...");
    }




    Serial.println(" done.");

  }
}


// millis() rollover fix - http://arduino.stackexchange.com/questions/12587/how-can-i-handle-the-millis-rollover
void sleep(unsigned long ms) {            // ms: duration
  unsigned long start = millis();         // start: timestamp
  for (;;) {
    unsigned long now = millis();         // now: timestamp
    unsigned long elapsed = now - start;  // elapsed: duration
    if (elapsed >= ms)                    // comparing durations: OK
      return;
  }
}

void deepsleep(unsigned long micros) {
  Serial.println("Going into deep sleep");
  delay(100);
  ESP.deepSleep(micros);
}


float getTemperature() {
  return temperature;
}
float getHumidity() {
  return humidity;
}
float getPressure() {
  return pressure;
}
float getAltitude() {
  return altitude;
}
