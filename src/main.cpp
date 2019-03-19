#include "config.h"
#include "led.h"
#include "ota.h"
#include "wifi.h"
#include "mqtt.h"
#include "meters.h"
#include "tasks.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  
  setup_led();
  setup_wifi();
  setup_mqtt();
  setup_ota();
  setup_meters();
  setup_tasks();

  print_wifi_details();
}

void loop() {
  execute_tasks();
}
