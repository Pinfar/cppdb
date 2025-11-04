#pragma once
#include "../storage/storage.h"
namespace DBCPP_Operators{
    class AbstractDbOperator {
        public:
            virtual bool Next() = 0;
            virtual std::unique_ptr<DataRow> Current() = 0;
            virtual TableDefinition GetMetadata() = 0;
    };
}