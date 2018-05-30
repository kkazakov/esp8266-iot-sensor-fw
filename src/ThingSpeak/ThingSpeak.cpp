#include "ThingSpeak.h"

const char * thingSpeakServer = "api.thingspeak.com";

/*
// 12.01 , XYZ, field1
void thingSpeakSend(float measurement, int digits, String writeAPIKey, String fieldName) {

  WiFiClient client;

  if (client.connect(thingSpeakServer, 80)) {

      char obs[10];
      dtostrf(measurement, 5, digits, obs);

      // Construct API request body
      String body = String(fieldName) + "=" + String(obs);

      Serial.print("TS: ");
      Serial.println(body);

      client.println("POST /update HTTP/1.1");
      client.println("Host: api.thingspeak.com");
      client.println("User-Agent: ESP8266 (nothans)/1.0");
      client.println("Connection: close");
      client.println("X-THINGSPEAKAPIKEY: " + writeAPIKey);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.println("Content-Length: " + String(body.length()));
      client.println("");
      client.print(body);

    }
    client.stop();

}
*/

void thingSpeakSendAll(float measurement1, String field1, float measurement2, String field2, float measurement3, String field3, int digits, String writeAPIKey) {

  WiFiClient client;

  if (client.connect(thingSpeakServer, 80)) {

      char obs[10];

      String body = "";

      if (field1.length() != 0) {
        dtostrf(measurement1, 5, digits, obs);
        body = String(field1) + "=" + String(obs);
      }
      if (field2.length() != 0) {
        dtostrf(measurement2, 5, digits, obs);
        body = body + "&" + String(field2) + "=" + String(obs);
      }
      if (field3.length() != 0) {
        dtostrf(measurement3, 5, digits, obs);
        body = body + "&" + String(field3) + "=" + String(obs);
      }

      client.println("POST /update HTTP/1.1");
      client.println("Host: api.thingspeak.com");
      client.println("User-Agent: ESP8266 (nothans)/1.0");
      client.println("Connection: close");
      client.println("X-THINGSPEAKAPIKEY: " + writeAPIKey);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.println("Content-Length: " + String(body.length()));
      client.println("");
      client.print(body);

    }


    delay(1000);

    client.stop();

}
