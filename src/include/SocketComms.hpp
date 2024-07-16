#include <stdlib.h>
#include <thread>
#include <cxxabi.h>
#include <stdint.h>
#include <cstdint>
#include <unistd.h>
#include <vector>
#include <functional>
#include <iostream>
#include <exception>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <SocketWrapper.hpp>
#include "communicationDefinitions.hpp"

using namespace std;
using namespace std::chrono;

enum errorSource {
  SOURCE_POWER,
  SOURCE_INVALID_FILE ,
  SOURCE_INVALID_REQUEST,
  SOURCE_OTHER
};

enum requestType {
  REQUEST_STATUS,
  REQUEST_PLAY,
  REQUEST_PAUSE,
  REQUEST_STOP,
  REQUEST_MENU,
  REQUEST_MENU_CHANGE
};

enum errorSeverity {
  SEVERITY_SIMPLE_SOFTWARE = 1,
  SEVERITY_MEDIUM_SOFTWARE = 2,
  SEVERITY_SIMPLE_HARDWARE = 3,
  SEVERITY_SEVERE_HARDWARE = 4
};

enum fileState {
  STATE_PLAYING,
  STATE_PAUSED
};



class errorMsg {
  public:
    string messageId;
    errorSource source;
    int code = 0;
    string name;
    errorSeverity severity;
    string getYAMLString();
};

class animationWithIcon {
  public:
    string animationName;
    string animationGnomeIconName;
};

class replyStatusMsg {
  public:
    string messageId;
    bool fileIsLoaded = false;
    fileState state;
    string fileSelected;
    float currentDraw;
    float voltage;
    bool lidState = false;
    vector<animationWithIcon> availableAnimations;
    string getYAMLString();
};

class SocketComms {
  public:
    //void setErrorStream(ostream &_errorStream);
    bool begin(int port = 1200);
    void setStatusRequestHandler(function<void(string, SocketHandler*)> _statusHandler);
    void setGeneralRequestHandler(function<void(string, requestType, string, SocketHandler*)> _generalRequestHandler);
    void setMenuChangeHandler(function<void(string, string, string, string, SocketHandler*)> _menuChangeHandler);
    void setNewFileHandler(function<void(string, string, SocketHandler*)> _newFileHandler);
  private:
    int bufferSize = 64;
    thread listener;
    SocketAcceptor acceptor;
    bool fileIsUpcomming = false;

    function<void(string, SocketHandler*)> statusRequestHandler;
    function<void(string, requestType, string, SocketHandler*)> generalRequestHandler;
    function<void(string, string, string, string, SocketHandler*)> menuChangeHandler;
    function<void(string, string, SocketHandler*)> newFileHandler;

    errorMsg createInvalidRequestError(string messageId);
    void handleIncommingYAML(string message, SocketHandler* socket); // So that this function can be passed into set_message_handler
};
