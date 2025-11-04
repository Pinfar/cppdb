#include "where_operator.h"

namespace DBCPP_Operators{
    bool WhereOperator::Next(){
        while(true) {
            bool hasAnyValue = innerOperator->Next();
            if(!hasAnyValue) return false;
            current = innerOperator->Current();
            bool valueMatches = condition->IsMatch(current);
            if(valueMatches) return true;
        }
    }

    std::unique_ptr<DataRow> WhereOperator::Current(){
        return std::move(current);
    }
}