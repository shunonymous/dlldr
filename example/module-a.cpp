#include <iostream>

extern "C" void ldhello()
{
    std::cout << "Hello, dlldr!" << std::endl;
}

extern "C" int ldadd(int a, int b)
{
    return a + b;
}
