#include <iostream>
#include <vector>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <string>
#include <experimental/source_location>

using namespace std;
using std::experimental::source_location;

class LoggerSettings {
  public:
    void begin(string configPath);
    int getLogLevel();
    bool checkIfOriginIsEnabled(origins origin);
  private:
    vector<origins> enabledOrigins;
    int logLevel;
}

class Logger {
  public:
    enum origins {
      SOCKET_HANDLER,
      MAIN,
      SOCKET_COMMS,
      SOCKET_ACCEPTOR,
      WIDGET_CONSTRUCTORS
    };
    bool begin(string configPath);
    void debug(string message, origins origin, const source_location functionSource = source_location::current());
    void info(string message, origins origin, const source_location functionSource = source_location::current());
    void warn(string message,  origins origin, const source_location functionSource = source_location::current());
    void error(string message, origins origin, const source_location functionSource = source_location::current());
  private:
    LoggerSettings settings;
};
