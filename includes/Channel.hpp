#pragma once

#include <set>
#include <string>

class Channel
{
private:
	std::string name;
	std::set<int> members;

public:
	Channel();
	Channel(const std::string& name);

	const std::string& getName() const;
	const std::set<int>& getMembers() const;

	void addMember(int fd);
	bool hasMemeber(int fd) const;
};
