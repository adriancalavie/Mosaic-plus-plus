#pragma once
#include <string>
#include <optional>
#include <iostream>
#include <vector>
#include <regex>
#include <fstream>

#include "PictureTools.h"
#include "Mosaic.h"
class Command
{
	using argument = const std::string&;

public:
	static const std::string mosaicDocs;
	static const std::string selectDocs;

public:
	enum class HelpType { none, condensed, expanded };
	enum class CommandType { unknown, mosaic, folder };

public:
	Command(std::vector<std::string> arguments, BasePictures& pool);

private:

	bool help();
	void makeMosaic(argument, const std::optional<uint8_t>&, BasePictures&) const;
	void selectFolder(BasePictures& pool, argument path) const;
	void selectPicturesExtension(BasePictures&, const std::string&) const;

private:

	HelpType m_help = HelpType::none;
	CommandType m_type = CommandType::unknown;
};