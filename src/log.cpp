#include "mqtt.h"
#include "config.h" 
#include "log.h"

void log(LogLevel level, const char *message)
{
   if ( LOGLEVEL == LogLevel::Verbose
        ||
        (LOGLEVEL == LogLevel::Info && level == LogLevel::Info)
        ||
        (LOGLEVEL == LogLevel::Error && level == LogLevel::Error)
        )
    {
        publish_mqtt(message, TOPIC_LOGGING);
    }
}

