#include <Meter.h>
#include <TaskSchedulerDeclarations.h>
#include "meters.h"
#include "config.h"
#include "mqtt.h"

Meter meter_main("main", MeterType::KWh, METER_KWH_MAIN_PULSES_PER_KWHR);
Meter meter_second("second", MeterType::KWh, METER_KWH_SECOND_PULSES_PER_KWHR);
Meter meter_gas("gas", MeterType::Gas, METER_GAS_PULSES_PER_M3);
Meter meter_water("water", MeterType::Water, METER_WATER_PULSES_PER_M3);

void meter_main_pulse() { meter_main.pulse(); }
void meter_second_pulse() { meter_second.pulse(); }
void meter_gas_pulse() { meter_gas.pulse(); }
void meter_water_pulse() { meter_water.pulse(); }

void setup_meters() {
  pinMode(METER_KWH_MAIN_PIN, INPUT);
  pinMode(METER_KWH_SECOND_PIN, INPUT);
  pinMode(METER_GAS_PULSES_PIN, INPUT);
  pinMode(METER_WATER_PIN, INPUT);
  
  attachInterrupt(METER_KWH_MAIN_PIN, meter_main_pulse, FALLING);
  attachInterrupt(METER_KWH_SECOND_PIN, meter_second_pulse, FALLING);
  attachInterrupt(METER_GAS_PULSES_PIN, meter_gas_pulse, FALLING);
  attachInterrupt(METER_WATER_PIN, meter_water_pulse, FALLING);
}

void update_meter_average() {
    meter_main.update_sliding_average_usage();
    meter_second.update_sliding_average_usage();
    meter_gas.update_sliding_average_usage();
    meter_water.update_sliding_average_usage();
}

void publish_report() {

  toggle_led(HIGH);
  
  publish_meter_reading(&meter_main);
  publish_meter_reading(&meter_second);
  publish_meter_reading(&meter_gas);
  publish_meter_reading(&meter_water);
  
  toggle_led(LOW);
}

void publish_meter_reading(Meter* meter) {
  JsonObject root = meter->get_json();

  char topic[25];
  snprintf(topic, 25, "%s/%s/reading", BASE_TOPIC, meter->id);

  publish_mqtt(root, topic);
}