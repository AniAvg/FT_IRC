#include "Server.hpp"

Server::Server(int port, const std::string& password) : listeningFd(-1), port(port), password(password)
{}

Server::~Server()
{
	if (listeningFd != -1)
		close(listeningFd);
}


void Server::init()
{
	//create socket
	listeningFd = socket(AF_INET, SOCK_STREAM, 0);
	if (listeningFd == -1)
		throw std::runtime_error(std::string("socket: ") + std::strerror(errno));

	// configure socket
	int yes = 1;
	if (setsockopt(listeningFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		int error = errno;
		close(listeningFd);
		listeningFd = -1;
		throw std::runtime_error(std::string("setsockopt: ") + std::strerror(errno));
	}

	//make socket non blocking
	if (fcntl(listeningFd, F_SETFL, O_NONBLOCK) == -1)
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



// getter functions
int Server::getListeningFd() const
{
	return listeningFd;
}

int Server::getPort() const
{
	return port;
}

// setter
void Server::setListeningFd(int fd)
{
	this->listeningFd = fd;
}


void Server::run()
{
	struct pollfd serverPoll;

	serverPoll.fd = listeningFd;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;

	pollFds.push_back(serverPoll);

	while (true)
	{
		int result = poll(&pollFds[0], pollFds.size(), -1);

		if (result == -1)
		{
			if (errno == EINTR)
				continue;

			throw std::runtime_error(std::string("poll: ") + std::strerror(errno));
		}

		for (size_t i = 0; i < pollFds.size(); ++i)
		{
			int fd = pollFds[i].fd;
			short events = pollFds[i].revents;

			if (events == 0)
				continue;

			if (fd == listeningFd)
			{
				if (events & POLLIN)
					acceptClient();
			}
			else if (events & (POLLHUP | POLLERR | POLLNVAL))
			{
				close(fd);
				clients.erase(fd);
				pollFds.erase(pollFds.begin() + i);
				--i;
			}
		}
	}
}


void Server::acceptClient()
{
	int clientFd = accept(listeningFd, NULL, NULL);
	if (clientFd == -1)
	{
		if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			return ;
		throw std::runtime_error(std::string("accept: ") + std::strerror(errno));
	}

	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
	{
		int error = errno;
		close(clientFd);
		throw std::runtime_error(std::string("fcntl client: ") + std::strerror(error));
	}

	clients.insert(std::make_pair(clientFd, Client(clientFd)));

	struct pollfd clientPoll;

	clientPoll.fd = clientFd;
	clientPoll.events = 0;
	clientPoll.revents = 0;

	pollFds.push_back(clientPoll);

	std::cout << "New client connected. FD: " << clientFd << std::endl;

}
