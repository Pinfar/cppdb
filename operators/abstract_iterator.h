#pragma once
#include "../storage/storage.h"
namespace DBCPP_Operators{
    class AbstractDbOperator {
        public:
            virtual bool Next() = 0;
            virtual std::unique_ptr<DataRow> Current() = 0;
            virtual TableDefinition GetMetadata() = 0;
            virtual void Reset() = 0;
            virtual ~AbstractDbOperator() {};
    };

    class BaseDbOperator : public AbstractDbOperator{
        protected:
            std::unique_ptr<AbstractDbOperator> innerOperator;
        public:
            BaseDbOperator(std::unique_ptr<AbstractDbOperator> innerOperator): 
                innerOperator{std::move(innerOperator)}
            {
            };
            virtual void Reset() override{
                innerOperator->Reset();
            }

    };
}