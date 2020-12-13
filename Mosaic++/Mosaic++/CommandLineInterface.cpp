#include "CommandLineInterface.h"
#include <iostream>

const std::map<std::string, CommandLineInterface::Command> CommandLineInterface::vocabulary = {
		{"mosaic", CommandLineInterface::Command::MOSAIC},
		{"mosaic-h", CommandLineInterface::Command::MOSAIC_H},
		{"mosaic--h", CommandLineInterface::Command::MOSAIC_HELP},
		{"folder", CommandLineInterface::Command::FOLDER},
		{"folder-h", CommandLineInterface::Command::FOLDER_H},
		{"folder--h", CommandLineInterface::Command::FOLDER_HELP},
		{"compile", CommandLineInterface::Command::COMPILE},
		{"compile-h", CommandLineInterface::Command::COMPILE_H},
		{"compile--h", CommandLineInterface::Command::COMPILE_HELP},
		{"selectortriangle", CommandLineInterface::Command::SELECTOR_TRIANGLE},
		{"selectorrectangle", CommandLineInterface::Command::SELECTOR_RECTANGLE},
		{"selector-h", CommandLineInterface::Command::SELECTOR_H},
		{"selector--h", CommandLineInterface::Command::SELECTOR_HELP},
};

CommandLineInterface::CommandLineInterface(const std::string& command)
{
	if (vocabulary.find(command) != vocabulary.end())
	{
		switch (vocabulary.find(command)->second)
		{
		case CommandLineInterface::Command::MOSAIC:

			break;
		case CommandLineInterface::Command::MOSAIC_H:

			break;
		case CommandLineInterface::Command::MOSAIC_HELP: 

			break;
		case CommandLineInterface::Command::FOLDER: 

			break;
		case CommandLineInterface::Command::FOLDER_H: 

			break;
		case CommandLineInterface::Command::FOLDER_HELP:

			break;
		case CommandLineInterface::Command::COMPILE:

			break;
		case CommandLineInterface::Command::COMPILE_H: 

			break;
		case CommandLineInterface::Command::COMPILE_HELP:

			break;
		case CommandLineInterface::Command::SELECTOR_TRIANGLE:

			break;
		case CommandLineInterface::Command::SELECTOR_RECTANGLE:

			break;
		case CommandLineInterface::Command::SELECTOR_H: 

			break;
		case CommandLineInterface::Command::SELECTOR_HELP:

			break;

		default:
			std::cerr << "how on earth did you manage to get here?" << std::endl; 
			break;
		}
	}
	else
	{
		std::cerr << "Bad input";
	}
}
