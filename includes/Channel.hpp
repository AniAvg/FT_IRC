#pragma once

#include <set>
#include <string>

class Channel
{
private:
	std::string name;
	std::set<int> members;
	std::set<int> operators;
	std::set<int> invited;
	std::string topic;
	std::string key;
	bool inviteOnly;
	bool topicOperatorsOnly;
	int userLimit;

public:
	Channel();
	Channel(const std::string& name);

	const std::string& getName() const;
	const std::set<int>& getMembers() const;
	const std::set<int>& getOperators() const;
	const std::string& getTopic() const;
	const std::string& getKey() const;
	bool isInviteOnly() const;
	bool isTopicOperatorsOnly() const;
	int getUserLimit() const;

	void addMember(int fd);
	void removeMember(int fd);
	bool hasMember(int fd) const;
	bool hasMemeber(int fd) const;
	void addOperator(int fd);
	void removeOperator(int fd);
	bool isOperator(int fd) const;
	void addInvite(int fd);
	bool isInvited(int fd) const;
	void removeInvite(int fd);
	void setTopic(const std::string& value);
	void setKey(const std::string& value);
	void removeKey();
	void setInviteOnly(bool value);
	void setTopicOperatorsOnly(bool value);
	void setUserLimit(int value);
};
