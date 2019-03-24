#ifndef LOG_H
#define LOG_H

enum class LogLevel
{
	Verbose,
    Info, 
	Error 
};

static LogLevel LOGLEVEL = LogLevel::Verbose;

void log(LogLevel level, const char *message);
#endif
