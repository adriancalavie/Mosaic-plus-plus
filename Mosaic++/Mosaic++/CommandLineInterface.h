#pragma once
#include <string>
#include <map>

class CommandLineInterface
{
public:
	CommandLineInterface(const std::string&);

	static const std::map<std::string, uint8_t> vocabulary;
};

