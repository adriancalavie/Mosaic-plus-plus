#include "CommandLineInterface.h"

const std::unordered_map<std::string, CommandLineInterface::ParameterType> CommandLineInterface::flags = {
	{"--shape",		ParameterType::TYPE},
	{"-s",			ParameterType::TYPE},

	{"--extension",	ParameterType::EXTENSION},
	{"-e",			ParameterType::EXTENSION},

	{"--directory",	ParameterType::PATH},
	{"-d",			ParameterType::PATH},

	{"--help",		ParameterType::NONE},
	{"-h",			ParameterType::NONE},

	{"--version",	ParameterType::NONE},
	{"-v",			ParameterType::NONE}
};

const std::unordered_set<std::string> CommandLineInterface::commands = { "make", "set_img_pool_dir" };


const std::unordered_set<std::string> CommandLineInterface::knownExtensions = { "jpg", "png" };