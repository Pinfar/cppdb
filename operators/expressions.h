#pragma once
#include "../metadata/cell.h"
#include "../types/type_interface.h"
#include <functional>
#include <memory>

namespace DBCPP_Operators
{
class Expression
{
  protected:
    ColumnType m_type;
    Expression(ColumnType type) : m_type(type)
    {
    }

  public:
    virtual DataCell Evaluate(DataRow *row) = 0;
    ColumnType GetType()
    {
        return m_type;
    }
};

using ExprOper_ptr = std::unique_ptr<Expression>;

class ConstantExpression : public Expression
{
  private:
    DataCell m_constant;

  public:
    ConstantExpression(DataCell constant) : Expression(constant.type), m_constant(constant)
    {
    }
    DataCell Evaluate(DataRow *row) override
    {
        return m_constant;
    }
};

class GetCellValueExpression : public Expression
{
  private:
    int m_index;

  public:
    GetCellValueExpression(int index, ColumnType type) : Expression(type), m_index(index)
    {
    }
    DataCell Evaluate(DataRow *row) override
    {
        return row->cells[m_index];
    }
};

template <typename T> class EqualsExpression : public Expression
{
  private:
    ExprOper_ptr m_lhs;
    ExprOper_ptr m_rhs;
    std::function<bool(T, T)> m_comparisonFunction;

  public:
    EqualsExpression(ExprOper_ptr &lhs, ExprOper_ptr &rhs, std::function<bool(T, T)> comparisonFunction)
        : Expression(ColumnType::Boolean), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)),
          m_comparisonFunction(comparisonFunction)
    {
    }
    DataCell Evaluate(DataRow *row) override;
};

template <typename T> inline DataCell EqualsExpression<T>::Evaluate(DataRow *row)
{
    T lhs = std::any_cast<T>(m_lhs->Evaluate(row).value);
    T rhs = std::any_cast<T>(m_rhs->Evaluate(row).value);
    return DataCell(m_comparisonFunction(lhs, rhs));
}

class BinaryExpressionOperator : public Expression
{
  private:
    ExprOper_ptr m_lhs;
    ExprOper_ptr m_rhs;
    DBCPP::Types::BinaryTypeOperator m_opFunction;

  public:
    BinaryExpressionOperator(ExprOper_ptr &lhs, ExprOper_ptr &rhs, DBCPP::Types::BinaryTypeOperator opFunction,
                             ColumnType type)
        : Expression(type), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)), m_opFunction(opFunction)
    {
    }
    auto Evaluate(DataRow *row) -> DataCell override
    {
        auto lhs = m_lhs->Evaluate(row);
        auto rhs = m_rhs->Evaluate(row);
        return m_opFunction(lhs, rhs);
    }
};
} // namespace DBCPP_Operators