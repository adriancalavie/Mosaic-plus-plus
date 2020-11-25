#pragma once
#include <string>
#include <optional>
#include <iostream>

class Command
{
	using argument = const std::string&;

public:
	static const std::string mosaicDocs;
	static const std::string selectDocs;

public:
	enum class HelpType { none, condensed, expanded };
	enum class CommandType { mosaic, folder };

public:
	Command(argument in);
	bool help();

private:

	void makeMosaic(argument path, const std::optional<uint8_t>& partitionSize);
	void selectFolder(argument path);


private:

	HelpType m_help;
	CommandType m_type;
};

