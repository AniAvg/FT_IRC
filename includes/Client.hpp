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

	void appendToInputBuffer(const std::string &data);
	bool extractMessage(std::string &message);

	bool isRegistered() const;

	const std::string &getNickname() const;
	const std::string &getUsername() const;

	bool isPasswordAuthenticated() const;

	void setNickname(const std::string &nickname);
	void setUsername(const std::string &username);
	void setPasswordAuthenticated(bool value);

	// explicit Client(int fd);
	// int getFd() const;
	// bool hasRegistrationInfo() const;
	// void setConnected(bool value);
	// const std::string &getOutputBuffer() const;
	// void queueMessage(const std::string &message);
	// bool hasPendingOutput() const;
};
