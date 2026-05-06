#include "compiler.h"
#include "../operators/projection_operator.h"
#include <stdexcept>
#include <functional>

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
        
        auto& conditionExpr = std::get<BinaryExpression>(*node->where->condition);
        ExprOper_ptr lhs = CreateExpressionOperator(conditionExpr.lhs.get());
        ExprOper_ptr rhs = CreateExpressionOperator(conditionExpr.rhs.get());
        if(lhs->GetType() != rhs->GetType()) Error("Sides of comparision has different types");
        auto type = lhs->GetType();

        ExprOper_ptr condition;

        if(type == ColumnType::Int){
            condition = CreateCondition<int>(lhs, rhs, conditionExpr.oper);
        }
        else if(type == ColumnType::String){
            condition = CreateCondition<std::string>(lhs, rhs, conditionExpr.oper);
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

    ExprOper_ptr Compiler::CreateExpressionOperator(AnyExpression *nodeExpr)
    {
        auto& node = std::get<LiteralExpression>(*nodeExpr);
        switch(node.token.type){
            case TokenType::String:{
                auto value = node.token.GetTokenValue();
                value = value.substr(1,value.length()-2);
                return std::make_unique<ConstantExpression>(value);
            }
            case TokenType::Number:{
                auto value = stoi(node.token.GetTokenValue());
                return std::make_unique<ConstantExpression>(value);
            };
            case TokenType::Identifier: {
                auto columnName = node.token.GetTokenValue();
                auto type = m_currentTableContext->getColumnType(columnName);
                int columnIdx = m_currentTableContext->getColumnIdx(columnName);
                return std::make_unique<GetCellValueExpression>(columnIdx, type);;
            }
            default:
                Error("Invalid expression node!");
                return {}; //unreachable
        }
    }
    
    template<typename T>
    ExprOper_ptr Compiler::CreateCondition(ExprOper_ptr& lhs, ExprOper_ptr& rhs, Token op)
    {
        auto opFunc = CreateOperator<T>(op);
        return std::make_unique<EqualsExpression<T>>(lhs, rhs, opFunc);
    }

    template <typename T>
    std::function<bool(T, T)> Compiler::CreateOperator(Token op)
    {
        switch (op.type)
        {
        case TokenType::Eq: return std::equal_to<T>();
        case TokenType::Neq: return std::not_equal_to<T>();
        case TokenType::Gt: return std::greater<T>();
        case TokenType::Lt: return std::less<T>();
        default:
            throw std::domain_error("Unsupported operator!");
        }
    }

    void Compiler::Error(std::string message)
    {
        throw std::domain_error(message);
    }
}