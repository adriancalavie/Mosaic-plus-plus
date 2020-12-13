#pragma once
#include <string>
#include <map>

class CommandLineInterface
{
	enum class Command {
		MOSAIC,
		MOSAIC_H,
		MOSAIC_HELP,
		FOLDER,
		FOLDER_H,
		FOLDER_HELP,
		COMPILE,
		COMPILE_H,
		COMPILE_HELP,
		SELECTOR_TRIANGLE,
		SELECTOR_RECTANGLE,
		SELECTOR_H,
		SELECTOR_HELP
	};


public:
	CommandLineInterface(const std::string&, const std::string&);

	static const std::map<std::string, CommandLineInterface::Command> vocabulary;
};

