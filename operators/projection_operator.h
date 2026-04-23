#pragma once
#include "../storage/storage.h"
#include "abstract_iterator.h"
#include "conditions.h"
#include <memory>

namespace DBCPP_Operators{

    class ProjectionOperator : public BaseDbOperator {
        private:
            ProjectionDefinition projection;
        public:
            ProjectionOperator(std::unique_ptr<AbstractDbOperator> innerOperator,ProjectionDefinition projection ): 
                BaseDbOperator(std::move(innerOperator)), 
                projection{std::move(projection)}
            {
            };
            ProjectionOperator(ProjectionOperator&) = delete;
            virtual bool Next() override;
            virtual std::unique_ptr<DataRow> Current() override;
            virtual TableDefinition GetMetadata() override;
    };
}