#include "include/Logger.hpp"

LoggerSettings Logger::loggerSettings; // Initialize loggerSettings because it is needed

bool Logger::begin(string configPath) {
  return loggerSettings.begin(configPath);
}

int Logger::getLogLevel() {
  return loggerSettings.getLogLevel();
}

void Logger::log(string message, string origin, int logSeverity, const source_location functionSource) {
  if (loggerSettings.getLogLevel() > 0 && logSeverity <= loggerSettings.getLogLevel() && (loggerSettings.getAllAereasAreEnabled() or loggerSettings.checkIfOriginIsEnabled(origin))) {
    string logSeverityStr;
    uint32_t color;

    switch(logSeverity) {
      case LOG_LEVEL_ERROR:
        logSeverityStr = "ERROR";
        color = LOG_ERROR_COLOR;
        break;

      case LOG_LEVEL_WARNING:
        logSeverityStr = "WARNING";
        color = LOG_WARNING_COLOR;
        break;

      case LOG_LEVEL_URGENT:
        logSeverityStr = "URGENT";
        color = LOG_URGENT_COLOR;
        break;

      case LOG_LEVEL_INFO:
        logSeverityStr = "INFO";
        if (loggerSettings.getUseLightmodeColors()) {
          color = LOG_INFO_LIGHTMODE_COLOR;
        } else {
          color = LOG_INFO_DARKMODE_COLOR;
        }
        break;

      case LOG_LEVEL_DEBUG:
        logSeverityStr = "DEBUG";
        color = LOG_DEBUG_COLOR;
        break;

      default:
        logSeverityStr = "severity=" + to_string(logSeverity);
        color = LOG_UNKNOWN_COLOR;
    }

    string messageHeader;

    if (loggerSettings.getUseLineNumbers()) {
      string fileName = functionSource.file_name(); // Get the path + filename of the function that called the logger
      stringstream fileNameStream(fileName); // Convert string to string stream
      while (getline(fileNameStream, fileName, LOGGER_SYSTEM_FOLDER_DELIMETOR)); // Remove path to file
      messageHeader = "[" + logSeverityStr + "] ["+ origin +"] [" + functionSource.function_name() + "() in " + fileName + " at line " + to_string(functionSource.line()) + "] ";
    } else {
      messageHeader = "[" + logSeverityStr + "] ["+ origin +"] ";
    }

    stringstream messageStream(message);
    while (getline(messageStream, message, LOGGER_NEWLINE_CHAR)) {
      printColor(color, messageHeader + message);
    }
  }
}

void Logger::debug(string message, string origin, const source_location functionSource) {
  log(message, origin, LOG_LEVEL_DEBUG, functionSource);
}

void Logger::info(string message, string origin, const source_location functionSource) {
  log(message, origin, LOG_LEVEL_INFO, functionSource);
}

void Logger::urgent(string message, string origin, const source_location functionSource) {
  log(message, origin, LOG_LEVEL_URGENT, functionSource);
}

void Logger::warn(string message, string origin, const source_location functionSource) {
  log(message, origin, LOG_LEVEL_WARNING, functionSource);
}

void Logger::error(string message, string origin, const source_location functionSource) {
  log(message, origin, LOG_LEVEL_ERROR, functionSource);
}

void Logger::printColor(uint32_t color, string content) {
  uint8_t r = (color & 0xFF000000) >> 24; // Separate color components from a single color
  uint8_t g = (color & 0x00FF0000) >> 16;
  uint8_t b = (color & 0x0000FF00) >> 8;
  cout << '\33' << '[' << '3' << '8' << ';' << '2' << ';' << int(r) << ';' << int(g) << ';' << int(b) << 'm' << content << '\33' << '[' << 'm' << endl;
  cout.flush();
}
