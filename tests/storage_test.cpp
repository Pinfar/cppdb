#include <gtest/gtest.h>
#include "../storage/storage.h"

StorageEngine InitStorage(){
    return StorageEngine(std::move(TableHeader{
        2,
        TableDefinition{
            "Table1",
            {
                Column {
                    "Column1",
                    ColumnType::Int
                },
                Column {
                    "Column2",
                    ColumnType::Int
                },
            }
        },
        {4,5}
    }),
    std::move(std::vector<std::shared_ptr<DataPage>>{
        std::make_shared<DataPage>(DataPage{
            4,
            {},
            -1
        }),
        std::make_shared<DataPage>(DataPage{
            5,
            {},
            -1
        })
    }));
}


TEST(StorageTests, StorageInitializedCorrectly) {
  StorageEngine engine = InitStorage();
  EXPECT_EQ(engine.getDatabaseHeaderPage()->tables.size(), 1);
}

TEST(StorageTests, InformationAboutPagesIsCorrect) {
  StorageEngine engine = InitStorage();
  auto header = engine.getDatabaseHeaderPage();
  EXPECT_EQ(header->tables.size(), 1);
  auto table = header->tables[0];
  for(auto pageId: table.dataPages){
    auto page = engine.getDataPage(pageId);
    ASSERT_TRUE((bool)page);
    EXPECT_EQ(page->id, pageId);
  }
}