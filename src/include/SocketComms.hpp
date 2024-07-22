#include <stdlib.h>
#include <Logger.hpp>
#include <thread>
#include <cxxabi.h>
#include <stdint.h>
#include <cstdint>
#include <unistd.h>
#include <map>
#include <vector>
#include <functional>
#include <iostream>
#include <exception>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <SocketWrapper.hpp>
#include <MenuConstructor.hpp>
#include "communicationDefinitions.hpp"

using namespace std;

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
  STATE_PAUSED,
  STATE_NOT_LOADED
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

class animation {
  public:
    menuConstructor* menuEntry;
    string icon;
};

class SocketComms {
  public:
    bool begin(int port = 1200);
    void addAnimationEntry(string name, string icon, menuConstructor* menuEntry);
    void setSelectedFile(fileState state, string _selectedFile = "");
    void setLidState(bool _lidState);
    void updateVoltage(double _cubeVoltage);
    void updateCurrentDraw(double _cubeCurrent);

    void setNewFileHandler(function<void(string, string, SocketHandler*)> _newFileHandler);
    void setPlayHandler(function<void(string, string, SocketHandler*)> _playHandler);
    void setPauseHandler(function<void(string, string, SocketHandler*)> _pauseHandler);
    void setStopHandler(function<void(string, string, SocketHandler*)> _stopHandler);
  private:
    int bufferSize = 64;
    thread listener;
    SocketAcceptor acceptor;
    bool fileIsUpcomming = false;
    unsigned int bytesInFileTransmission;
    string fileNameInTransmision;

    function<void(string, string, SocketHandler*)> newFileHandler;
    function<void(string, string, SocketHandler*)> playHandler;
    function<void(string, string, SocketHandler*)> pauseHandler;
    function<void(string, string, SocketHandler*)> stopHandler;

    map<string, animation> animations;
    bool lidState = true;
    bool fileIsLoaded = false;
    bool fileIsPlaying = false;
    double cubeVoltage = 0;
    double cubeCurrent = 0;
    string selectedFile;
    errorMsg createInvalidRequestError(string messageId);
    void handleIncommingYAML(string message, SocketHandler* socket); // So that this function can be passed into set_message_handler
};
