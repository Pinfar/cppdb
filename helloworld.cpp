#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <memory>

struct DataCell{
    //this is not the most performant way of doing this, but I am still learning,
    //I read reddit post that says it is 70% slower than raw unions!
    //So it might be a hotspot in the future, we are going to see!
    std::variant<std::string, int> value; 
};

struct DataRow{
    std::vector<DataCell> cells;
};

int main()
{
    std::vector<std::unique_ptr<DataRow>> rows;
    rows.push_back(std::make_unique<DataRow>(DataRow{std::vector<DataCell>{{"AAA"}, {10}}}));
}
