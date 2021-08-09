#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

class Data
{
public:

	class Info
	{
	public:
		static const std::string VERSION;
		static const std::string GENERAL_HELP;
	};

	class Errors
	{
	public:
		static const std::string BASE_PICTURES_EMPTY;
		static const std::string PICTURE_FOR_MOSAIC_EMPTY;
		static const std::string UNSUPPORTED_PICTURE;
		static const std::string ANOTHER_ERROR;
	};

	class Defaults
	{
	public:
		static const std::string PATH_BASE_PICTURES;
		static const std::string PATH_PICTURES_FOR_MOSAIC;
		static const std::string PATH_DATA_BASE_FILE;
		static const std::string PATH_TEST_IMAGE;
		static const std::string PATH_RESULT_IMAGE;

	};

};

