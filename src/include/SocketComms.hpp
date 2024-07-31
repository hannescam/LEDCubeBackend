#pragma once
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
#include <ErrorConstructor.hpp>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <SocketWrapper.hpp>
#include <MenuConstructor.hpp>
#include <GlobalEnums.hpp>
#include "communicationDefinitions.hpp"

using namespace std;

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
    bool sendError(ErrorConstructor error, string messageId = "");

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

    SocketHandler* connectedClient;
    map<string, animation> animations;
    bool lidState = true;
    bool fileIsLoaded = false;
    bool fileIsPlaying = false;
    double cubeVoltage = 0;
    double cubeCurrent = 0;
    string selectedFile;
    string createInvalidRequestError(string messageId);
    void handleIncommingYAML(string message, SocketHandler* socket); // So that this function can be passed into set_message_handler
};
