#include "include/SocketComms.hpp"

/*void SocketComms::setErrorStream(ostream &_errorStream) {
  errorStream = _errorStream;
}*/

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
      fileIsUpcomming = true;
      socket->setUseReveiveHandler(false);
      return;
    } else {
      cerr << "Request type is invalid, sending a error message..." << endl;
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
    cerr << "Packet type is invalid, sending a error message..." << endl;
    socket->sendString(createInvalidRequestError(messageId).getYAMLString());
  }
}

bool SocketComms::begin(int port) {
  bool success = acceptor.begin(port);

  acceptor.setClientHandler([this](SocketHandler* socket) {
    cout << "New client connected with id: " << socket->getConnectionId() << endl;

    socket->setReceiveHandler([this](string message, SocketHandler* socket) {
      cout << "-----------------------------------------------------------------" << endl << "Got message from client with id: " << socket->getConnectionId() << endl << message << "-----------------------------------------------------------------" << endl;
      try {
        this->handleIncommingYAML(message, socket);
      } catch (exception &error) {
        cerr << "Stantard error while parsing incoming YAML: " << error.what() << endl;
      } catch (...) {
        cerr << "Unknown error while parsing incoming YAML: " << __cxxabiv1::__cxa_current_exception_type()->name() << endl;
      }
    });

    while (socket->isOpen()) usleep(10000);
  });

  acceptor.startListening();
  return success;
}
