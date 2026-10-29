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
	Client(int fd);

	const std::string &getInputBuffer() const;
	void appendToInputBuffer(const std::string &data);
	void removeFromInputBuffer(std::size_t length);

	bool extractMessage(std::string &message);
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
	// const std::string &getOutputBuffer() const;
	// void queueMessage(const std::string &message);
	// bool hasPendingOutput() const;
};
