#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

class Data
{
public:

	static class Info
	{
	public:
		static const std::string VERSION;
		static const std::string GENERAL_HELP;
	};

	static class Errors
	{
	public:
		static const std::string BASE_PICTURES_EMPTY;
		static const std::string PICTURE_FOR_MOSAIC_EMPTY;
		static const std::string UNSUPPORTED_PICTURE;
		static const std::string ANOTHER_ERROR;
	};

	static class Defaults
	{
	public:
		static const std::string PATH_BASE_PICTURES;
		static const std::string PATH_PICTURES_FOR_MOSAIC;
		static const std::string PATH_DATA_BASE_FILE;
		static const std::string PATH_TEST_image;
		static const std::string PATH_RESULT_image;

	};

};

