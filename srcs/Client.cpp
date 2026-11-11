#include "Client.hpp"

// Client::Client(int fd) : fd(fd), passwordAuthenticated(false), connected(true)

Client::Client() : passwordAuthenticated(false), registrationCompleted(false)
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

bool Client::isRegistered() const
{
	return (passwordAuthenticated && !nickname.empty()
			&& !username.empty());
}



// getters and setters
const std::string& Client::getNickname() const
{
	return nickname;
}

const std::string& Client::getUsername() const
{
	return username;
}


bool Client::isPasswordAuthenticated() const
{
	return passwordAuthenticated;
}


bool Client::isRegistrationCompleted() const
{
	return registrationCompleted;
}

void Client::setRegistrationCompleted(bool value)
{
	registrationCompleted = value;
}





void Client::setNickname(const std::string &nickname)
{
	this->nickname = nickname;
}

void Client::setUsername(const std::string &username)
{
	this->username = username;
}

void Client::setPasswordAuthenticated(bool value)
{
	this->passwordAuthenticated = value;
}



void Client::appendToOutputBuffer(const std::string &data)
{
	outputBuffer += data;
}
const std::string& Client::getOutputBuffer() const
{
	return outputBuffer;
}



void Client::removeFromOutputBuffer(std::size_t length)
{
	outputBuffer.erase(0, length);
}

bool Client::hasPendingOutput() const
{
	return !outputBuffer.empty();
}



