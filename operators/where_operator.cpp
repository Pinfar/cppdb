#include "where_operator.h"

namespace DBCPP_Operators{
    bool WhereOperator::Next(){
        while(true) {
            bool hasAnyValue = innerOperator->Next();
            if(!hasAnyValue) return false;
            bool valueMatches = condition->IsMatch(innerOperator->Current());
            if(valueMatches) return true;
        }
    }

    DataRow& WhereOperator::Current(){
        return innerOperator->Current();
    }
}