#include "Channel.hpp"

Channel::Channel()
{}

Channel::Channel(const std::string& name) : name(name)
{}


const std::string& Channel::getName() const
{
	return name;
}

void Channel::addMember(int fd)
{
	members.insert(fd);
}

bool Channel::hasMemeber(int fd) const
{
	return members.find(fd) != members.end();
}
