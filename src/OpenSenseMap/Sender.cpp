#include "Sender.h"


WiFiClient client;

char openSenseMapServer[] = "ingress.opensensemap.org";

void openSenseMapSend(float measurement, int digits, String senseBoxId, String sensorId) {

  // Convert Float to String
  char obs[10];
  dtostrf(measurement, 5, digits, obs);

  String jsonValue = "{\"value\":";
  jsonValue += obs;
  jsonValue += "}";

  //Serial.println("-------------------------------------");
  //Serial.print("Connecting to OSeM Server... ");
  if (client.connect(openSenseMapServer, 80)) {
    //Serial.println("connected!");
    //Serial.println("-------------------------------------");

    client.print("POST /boxes/"); client.print(senseBoxId); client.print("/"); client.print(sensorId); client.println(" HTTP/1.1");
    client.print("Host:");
    client.println(openSenseMapServer);
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.print("Content-Length: "); client.println(jsonValue.length());
    client.println();

    client.println(jsonValue);
  } else {
    //Serial.println("failed!");
    //Serial.println("-------------------------------------");
  }

  waitForServerResponse();
}




void waitForServerResponse() {

  boolean repeat = true;
  do {
    if (client.available()) {
      char c = client.read();
      //Serial.print(c);
    }

    if (!client.connected()) {
      // Serial.println();
      // Serial.println("--------------");
      // Serial.println("Disconnecting.");
      // Serial.println("--------------");
      client.stop();
      repeat = false;
    }
  } while (repeat);
}
