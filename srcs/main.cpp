#include "Server.hpp"

#include <iostream>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port <password>" << std::endl;
		return 1;
	}


	char *end;
	errno = 0;
	long port = std::strtol(argv[1], &end, 10);

	if (errno != 0 || *argv[1] == '\0' || *end != '\0'
			|| port < 1 || port > 65535)
	{
		std::cerr << "Error: invalid port" << std::endl;
		return 1;
	}

	try
	{
		Server server(static_cast<int>(port), argv[2]);
		server.init();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
