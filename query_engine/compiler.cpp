#include "compiler.h"
#include "operators/full_scan.h"
#include "operators/projection_operator.h"
#include "operators/where_operator.h"
#include <functional>
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
    m_currentTableContext = m_metadata.GetTableDefinition(node->from->tableName);
    return std::make_unique<FullScanExecutionPlanNode>(node->from->tableName);
}
DBCPP_Operators::ExecutionPlanNode_ptr Compiler::CreateFilterNode(DBCPP::SqlInterface::SelectNode *node)
{
    using namespace DBCPP::SqlInterface;
    auto scan = CreateTableAccessNode(node);
    ExprOper_ptr condition = CreateExpressionOperator(node->where->condition.get());
    return std::make_unique<FilterExecutionPlanNode>(scan, condition);
}

DBCPP_Operators::ExecutionPlanNode_ptr Compiler::CreateProjectionNode(DBCPP::SqlInterface::SelectNode *node)
{
    auto where = CreateFilterNode(node);
    std::vector<ExprOper_ptr> columns;
    std::vector<std::string> names;
    for (auto &column : node->columnList->columns)
    {
        auto expr_node = CreateExpressionOperator(column.get());
        columns.push_back(std::move(expr_node));
        names.emplace_back("Column"); // TODO - we need to think how to solve this
    }

    return std::make_unique<ProjectionExecutionPlanNode>(where, names, columns);
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
        return std::make_unique<ConstantExpression>(value);
    }
    case TokenType::Number: {
        auto value = stoi(node.token.GetTokenValue());
        return std::make_unique<ConstantExpression>(value);
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
ExprOper_ptr Compiler::operator()(BinaryExpression &exp)
{
    ExprOper_ptr lhs = CreateExpressionOperator(exp.lhs.get());
    ExprOper_ptr rhs = CreateExpressionOperator(exp.rhs.get());

    return CreateBinary(lhs, rhs, exp.oper);
}

ExprOper_ptr Compiler::operator()(LogicalExpression &exp)
{
    ExprOper_ptr lhs = CreateExpressionOperator(exp.lhs.get());
    ExprOper_ptr rhs = CreateExpressionOperator(exp.rhs.get());

    return CreateBinary(lhs, rhs, exp.oper);
}

auto Compiler::CreateBinary(ExprOper_ptr &lhs, ExprOper_ptr &rhs, Token op) -> ExprOper_ptr
{
    auto opFunc = m_typeResolver.ResolveBinaryOperator({lhs->GetType(), rhs->GetType(), op.type});
    return std::make_unique<BinaryExpressionOperator>(lhs, rhs, opFunc.func, opFunc.returnType);
}

void Compiler::Error(std::string message)
{
    throw std::domain_error(message);
}
} // namespace DBCPP::QueryEngine