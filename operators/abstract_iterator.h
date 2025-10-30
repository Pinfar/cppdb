#pragma once
#include "../storage/storage.h"

class AbstractDbOperator {
    public:
        virtual bool Next() = 0;
        virtual DataRow& Current() = 0;
};