#include "Display.hpp"

ImVec4 GetTextColourFromAccess(DirectoryAccess access)
{
	ImVec4 colour;
	switch (access)
	{
	case DirectoryAccess::Granted:
		colour = ACCESS_COLOUR;
		break;
	case DirectoryAccess::Denied:
		colour = NO_ACCESS_COLOUR;
		break;
	case DirectoryAccess::NotFound:
		colour = NO_EXIST_COLOUR;
		break;
	default:
		colour = NO_IDEA_COLOUR;
		break;
	}
	return colour;
}

std::tuple<std::string, float> GetFileSizeDenom(uintmax_t fileSizeBytes)
{
	std::string denoms[] = { "B", "KB", "MB", "GB", "TB" };
	int denomIndex = 0;
	float fileSizeDenom = (float)fileSizeBytes;
	while (fileSizeDenom > 1000 && denomIndex < 4)
	{
		fileSizeDenom /= 1000;
		denomIndex++;
	}

	std::string denom = denoms[denomIndex];
	return std::make_tuple(denom, fileSizeDenom);
}

std::string utf8_encode(const std::wstring &wstring)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(), NULL, 0, NULL, NULL);
	std::string string(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(), &string[0], size, NULL, NULL);
	return string;
}