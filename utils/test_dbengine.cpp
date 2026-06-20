#include "test_dbengine.h"
#include "../operators/execution_plan.h"
#include "../query_engine/compiler.h"
#include "../storage/dbreader.h"
#include "../storage/dbwriter.h"
#include "formatter.h"

namespace DBCPP::Utils
{
static DataRow CreateRow(int value1, std::string value2)
{
    return DataRow{std::vector<DataCell>{{value1}, {value2}}};
    ;
}

std::unique_ptr<StorageEngine> TestDbEngine::InitStorage()
{
    std::unique_ptr<StorageEngine> engine{new StorageEngine({}, {})};
    DBCPP_Storage::DBWriter writer{*engine, {3}};
    writer.CreateTable(TableDefinition{"Table1",
                                       {
                                           Column{"Column1", ColumnType::Int},
                                           Column{"Column2", ColumnType::String},
                                       }});
    std::vector numbers{1, 2, 4, 11, 12, 14, 21, 22, 24};
    std::vector letters{"A", "B", "C", "D", "E", "F", "G", "H", "I"};
    for (int i = 0; i < numbers.size(); i++)
    {
        writer.InsertRecord("Table1", CreateRow(numbers[i], letters[i]));
    }
    return engine;
}

TestDbEngine::TestDbEngine()
{
    m_engine = InitStorage();
}

StorageEngine *TestDbEngine::GetStorage()
{
    return m_engine.get();
}
DBCPP::SqlInterface::Select_ptr TestDbEngine::CompileSql(std::string &sql)
{
    using namespace DBCPP::SqlInterface;
    Scanner scanner{&sql};
    auto tokens = scanner.tokenizeSource();
    Parser parser{tokens};
    return parser.Parse();
}
std::string TestDbEngine::RunSql(std::string &sql)
{

    auto select = CompileSql(sql);
    DBCPP::Storage::DbMetadata metadata{GetStorage()};
    DBCPP::QueryEngine::Compiler compiler{metadata};
    DBCPP_Operators::ExecutionPlan executionPlan = compiler.PlanQuery(select.get());
    DBCPP_Storage::DBReader reader{GetStorage()};
    auto execution = reader.ExecutePlan(std::move(executionPlan));

    auto tableData = execution->GetMetadata();
    std::string result = GetSerializedOpearatorOutput(*execution, tableData);
    return result;
}
} // namespace DBCPP::Utils