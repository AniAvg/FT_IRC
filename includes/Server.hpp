#pragma once

#include <poll.h>
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
	// Server(int port, const std::string &password);
	~Server();
	// int getListeningFd() const;
	// int getPort() const;
	// const std::string &getPassword() const;
	// std::map<int, Client> &getClients();
	// const std::map<int, Client> &getClients() const;
	// std::map<std::string, Channel> &getChannels();
	// const std::map<std::string, Channel> &getChannels() const;
	// std::vector<struct pollfd> &getPollFds();
	// const std::vector<struct pollfd> &getPollFds() const;
	// void setListeningFd(int fd);
	// void addClient(const Client &client);
	// void removeClient(int fd);
	// void addChannel(const Channel &channel);
	// void removeChannel(const std::string &channelName);
};
