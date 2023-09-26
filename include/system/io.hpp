#pragma once

#include <fstream>
#include <filesystem>
#include <vector>

namespace system_u {

namespace fs = std::filesystem;    

std::string read_file(fs::path filename);
std::vector<std::string> read_file_lines(fs::path filename);

}