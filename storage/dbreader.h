#pragma once
#include"../metadata/cell.h"
#include"../operators/execution_plan.h"
#include"storage.h"
#include"../operators/abstract_iterator.h"
#include"../operators/conditions.h"

namespace DBCPP_Storage{
    using namespace DBCPP_Operators;
    class DBReader{
        private:
            StorageEngine* engine;
            std::unique_ptr<AbstractDbOperator> ConvertNode(PlanNode* node);
        public:   
            DBReader(StorageEngine* engine): engine(engine){};   
            std::unique_ptr<AbstractDbOperator> ExecutePlan(ExecutionPlan executionPlan);
    };
}