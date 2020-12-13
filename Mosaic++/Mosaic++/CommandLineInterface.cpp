#include "CommandLineInterface.h"

const std::map<std::string, uint8_t> CommandLineInterface::vocabulary = {
		{"mosaic", 0},
		{"mosaic-h", 1},
		{"mosaic--h", 2},
		{"folder", 3},
		{"folder-h", 4},
		{"folder--h", 5},
		{"compile", 6},
		{"compile-h", 7},
		{"compile--h", 8},
		{"selectortriangle", 9},
		{"selectorrectangle", 10},
		{"selector-h", 11},
		{"selector--h", 12},
};

CommandLineInterface::CommandLineInterface(const std::string& command)
{
	
}
