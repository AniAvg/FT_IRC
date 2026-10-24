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
	listeningFd = sockect(AF_INET, SOCK_STREAM, 0);
	if (listeningFd == -1)
		throw std::runtime_error(std::string("socket: ") + std::strerror(errno));

	// configure socket
	int yes = 1;
	if (setsocket(listeningFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		int error = errno;
		close(listeningFd);
		listeningFd = -1;
		throw std::runtime_error(std::string("setsockopt: ") + std::strerror(errno));
	}

	//make socket non blocking
	if (fcnl(listeningFd, F_SETFL, O_NONBLOCK) == -1)
	{
		int error = errno;
		close(listeningFd);
		listeningFd = -1;
		throw std::runtime_error(std::string("fcntl: ") + std::strerror(error));
	}

	//prepare the address
	sockaddr_in address;
	std::memset(&address, 0, sizeof(address));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(static_cast<unsigned short>(port));

	//bind the socket to the port
	if (bind(listeningFd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) == -1)
	{
		int error = errno;
		close(listeningFd);
		listeningFd = -1;
		throw std::runtime_error(std::string("bind: ") + std::strerror(error));
	}

	//start listening
	if (listen(listeningFd, SOMAXCONN) == -1)
	{
		int error = errno;
		close(listeningFd);
		listeningFd = -1;
		throw std::runtime_error(std::string("listen: ") + std::strerror(error));
	}

	std::cout << "Server is listening on port " << port << std::endl;
}
