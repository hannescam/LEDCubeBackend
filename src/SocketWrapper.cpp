#include "include/SocketWrapper.hpp"

void SocketAcceptor::startClientHandler(SocketHandler* socketConnection) {
  clientHandlerFunction(socketConnection);
}

void SocketAcceptor::listener() {
  int connfd;
  struct sockaddr_in cli;
  unsigned int cliLength;
  while (!stopListeningVar) {
    cliLength = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, &cliLength);
    if (connfd < 0) {
      cerr << "Failed to accept incoming connection request" << endl;
    } else {
      cout << "Accepted incoming connection request" << endl;
      usleep(5000);

      clientHandlers.resize(clientHandlers.size() + 1);
      clientHandlers.at(clientHandlers.size() - 1).setPacketSize(packetSize);
      clientHandlers.at(clientHandlers.size() - 1).setConnectionFileDescriptor(ref(connfd));
      clientHandlers.at(clientHandlers.size() - 1).startListner();

      thread clientHandlerThread(&SocketAcceptor::startClientHandler, this, &clientHandlers.at(clientHandlers.size() - 1));
      //clientHandlerThread.detach();
      //while (clientHandlers.at(clientHandlers.size() - 1).isOpen()) usleep(50000); // Prevent multiple clients from joining at the same time
      cout << "run" << endl;
      clientHandlerThreads.push_back(move(clientHandlerThread));
    }
    usleep(1000);
  }
  stopListeningVar = false;
}

bool SocketAcceptor::begin(int port, int _packetSize) {
  packetSize = _packetSize;
  struct sockaddr_in servaddr;

  cout << "Trying to create a C socket..." << endl;
  sockfd = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP/IP socket using the C socket api
  if (sockfd == -1) {
    cerr << "Error creating socket" << endl;
    return false;
  }

  cout << "Successfully created socket" << endl;

  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  cout << "Trying to bind the socket to address/port" << endl;
  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
    cerr << "Error binding socket to address/port" << endl;
    return false;
  }

  cout << "Successfully bound socket to address/port" << endl;
  cout << "Trying to start listening on the socket" << endl;
  // Now server is ready to listen and verification
  if ((listen(sockfd, 5)) != 0) {
    cerr << "Error while starting to listen on the socket" << endl;
    return false;
  }

  cout << "Started listening on socket" << endl;

  return true;
}

void SocketAcceptor::startListening() {
  cout << "Starting listener thread..." << endl;

  thread _listenerThread(&SocketAcceptor::listener, this);
  _listenerThread.detach();
  listnerThread = move(_listenerThread);

  cout << "Successfully started listener thread" << endl;
}

void SocketAcceptor::stopListening() {
  stopListeningVar = true;
}

void SocketAcceptor::closeSocket() {
  cout << "Closing socket..." << endl;
  close(sockfd);
}

void SocketAcceptor::setClientHandler(function<void(SocketHandler*)> _clientHandler) {
  clientHandlerFunction = _clientHandler;
}
