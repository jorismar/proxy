#include "virtualfile.h"

/**************************************************************************************/

VirtualFile::VirtualFile() {
    this->name = "";
    this->type = "";
    this->modifd = "";
    this->bin  = NULL;
    this->len  = 0;
}

/**************************************************************************************/

VirtualFile::VirtualFile(std::string filename, std::string type, std::string modified) {
    this->name = filename;
    this->type = type;
    this->modifd = modified;
    this->bin  = NULL;
    this->len  = 0;
}

/**************************************************************************************/

VirtualFile::VirtualFile(std::string filename, std::string type, std::string modified, t_byte* bin, t_size size) {
    this->name = filename;
    this->type = type;
    this->modifd = modified;
    this->bin  = (t_byte*) malloc(sizeof(t_byte) * size);
    this->setBinary(bin, size);
}

/**************************************************************************************/

VirtualFile::~VirtualFile() {
    if(bin != NULL)
        free(this->bin);
}

/**************************************************************************************/

void VirtualFile::setFilename(std::string filename) {
    this->name = filename;
}

/**************************************************************************************/

void VirtualFile::setFiletype(std::string type) {
    this->type = type;
}

/**************************************************************************************/

void VirtualFile::setModifiedDate(std::string modified) {
    this->modifd = modified;
}

/**************************************************************************************/

void VirtualFile::setBinary(t_byte * bin, t_size size) {
    if(bin != NULL)
        free(this->bin);
    
    this->len = size;
    this->bin = (t_byte*) malloc(sizeof(t_byte) * size);
        
    for(int i = 0; i < size; i++)
        this->bin[i] = bin[i];    
}

/**************************************************************************************/

std::string VirtualFile::filename() {
    return this->name;
}

/**************************************************************************************/

std::string VirtualFile::filetype() {
    return this->type;
}

/**************************************************************************************/

std::string VirtualFile::fileModifiedDate() {
    return this->modifd;
}

/**************************************************************************************/

t_byte * VirtualFile::binary() {
    return this->bin;
}

/**************************************************************************************/

t_size VirtualFile::size() {
    return this->len;
}
