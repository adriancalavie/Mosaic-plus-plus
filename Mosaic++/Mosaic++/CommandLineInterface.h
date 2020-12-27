#pragma once

//ATTENTION: HARDCODED FOR "make" COMMAND ONLY!!!

#include <iostream>
#include "PictureTools.h"
#include "StopWatch.h"
#include "Mosaic.h"
#include "Data.h"
#include <unordered_set>
#include <io.h>   // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <regex>

//errors


class CommandLineInterface
{
	static const enum class Parameter { PATH, EXTENSION, TYPE, SIZE, METHOD, NONE };
	static const std::unordered_set<std::string> KNOWN_EXTENSIONS;
	static const std::unordered_set<std::string> COMMANDS;
	static const std::unordered_map<std::string, Parameter> FLAGS;

public:

	CommandLineInterface(int argc, char* args[]);

	void make(const std::vector<std::string>& params);
	void make(const std::vector<std::string>& params, const std::unordered_map<std::string, std::string>& flags);
	void make(const std::string& flag);
private:

	inline bool isExtension(const std::string& parameter);
	inline bool isDir(const std::string& parameter);
	inline bool isType(const std::string& parameter);
	inline bool isSize(const std::string& parameter);
	inline bool isMethod(const std::string& parameter);
	inline bool inCheck(const std::string& flag, const std::string& parameter);
	inline bool isFile(const std::string& parameter);
	inline bool isPath(const std::string& parameter);
};

