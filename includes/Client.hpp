#pragma once

#include <cstddef>
#include <string>

class Client
{
private:
	// int fd;
	std::string nickname;
	std::string username;
	bool passwordAuthenticated;
	bool registrationCompleted;
	std::string inputBuffer;
	std::string outputBuffer;
	// bool connected;

public:
	Client();
	// Client(int fd);

	void appendToInputBuffer(const std::string &data);
	bool extractMessage(std::string &message);

	bool isRegistered() const;

	const std::string &getNickname() const;
	const std::string &getUsername() const;

	bool isPasswordAuthenticated() const;

	bool isRegistrationCompleted() const;
	void setRegistrationCompleted(bool value);

	void setNickname(const std::string &nickname);
	void setUsername(const std::string &username);
	void setPasswordAuthenticated(bool value);

	void appendToOutputBuffer(const std::string& data);
	const std::string& getOutputBuffer() const;
	void removeFromOutputBuffer(std::size_t length);
	bool hasPendingOutput() const;
};
