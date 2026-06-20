#include <any>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

struct A
{
    ~A()
    {
        std::cout << "Destroyed!" << std::endl;
    }
};

int main()
{
    std::any a = A();
}
