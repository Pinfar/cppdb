#include"storage.h"

StorageEngine::StorageEngine(){
    this->headerPage = std::make_shared<DataBaseHeaderPage>();
}

StorageEngine::StorageEngine(TableHeader header, std::vector<std::shared_ptr<DataPage>> cache){
    this->headerPage = std::make_shared<DataBaseHeaderPage>(DataBaseHeaderPage{{header}});
    this->dataPageCache = cache;
}


std::shared_ptr<DataBaseHeaderPage> StorageEngine::getDatabaseHeaderPage(){
    return this->headerPage;
}

std::shared_ptr<DataPage> StorageEngine::getDataPage(int id){
    for(auto& page: this->dataPageCache){
        if(page->id == id){
            return page;
        }
    }
    return {};
}