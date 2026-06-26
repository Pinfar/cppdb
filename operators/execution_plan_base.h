#pragma once
#include "operators/abstract_iterator.h"
namespace DBCPP_Operators
{
struct TranslateContext
{
    StorageEngine *storageEngine;
};
class ExecutionPlanNodeBase
{
  public:
    virtual ~ExecutionPlanNodeBase() = default;
    virtual auto Translate(TranslateContext *context) -> DbOperator_Ptr = 0;
};
using ExecutionPlanNode_ptr = std::unique_ptr<ExecutionPlanNodeBase>;
struct ExecutionPlan
{
    ExecutionPlanNode_ptr rootNode;
    std::vector<std::string> column_labels;
};
} // namespace DBCPP_Operators