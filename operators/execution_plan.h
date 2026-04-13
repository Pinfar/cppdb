#pragma once
#include <memory>
#include <vector>
#include <any>
#include "../metadata/cell.h"

namespace DBCPP_Operators {
    enum class DbOperator{
        Projection = 1,
        Filter = 2,
        TableAccess = 3,
    };

    enum class ConditionType {
        EqConst,
        GtConst,
        LtConst,
        GeConst,
        LeConst,
        Eq,
        Gt,
        Lt,
        Ge,
        Le,
    };

    struct ConditionDefinition {
        ConditionType type;
        ColumnType dataType;
        int columnIdx;
        std::any value;
        int otherColumnIdx;
    };

    struct PlanNode {
        DbOperator dbOperator;
        std::vector<std::any> parameters;
        std::unique_ptr<PlanNode> lparent; //left parent is the main execution path
        std::unique_ptr<PlanNode> rparent; //right parent is reserved for joins,unions etc
    };

    using PlanNode_ptr = std::unique_ptr<PlanNode>;

    struct ExecutionPlan
    {
        std::unique_ptr<PlanNode> rootNode;
    };
}