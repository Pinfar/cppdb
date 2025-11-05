#include"storage.h"
#include<stdexcept>


DataBaseHeaderPage& StorageEngine::getDatabaseHeaderPage(){
    return this->headerPage;
}

DataPage* StorageEngine::getDataPage(int id){
    for(auto& page: this->dataPageCache){
        if(page->id == id){
            return page.get();
        }
    }
    throw std::logic_error("Page not found!");
}

int StorageEngine::createNewDataPage(){
    int id = nextPageId;
    dataPageCache.emplace_back(new DataPage{id,{},-1});
    nextPageId++;
    return id;
}

DataPage StorageEngine::EMPTY_PAGE{
    -1,{},-1
};