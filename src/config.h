// config

#define WIFI_SSID "..."
#define WIFI_PASSWORD "..."
#define MQTT_SERVER "..."
#define NODE_NAME "meter-monitor"

#define BASE_TOPIC "meter"
#define TOPIC_HEARTBEAT "meter/heartbeat"
#define TOPIC_LOGGING "meter/log"

#define REPORT_INTERVAL 3000

#define METER_KWH_MAIN_PIN D0
#define METER_KWH_MAIN_PULSES_PER_KWHR 500

#define METER_KWH_SECOND_PIN D1
#define METER_KWH_SECOND_PULSES_PER_KWHR 1000

#define METER_GAS_PULSES_PIN A0
#define METER_GAS_PULSES_PER_M3 1000

#define METER_WATER_PIN D2
#define METER_WATER_PULSES_PER_M3 1000

