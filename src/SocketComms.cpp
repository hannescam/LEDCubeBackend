#include "include/SocketComms.hpp"

void SocketComms::addAnimationEntry(string name, string icon, menuConstructor *menuEntry) {
  animation currentAnimation;
  currentAnimation.icon = icon;
  currentAnimation.menuEntry = menuEntry;
  animations[name] = move(currentAnimation);
}

void SocketComms::setSelectedFile(fileState state, string _selectedFile) {
  if (_selectedFile != "") {
    selectedFile = _selectedFile;
  }

  switch (state) {
    case STATE_PLAYING:
      fileIsPlaying = true;
      fileIsLoaded = true;
      break;

    case STATE_PAUSED:
      fileIsPlaying = false;
      fileIsLoaded = true;
      break;

    case STATE_NOT_LOADED:
      fileIsLoaded = false;
      break;
  }
}

void SocketComms::setLidState(bool _lidState) {
  lidState = _lidState;
}

void SocketComms::updateVoltage(double _cubeVoltage) {
  cubeVoltage = _cubeVoltage;
}

void SocketComms::updateCurrentDraw(double _cubeCurrent) {
  cubeCurrent = _cubeCurrent;
}

void SocketComms::setNewFileHandler(function<void(string, string, SocketHandler*)> _newFileHandler) {
  newFileHandler = _newFileHandler;
}

void SocketComms::setPlayHandler(function<void (string, string, SocketHandler *)> _playHandler) {
  playHandler = _playHandler;
}

void SocketComms::setPauseHandler(function<void (string, string, SocketHandler *)> _pauseHandler) {
  pauseHandler = _pauseHandler;
}

void SocketComms::setStopHandler(function<void (string, string, SocketHandler *)> _stopHandler) {
  stopHandler = _stopHandler;
}

errorMsg SocketComms::createInvalidRequestError(string messageId) {
  errorMsg error;
  error.messageId = messageId;
  error.source = SOURCE_INVALID_REQUEST;
  error.name = "Received an invalid request that could not be parsed";
  error.severity = SEVERITY_SIMPLE_SOFTWARE;
  return error;
}

void SocketComms::handleIncommingYAML(string message, SocketHandler* socket) { // TODO: Add remote errors, Document
  YAML::Node messageYAML = YAML::Load(message);
  string messageId = messageYAML[INTERNAL_NETWOTK_ID_NAME].as<string>(); // Extract the message id for resending and logging
  string requestedFile;

  if (messageYAML[PACKET_TYPE_NAME].as<string>() == PACKET_REQUEST_NAME) { // check if the packet is the right type (request)
    string requestTypeName = messageYAML[REQUEST_TYPE].as<string>(); // Extract the request type name (like status or play) from the YAML
    try {
      requestedFile = messageYAML[REQUEST_FILE].as<string>();
    } catch (...) {
      if (requestTypeName != REQUEST_TYPE_STATUS) {
        Logger::urgent("No filename was present in the request even though it was expected", LOG_AEREA_SOCKET_COMMS);
        return;
      }
    }
    if (requestTypeName == REQUEST_TYPE_STATUS) { // Check if the request is a status request
      Logger::debug("Got status request, assembling status reply...", LOG_AEREA_SOCKET_COMMS);
      YAML::Node statusMessage; // Initialize the YAML node
      statusMessage[PACKET_TYPE_NAME] = PACKET_REPLY_NAME;
      statusMessage[REPLY_TYPE_NAME] = REPLY_TYPE_STATUS;
      statusMessage[INTERNAL_NETWOTK_ID_NAME] = messageId; // Save the required constants/variables into the YAML node
      statusMessage[REPLY_STATUS_FILE_LOADED] = fileIsLoaded;
      string fileStateStr;

      if (fileIsPlaying) {
        fileStateStr = REPLY_STATUS_FILE_STATE_PLAYING;
      } else {
        fileStateStr = REPLY_STATUS_FILE_STATE_PAUSED;
      }

      statusMessage[REPLY_STATUS_FILE_STATE] = fileStateStr; // Put the string error source into the YAML node
      statusMessage[REPLY_STATUS_FILE_SELECTED] = selectedFile; // Save the required variables into the YAML node
      statusMessage[REPLY_STATUS_CURRENT_DRAW] = cubeCurrent;
      statusMessage[REPLY_STATUS_VOLTAGE] = cubeVoltage;
      statusMessage[REPLY_STATUS_LID_STATE] = lidState;

      for (const auto& [key, _] : animations) { // Extract the names of the keys of the C++ map (dictionary in this case string and animation)
        statusMessage[REPLY_STATUS_AVAILIBLE_ANIMATIONS][key] = animations.at(key).icon; // Insert all the information for the animation entry into a YAML sub-node
      }
      Logger::debug("Successfully assembled status reply, sending status reply...", LOG_AEREA_SOCKET_COMMS);
      socket->sendString(YAML::Dump(statusMessage));
    } else if (requestTypeName == REQUEST_TYPE_PLAY) {
      Logger::debug("Play request received for file: " + requestedFile, LOG_AEREA_SOCKET_COMMS);
      if (playHandler) playHandler(messageId, requestedFile, socket);
    } else if (requestTypeName == REQUEST_TYPE_PAUSE) {
      Logger::debug("Pause request received for file: " + requestedFile, LOG_AEREA_SOCKET_COMMS);
      if (pauseHandler) pauseHandler(messageId, requestedFile, socket);
    } else if (requestTypeName == REQUEST_TYPE_STOP) {
      Logger::debug("Stop request received for file: " + requestedFile, LOG_AEREA_SOCKET_COMMS);
      if (stopHandler) stopHandler(messageId, requestedFile, socket);
    } else if (requestTypeName == REQUEST_TYPE_MENU) {
      try {
        Logger::debug("Sending menu reply to client...", LOG_AEREA_SOCKET_COMMS);
        socket->sendString(animations.at(requestedFile).menuEntry->getYAMLString(messageId));
      } catch (...) {
        Logger::urgent("Unable to get menu entry from file: " + requestedFile, LOG_AEREA_SOCKET_COMMS);
      }
    } else if (requestTypeName == REQUEST_TYPE_MENU_CHANGE) {
      try {
        Logger::debug("Adding new widget with path: " + messageYAML[REQUEST_MENU_OBJECT_PATH].as<string>() + " and value: " + messageYAML[REQUEST_MENU_OBJECT_VALUE].as<string>(), LOG_AEREA_SOCKET_COMMS);
        if (!animations.at(requestedFile).menuEntry->addIncomingValue(messageYAML[REQUEST_MENU_OBJECT_VALUE].as<string>(), messageYAML[REQUEST_MENU_OBJECT_PATH].as<string>())) {
          Logger::urgent("Failed to add menu entry because of a non-fatal error (probably invalid path)", LOG_AEREA_SOCKET_COMMS);
        }
      } catch (...) {
        Logger::urgent("Unable to add a new value to menu from file: " + requestedFile, LOG_AEREA_SOCKET_COMMS);
      }
    } else if (requestTypeName == REQUEST_TYPE_FILE_UPLOAD) {
      bytesInFileTransmission = messageYAML[REQUEST_FILE_BYTE_COUNT].as<unsigned int>();
      fileNameInTransmision = requestedFile;
      socket->setUseReveiveHandler(false);
      fileIsUpcomming = true;
      return;
    } else {
      Logger::urgent("Request type is invalid, sending a error message...", LOG_AEREA_SOCKET_COMMS);
      socket->sendString(createInvalidRequestError(messageId).getYAMLString());
      return;
    }

  } else {
    Logger::urgent("Packet type is invalid, sending a error message...", LOG_AEREA_SOCKET_COMMS);
    socket->sendString(createInvalidRequestError(messageId).getYAMLString());
  }
}

bool SocketComms::begin(int port) {
  bool success = acceptor.begin(port);

  acceptor.setClientHandler([this](SocketHandler* socket) {
    Logger::info("New client connected with id: " + to_string(socket->getConnectionId()) , LOG_AEREA_SOCKET_COMMS);
    socket->setReceiveHandler([this](string message, SocketHandler* socket) {
      Logger::debug("-----------------------------------------------------------------", LOG_AEREA_SOCKET_COMMS);
      Logger::debug("Got message from client with id: " + to_string(socket->getConnectionId()), LOG_AEREA_SOCKET_COMMS);
      Logger::debug(message + "-----------------------------------------------------------------", LOG_AEREA_SOCKET_COMMS);

      try {
        this->handleIncommingYAML(message, socket);
      } catch (exception &error) {
        Logger::warn("Standard error while parsing incoming YAML: " + string(error.what()), LOG_AEREA_SOCKET_COMMS);
      } catch (...) {
        Logger::warn("Unknown error while parsing incoming YAML: " + string(__cxxabiv1::__cxa_current_exception_type()->name()), LOG_AEREA_SOCKET_COMMS);
      }
    });

    while (socket->isOpen()) {
      if (this->fileIsUpcomming) {
        socket->receiveFile(this->fileNameInTransmision, this->bytesInFileTransmission);
        this->fileIsUpcomming = false;
      }

      usleep(1000);
    }
  });

  acceptor.startListening();
  return success;
}
