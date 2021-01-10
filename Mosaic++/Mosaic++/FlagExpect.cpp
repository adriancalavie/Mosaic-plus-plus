#include "FlagExpect.h"

FlagExpect::FlagExpect(const std::vector<std::string>& synonims, const Parameter& type, const std::optional<std::string>& command, const std::string& parameter)
	:Flag(synonims, type, command), m_parameter(parameter)
{

}

inline bool FlagExpect::isExtension() const
{
	return Data::Info::KNOWN_EXTENSIONS.find(this->m_parameter) != Data::Info::KNOWN_EXTENSIONS.end();
}

inline bool FlagExpect::isDir() const
{
	if (_access(this->m_parameter.c_str(), 0) == 0)
	{
		struct stat status;

		stat(this->m_parameter.c_str(), &status);

		if (status.st_mode & S_IFDIR)
		{
			return true;
		}

		else
		{
			//this is a file path, not a directory path

			return false;
		}
	}
	else
	{
		//The path doesn't exist

		return false;
	}

}

inline bool FlagExpect::isType() const
{
	std::regex poolType("diamond|rectangle|square|triangle", std::regex_constants::icase);

	return std::regex_match(this->m_parameter, poolType);
}

inline bool FlagExpect::isSize() const
{
	std::regex isNumber("[1-9]+[0-9]*");

	return std::regex_match(this->m_parameter, isNumber);
}

inline bool FlagExpect::isMethod() const
{
	std::regex poolMethod("cropp|resize", std::regex_constants::icase);

	return std::regex_match(this->m_parameter, poolMethod);
}

inline bool FlagExpect::isFile() const
{
	std::string matcher = ".*.(";

	for (auto extension : Data::Info::KNOWN_EXTENSIONS)
	{
		matcher += extension + '|';
	}

	matcher[matcher.size() - 1] = ')';
	return std::regex_match(this->m_parameter, std::regex(matcher));
}

inline bool FlagExpect::isPath() const
{
	struct stat buffer;
	return (stat(this->m_parameter.c_str(), &buffer) == 0);
}

inline bool FlagExpect::inCheck() const
{
	switch (this->m_type)
	{
	case Parameter::EXTENSION:
		return isExtension();
		break;

	case Parameter::PATH:
		return isDir();
		break;

	case Parameter::TYPE:
		return isType();
		break;

	case Parameter::SIZE:
		return isSize();
		break;

	case Parameter::METHOD:
		return isMethod();
		break;

	case Parameter::HELP:

	case Parameter::VERSION:

	default:
		std::cerr << "No idea how you got here";
		break;
	}

	return false;
}


