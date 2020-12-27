#pragma once

#include <string>
#include <unordered_map>

class Data
{

private:
	static const enum class HelpTypes { MAKE_HELP, GENERAL_HELP, SET_POOL_HELP };

public:

	static class Info
	{
	public:
		static const std::string VERSION;
		
		static const std::unordered_map<std::string, HelpTypes>	HELP_LEVEL;

		static const std::string MAKE_HELP;
		static const std::string GENERAL_HELP;
		static const std::string SET_POOL_HELP;

	};
	static class Errors
	{
	public:
		static const std::string PARAMETER_COUNT;
		static const std::string UNKNOWN_COMMAND;
		static const std::string WRONG_ARGUMENT_ORDER;
		static const std::string WRONG_INPUT;
		static const std::string WRONG_ARGUMENT;

		static const std::string OUT_OF_BOUNDS;
	};

	static class Defaults
	{
	public:
		static const std::string PARENT_PATH;
		static const std::string PATH_BASE_PICTURES;
		static const std::string PATH_PICTURES_FOR_MOSAIC;
		static const std::string PATH_DATA_BASE_FILE;
		static const std::string PATH_TEST_IMAGE;
		static const std::string PATH_RESULT_IMAGE;
	};

};

