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

	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		throw std::runtime_error("Signal: failed to ignore SIGPIPE");

	// configure socket
	int yes = 1;
	if (setsockopt(listeningFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		int error = errno;
		close(listeningFd);
		listeningFd = -1;
		throw std::runtime_error(std::string("setsockopt: ") + std::strerror(error));
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
	bool serverRegistered = false;
	for (std::size_t i = 0; i < pollFds.size(); i++)
	{
		if (pollFds[i].fd == listeningFd)
		{
			serverRegistered = true;
			break ;
		}
	}

	if (!serverRegistered)
	{
		struct pollfd serverPoll;

		serverPoll.fd = listeningFd;
		serverPoll.events = POLLIN;
		serverPoll.revents = 0;

		pollFds.push_back(serverPoll);
	}

	while (true)
	{
		int result = poll(&pollFds[0], pollFds.size(), -1);

		if (result == -1)
		{
			if (errno == EINTR)
				continue ;

			throw std::runtime_error(std::string("poll: ") + std::strerror(errno));
		}

		for (std::size_t i = pollFds.size(); i > 0; --i)
		{
			int fd = pollFds[i - 1].fd;
			short events = pollFds[i - 1].revents;

			if (events == 0)
				continue ;

			if (fd == listeningFd)
			{
				if (events & (POLLERR | POLLHUP | POLLNVAL))
					throw std::runtime_error("poll: listening socket error");
				if (events & POLLIN)
					acceptClient();
				continue ;
			}

			if (events & POLLIN)
				handleClient(fd);
			if (events & POLLOUT)
				sendMessage(fd);
			if (events & (POLLHUP | POLLERR | POLLNVAL))
				disconnectClient(fd);
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

	clients.insert(std::make_pair(clientFd, Client()));

	struct pollfd clientPoll;

	clientPoll.fd = clientFd;
	clientPoll.events = POLLIN;
	clientPoll.revents = 0;

	pollFds.push_back(clientPoll);

	std::cout << "New client connected. FD: " << clientFd << std::endl;

}



void Server::checkRegistration(int fd)
{
	std::map<int, Client>::iterator it = clients.find(fd);

	if (it == clients.end())
		return ;

	if (it->second.isRegistered() &&
		!it->second.isRegistrationCompleted())
	{
		std::string nickname = it->second.getNickname();

		queueMessage(fd, ":ircserv 001 " + nickname + " :Welcome to the IRC server\r\n");

		it->second.setRegistrationCompleted(true);

		std::cout << "FD " << fd << " is now fully registered!" << std::endl;
	}
}



void Server::handleClient(int fd)
{
	char buffer[512];

	// read the bytes currently available from this clinent
	ssize_t bytesReceived = recv(fd, buffer, sizeof(buffer), 0);

	if (bytesReceived > 0)
	{
		std::map<int, Client>::iterator it = clients.find(fd);
		if (it == clients.end())
			return ;

		it->second.appendToInputBuffer(std::string(buffer, static_cast<std::size_t>(bytesReceived)));

		// std::string& input= const_cast<std::string &>(it->second.getInputBuffer());

		std::string message;
		// std::string::size_type end;
		// while ((end = input.find("\r\n")) != std::string::npos)
		while (it->second.extractMessage(message))
		{
			handleMessage(fd, message);
		}
	}
	else if (bytesReceived == 0)
	{
		disconnectClient(fd);
	}
	else
	{
		if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			return ;

		std::cerr << "recv error on FD " << fd << ": " << std::strerror(errno) << std::endl;

		disconnectClient(fd);
	}
}

void Server::disconnectClient(int fd)
{
	std::map<int, Client>::iterator clientIt = clients.find(fd);

	if (clientIt == clients.end())
		return ;

	close(fd);
	clients.erase(clientIt);

	for (std::vector<struct pollfd>::iterator it = pollFds.begin(); it != pollFds.end(); ++it)
	{
		if (it->fd == fd)
		{
			pollFds.erase(it);
			break ;
		}
	}

	std::cout << "Client disconnected. FD: " << fd << std::endl;
}





void Server::handlePass(int fd, const std::vector<std::string>& params)
{
	std::map<int, Client>::iterator it = clients.find(fd);
	if (it == clients.end())
		return ;

	if (params.size() != 1)
	{
		std::cout << "PASS: missing or invalid parameter count" << std::endl;
		return ;
	}

	if (it->second.isRegistered())
	{
		std::cout << "PASS: client is already authenticated" << std::endl;
		return ;
	}

	if (params[0] == password)
	{
		it->second.setPasswordAuthenticated(true);
		std::cout << "FD " << fd << " authenticated successfully" << std::endl;
	}
	else
	{
		queueMessage(fd, ":ircserv 464 * :Password incorrect\r\n");

		std::cout << "FD " << fd << " provided an incorrect password" << std::endl;
	}
}


/// dup nickname checking
static bool equalsIgnoreCase(const std::string& a, const std::string& b)
{
	if (a.size() != b.size())
		return false;

	for (std::size_t i = 0; i < a.size(); ++i)
	{
		unsigned char ca = static_cast<unsigned char>(a[i]);
		unsigned char cb = static_cast<unsigned char>(b[i]);

		if (std::tolower(ca) != std::tolower(cb))
			return false;
	}

	return true;
}




void Server::handleNick(int fd, const std::vector<std::string>& params)
{
	std::map<int, Client>::iterator clientIt = clients.find(fd);
	if (clientIt == clients.end())
		return ;

	if (params.size() != 1 || params[0].empty())
	{
		std::cout << "NICK: missing or invalid parameter" << std::endl;
		return ;
	}

	const std::string& nickname = params[0];
	if (nickname.size() > 9 || !std::isalpha(static_cast<unsigned char>(nickname[0])))
	{
		std::cout << "NICK: invalid nickname" << std::endl;
		return ;
	}

	for (std::size_t i = 0; i < nickname.size(); ++i)
	{
		unsigned char c = static_cast<unsigned char>(nickname[i]);

		if (!std::isalnum(c) && c != '-' && c != '_')
		{
			std::cout << "NICK: invalid nickname" << std::endl;
			return ;
		}
	}
	for (std::map<int, Client>::iterator it = clients.begin();
		it != clients.end(); ++it)
	{
		if (it->first != fd && !it->second.getNickname().empty()
			&& equalsIgnoreCase(it->second.getNickname(), nickname))
		{
			std::cout << "NICK: nickname is already in use" << std::endl;
			return;
		}
	}

	clientIt->second.setNickname(nickname);
	checkRegistration(fd);

	std::cout << "FD " << fd << " changed nickname to " << nickname << std::endl;
}



void Server::handleUser(int fd, const std::vector<std::string>& params)
{
	std::map<int, Client>::iterator it = clients.find(fd);
	if (it == clients.end())
		return ;

	if (params.size() != 4)
	{
		std::cout << "USER: invalid parameter count" << std::endl;
		return ;
	}
	if (it->second.isRegistered())
	{
		std::cout << "USER: client is already registered" << std::endl;
		return ;
	}
	if (params[0].empty())
	{
		std::cout << "USER: username cannot be empty" << std::endl;
		return ;
	}

	it->second.setUsername(params[0]);
	checkRegistration(fd);

	std::cout << "FD " << fd << " set username to " << params[0] << std::endl;

	if (it->second.isRegistered())
		std::cout << "FD " << fd << " is now fully registered!" << std::endl;
}



void Server::handleMessage(int fd, const std::string& message)
{
	std::string command;
	std::vector<std::string> params;
	std::string::size_type pos = 0;

	while  (pos < message.size() && message[pos] == ' ')
		++pos;

	std::string::size_type start = pos;

	while (pos < message.size() && message[pos] != ' ')
		++pos;

	command = message.substr(start, pos - start);

	for (std::string::size_type i = 0; i < command.size(); ++i)
		command[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(command[i])));

	while (pos < message.size())
	{
		while (pos < message.size() && message[pos] == ' ')
			++pos;
		if (pos >= message.size())
			break ;

		if (message[pos] == ':')
		{
			params.push_back(message.substr(pos + 1));
			break ;
		}

		start = pos;
		while (pos < message.size() && message[pos] != ' ')
			++pos;

		params.push_back(message.substr(start, pos - start));
	}


	if (command == "PASS")
		handlePass(fd, params);
	else if (command == "NICK")
		handleNick(fd, params);
	else if (command == "USER")
		handleUser(fd, params);
	else
		std::cout << "Unknown or not implemented command: " << command << std::endl;


	for (std::size_t i = 0; i < params.size(); ++i)
	{
		std::cout << "Parameter " << i + 1 << ": " << params[i] << std::endl;
	}
}



void Server::queueMessage(int fd, const std::string& message)
{
	std::map<int, Client>::iterator it = clients.find(fd);
	if (it == clients.end())
		return ;

	it->second.appendToOutputBuffer(message);
	for (std::size_t i = 0; i < pollFds.size(); ++i)
	{
		if (pollFds[i].fd == fd)
		{
			pollFds[i].events |= POLLOUT;
			break ;
		}
	}
}


void Server::sendMessage(int fd)
{
	std::map<int, Client>::iterator it = clients.find(fd);
	if (it == clients.end())
		return ;

	const std::string& buffer = it->second.getOutputBuffer();
	if (buffer.empty())
		return ;

	ssize_t bytesSent = send(fd, buffer.c_str(), buffer.size(), 0);
	if (bytesSent > 0)
		it->second.removeFromOutputBuffer(static_cast<std::size_t>(bytesSent));
	else if (bytesSent == 0)
	{
		disconnectClient(fd);
		return ;
	}
	else
	{
		if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			return ;

		std::cerr << "send error on FD " << fd << ": "
				<< std::strerror(errno) << std::endl;
		disconnectClient(fd);
		return ;
	}

	if (!it->second.hasPendingOutput())
	{
		for (std::size_t i = 0; i < pollFds.size(); ++i)
		{
			if (pollFds[i].fd == fd)
			{
				pollFds[i].events &= ~POLLOUT;
				break ;
			}
		}
	}

}



void Server::handleJoin(int fd, const std::vector<std::string>& params)
{
	std::map<int, Client>::iterator clientIt = clients.find(fd);

	if (clientIt == clients.end())
		return;

	if (!clientIt->second.isRegistered())
	{
		queueMessage(fd, ":ircserv 451 * :You have not registered\r\n");
		return;
	}

	// Check parameters
	if (params.empty())
	{
		queueMessage(fd, ":ircserv 461 " + clientIt->second.getNickname()
			+ " JOIN :Not enough parameters\r\n");
		return;
	}

	// Get channel name
	std::string channelName = params[0];

	// Validate channel name
	if (channelName.empty() || channelName[0] != '#')
	{
		queueMessage(fd, ":ircserv 403 " + channelName + " :No such channel\r\n");
		return;
	}

	// Find or create channel
	std::map<std::string, Channel>::iterator channelIt = channels.find(channelName);

	if (channelIt == channels.end())
	{
		Channel newChannel(channelName);
		channels.insert(std::make_pair(channelName, newChannel));

		channelIt = channels.find(channelName);
	}

	// add client to channel
	channelIt->second.addMember(fd);

	// send JOIN confirmation
	queueMessage(fd, ":" + clientIt->second.getNickname() + "!"
		+ clientIt->second.getUsername() + "@localhost JOIN " + channelName + "\r\n");

	std::cout << clientIt->second.getNickname() << " joined "
		<< channelName << std::endl;
}
