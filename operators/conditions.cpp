#include "conditions.h"

namespace DBCPP_Operators
{
    template<typename T>
    bool Equals<T>::IsMatch(DataRow& row){
        return std::get<T>(row.cells[colIndex].value) == value;
    }

    template class Equals<int>;
    template class Equals<std::string>;
}