#pragma once
#include <vector>
#include <string>
#include <optional>


class Flag
{
public:
	static enum class Parameter {
		PATH,
		EXTENSION,
		TYPE,
		SIZE,
		METHOD,
		HELP,
		VERSION,
		NONE
	};

	Flag(const std::vector<std::string>& synonims, const Parameter& param, const std::optional<std::string>& command);

	std::vector<std::string> getSynonyms();
	Parameter getType();
	std::optional<std::string> getCommand();

protected:
	std::vector<std::string>    m_synonyms;
	Parameter                    m_type;
	std::optional<std::string>    m_command;

};
#include "Flag.h"

Flag::Flag(const std::vector<std::string>& synonyms, const Parameter& param, const std::optional<std::string>& command) : m_synonyms(synonyms), m_type(param), m_command(command) {}

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

