#include <system/str_tools.hpp>

namespace system_u::str_tools
{

std::vector<std::string> split(std::string s, char sep)
{
    std::vector<std::string> split_s{};

    int cursor{0};
    int aux{cursor};

    cursor = s.find(sep, cursor);

    while (cursor >= 0)
    {
        auto subs {s.substr(aux, cursor - aux)};
        if(subs.size() > 0) split_s.emplace_back(subs);
        aux = ++cursor;
        cursor = s.find(sep, cursor);
    }

    if(s.size() > aux)
    {
        split_s.emplace_back(s.substr(aux));   
    }
    
    return split_s;
}

}