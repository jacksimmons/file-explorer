#include <tuple>
#include <string>
#include "ImGuiInclude.hpp"

// Access-based colours
constexpr ImVec4 ACCESS_COLOUR =
ImVec4(255, 255, 255, 1);
constexpr ImVec4 NO_ACCESS_COLOUR =
ImVec4(255, 255, 0, 1);
constexpr ImVec4 NO_EXIST_COLOUR =
ImVec4(255, 0, 0, 1);
constexpr ImVec4 NO_IDEA_COLOUR =
ImVec4(255, 0, 255, 1);

enum DirectoryAccess
{
	DirectoryAccess_Granted,
	DirectoryAccess_Denied,
	DirectoryAccess_DirNotFound
};

void DrawFileDisplay(bool update);
void UpdateFiles();

// Returns one of the access colours based on provided DirectoryAccess enum.
ImVec4 GetTextColourFromAccess(DirectoryAccess access);

// Gets the lowest denominator (B, KB, MB, etc...) that fileSizeBytes fits into less than 1000 times.
std::tuple<std::string, float> GetFileSizeDenom(uintmax_t fileSizeBytes);

std::string utf8_encode(const std::wstring &wstring);
