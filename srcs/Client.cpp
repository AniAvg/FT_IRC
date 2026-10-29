#include "Client.hpp"

Client::Client(int fd) : fd(fd), passwordAuthenticated(false), connected(true)
{}

const std::string& Client::getInputBuffer() const
{
	return inputBuffer;
}

void Client::appendToInputBuffer(const std::string &data)
{
	inputBuffer += data;
}

void Client::removeFromInputBuffer(std::size_t length)
{
	inputBuffer.erase(0, length);
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

