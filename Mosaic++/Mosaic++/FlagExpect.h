#pragma once
#include "Data.h"
#include "Flag.h"
#include <regex>
#include <iostream>
#include <io.h>   // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().

class FlagExpect : Flag
{
private:
	std::string m_parameter;

public:
	FlagExpect(const std::vector<std::string>& synonims, const Parameter& type, const std::optional<std::string>& command, const std::string& parameter);

private:
	inline bool isExtension() const;
	inline bool isDir() const;
	inline bool isType() const;
	inline bool isSize() const;
	inline bool isMethod() const;
	inline bool isFile() const;
	inline bool isPath() const;
	inline bool inCheck() const;

};

