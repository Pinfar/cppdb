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
        m_currentTableContext = m_metadata.GetTableDefinition(node->from->tableName);
        return std::unique_ptr<PlanNode>( new PlanNode{
            DbOperator::TableAccess,
            { node->from->tableName },
            {},
            {}
        });
    }
    DBCPP_Operators::PlanNode_ptr Compiler::CreateFilterNode(DBCPP::SqlInterface::SelectNode *node)
    {
        using namespace DBCPP::SqlInterface;
        auto scan = CreateTableAccessNode(node);
        //TODO - fix
        int columnIdx = m_currentTableContext->getColumnIdx(node->where->condition->lhs->token.GetTokenValue());;
        Token token = node->where->condition->rhs->token;
        ConditionDefinition condition;
        if(token.type == TokenType::Number){
            int value = std::stoi(token.GetTokenValue());
            condition = {ConditionType::Eq,ColumnType::Int, columnIdx, value, -1 };
        }
        else if(token.type == TokenType::String){
            std::string value = token.GetTokenValue();
            value = value.substr(1,value.length()-2);
            condition = {ConditionType::Eq,ColumnType::String, columnIdx, value, -1 };
        }
        else {
            Error("Unsupported filtering!");
            return {};
        }
        return std::unique_ptr<PlanNode>( new PlanNode{
            DbOperator::Filter,
            {condition},
            std::move(scan),
            {}
        });
    }

    DBCPP_Operators::PlanNode_ptr Compiler::CreateProjectionNode(DBCPP::SqlInterface::SelectNode *node)
    {
        auto where = CreateFilterNode(node);
        std::vector<int> columns;
        auto& tableColumns = m_currentTableContext->columns;
        for(auto& column: node->columnList->columns)
        {
            int idx = m_currentTableContext->getColumnIdx(column);
            columns.push_back(idx);
        }
        
        return std::unique_ptr<PlanNode>( new PlanNode{
            DbOperator::Projection,
            {ProjectionDefinition{std::move(columns)}},
            std::move(where),
            {}
        });
    }

    void Compiler::Error(std::string message)
    {
        throw std::domain_error(message);
    }
}