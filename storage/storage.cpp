#include"storage.h"

StorageEngine::StorageEngine(){
}

StorageEngine::StorageEngine(TableHeader header, std::vector<DataPage> cache){
    this->headerPage = DataBaseHeaderPage{{header}};
    this->dataPageCache = cache;
}


const DataBaseHeaderPage& StorageEngine::getDatabaseHeaderPage(){
    return this->headerPage;
}

const DataPage& StorageEngine::getDataPage(int id){
    for(auto& page: this->dataPageCache){
        if(page.id == id){
            return page;
        }
    }
    return this->EMPTY_PAGE;
}

const DataPage StorageEngine::EMPTY_PAGE{
    -1,{},-1
};