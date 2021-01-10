#include "FlagNoExpect.h"

std::string FlagNoExpect::getValue()
{
	if (m_type == Parameter::HELP)
	{
		if (m_command == "make")
			return Data::Info::HELP_LEVEL.at(Data::HelpTypes::MAKE_HELP);
		else if (m_command == "set_img_pool_dir")
			return Data::Info::HELP_LEVEL.at(Data::HelpTypes::SET_POOL_HELP);
	}
	else if (m_type == Parameter::VERSION)
	{
		return Data::Info::VERSION;
	}
}

FlagNoExpect::FlagNoExpect(const std::vector<std::string>& synonyms, const Parameter& type, const std::optional<std::string>& command) :
	Flag(synonyms, type, command) {}