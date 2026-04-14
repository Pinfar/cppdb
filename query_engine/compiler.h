#pragma once
#include "../operators/execution_plan.h"
#include "../sql_interface/parser.h"
#include "../storage/dbmetadata.h"

namespace DBCPP::QueryEngine {
    using namespace DBCPP::Storage;
    class Compiler{
        public:
            Compiler(DbMetadata metadata) : m_metadata(metadata) {}
            DBCPP_Operators::ExecutionPlan PlanQuery(DBCPP::SqlInterface::SelectNode* node);
        private:
            DbMetadata m_metadata;
            TableDefinition* m_currentTableContext;
            DBCPP_Operators::PlanNode_ptr CreateTableAccessNode(DBCPP::SqlInterface::SelectNode* node);
            DBCPP_Operators::PlanNode_ptr CreateFilterNode(DBCPP::SqlInterface::SelectNode* node);
            DBCPP_Operators::PlanNode_ptr CreateProjectionNode(DBCPP::SqlInterface::SelectNode* node);
            void Error(std::string message);
        
    };
}