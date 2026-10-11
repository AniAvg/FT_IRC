#pragma once

#include <cstddef>
#include <string>

class Client
{
private:
	int fd;
	std::string nickname;
	std::string username;
	bool passwordAuthenticated;
	std::string inputBuffer;
	std::string outputBuffer;
	bool connected;

public:
	// Client();
	// explicit Client(int fd);
	// int getFd() const;
	// const std::string &getNickname() const;
	// const std::string &getUsername() const;
	// bool isPasswordAuthenticated() const;
	// bool hasRegistrationInfo() const;
	// bool isConnected() const;
	// void setNickname(const std::string &value);
	// void setUsername(const std::string &value);
	// void setPasswordAuthenticated(bool value);
	// void setConnected(bool value);
	// std::string &getInputBuffer();
	// const std::string &getOutputBuffer() const;
	// void appendInput(const std::string &data);
	// void queueMessage(const std::string &message);
	// void removeSentOutput(std::size_t length);
	// bool hasPendingOutput() const;
};
