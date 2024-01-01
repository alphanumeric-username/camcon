#include <system/str_tools.hpp>
// #include <stdlib.h>
#include <cctype>
#include <cwctype>
#include <algorithm>

namespace sys::str_tools
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


std::string trim(std::string s)
{
    if(s.empty()) {
        return "";
    }

    int i;
    for(i = 0; s[i] == ' ' || s[i] == '\n'; i++);
    
    int j;
    for(j = s.length() - 1; s[j] == ' ' || s[j] == '\n'; j--);

    return s.substr(i, j - i + 1);
}

std::wstring trim(std::wstring ws)
{
    if(ws.empty()) {
        return L"";
    }

    int i;
    for(i = 0; ws[i] == L' ' || ws[i] == L'\n'; i++);
    
    int j;
    for(j = ws.length() - 1; ws[j] == L' ' || ws[j] == L'\n'; j--);

    return ws.substr(i, j - i + 1);
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

bool is_integer(std::wstring ws)
{
    for(int i {0}; i < ws.length(); i++)
    {
        auto c {ws[i]};

        bool is_num = c == L'0' || c == L'1' || c == L'2' || c == L'3' || 
                      c == L'4' || c == L'5' || c == L'6' || c == L'7' || 
                      c == L'8' || c == L'9';
        
        if(is_num) 
        {
            continue;
        } else if(c == L'-')
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