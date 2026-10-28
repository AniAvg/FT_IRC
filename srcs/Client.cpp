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
