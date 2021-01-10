#pragma once
#include "Flag.h"
#include "Data.h"

class FlagNoExpect: public Flag
{
public:
	std::string getValue();

	FlagNoExpect(const std::vector<std::string>& synonyms, const Parameter& type, const std::optional<std::string>& command);
};

