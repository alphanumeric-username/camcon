#include <mf_wrapper/vde.hpp>

#include <mf_wrapper/init.hpp>

#include <iostream>

int main()
{
    camcon::initialize();

    camcon::VideoDeviceEnumerator vde{};

    std::wcout << "Available devices:\n";
    std::wcout << "index | name | symbolic link\n";
    for(int i = 0; i < vde.count(); i++)
    {
        std::wcout << i << " | " << vde.getDeviceName(i) << " | " << vde.getSymbolicLink(i) << '\n';
    }

    vde.release();

    camcon::finalize();

    return 0;
}