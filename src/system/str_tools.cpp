#include <system/str_tools.hpp>
// #include <stdlib.h>
#include <cctype>
#include <cwctype>

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


std::string join(std::vector<std::string> ses, char sep)
{
    std::string s {ses[0]};
    for(int i = 1; i < ses.size(); i++)
    {
        s += sep + ses[i];
    }

    return s;
}

std::wstring join(std::vector<std::wstring> wses, wchar_t sep)
{
    std::wstring ws {wses[0]};
    for(int i = 1; i < wses.size(); i++)
    {
        ws += sep + wses[i];
    }

    return ws;
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

std::string tolower(std::string s)
{
    std::vector<char> lower_chars {};
    for(auto c : s)
    {
        lower_chars.emplace_back(std::tolower(c));
    }

    lower_chars.emplace_back('\0');

    return std::string {lower_chars.data()};
}

std::wstring tolower(std::wstring ws)
{
    std::vector<wchar_t> lower_chars {};
    for(auto c : ws)
    {
        lower_chars.emplace_back(std::towlower(c));
    }

    lower_chars.emplace_back(L'\0');

    return std::wstring {lower_chars.data()};
}


std::wstring stows(std::string s)
{
    auto l = s.length() + 1;
    wchar_t* ws_c { new wchar_t[l] };

    // mbstowcs_s
    std::mbstowcs(ws_c, s.c_str(), l);

    std::wstring ws {ws_c};

    delete ws_c;

    return ws;
}


std::string wstos(std::wstring ws)
{
    auto l = ws.length() + 1;
    char* s_c { new char[l] };

    std::wcstombs(s_c, ws.c_str(), l);

    std::string s {s_c};

    delete s_c;

    return s;
}


}