#include "OpenSenseMap.h"

char openSenseMapServer[] = "ingress.opensensemap.org";

void openSenseMapSend(float measurement, int digits, String senseBoxId, String sensorId) {

  WiFiClient client;

  // Convert Float to String
  char obs[10];
  dtostrf(measurement, 5, digits, obs);

  String jsonValue = "{\"value\":";
  jsonValue += obs;
  jsonValue += "}";

  if (client.connect(openSenseMapServer, 80)) {
    client.print("POST /boxes/"); client.print(senseBoxId); client.print("/"); client.print(sensorId); client.println(" HTTP/1.1");
    client.print("Host:");
    client.println(openSenseMapServer);
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.print("Content-Length: "); client.println(jsonValue.length());
    client.println();

    client.println(jsonValue);
  }

  client.stop();
}
