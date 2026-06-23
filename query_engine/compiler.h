#pragma once
#include "../operators/execution_plan.h"
#include "../operators/expressions.h"
#include "../sql_interface/parser.h"
#include "../storage/dbmetadata.h"
#include "../types/type_interface.h"
#include <functional>

namespace DBCPP::QueryEngine
{
using namespace DBCPP::Storage;
using namespace DBCPP::SqlInterface;
using namespace DBCPP_Operators;
class Compiler
{
  public:
    Compiler(DbMetadata metadata) : m_metadata(metadata)
    {
    }
    ExecutionPlan PlanQuery(SelectNode *node);

    ExprOper_ptr operator()(LiteralExpression &exp);
    ExprOper_ptr operator()(BinaryExpression &exp);
    ExprOper_ptr operator()(LogicalExpression &exp);

  private:
    DbMetadata m_metadata;
    TableDefinition *m_currentTableContext;
    Types::TypeResolver m_typeResolver;

    PlanNode_ptr CreateTableAccessNode(SelectNode *node);
    PlanNode_ptr CreateFilterNode(SelectNode *node);
    PlanNode_ptr CreateProjectionNode(SelectNode *node);
    ExprOper_ptr CreateExpressionOperator(AnyExpression *nodeExpr);
    ExprOper_ptr CreateBinary(ExprOper_ptr &lhs, ExprOper_ptr &rhs, Token op);
    void Error(std::string message);
};
} // namespace DBCPP::QueryEngine