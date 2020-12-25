#pragma once
#include <string>


class Data
{
public:

	static class Info
	{
	public:
		static const std::string VERSION;
		static const std::string MAKE_HELP;
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
	};
};

