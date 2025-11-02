#include <iostream>
#include <vector>
#include <string>
#include <variant>

using namespace std;


int main()
{
    std::variant<int, string> varn;
    varn = "xyz";
    varn = 1;
    // cout<<"Converted back: "<<b<<endl;
    cout << endl;
}
