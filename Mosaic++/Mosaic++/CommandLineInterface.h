#pragma once

//ATTENTION: HARDCODED FOR "make" COMMAND ONLY!!!

#include <iostream>
#include "BasePictures.h"
#include "PictureTools.h"
#include "StopWatch.h"
#include "Mosaic.h"
#include <unordered_set>
#include <io.h>   // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <regex>


class CommandLineInterface
{
	static const enum class ParameterType { PATH, EXTENSION, TYPE, NONE };
	static const std::unordered_set<std::string> knownExtensions;
	static const std::unordered_set<std::string> commands;
	static const std::unordered_map<std::string, ParameterType> flags;
public:

	CommandLineInterface(int argc, char* args[]);

	void make(const std::vector<std::string>& params);
	void make(const std::vector<const std::string&>& params, const std::unordered_map<std::string, std::string>& flags);
	void make(const std::string& flag);
private:

	inline bool isExtension(const std::string& parameter);
	inline bool isDir(const std::string& parameter);
	inline bool isType(const std::string& parameter);
	inline bool inCheck(const std::string& flag, const std::string& parameter);
	inline bool isFile(const std::string& parameter);
	inline bool isPath(const std::string& parameter);
};

