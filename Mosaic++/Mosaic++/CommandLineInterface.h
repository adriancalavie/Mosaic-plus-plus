#pragma once

//ATTENTION: HARDCODED FOR "make" COMMAND ONLY!!!

#include <iostream>
#include "../Picture Tools/PictureTools.h"
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
private:
	static const enum class Parameter { PATH, EXTENSION, TYPE, SIZE, METHOD, HELP, VERSION };

	static const enum class PathType {SOURCE, PROCESSED, DATABASE, NUMBER_PHOTOS};

	static const std::unordered_set<std::string> KNOWN_EXTENSIONS;
	static const std::unordered_set<std::string> COMMANDS;
	static const std::unordered_map<std::string, Parameter> FLAGS;
	static std::unordered_map<PathType, std::string> PATHS;

	//static std::string DEFAULT_PATH;

public:

	CommandLineInterface(int argc, char* args[]);

	void make(const std::vector<std::string>& params);
	void make(const std::vector<std::string>& params, const std::unordered_map<std::string, std::string>& flags);
	void make();

	void setImgPoolDir(const std::vector<std::string>& params);
	void setImgPoolDir(const std::vector<std::string>& params, const std::unordered_map<std::string, std::string>& flags);
	void setImgPoolDir();

	void commandController(const std::string& command, const std::vector<std::string>& params);
	void commandController(const std::string& command, const std::vector<std::string>& params, const std::unordered_map<std::string, std::string>& flags);
	void commandController(const std::string& command);


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

