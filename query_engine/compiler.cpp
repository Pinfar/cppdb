#include "compiler.h"
#include "metadata/cell.h"
#include "operators/full_scan.h"
#include "operators/projection_operator.h"
#include "operators/single_row_operator.h"
#include "operators/where_operator.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace DBCPP::QueryEngine
{
using namespace DBCPP_Operators;
ExecutionPlan Compiler::PlanQuery(DBCPP::SqlInterface::SelectNode *node)
{
    auto projectionOperator = CreateProjectionNode(node);

    ExecutionPlan executionPlan{std::move(projectionOperator), std::move(node->columnList->names)};

    return executionPlan;
}

DBCPP_Operators::ExecutionPlanNode_ptr Compiler::CreateTableAccessNode(DBCPP::SqlInterface::SelectNode *node)
{
    if (!node->from)
    {
        return std::make_unique<SingleRowPlanNode>();
    }
    m_currentTableContext = m_metadata.GetTableDefinition(node->from->tableName);
    return std::make_unique<FullScanExecutionPlanNode>(node->from->tableName);
}
DBCPP_Operators::ExecutionPlanNode_ptr Compiler::CreateFilterNode(DBCPP::SqlInterface::SelectNode *node)
{
    auto scan = CreateTableAccessNode(node);
    if (!node->where)
    {
        return scan;
    }
    ExprOper_ptr condition = CreateExpressionOperator(node->where->condition.get());
    return std::make_unique<FilterExecutionPlanNode>(scan, condition);
}

DBCPP_Operators::ExecutionPlanNode_ptr Compiler::CreateProjectionNode(DBCPP::SqlInterface::SelectNode *node)
{
    auto where = CreateFilterNode(node);
    std::vector<ExprOper_ptr> columns;
    for (auto &column : node->columnList->columns)
    {
        auto expr_node = CreateExpressionOperator(column.get());
        columns.push_back(std::move(expr_node));
    }

    return std::make_unique<ProjectionExecutionPlanNode>(where, columns);
}

ExprOper_ptr Compiler::CreateExpressionOperator(AnyExpression *nodeExpr)
{
    auto result = std::visit(*this, *nodeExpr);
    return result;
}

ExprOper_ptr Compiler::operator()(LiteralExpression &node)
{
    switch (node.token.type)
    {
    case TokenType::String: {
        auto value = node.token.GetTokenValue();
        value = value.substr(1, value.length() - 2);
        DataCell cell{value};
        return std::make_unique<ConstantExpression>(cell);
    }
    case TokenType::Number: {
        auto value = stoi(node.token.GetTokenValue());
        DataCell cell{value};
        return std::make_unique<ConstantExpression>(cell);
    };
    case TokenType::Identifier: {
        auto columnName = node.token.GetTokenValue();
        auto type = m_currentTableContext->getColumnType(columnName);
        int columnIdx = m_currentTableContext->getColumnIdx(columnName);
        return std::make_unique<GetCellValueExpression>(columnIdx, type);
        ;
    }
    default:
        Error("Invalid expression node!");
        return {}; // unreachable
    }
}
ExprOper_ptr Compiler::operator()(BinaryExpression &node)
{
    ExprOper_ptr lhs = CreateExpressionOperator(node.lhs.get());
    ExprOper_ptr rhs = CreateExpressionOperator(node.rhs.get());

    return CreateBinary(lhs, rhs, node.oper);
}

ExprOper_ptr Compiler::operator()(LogicalExpression &node)
{
    ExprOper_ptr lhs = CreateExpressionOperator(node.lhs.get());
    ExprOper_ptr rhs = CreateExpressionOperator(node.rhs.get());

    return CreateBinary(lhs, rhs, node.oper);
}

auto Compiler::CreateBinary(ExprOper_ptr &lhs, ExprOper_ptr &rhs, Token operatorToken) -> ExprOper_ptr
{
    auto opFunc = m_typeResolver.ResolveBinaryOperator({lhs->GetType(), rhs->GetType(), operatorToken.type});
    return std::make_unique<BinaryExpressionOperator>(lhs, rhs, opFunc.func, opFunc.returnType);
}

void Compiler::Error(std::string &&message)
{
    throw std::domain_error(message);
}
} // namespace DBCPP::QueryEngine