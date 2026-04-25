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
        return PlanNode::CreateTableAccess(node->from->tableName);
    }
    DBCPP_Operators::PlanNode_ptr Compiler::CreateFilterNode(DBCPP::SqlInterface::SelectNode *node)
    {
        using namespace DBCPP::SqlInterface;
        auto scan = CreateTableAccessNode(node);
        //TODO - fix
        ColumnType lhsType = GetReturnedType(node->where->condition->lhs.get());
        ColumnType rhsType = GetReturnedType(node->where->condition->rhs.get());
        if(lhsType != rhsType) Error("Sides of comparision has different types");
        auto type = lhsType;

        ExprOper_ptr<bool> condition;
        if(type == ColumnType::Int){
            ExprOper_ptr<int> lhs = CreateIntExpressionOperator(node->where->condition->lhs.get());
            ExprOper_ptr<int> rhs = CreateIntExpressionOperator(node->where->condition->rhs.get());
            condition = std::make_unique<EqualsExpression<int>>(lhs, rhs);
        }
        else if(type == ColumnType::String){
            ExprOper_ptr<std::string> lhs = CreateStringExpressionOperator(node->where->condition->lhs.get());
            ExprOper_ptr<std::string> rhs = CreateStringExpressionOperator(node->where->condition->rhs.get());
            condition = std::make_unique<EqualsExpression<std::string>>(lhs, rhs);
        }
        else {
            Error("Unsupported filtering!");
            return {};
        }
        return PlanNode::CreateFilter(scan, condition);
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

    ColumnType Compiler::GetReturnedType(ExpressionNode *node)
    {
        switch(node->token.type){
            case TokenType::Number: return ColumnType::Int;
            case TokenType::String: return ColumnType::String;
            case TokenType::Identifier: {
                auto columnName = node->token.GetTokenValue();
                return m_currentTableContext->getColumnType(columnName);
            }
            default:
                Error("Invalid expression node!");
                return ColumnType::Unknown; //unreachable
        }
    }

    ExprOper_ptr<std::string> Compiler::CreateStringExpressionOperator(ExpressionNode *node)
    {
        switch(node->token.type){
            case TokenType::String:{
                auto value = node->token.GetTokenValue();
                value = value.substr(1,value.length()-2);
                return std::make_unique<ConstantExpression<std::string>>(value);
            };
            case TokenType::Identifier: {
                int columnIdx = m_currentTableContext->getColumnIdx(node->token.GetTokenValue());
                return std::make_unique<GetCellValueExpression<std::string>>(columnIdx);;
            }
            default:
                Error("Invalid expression node!");
                return {}; //unreachable
        }
    }

    ExprOper_ptr<int> Compiler::CreateIntExpressionOperator(ExpressionNode *node)
    {
        switch(node->token.type){
            case TokenType::Number:{
                auto value = stoi(node->token.GetTokenValue());
                return std::make_unique<ConstantExpression<int>>(value);
            };
            case TokenType::Identifier: {
                int columnIdx = m_currentTableContext->getColumnIdx(node->token.GetTokenValue());
                return std::make_unique<GetCellValueExpression<int>>(columnIdx);;
            }
            default:
                Error("Invalid expression node!");
                return {}; //unreachable
        }
    }

    void Compiler::Error(std::string message)
    {
        throw std::domain_error(message);
    }
}