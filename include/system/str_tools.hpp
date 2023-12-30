#pragma once

#include<string>
#include<vector>

namespace system_u::str_tools
{

std::vector<std::string> split(std::string s, char sep=' ');

std::string join(std::vector<std::string> ses, char sep=' ');
std::wstring join(std::vector<std::wstring> wses, wchar_t sep=L' ');

std::string tolower(std::string s);
std::wstring tolower(std::wstring ws);

std::wstring stows(std::string s);
std::string wstos(std::wstring ws);

bool is_integer(std::string s);

}