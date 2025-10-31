#pragma once
#include "../storage/storage.h"
#include "abstract_iterator.h"
namespace DBCPP_Operators{
    class FilterOperator : public AbstractDbOperator {
        private:
            AbstractDbOperator& baseOperator;
        public:
            FilterOperator(AbstractDbOperator& baseOperator);
            FilterOperator(FilterOperator&) = delete;
            virtual bool Next();
            virtual DataRow& Current();
    };
}