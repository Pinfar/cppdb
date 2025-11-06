#pragma once
#include"../metadata/cell.h"
#include"../sql_interface/execution_plan.h"
#include"storage.h"
#include"../operators/abstract_iterator.h"
#include"../operators/conditions.h"

namespace DBCPP_Storage{

    class DBReader{
        private:
            StorageEngine* engine;
            std::unique_ptr<DBCPP_Operators::AbstractDbOperator> ConvertNode(DBCPP_SqlInterface::PlanNode* node);
        public:   
            DBReader(StorageEngine* engine): engine(engine){};   
            std::unique_ptr<DBCPP_Operators::AbstractDbOperator> ExecutePlan(DBCPP_SqlInterface::ExecutionPlan executionPlan);
    };
}