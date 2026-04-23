#pragma once
#include <memory>
#include <vector>
#include <any>
#include "../metadata/cell.h"
#include "../operators/expressions.h"

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

    struct ProjectionDefinition{
        std::vector<int> columns;
    };

    struct PlanNode {
        DbOperator dbOperator;
        std::vector<std::any> parameters;
        std::unique_ptr<PlanNode> lparent; //left parent is the main execution path
        std::unique_ptr<PlanNode> rparent; //right parent is reserved for joins,unions etc
        ExprOper_ptr<bool> condition;


        static std::unique_ptr<PlanNode> CreateTableAccess(std::string tableName){
            return std::unique_ptr<PlanNode>( new PlanNode{
                DbOperator::TableAccess,
                { tableName },
                {},
                {}
            });
        }

        static std::unique_ptr<PlanNode> CreateFilter(std::unique_ptr<PlanNode> &scan, ExprOper_ptr<bool> &condition){
            return std::unique_ptr<PlanNode>( new PlanNode{
                DbOperator::Filter,
                std::vector<std::any>{},
                std::move(scan),
                {},
                std::move(condition)
            });
        }

        static std::unique_ptr<PlanNode> CreateProjection(std::unique_ptr<PlanNode> &where, std::vector<int> &columns){
            return std::unique_ptr<PlanNode>( new PlanNode{
                DbOperator::Projection,
                {ProjectionDefinition{std::move(columns)}},
                std::move(where),
                {}
            });
        }
    };

    using PlanNode_ptr = std::unique_ptr<PlanNode>;

    struct ExecutionPlan
    {
        std::unique_ptr<PlanNode> rootNode;
    };
}