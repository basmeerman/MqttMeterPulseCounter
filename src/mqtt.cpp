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

void mqtt_publish_meter_reading(Meter* meter) {
  // if (!meter.has_readings()) return;
  StaticJsonDocument<JSON_BUFFER_SIZE> jsondoc;
  
  // create JSON
  JsonObject root = jsondoc.to<JsonObject>();
  root["count"] = meter->count;
  root["power"] = meter->get_power();
  root["kwhr"] = meter->elapsed_kwhr;
  root["delay"] = meter->pulse_length;
  root["avg_power"] = meter->average_power.avg();
  
  // write JSON to buffer
  char buffer[256];
  serializeJson(root, buffer);
  
  // publish
  char topic[20];
  snprintf(topic, 20, "meter/%d/reading", meter->id);
  mqtt_client.publish(topic, buffer);
}

void mqtt_publish_uptime() {
  static char buffer[10];
  snprintf(buffer, 10, "%lu", millis() / 1000);
  mqtt_client.publish("meter/uptime", buffer);
}

void publish_report() {
  ensure_wifi_connected();
  ensure_mqtt_connected();

  mqtt_client.loop();

  toggle_led(HIGH);
  
  mqtt_publish_meter_reading(&meter1);
  mqtt_publish_meter_reading(&meter2);
  mqtt_publish_meter_reading(&meter3);
  mqtt_publish_uptime();
  
  toggle_led(LOW);
}
