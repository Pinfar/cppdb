#include"dbreader.h"
#include<stdexcept>
#include"../operators/full_scan.h"
#include"../operators/where_operator.h"
#include"../operators/projection_operator.h"

namespace DBCPP_Storage{
    using namespace DBCPP_Operators;

    std::unique_ptr<AbstractDbOperator> DBReader::ExecutePlan(ExecutionPlan executionPlan){
        return ConvertNode(executionPlan.rootNode.get());
    }

    std::unique_ptr<AbstractDbOperator> DBReader::ConvertNode(PlanNode* node){
        std::unique_ptr<AbstractDbOperator> innerOperator;
        if(node->lparent){
            innerOperator = ConvertNode(node->lparent.get());
        }
        switch(node->dbOperator){
            case DbOperator::TableAccess:
            {
                auto tableName = std::any_cast<std::string>(node->parameters[0]);
                return std::make_unique<FullScanOperator>(engine, tableName);
            }
            case DbOperator::Filter:
            {
                return std::make_unique<WhereOperator>(std::move(innerOperator), std::move(node->condition));
            }
            case DbOperator::Projection:
            {
                auto projection = std::any_cast<ProjectionDefinition>(node->parameters[0]);
                return std::make_unique<ProjectionOperator>(std::move(innerOperator), projection);
            }
            default:
                throw std::logic_error("Plan can't be created! DBOperator not found!");
        }
    }

}