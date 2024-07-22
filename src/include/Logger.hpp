#ifndef LOGGER_INCLUDED
#define LOGGER_INCLUDED

#include "LogAereas.hpp"
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

#define LOGGER_NEWLINE_CHAR '\n'
#define LOGGER_SYSTEM_FOLDER_DELIMETOR '/'

#define LOGGER_ALLOW_ALL_LOG_AEREAS_AEREA "ALL"
#define LOGGER_USE_LIGHT_MODE_COLORS_KEY_NAME "uselightmodecolors"
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
#define LOG_INFO_LIGHTMODE_COLOR 0x00000000
#define LOG_INFO_DARKMODE_COLOR 0xFFFFFF00
#define LOG_DEBUG_COLOR 0x0099FF00
#define LOG_UNKNOWN_COLOR 0x00FF0000

#define MAX_LOG_LEVEL LOG_LEVEL_DEBUG

class LoggerSettings {
  public:
    bool begin(string configPath);
    int getLogLevel();
    bool getUseLineNumbers();
    bool getAllAereasAreEnabled();
    bool getUseLightmodeColors();
    bool checkIfOriginIsEnabled(string origin);
  private:
    vector<string> enabledOrigins;
    bool useLightmodeColors;
    bool useLineNumbers;
    int logLevel;
};

class Logger {
  public:
    static bool begin(string configPath);
    static int getLogLevel();
    static void log(string message, string origin, int logSeverity, const source_location functionSource = source_location::current());
    static void debug(string message, string origin = LOG_ORIGIN_UNDEFINED, const source_location functionSource = source_location::current());
    static void info(string message, string origin = LOG_ORIGIN_UNDEFINED, const source_location functionSource = source_location::current());
    static void urgent(string message, string origin = LOG_ORIGIN_UNDEFINED, const source_location functionSource = source_location::current());
    static void warn(string message,  string origin = LOG_ORIGIN_UNDEFINED, const source_location functionSource = source_location::current());
    static void error(string message, string origin = LOG_ORIGIN_UNDEFINED, const source_location functionSource = source_location::current());
  private:
    static void printColor(uint32_t color, string content);
    static LoggerSettings loggerSettings;
};
#endif
