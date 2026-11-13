#include "Channel.hpp"

Channel::Channel() : inviteOnly(false), topicOperatorsOnly(false), userLimit(0)
{}

Channel::Channel(const std::string& name) : name(name), inviteOnly(false),
	topicOperatorsOnly(false), userLimit(0)
{}


const std::string& Channel::getName() const
{
	return name;
}

const std::set<int>& Channel::getMembers() const
{
	return members;
}

const std::set<int>& Channel::getOperators() const
{
	return operators;
}

const std::string& Channel::getTopic() const
{
	return topic;
}

const std::string& Channel::getKey() const
{
	return key;
}

bool Channel::isInviteOnly() const
{
	return inviteOnly;
}

bool Channel::isTopicOperatorsOnly() const
{
	return topicOperatorsOnly;
}

int Channel::getUserLimit() const
{
	return userLimit;
}



void Channel::addMember(int fd)
{
	members.insert(fd);
}

void Channel::removeMember(int fd)
{
	members.erase(fd);
	operators.erase(fd);
}

bool Channel::hasMember(int fd) const
{
	return members.find(fd) != members.end();
}

bool Channel::hasMemeber(int fd) const
{
	return hasMember(fd);
}

void Channel::addOperator(int fd)
{
	operators.insert(fd);
}

void Channel::removeOperator(int fd)
{
	operators.erase(fd);
}

bool Channel::isOperator(int fd) const
{
	return operators.find(fd) != operators.end();
}

void Channel::addInvite(int fd)
{
	invited.insert(fd);
}

bool Channel::isInvited(int fd) const
{
	return invited.find(fd) != invited.end();
}

void Channel::removeInvite(int fd)
{
	invited.erase(fd);
}

void Channel::setTopic(const std::string& value)
{
	topic = value;
}

void Channel::setKey(const std::string& value)
{
	key = value;
}

void Channel::removeKey()
{
	key.clear();
}

void Channel::setInviteOnly(bool value)
{
	inviteOnly = value;
}

void Channel::setTopicOperatorsOnly(bool value)
{
	topicOperatorsOnly = value;
}

void Channel::setUserLimit(int value)
{
	userLimit = value;
}
