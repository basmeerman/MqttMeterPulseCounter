#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <TaskSchedulerDeclarations.h>
#include <Meter.h>
#include "wifi.h"

#ifndef MQTT_H
#define MQTT_H

const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(5);

extern Task t_publish_report;
extern Task t_monitor_mqtt;

static PubSubClient mqtt_client(espClient);

void setup_mqtt();
void monitor_mqtt();
void publish_report();

#endif
