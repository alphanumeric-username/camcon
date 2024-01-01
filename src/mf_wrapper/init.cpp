#include <mf_wrapper/init.hpp>

#include <mfapi.h>

namespace mfw
{

void initialize()
{
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    MFStartup(MF_VERSION);
}

void finalize()
{
    MFShutdown();
    CoUninitialize();
}

}
