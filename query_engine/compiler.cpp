#include "compiler.h"
#include "../operators/projection_operator.h"
#include <stdexcept>

namespace DBCPP::QueryEngine {
    using namespace DBCPP_Operators;
    ExecutionPlan Compiler::PlanQuery(DBCPP::SqlInterface::SelectNode* node){
        auto projectionOperator = CreateProjectionNode(node);
         
        ExecutionPlan executionPlan {
            std::move(projectionOperator)
        };

        return executionPlan;
    }

    DBCPP_Operators::PlanNode_ptr Compiler::CreateTableAccessNode(DBCPP::SqlInterface::SelectNode *node)
    {
        return std::unique_ptr<PlanNode>( new PlanNode{
            DbOperator::TableAccess,
            { node->from->tableName },
            {},
            {}
        });
    }
    DBCPP_Operators::PlanNode_ptr Compiler::CreateFilterNode(DBCPP::SqlInterface::SelectNode *node)
    {
        auto scan = CreateTableAccessNode(node);
        //TODO - fix
        int columnIdx = 0;
        int value = std::stoi(node->where->condition->rhs->token.GetTokenValue());
        //end to do
        return std::unique_ptr<PlanNode>( new PlanNode{
            DbOperator::Filter,
            {ConditionDefinition{ConditionType::Eq,ColumnType::Int, columnIdx, value, -1 }},
            std::move(scan),
            {}
        });
    }
    DBCPP_Operators::PlanNode_ptr Compiler::CreateProjectionNode(DBCPP::SqlInterface::SelectNode *node)
    {
        auto where = CreateFilterNode(node);         
        return std::unique_ptr<PlanNode>( new PlanNode{
            DbOperator::Projection,
            {ProjectionDefinition{{1,0}}},
            std::move(where),
            {}
        });
    }

    void Compiler::Error(std::string message)
    {
        throw std::domain_error(message);
    }
}