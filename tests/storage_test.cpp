#include "../storage/dbwriter.h"
#include "../storage/storage.h"
#include <gtest/gtest.h>

std::unique_ptr<StorageEngine> InitStorage()
{
    std::unique_ptr<StorageEngine> engine{new StorageEngine({}, {})};
    DBCPP_Storage::DBWriter writer{*engine, {3}};
    writer.CreateTable(TableDefinition{"Table1",
                                       {
                                           Column{"Column1", ColumnType::Int},
                                           Column{"Column2", ColumnType::Int},
                                       }});
    return std::move(engine);
}

TEST(StorageTests, StorageInitializedCorrectly)
{
    auto engine = InitStorage();
    EXPECT_EQ(engine->getDatabaseHeaderPage().tables.size(), 1);
}

TEST(StorageTests, InformationAboutPagesIsCorrect)
{
    auto engine = InitStorage();
    const auto &header = engine->getDatabaseHeaderPage();
    EXPECT_EQ(header.tables.size(), 1);
    auto table = header.tables[0];
    for (auto pageId : table.dataPages)
    {
        auto *const page = engine->getDataPage(pageId);
        EXPECT_EQ(page->id, pageId);
    }
}