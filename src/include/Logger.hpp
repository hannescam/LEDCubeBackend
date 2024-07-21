#ifndef LOGGER_INCLUDED
#define LOGGER_INCLUDED

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cxxabi.h>
#include <yaml-cpp/yaml.h>
#include <string>
#include <experimental/source_location>

using namespace std;
using std::experimental::source_location;


#define LOGGER_SYSTEM_FOLDER_DELIMETOR '/'

#define LOGGER_ALLOW_ALL_LOG_AEREAS_AEREA "ALL"
#define LOGGER_LOG_LEVEL_KEY_NAME "loglevel"
#define LOGGER_USE_LINE_NUMBERS_KEY_NAME "uselinenumbers"
#define LOGGER_LOG_AEREAS_KEY_NAME "logaereas"

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_URGENT 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4

#define LOG_ERROR_COLOR 0xFF000000
#define LOG_WARNING_COLOR 0xFF5F0000
#define LOG_URGENT_COLOR 0xFF00F400
#define LOG_INFO_COLOR 0xFFFFFF00
#define LOG_DEBUG_COLOR 0x0099FF00
#define LOG_UNKNOWN_COLOR 0x00FF0000

#define LOG_ORIGIN_UNDEFINED "UNDEFINED"

#define MAX_LOG_LEVEL LOG_LEVEL_DEBUG

class LoggerSettings {
  public:
    bool begin(string configPath);
    int getLogLevel();
    bool getUseLineNumbers();
    bool getAllAereasAreEnabled();
    bool checkIfOriginIsEnabled(string origin);
  private:
    vector<string> enabledOrigins;
    bool useLineNumbers;
    int logLevel;
};

class Logger {
  public:
    bool begin(string configPath);
    int getLogLevel();
    void log(string message, string origin, int logSeverity, const source_location functionSource = source_location::current());
    void debug(string message, string origin = LOG_ORIGIN_UNDEFINED, const source_location functionSource = source_location::current());
    void info(string message, string origin = LOG_ORIGIN_UNDEFINED, const source_location functionSource = source_location::current());
    void urgent(string message, string origin = LOG_ORIGIN_UNDEFINED, const source_location functionSource = source_location::current());
    void warn(string message,  string origin = LOG_ORIGIN_UNDEFINED, const source_location functionSource = source_location::current());
    void error(string message, string origin = LOG_ORIGIN_UNDEFINED, const source_location functionSource = source_location::current());
  private:
    void printColor(uint32_t color, string content);
    LoggerSettings loggerSettings;
};
#endif
