#pragma once
#include"../metadata/cell.h"

namespace DBCPP_Operators
{
    class Condition
    {
        public:
            virtual bool IsMatch(DataRow& row) = 0;
    };

    template<typename T>
    class Equals: public Condition
    {
        public:
            Equals(T value, int colIndex): value(value), colIndex(colIndex){};
            virtual bool IsMatch(DataRow& row) override;
        private:
            T value;
            int colIndex;
    };
}