#pragma once

#include <string>

namespace camcon::arg_parse
{

struct RTControlArgs
{
    int index{0};
    bool indexWasProvided{false};
    bool syntaxIsValid{true};
};

RTControlArgs parseRTControlArgs(int argc, wchar_t** argv);

}