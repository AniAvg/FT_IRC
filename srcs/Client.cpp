#include "Client.hpp"

Client::Client(int fd) : fd(fd), passwordAuthenticated(false), connected(true)
{}



void Client::appendToInputBuffer(const std::string &data)
{
	inputBuffer += data;
}


bool Client::extractMessage(std::string &message)
{
	std::string::size_type end = inputBuffer.find("\r\n");

	if (end == std::string::npos)
		return false;

	message = inputBuffer.substr(0, end);
	inputBuffer.erase(0, end + 2);
	return true;
}

