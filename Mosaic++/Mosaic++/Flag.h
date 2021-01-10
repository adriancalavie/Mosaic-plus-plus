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