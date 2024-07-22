#include "include/SocketComms.hpp"

void SocketComms::setStatusRequestHandler(function<void(string, SocketHandler*)> _statusRequestHandler) {
  statusRequestHandler = _statusRequestHandler;
}

void SocketComms::setGeneralRequestHandler(function<void(string, requestType, string, SocketHandler*)> _generalRequestHandler) {
  generalRequestHandler = _generalRequestHandler;
}

void SocketComms::setMenuChangeHandler(function<void(string, string, string, string, SocketHandler*)> _menuChangeHandler) {
  menuChangeHandler = _menuChangeHandler;
}

void SocketComms::setNewFileHandler(function<void(string, string, SocketHandler*)> _newFileHandler) {
  newFileHandler = _newFileHandler;
}

errorMsg SocketComms::createInvalidRequestError(string messageId) {
  errorMsg error;
  error.messageId = messageId;
  error.source = SOURCE_INVALID_REQUEST;
  error.name = "Received an invalid request that could not be parsed";
  error.severity = SEVERITY_SIMPLE_SOFTWARE;
  return error;
}

void SocketComms::handleIncommingYAML(string message, SocketHandler* socket) {
  YAML::Node messageYAML = YAML::Load(message);
  string messageId = messageYAML[INTERNAL_NETWOTK_ID_NAME].as<string>(); // Extract the message id for resending and logging
  if (messageYAML[PACKET_TYPE_NAME].as<string>() == PACKET_REQUEST_NAME) { // check if the packet is the right type (request)
    string requestTypeName = messageYAML[REQUEST_TYPE].as<string>(); // Extract the request type name (like status or play) from the YAML
    requestType type;
    if (requestTypeName == REQUEST_TYPE_STATUS) { // Check if the request is a status request
      type = REQUEST_STATUS;
    } else if (requestTypeName == REQUEST_TYPE_PLAY) {
      type = REQUEST_PLAY;
    } else if (requestTypeName == REQUEST_TYPE_PAUSE) {
      type = REQUEST_PAUSE;
    } else if (requestTypeName == REQUEST_TYPE_STOP) {
      type = REQUEST_STOP;
    } else if (requestTypeName == REQUEST_TYPE_MENU) {
      type = REQUEST_MENU;
    } else if (requestTypeName == REQUEST_TYPE_MENU_CHANGE) {
      type = REQUEST_MENU_CHANGE;
    } else if (requestTypeName == REQUEST_TYPE_FILE_UPLOAD) {
      bytesInFileTransmission = messageYAML[REQUEST_FILE_BYTE_COUNT].as<unsigned int>();
      fileNameInTransmision = messageYAML[REQUEST_FILE].as<string>();
      socket->setUseReveiveHandler(false);
      fileIsUpcomming = true;
      return;
    } else {
      Logger::urgent("Request type is invalid, sending a error message...", LOG_AEREA_SOCKET_COMMS);
      socket->sendString(createInvalidRequestError(messageId).getYAMLString());
      return;
    }

    switch (type) {
      case REQUEST_STATUS:
        if (statusRequestHandler) statusRequestHandler(messageId, socket); // If yes call the handler for it including the message id
        break;

      default:
        if (generalRequestHandler) generalRequestHandler(messageId, type, messageYAML[REQUEST_FILE].as<string>(), socket);
        break;

      case REQUEST_MENU_CHANGE:
        if (menuChangeHandler) menuChangeHandler(messageId, messageYAML[REQUEST_FILE].as<string>(), messageYAML[REQUEST_MENU_OBJECT_PATH].as<string>(), messageYAML[REQUEST_MENU_OBJECT_VALUE].as<string>(), socket);
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
