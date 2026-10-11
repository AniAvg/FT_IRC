#pragma once

#include <set>
#include <string>

class Channel
{
private:
	std::string name;
	std::set<int> members;
	std::set<int> operators;
	std::set<int> invitedClients;
	std::string topic;
	std::string key;
	bool inviteOnly;
	bool topicRestricted;
	int userLimit;

public:
	// Channel();
	// explicit Channel(const std::string &name);
	// const std::string &getName() const;
	// const std::set<int> &getMembers() const;
	// const std::set<int> &getOperators() const;
	// const std::set<int> &getInvitedClients() const;
	// const std::string &getTopic() const;
	// const std::string &getKey() const;
	// bool isInviteOnly() const;
	// bool isTopicRestricted() const;
	// bool hasKey() const;
	// int getUserLimit() const;
	// void setTopic(const std::string &value);
	// void setKey(const std::string &value);
	// void removeKey();
	// void setInviteOnly(bool value);
	// void setTopicRestricted(bool value);
	// void setUserLimit(int value);
	// void removeUserLimit();
	// bool hasMember(int fd) const;
	// bool hasOperator(int fd) const;
	// bool isInvited(int fd) const;
	// void addMember(int fd);
	// void removeMember(int fd);
	// void addOperator(int fd);
	// void removeOperator(int fd);
	// void invite(int fd);
	// void removeInvite(int fd);
};
