#include "ThingSpeak.h"

const char * thingSpeakServer = "api.thingspeak.com";

// 12.01 , XYZ, field1
void thingSpeakSend(float measurement, int digits, String writeAPIKey, String fieldName) {

  WiFiClient client;

  if (client.connect(thingSpeakServer, 80)) {

      char obs[10];
      dtostrf(measurement, 5, digits, obs);

      // Construct API request body
      String body = String(fieldName) + "=" + String(obs);

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
