#include "include/Logger.hpp"

bool LoggerSettings::begin(string configPath) {
  try {
    YAML::Node loggerSettingsYAML = YAML::LoadFile(configPath);
    logLevel = loggerSettingsYAML[LOGGER_LOG_LEVEL_KEY_NAME].as<int>();
    useLineNumbers = loggerSettingsYAML[LOGGER_USE_LINE_NUMBERS_KEY_NAME].as<bool>();
    enabledOrigins = loggerSettingsYAML[LOGGER_LOG_AEREAS_KEY_NAME].as<vector<string>>();
    return true;
  } catch (exception &error) {
    cerr << "Standard error while parsing logger config YAML: " << error.what() << endl;
    return false;
  } catch (...) {
    cerr << "Unknown error while parsing logger config YAML: " << __cxxabiv1::__cxa_current_exception_type()->name() << endl;
    return false;
  }
}

int LoggerSettings::getLogLevel() {
  return logLevel;
}

bool LoggerSettings::getUseLineNumbers() {
  return useLineNumbers;
}

bool LoggerSettings::getAllAereasAreEnabled() {
  return checkIfOriginIsEnabled(LOGGER_ALLOW_ALL_LOG_AEREAS_AEREA);
}

bool LoggerSettings::checkIfOriginIsEnabled(string origin) {
  return find(enabledOrigins.begin(), enabledOrigins.end(), origin) != enabledOrigins.end();
}