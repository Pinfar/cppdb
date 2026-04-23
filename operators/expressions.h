#pragma once
#include "../metadata/cell.h"
#include<variant>
#include<memory>

namespace DBCPP_Operators {
    template<typename T>
    class Expression {
        public:
            virtual T Evaluate(DataRow* row) = 0;
    };

    template<typename T>
    using ExprOper_ptr = std::unique_ptr<Expression<T>>;

    template<typename T>
    class ConstantExpression : public Expression<T> {
        private:
            T m_constant;
        public:
            ConstantExpression(T constant) : m_constant(constant) {}
            virtual T Evaluate(DataRow* row); 
    };

    template <typename T>
    inline T ConstantExpression<T>::Evaluate(DataRow* row)
    {
        return m_constant;
    }

    template<typename T>
    class GetCellValueExpression : public Expression<T> {
        private:
            int m_index;
        public:
            GetCellValueExpression(int index) : m_index(index) {}
            virtual T Evaluate(DataRow* row); 
    };

    template <typename T>
    inline T GetCellValueExpression<T>::Evaluate(DataRow *row)
    {
        return std::get<T>(row->cells[m_index].value);
    }

    template<typename T>
    class EqualsExpression : public Expression<bool> {
        private:
            ExprOper_ptr<T> m_lhs;
            ExprOper_ptr<T> m_rhs;
        public:
            EqualsExpression(ExprOper_ptr<T>& lhs, ExprOper_ptr<T>& rhs) : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
            virtual bool Evaluate(DataRow* row); 
    };

    template <typename T>
    inline bool EqualsExpression<T>::Evaluate(DataRow *row)
    {
        return m_lhs->Evaluate(row) == m_rhs->Evaluate(row);
    }
}