#include <ESP8266WiFi.h>
#include <TaskSchedulerDeclarations.h>

#ifndef WIFI_H
#define WIFI_H

static WiFiClient espClient;

extern Task t_monitor_wifi;

void setup_wifi();
void monitor_wifi();
void ensure_wifi_connected();
void print_wifi_details();
#endif
