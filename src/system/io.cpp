#include <system/io.hpp>

#include <system/str_tools.hpp>
#include <exception>
#include <sstream>
#include <iostream>

namespace system_u
{

std::string read_file(fs::path filename) 
{
    std::fstream file{};
    
    file.open(filename, std::ios::in | std::ios::ate);

    if(!file.is_open()) 
    {
        std::stringstream msg_ss{};

        msg_ss << "Couldn't open file " << filename;

        std::cerr << msg_ss.str() << '\n';

        return "";
    }

    long long filesize {file.tellg()};
    file.seekg(0);
    

    char* file_contents = new char[filesize + 1];
        file.read(file_contents, filesize);
        // std::cout << "fsize: " << filesize << " gcount " << file.gcount() << '\n';
        file_contents[file.gcount()] = '\0';
        
        std::string file_contents_str{file_contents};
    delete file_contents;

    return file_contents_str;
}

std::vector<std::string> read_file_lines(fs::path filename) {
    return str_tools::split(read_file(filename), '\n');
}

void save_file(fs::path filename, std::string contents)
{
    std::fstream file{};

    file.open(filename, std::ios::out);

    if(!file.is_open()) 
    {
        std::stringstream msg_ss{};

        msg_ss << "Couldn't open file " << filename;

        std::cerr << msg_ss.str() << '\n';

        return;
    }

    file << contents;

    file.close();
}

}