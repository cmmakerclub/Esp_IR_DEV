#include <MqttConnector.h>
#include "DHT.h"

MqttConnector::prepare_data_hook_t on_prepare_data =
[&](JsonObject * root) -> void {
    float t_dht = 0;
    float h_dht = 0;

    JsonObject& data = (*root)["d"];
    JsonObject& info = (*root)["info"];

    data["myName"] = DEVICE_NAME;
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)  ) {
      Serial.println("Failed to read from DHT sensor!");
      data["humid"] = -1.0;
      data["temp"] = -1.0;
    }
    else {
      data["humid"] = h;
      data["temp"] = t;
    }
};
