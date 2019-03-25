#include "Meter.h"
#include "Arduino.h"
#include <ArduinoJson.h>
#include <TimeLib.h> 

Meter::Meter(const char *id, MeterType meter_type, unsigned int pulses_per_unit)
: id(id)
, meter_type(meter_type)
, _pulses_per_unit(pulses_per_unit)
, count(0)
, current_usage(0)
, total_usage(0)
, sliding_average_usage(5)
{
}

// interrupt callback
void Meter::pulse()
{
  // debounce input, ignore triggers less than 1/2s = 500ms = 500,000µs
  if ((millis() - _last_pulse_time) < 500) return;
  
  // used to measure time between pulses.
  _pulse_time = millis();
  
  // increment pulse count
  count++;
  
  // calculate pulse length
  pulse_length = _pulse_time - _last_pulse_time;
  
  // calculate usage based on pulse length
  current_usage = calculate_usage(pulse_length);

  // find usage elapsed (kWhr example)
  total_usage = (1.0 * count / (_pulses_per_unit));
  
  // remember pulse time for next time
  _last_pulse_time = _pulse_time;
}

bool Meter::has_readings() const
{
  return count > 0;
}

double Meter::get_current_usage()
{
  unsigned long current_pulse_length = millis() - _last_pulse_time;
  
  // if current pulse is shorter than previous whole pulse, then we haven't had a pulse yet
  // so use previous whole pulse length instead.
  if (current_pulse_length < pulse_length) current_pulse_length = pulse_length;
  
  // calculate usage based on pulse length
  return pulse_length == 0
    ? 0
    : calculate_usage(current_pulse_length); 
    // TODO hier stond pulse_length leek me fout ivm dat dit bovenstande bepaling negeert.
}

void Meter::update_sliding_average_usage()
{
  // update running average
  sliding_average_usage.add(get_current_usage());
}

unsigned long Meter::calculate_usage(unsigned long pulse_length)
{
  switch(meter_type)
  {
    case MeterType::KWh: // KWh
      return ((1000.0 / _pulses_per_unit) * 3600.0 / pulse_length * 1000.0);
      break;
    case MeterType::Gas: // M3/hr
      return 99;
      //TODO: implement calculation
      break;
    case MeterType::Water: //Ltr/min
      return 99;
      //TODO: implement calculation
      break;
  }
}

JsonObject Meter::get_json()
{
  StaticJsonDocument<JSON_BUFFER_SIZE> jsondoc;
  
  // create JSON
  JsonObject root = jsondoc.to<JsonObject>();

  switch(meter_type)
  {
    case MeterType::KWh:
      root["current_power"] = get_current_usage();
      root["sliding_average_power"] = sliding_average_usage.avg();
      root["power_unit"] = "kWhr";
      root["kwhr"] = total_usage;
      break;
    case MeterType::Gas:
      root["current_flow"] = get_current_usage();
      root["sliding_average_flow"] = sliding_average_usage.avg();
      root["flow_unit"] = "m3/hr";
      root["m3"] = total_usage;
      break;
    case MeterType::Water:
      root["current_flow"] = get_current_usage();
      root["sliding_average_flow"] = sliding_average_usage.avg();
      root["flow_unit"] = "ltr/min";
      root["m3"] = total_usage;
      break;
  }
  root["count"] = count;
  root["delay"] = pulse_length;
  
  char tt[19];
  snprintf(tt, 19, "%04d-%02d-%02d %02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
  root["time"] = tt;
  
  return root;
}

