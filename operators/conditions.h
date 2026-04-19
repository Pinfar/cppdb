#pragma once
#include"../metadata/cell.h"
#include <memory>
#include"execution_plan.h"
#include<stdexcept>

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
    static bool IsEqual(DataRow* row, ConditionDefinition definition){
        return std::get<T>(row->cells[definition.columnIdx].value) == std::any_cast<T>(definition.value);
    }
    
    class ParametrizedCondition : public Condition
    {
        private:
            ConditionDefinition definition;
            void SetIsMatch(){
                if(definition.dataType == ColumnType::Int)
                    IsMatchPtr = IsEqual<int>;
                else if(definition.dataType == ColumnType::String){
                    IsMatchPtr = IsEqual<std::string>;
                } else {
                    throw std::domain_error("Invalid data type in comparision.");
                }
            }
            bool (*IsMatchPtr)(DataRow*, ConditionDefinition);
        public:
            ParametrizedCondition(ConditionDefinition definition):definition(definition){
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