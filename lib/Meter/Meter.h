#include "Arduino.h"
#include <RunningAverage.h>
#include <ArduinoJson.h>

#ifndef METER_H
#define METER_H

const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(7);

enum class MeterType
{
	KWh, //kWhr
	Gas, //m3/hr
	Water //ltr/min
};

class Meter
{
	public:
		Meter(const char *id, MeterType meter_type, unsigned int pulses_per_unit);
		
		void pulse();
		bool has_readings() const;
		double get_current_usage();
		void update_sliding_average_usage();
		JsonObject get_json();
		
		const char *id;
		MeterType meter_type;
		volatile unsigned long pulse_length;
		volatile unsigned long count;
		volatile unsigned long current_usage;
		volatile double total_usage;
		
		RunningAverage sliding_average_usage;
  
  private:
  		unsigned long calculate_usage(unsigned long pulse_length);

		volatile unsigned int _pulses_per_unit;
    	volatile unsigned long _pulse_time;
		volatile unsigned long _last_pulse_time;
};

#endif
