#include <gtest/gtest.h>
#include "../storage/storage.h"

StorageEngine InitStorage(){
    std::vector<DataPage> data{
        {
                4,
                std::vector<DataRow>{},
                -1
        },
        {
                5,
                std::vector<DataRow>{},
                -1
        }
    };
    DataBaseHeaderPage header{{TableHeader{
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
    }}};
    return StorageEngine(header, std::move(data));
}


TEST(StorageTests, StorageInitializedCorrectly) {
  StorageEngine engine = InitStorage();
  EXPECT_EQ(engine.getDatabaseHeaderPage().tables.size(), 1);
}

TEST(StorageTests, InformationAboutPagesIsCorrect) {
  StorageEngine engine = InitStorage();
  const auto& header = engine.getDatabaseHeaderPage();
  EXPECT_EQ(header.tables.size(), 1);
  auto table = header.tables[0];
  for(auto pageId: table.dataPages){
    const auto page = engine.getDataPage(pageId);
    EXPECT_EQ(page->id, pageId);
  }
}