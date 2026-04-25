#pragma once
#include "../metadata/cell.h"
#include<variant>
#include<memory>
#include<functional>

namespace DBCPP_Operators {
    class Expression {
        protected:
            ColumnType m_type;
            Expression(ColumnType type) : m_type(type) {}
        public:
            virtual DataCell Evaluate(DataRow* row) = 0;
            ColumnType GetType()
            {
                return m_type;
            }
    };

    using ExprOper_ptr = std::unique_ptr<Expression>;

    class ConstantExpression : public Expression {
        private:
            DataCell m_constant;
        public:
            ConstantExpression(DataCell constant) : Expression(constant.type), m_constant(constant) {}
            virtual DataCell Evaluate(DataRow* row)
            {
                return m_constant;
            } 
    };

    class GetCellValueExpression : public Expression {
        private:
            int m_index;
        public:
            GetCellValueExpression(int index, ColumnType type) : Expression(type), m_index(index) {}
            virtual DataCell Evaluate(DataRow* row){
                return row->cells[m_index];
            }
    };

    template<typename T>
    class EqualsExpression : public Expression {
        private:
            ExprOper_ptr m_lhs;
            ExprOper_ptr m_rhs;
        public:
            EqualsExpression(ExprOper_ptr& lhs, ExprOper_ptr& rhs) : Expression(ColumnType::Boolean), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
            virtual DataCell Evaluate(DataRow* row); 
    };

    template <typename T>
    inline DataCell EqualsExpression<T>::Evaluate(DataRow *row)
    {
        T lhs = std::any_cast<T>(m_lhs->Evaluate(row).value);
        T rhs = std::any_cast<T>(m_rhs->Evaluate(row).value);
        return DataCell(std::equal_to<T>()(lhs, rhs));
    }
}