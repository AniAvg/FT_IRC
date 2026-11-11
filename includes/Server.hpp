#pragma once

#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <csignal>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdexcept>

#include <map>
#include <string>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"

class Server
{
private:
	int listeningFd;
	int port;
	std::string password;
	std::map<int, Client> clients;
	std::map<std::string, Channel> channels;
	std::vector<struct pollfd> pollFds;

public:
	Server(int port, const std::string &password);
	~Server();

	int getListeningFd() const;
	int getPort() const;
	void setListeningFd(int fd);

	void init();
	void run();
	void acceptClient();

	void checkRegistration(int fd);

	void handleClient(int fd);
	void disconnectClient(int fd);

	void handleMessage(int fd, const std::string& message);
	void handlePass(int fd, const std::vector<std::string>& params);
	void handleNick(int fd, const std::vector<std::string>& params);
	void handleUser(int fd, const std::vector<std::string>& params);

	void queueMessage(int fd, const std::string& message);
	void sendMessage(int fd);

	void handleJoin(int fd, const std::vector<std::string>& params);
};
