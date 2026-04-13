#pragma once
#include "../operators/execution_plan.h"
#include "../sql_interface/parser.h"

namespace DBCPP::QueryEngine {
    class Compiler{
        public:
            DBCPP_Operators::ExecutionPlan PlanQuery(DBCPP::SqlInterface::SelectNode* node);
        private:
            DBCPP_Operators::PlanNode_ptr CreateTableAccessNode(DBCPP::SqlInterface::SelectNode* node);
            DBCPP_Operators::PlanNode_ptr CreateFilterNode(DBCPP::SqlInterface::SelectNode* node);
            DBCPP_Operators::PlanNode_ptr CreateProjectionNode(DBCPP::SqlInterface::SelectNode* node);
            void Error(std::string message);
        
    };
}