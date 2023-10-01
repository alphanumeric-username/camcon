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

bool is_integer(std::string s)
{
    for(int i {0}; i < s.length(); i++)
    {
        auto c {s[i]};

        bool is_num = c == '0' || c == '1' || c == '2' || c == '3' || 
                      c == '4' || c == '5' || c == '6' || c == '7' || 
                      c == '8' || c == '9';
        
        if(is_num) 
        {
            continue;
        } else if(c == '-')
        {
            if(i == 0) {
                continue;
            }
            return false;
        } else {
            return false;
        }
    }

    return true;
}

}