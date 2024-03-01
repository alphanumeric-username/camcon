#include <camcon/arg_parse.hpp>

#include <system/str_tools.hpp>
#include <string>

namespace camcon::arg_parse
{

RTControlArgs parseRTControlArgs(int argc, wchar_t** argv)
{
    RTControlArgs args{};

    if(argc == 1 && sys::str_tools::is_integer(argv[0]))
    {
        args.index = std::stoi(argv[0]);
        args.indexWasProvided = true;
    } else
    {
        args.indexWasProvided = false;
    }

    return args;
}

}