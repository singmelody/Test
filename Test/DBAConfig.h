#pragma once

#include <string>

class DBAConfig
{
public:
	DBAConfig(void);
	virtual ~DBAConfig(void);

	static void FillConfig(const std::string& sectionName = "DBAConfig");

	static bool DBAUseSHM;
};

class SHMConfig
{
public:
	static void FillConfig();
	static std::string DBAArgs;
};

