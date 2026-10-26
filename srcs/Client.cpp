#include "Client.hpp"

Client::Client(int fd) : fd(fd), passwordAuthenticated(false), connected(true)
{}

