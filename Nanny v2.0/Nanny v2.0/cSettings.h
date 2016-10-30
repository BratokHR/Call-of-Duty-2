#pragma once
#include <Windows.h>
#include <string>
//#include "cPrintEngine.h"

class cSetting {
public:
	std::string	path;
	bool init;

	BOOL Save(char *section, char *key, char *value)
	{
		if (!this->init) {
			__PRINT__::print::PrintInit();
			this->init = true;
		}

		__PRINT__::print::PrintConsole("^2] ^7Nanny ^3Saved:\n^1] ^7Path: %s\n^1] ^7Section: %s\n^1] ^7Key: %s\n^1] ^7Value: %s\n^2] ^7by BratokHR\n", this->path.c_str(), section, key, value);

		return (BOOL)WritePrivateProfileString((LPCSTR)section, (LPCSTR)key, (LPCSTR)value, this->path.c_str());
	}

	BOOL Load(char *section, char *key, char *value)
	{
		BOOL bRet = (BOOL)GetPrivateProfileString((LPCSTR)section, (LPCSTR)key, 0, value, 255, this->path.c_str());

		return bRet;
	}
};