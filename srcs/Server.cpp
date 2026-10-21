#include "Server.hpp"

Server::Server(int port, const std::string& password)
{}

Server::~Server()
{
	if (listeningFd != -1)
		close(listeningFd);
}


void Server::init()
{
	//create socket
	// configure socket
	//make socket non blocking
	//prepare the addredd
	//bind the socket to the port
	//start listening
}
