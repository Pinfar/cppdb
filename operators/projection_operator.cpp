#include "projection_operator.h"

namespace DBCPP_Operators{
    bool ProjectionOperator::Next(){
        return innerOperator->Next();
    }

    std::unique_ptr<DataRow> ProjectionOperator::Current(){
        auto innerCurrent = innerOperator->Current();
        std::vector<DataCell> cells;
        for(auto i : projection.columns){
            auto cell = innerCurrent->cells[i];
            cells.push_back(cell);
        } 
        return std::unique_ptr<DataRow>(new DataRow{cells});
    }
}