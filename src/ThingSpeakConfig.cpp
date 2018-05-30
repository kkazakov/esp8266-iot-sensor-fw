#include "ThingSpeakConfig.h"

int EEPROM_START_POSITION_TS = 180; // ssid = 32 + passwd = 32 + 3 = OK + 4 x openSenseMap

char thingSpeakWriteKey[18] = "";
char thingSpeakTemperatureField[10] = "";
char thingSpeakHumidityField[10] = "";
char thingSpeakPressureField[10] = "";

void loadThingSpeakSettings() {
  EEPROM.begin(512);
  EEPROM.get(EEPROM_START_POSITION_TS, thingSpeakWriteKey);
  EEPROM.get(EEPROM_START_POSITION_TS+sizeof(thingSpeakWriteKey), thingSpeakTemperatureField);
  EEPROM.get(EEPROM_START_POSITION_TS+sizeof(thingSpeakWriteKey)+sizeof(thingSpeakTemperatureField), thingSpeakHumidityField);
  EEPROM.get(EEPROM_START_POSITION_TS+sizeof(thingSpeakWriteKey)+sizeof(thingSpeakTemperatureField)+sizeof(thingSpeakHumidityField), thingSpeakPressureField);
  EEPROM.end();

  if (strlen(thingSpeakWriteKey) != 16) {
    thingSpeakWriteKey[0] = 0;
    thingSpeakTemperatureField[0] = 0;
    thingSpeakHumidityField[0] = 0;
    thingSpeakPressureField[0] = 0;
  }

  Serial.println("Recovered ThingSpeak settings:");
  Serial.println(strlen(thingSpeakWriteKey)>0?thingSpeakWriteKey: "<no thingSpeakWriteKey>");
  Serial.println(strlen(thingSpeakTemperatureField)>0?thingSpeakTemperatureField: "<no thingSpeakTemperatureField>");
  Serial.println(strlen(thingSpeakHumidityField)>0?thingSpeakHumidityField: "<no thingSpeakHumidityField>");
  Serial.println(strlen(thingSpeakPressureField)>0?thingSpeakPressureField: "<no thingSpeakPressureField>");

}

void saveThingSpeakSettings(String writeAPIKey, String temperatureField, String humidityField, String pressureField) {

  writeAPIKey.toCharArray(thingSpeakWriteKey, sizeof(thingSpeakWriteKey) - 1);
  temperatureField.toCharArray(thingSpeakTemperatureField, sizeof(thingSpeakTemperatureField) - 1);
  humidityField.toCharArray(thingSpeakHumidityField, sizeof(thingSpeakHumidityField) - 1);
  pressureField.toCharArray(thingSpeakPressureField, sizeof(thingSpeakPressureField) - 1);

  Serial.println("saving ...");
  Serial.println("Write Key: " + writeAPIKey);
  Serial.println("temperature field: " + temperatureField);
  Serial.println("humidity field: " + humidityField);
  Serial.println("pressure field: " + pressureField);


  EEPROM.begin(512);

  EEPROM.put(EEPROM_START_POSITION_TS, thingSpeakWriteKey);
  EEPROM.put(EEPROM_START_POSITION_TS+sizeof(thingSpeakWriteKey), thingSpeakTemperatureField);
  EEPROM.put(EEPROM_START_POSITION_TS+sizeof(thingSpeakWriteKey)+sizeof(thingSpeakTemperatureField), thingSpeakHumidityField);
  EEPROM.put(EEPROM_START_POSITION_TS+sizeof(thingSpeakWriteKey)+sizeof(thingSpeakTemperatureField)+sizeof(thingSpeakHumidityField), thingSpeakPressureField);

  EEPROM.commit();
  EEPROM.end();

}

String getThingSpeakAPIKey() {
  return String(thingSpeakWriteKey);
}

String getThingSpeakTemperatureField() {
  return String(thingSpeakTemperatureField);
}

String getThingSpeakHumidityField() {
  return String(thingSpeakHumidityField);
}

String getThingSpeakPressureField() {
  return String(thingSpeakPressureField);
}
