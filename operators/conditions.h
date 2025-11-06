#pragma once
#include"../metadata/cell.h"
#include <memory>
#include"../sql_interface/execution_plan.h"

namespace DBCPP_Operators
{
    class Condition
    {
        public:
            virtual bool IsMatch(std::unique_ptr<DataRow>& row) = 0;
    };

    template<typename T>
    class Equals: public Condition
    {
        public:
            Equals(T value, int colIndex): value(value), colIndex(colIndex){};
            virtual bool IsMatch(std::unique_ptr<DataRow>& row) override;
        private:
            T value;
            int colIndex;
    };


    template<typename T>
    static bool IsEqual(DataRow* row, DBCPP_SqlInterface::ConditionDefinition definition){
        return std::get<T>(row->cells[definition.columnIdx].value) == std::any_cast<T>(definition.value);
    }
    
    class ParametrizedCondition : public Condition
    {
        private:
            DBCPP_SqlInterface::ConditionDefinition definition;
            void SetIsMatch(){
                IsMatchPtr = IsEqual<int>;
            }
            bool (*IsMatchPtr)(DataRow*, DBCPP_SqlInterface::ConditionDefinition);
        public:
            ParametrizedCondition(DBCPP_SqlInterface::ConditionDefinition definition):definition(definition){
                SetIsMatch();
            }

            bool IsMatch(DataRow* row){
                return IsMatchPtr(row, definition);
            }

            virtual bool IsMatch(std::unique_ptr<DataRow>& row) override{
                return IsMatch(row.get());
            }
    };
}