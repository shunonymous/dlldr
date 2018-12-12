#if _WIN32
#include <windows.h>
//#define UNICODE
#endif

#include <iostream>
#include <string>

#include "dlldr.hpp"

int main()
{
//    filesystem::path libpath = "./";
    dlldr::shared_library lib1("./module-a", dlldr::shared_library::add_decorations);

    auto ldhello = lib1.get_if<void()>("ldhello");
    auto ldadd = lib1.get_if<int(int, int)>("ldadd");

    ldhello();
    int x = ldadd(5, 4);

    std::cout << x << std::endl;
}
