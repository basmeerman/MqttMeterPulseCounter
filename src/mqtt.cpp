#include <TaskSchedulerDeclarations.h>
#include "led.h"
#include "mqtt.h"
#include "wifi.h"
#include "config.h"
#include "meters.h"

void setup_mqtt() {
  mqtt_client.setServer(MQTT_SERVER, 1883);
}

bool mqtt_connected(){
  return mqtt_client.connected();
}

void ensure_mqtt_connected() {
  monitor_mqtt();
}

void monitor_mqtt() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    toggle_led(HIGH);
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect(NODE_NAME)) {
      Serial.println("mqtt connected");
      toggle_led(LOW);
      delay(1000);
      //TODO replace delay
    } else {
      WiFi.printDiag(Serial);
      Serial.print("mqtt failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      //TODO replace delay
    }
  }
}

void publish_mqtt(JsonObject &json, const char *topic)
{
  ensure_wifi_connected();
  ensure_mqtt_connected();

  mqtt_client.loop();

  char buffer[256];
  serializeJson(json, buffer);
  
  mqtt_client.publish(topic, buffer);
}

void publish_mqtt(const char *message, const char *topic)
{
  ensure_wifi_connected();
  ensure_mqtt_connected();

  mqtt_client.loop();

  mqtt_client.publish(topic, message);
}
