#pragma once
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <Logger.hpp>
#include <string>
#include "communicationDefinitions.hpp"

using namespace std;

class ErrorConstructor {
  public:
    enum errorSource {
      SOURCE_POWER,
      SOURCE_INVALID_FILE ,
      SOURCE_INVALID_REQUEST,
      SOURCE_OTHER
    };

    enum errorSeverity {
      SEVERITY_SIMPLE_SOFTWARE = 1,
      SEVERITY_MEDIUM_SOFTWARE = 2,
      SEVERITY_SIMPLE_HARDWARE = 3,
      SEVERITY_SEVERE_HARDWARE = 4
    };

    void setErrorCode(int _code);
    void setErrorName(string _name);
    void setErrorSource(errorSource _source);
    void setErrorSeverity(errorSeverity _severity);
    string getYAMLString(string messageId);
  private:
    int code = 0;
    string name = ERROR_MESSAGE_DEFAULT_NAME;
    errorSource source = SOURCE_OTHER;
    errorSeverity severity = SEVERITY_SIMPLE_SOFTWARE;
};
