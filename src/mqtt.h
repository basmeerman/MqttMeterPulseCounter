#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <TaskSchedulerDeclarations.h>
#include <Meter.h>
#include "wifi.h"

#ifndef MQTT_H
#define MQTT_H

extern Task t_publish_report;
extern Task t_monitor_mqtt;

static PubSubClient mqtt_client(espClient);

void setup_mqtt();
void monitor_mqtt();
void publish_mqtt(JsonObject &json, const char *topic);
void publish_mqtt(const char *message, const char *topic);
#endif
