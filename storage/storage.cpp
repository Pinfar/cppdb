#include"storage.h"


DataBaseHeaderPage& StorageEngine::getDatabaseHeaderPage(){
    return this->headerPage;
}

DataPage* StorageEngine::getDataPage(int id){
    for(auto& page: this->dataPageCache){
        if(page.id == id){
            return &page;
        }
    }
    return &this->EMPTY_PAGE;
}

DataPage StorageEngine::EMPTY_PAGE{
    -1,{},-1
};