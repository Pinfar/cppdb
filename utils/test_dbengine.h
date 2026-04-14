#pragma once
#include "../storage/storage.h"
#include "../operators/execution_plan.h"
#include "../sql_interface/parser.h"

namespace DBCPP::Utils {
    class TestDbEngine{
        private:
            std::unique_ptr<StorageEngine> m_engine;

            std::unique_ptr<StorageEngine> InitStorage();

        public:
            TestDbEngine();
            StorageEngine* GetStorage();        
            DBCPP::SqlInterface::Select_ptr CompileSql(std::string& sql); 
            std::string RunSql(std::string& sql);
    };
}