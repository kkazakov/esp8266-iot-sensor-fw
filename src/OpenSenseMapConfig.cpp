#include "OpenSenseMapConfig.h"

int EEPROM_START_POSITION_OSM = 70; // ssid = 32 + passwd = 32 + 3 = OK

char openSenseMapBoxId[26] = "";
char openSenseMapTempId[26] = "";
char openSenseMapHumidityId[26] = "";
char openSenseMapPressureId[26] = "";

void loadOpenSenseMapSettings() {
  EEPROM.begin(512);
  EEPROM.get(EEPROM_START_POSITION_OSM, openSenseMapBoxId);
  EEPROM.get(EEPROM_START_POSITION_OSM+sizeof(openSenseMapBoxId), openSenseMapTempId);
  EEPROM.get(EEPROM_START_POSITION_OSM+sizeof(openSenseMapBoxId)+sizeof(openSenseMapTempId), openSenseMapHumidityId);
  EEPROM.get(EEPROM_START_POSITION_OSM+sizeof(openSenseMapBoxId)+sizeof(openSenseMapTempId)+sizeof(openSenseMapHumidityId), openSenseMapPressureId);
  EEPROM.end();

  if (strlen(openSenseMapBoxId) != 24) {
    openSenseMapBoxId[0] = 0;
    openSenseMapTempId[0] = 0;
    openSenseMapHumidityId[0] = 0;
    openSenseMapPressureId[0] = 0;
  }

  Serial.println("Recovered OSM settings:");
  Serial.println(strlen(openSenseMapBoxId)>0?openSenseMapBoxId: "<no openSenseMapBoxId>");
  Serial.println(strlen(openSenseMapTempId)>0?openSenseMapTempId: "<no openSenseMapTempId>");
  Serial.println(strlen(openSenseMapHumidityId)>0?openSenseMapHumidityId: "<no openSenseMapHumidityId>");
  Serial.println(strlen(openSenseMapPressureId)>0?openSenseMapPressureId: "<no openSenseMapPressureId>");

}


void saveOpenSenseMapSettings(String senseBoxId, String senseBoxTemperatureId, String senseBoxHumidityId, String senseBoxPressureId) {
  senseBoxId.toCharArray(openSenseMapBoxId, sizeof(openSenseMapBoxId) - 1);
  senseBoxTemperatureId.toCharArray(openSenseMapTempId, sizeof(openSenseMapTempId) - 1);
  senseBoxHumidityId.toCharArray(openSenseMapHumidityId, sizeof(openSenseMapHumidityId) - 1);
  senseBoxPressureId.toCharArray(openSenseMapPressureId, sizeof(openSenseMapPressureId) - 1);

  Serial.println("saving ...");
  Serial.println("Box id: " + senseBoxId);
  Serial.println("temperature id: " + senseBoxTemperatureId);
  Serial.println("humidity id: " + senseBoxHumidityId);
  Serial.println("pressure id: " + senseBoxPressureId);


  EEPROM.begin(512);

  EEPROM.put(EEPROM_START_POSITION_OSM, openSenseMapBoxId);
  EEPROM.put(EEPROM_START_POSITION_OSM+sizeof(openSenseMapBoxId), openSenseMapTempId);
  EEPROM.put(EEPROM_START_POSITION_OSM+sizeof(openSenseMapBoxId)+sizeof(openSenseMapTempId), openSenseMapHumidityId);
  EEPROM.put(EEPROM_START_POSITION_OSM+sizeof(openSenseMapBoxId)+sizeof(openSenseMapTempId)+sizeof(openSenseMapHumidityId), openSenseMapPressureId);

  EEPROM.commit();
  EEPROM.end();

}

String getOpenSenseBoxId() {
  return String(openSenseMapBoxId);
}

String getOpenSenseTemperatureId() {
  return String(openSenseMapTempId);
}

String getOpenSenseHumidityId() {
  return String(openSenseMapHumidityId);
}

String getOpenSensePressureId() {
  return String(openSenseMapPressureId);
}
