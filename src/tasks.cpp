//  #define _TASK_TIMECRITICAL      // Enable monitoring scheduling overruns
//  #define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass 
//  #define _TASK_STATUS_REQUEST    // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
//  #define _TASK_WDT_IDS           // Compile with support for wdt control points and task ids
//  #define _TASK_LTS_POINTER       // Compile with support for local task storage pointer
//  #define _TASK_PRIORITY          // Support for layered scheduling priority
//  #define _TASK_MICRO_RES         // Support for microsecond resolution
//  #define _TASK_STD_FUNCTION      // Support for std::function (ESP8266 ONLY)
//  #define _TASK_DEBUG             // Make all methods and variables public for debug purposes

#include <TaskScheduler.h>
#include "wifi.h"
#include "mqtt.h"
#include "meters.h"
#include "ota.h"
#include "config.h" 

Scheduler runner;

void publish_heartbeat();

Task t_monitor_wifi(30UL*TASK_SECOND, TASK_FOREVER, &monitor_wifi);
Task t_monitor_mqtt(30UL*TASK_SECOND, TASK_FOREVER, &monitor_mqtt);
Task t_monitor_ota(0, TASK_FOREVER, &monitor_ota);
Task t_update_meter_average(5UL*TASK_SECOND, TASK_FOREVER, &update_meter_average);
Task t_publish_report(30UL*TASK_SECOND, TASK_FOREVER, &publish_report);
Task t_publish_heartbeat(30UL*TASK_SECOND, TASK_FOREVER, &publish_heartbeat);

void setup_tasks() {
  runner.init();

  runner.addTask(t_monitor_wifi);
  t_publish_report.enable();

  runner.addTask(t_monitor_mqtt);
  t_publish_report.enable();

  runner.addTask(t_monitor_ota);
  t_publish_report.enable();

  runner.addTask(t_update_meter_average);
  t_update_meter_average.enable();

  runner.addTask(t_publish_report);
  t_publish_report.enable();

  runner.addTask(t_publish_heartbeat);
  t_publish_heartbeat.enable();

  runner.startNow();
}

void execute_tasks() {
  runner.execute();
}

void publish_heartbeat()
{
  StaticJsonDocument<JSON_BUFFER_SIZE> jsondoc;

  // create JSON
  JsonObject root = jsondoc.to<JsonObject>();
  //root["time"] = ;
  //TODO implement.
  root["uptime"] = millis() / 1000;
  root["freemem"] = ESP.getFreeHeap();
  
  publish_mqtt(root, TOPIC_HEARTBEAT);
}
