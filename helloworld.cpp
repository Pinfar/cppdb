#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <any>

struct A{
    ~A(){
        std::cout<<"Destroyed!"<<std::endl;
    }
};

int main()
{
    std::any a = A();
}
