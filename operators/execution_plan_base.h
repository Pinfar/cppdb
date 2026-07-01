#pragma once
#include "operators/abstract_iterator.h"
#include "storage/storage.h"
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
};
} // namespace DBCPP_Operators