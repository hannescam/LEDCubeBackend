// TODO: Comment the code
#include "include/SocketWrapper.hpp"

// Some setters

void SocketHandler::setConnectionFileDescriptor(int _connfd) {
  connfd = _connfd;
}

void SocketHandler::setPacketSize(int _packetSize) {
  packetSize = _packetSize;
}

void SocketHandler::setReceiveHandler(function<void (string, SocketHandler*)> _receiveHandler) {
  receiveHandler = _receiveHandler;
}

void SocketHandler::setDisconnectHandler(function<void (SocketHandler*)> _disconnectHandler) {
  disconnectHandler = _disconnectHandler;
}

void SocketHandler::setUseReveiveHandler(bool _useHandler) {
  useHandler = _useHandler;
}

// Some getters

bool SocketHandler::isOpen() {
  return isConnOpen;
}

unsigned long SocketHandler::getConnectionId() {
  return connectionId;
}

// Function to receive a byte array (aka char*) with a given size

char* SocketHandler::receiveByteArry(int& size, bool nonblocking) {
  unsigned int sizeUneditable = size;
  char* buffer = new char[sizeUneditable]; // Create a new buffer with the given size
  bzero(buffer, sizeUneditable); // Overwrite the content of that array with zeros
  int sizeReturn;
  if (nonblocking) { // Check if the read should be non-blocking
    sizeReturn = recv(connfd, buffer, sizeUneditable, MSG_DONTWAIT | MSG_WAITALL); // Make a non blocking read
  } else {
    sizeReturn = recv(connfd, buffer, sizeUneditable, MSG_WAITALL); // Make a blocking read
  }
  size = sizeReturn;
  return buffer; // NO I WON'T DOCUMENT THIS
}

bool SocketHandler::receiveFile(string filename, unsigned int fileSize) {
  try {
    ofstream outputFile;
    outputFile.open(filename);
    int actualSize;
    int cnt;
    char* buffer = new char[packetSize];
    unsigned int bytesReceived = 0;
    Logger::info("Receiving file: " + filename, LOG_AEREA_SOCKET_HANDLER);
    while (fileSize > bytesReceived) {
      bzero(buffer, packetSize); // Overwrite the content of that array with zeros
      actualSize = recv(connfd, buffer, packetSize, MSG_WAITALL);
      if (actualSize > 0) {
        triggerKeepalive();
        bytesReceived += actualSize;
        cnt = 0;
        while (cnt < actualSize) {
          outputFile << buffer[cnt];
          cnt++;
        }
      }
    }
    outputFile.flush();
    outputFile.close();
    return true;
  } catch (exception &error) {
    Logger::warn("Standard error while trying to receive file: " + string(error.what()), LOG_AEREA_SOCKET_HANDLER);
    return false;
  } catch (...) {
    Logger::warn("Unknown error while trying to receive file: " + string(__cxxabiv1::__cxa_current_exception_type()->name()), LOG_AEREA_SOCKET_HANDLER);
    return false;
  }
}

// INTERNAL: function to receive a entire message/the entire buffer (blocking)

string SocketHandler::receiveStringUntilMessageEnd() {
  string finalBuffer;
  string bufferStr;
  char* buffer = new char[packetSize]; // Not important to document
  while (isConnOpen && useHandler) { // Only loop when the socket is open and the handler is used
    bzero(buffer, packetSize); // Clear the buffer
    int readBytes = recv(connfd, buffer, packetSize, MSG_DONTWAIT | MSG_WAITALL); // Read from the socket and not be blocking (so it doesn't try to fill the buffer when reading from the socket)
    if (((readBytes == 0) | (readBytes == -1)) && finalBuffer.size() > 0) break; // Check if there is something in the read buffer (and there wasn't a receive error) and there is something to return
    if (readBytes > 0) { // Run when something got read
      bufferStr = buffer; // Convert the buffer char* to s C++ string
      bufferStr.resize(readBytes); // Remove the trash on the end of the buffer
      finalBuffer += bufferStr; // Add the buffer (string) to the 'final' buffer that will be returned
    }
    usleep(100); // CPU frying protection
  }
  return finalBuffer;
}

// Send a byte array (char*) to the client

bool SocketHandler::sendByteArray(const char *data, int size) {
  if (write(connfd, data, size) == size) { // Write it out and check if it was successful
    return true; // Success
  } else {
    return false; // Failure
  }
}

// Send a string to the client

bool SocketHandler::sendString(string data) {
  data = to_string(data.size()) + "\n" + data; // Append the length of the string to the beginning and separate it using a newline
  return sendByteArray(data.c_str(), data.size()); // Send it and report if it was successful
}

// Start the listener and other initialization stuff

void SocketHandler::startListner() {
  thread _listenerThread(&SocketHandler::listener, this); // Start thread for listening on the socket for new data
  _listenerThread.detach(); // Detach
  listenerThread = move(_listenerThread); // Move the thread to a 'permanent' home

  thread _stopHandlerThread(&SocketHandler::stopHandler, this);  // Start thread for handling/detecting the disconnection of the client
  _stopHandlerThread.detach(); // Detach
  stopHandlerThread = move(_stopHandlerThread); // Move the thread to a 'permanent' home

  connectionId = time(0); // Get the system time as an id for the connection/client
}

// Reset the keep-alive timer

void SocketHandler::triggerKeepalive() {
  lastKeepaliveRequest = chrono::high_resolution_clock::now(); // Reset timer
}

// INTERNAL: function for distinguishing keep-alive requests and normal messages

string SocketHandler::keepaliveHandler(string input) {
  YAML::Node keepalive = YAML::Load(input); // Try to load the string as YAML
  YAML::Node parameter; // Stuff...
  if ((parameter = keepalive[PACKET_TYPE_NAME])) { // Check if PACKET_TYPE_NAME is a valid key
    if (parameter.as<string>() == PACKET_REQUEST_NAME) { // Check if the packet type is a request
      if ((parameter = keepalive[REQUEST_TYPE])) { // Check if REQUEST_TYPE is a valid key
        if (parameter.as<string>() == REQUEST_TYPE_KEEPALIVE) { // Check if the request type is a keep-alive request
          lastKeepaliveRequest = chrono::high_resolution_clock::now(); // If yes reset the timer
          return ""; // And discard the message
        }
      }
    }
  }
  return input; // If it isn't a match return the message back
}

// INTERNAL (thread): Listener for incoming messages and keep-alive requests

void SocketHandler::listener() {
  while (isConnOpen) { // Only run while the socket is open
    if (useHandler) { // And when it should use the handler
      string message = receiveStringUntilMessageEnd(); // Receive/wait for a message
      if (keepaliveHandler(message) != "" && receiveHandler) receiveHandler(message, this); // Check if the message is not a keep-alive request and if the handler exists and call the handler if all of those conditions are met
    }
  }
}

// INTERNAL (thread): Checks if ether the grace period isn't over or the last keep-alive request wasn't longer than a specified time ago

void SocketHandler::stopHandler() {
  chrono::time_point<chrono::high_resolution_clock> intialConnectionTime = chrono::high_resolution_clock::now(); // Start the grace period timer for the keep-alive requests

  while (isConnOpen) { // Only run when the connection is open
    auto currentTime = chrono::high_resolution_clock::now(); // Get the current time using chrono
    unsigned long timeElapsedSinceLastKeepalive = (chrono::duration_cast<chrono::milliseconds>(currentTime - lastKeepaliveRequest)).count(); // Calculate how much time has elapsed since the last keep-alive request was received
    unsigned long timeElapsedSinceConnectionStart = (chrono::duration_cast<chrono::milliseconds>(currentTime - intialConnectionTime)).count(); // Calculate how much time has elapsed since the grace period has started (grace period starts when initializing the stopHandler() thread)

    if (timeElapsedSinceLastKeepalive > KEEPALIVE_TIMEOUT && timeElapsedSinceConnectionStart > KEEPALIVE_GRACE_TIME_AFTER_START && isConnOpen) { // Checks if it should terminate the connection because the timeout is over
      Logger::urgent("Client went unresponsive, assuming client disconnected", LOG_AEREA_SOCKET_HANDLER);
      isConnOpen = false; // Report to all threads that the client disconnected
      if (listenerThread.joinable()) listenerThread.join(); // Join the listener to wait until it has closed
      close(connfd); // Close the sockets
      if (disconnectHandler) disconnectHandler(this); // Run the disconnect handler if it exists
    }

    usleep(1000); // Anti CPU at 100% pegger
  }
}
