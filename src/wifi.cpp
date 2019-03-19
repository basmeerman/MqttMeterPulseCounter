#include <ESP8266WiFi.h>
#include <TaskSchedulerDeclarations.h>
#include "led.h"
#include "wifi.h"
#include "config.h"

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect (false);
  WiFi.setAutoReconnect (true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting in 5 seconds...");
    delay(5000);
    //TODO replace delay
    ESP.restart();
  }
}

void print_wifi_details() {
  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());
}

bool wifi_connected() {
  return WiFi.status() == WL_CONNECTED;
}

void ensure_wifi_connected() {
  monitor_wifi();
}

void monitor_wifi() {
  while (!wifi_connected()) {
    toggle_led(HIGH);
    Serial.print("Attempting Wifi connection...");
    setup_wifi();
    toggle_led(LOW);
  }
}
