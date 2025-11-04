#pragma once
#include "../storage/storage.h"
#include "abstract_iterator.h"
#include "conditions.h"
#include <memory>

namespace DBCPP_Operators{
    class WhereOperator : public AbstractDbOperator {
        private:
            std::unique_ptr<AbstractDbOperator> innerOperator;
            std::unique_ptr<DBCPP_Operators::Condition> condition;
            std::unique_ptr<DataRow> current;
        public:
            WhereOperator(std::unique_ptr<AbstractDbOperator> innerOperator,std::unique_ptr<DBCPP_Operators::Condition> condition ): 
                innerOperator{std::move(innerOperator)}, 
                condition{std::move(condition)}
            {
            };
            WhereOperator(WhereOperator&) = delete;
            virtual bool Next() override;
            virtual std::unique_ptr<DataRow> Current() override;
    };
}