#pragma once
#include <string>

namespace HelperFunctions
{
	int StringToWide(std::wstring& ws, const std::string& s)
	{
		std::wstring wsTmp(s.begin(), s.end());
		ws = wsTmp;

		return 0;
	}
}