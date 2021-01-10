#include "Flag.h"

Flag::Flag(const std::vector<std::string>& synonyms, const Parameter& type, const std::optional<std::string>& command) : m_synonyms(synonyms), m_type(type), m_command(command) {}

std::vector<std::string> Flag::getSynonyms()
{
	return m_synonyms;
}

Flag::Parameter Flag::getType()
{
	return m_type;
}

std::optional<std::string> Flag::getCommand()
{
	return m_command;
}