#include "virtualfile.h"

VirtualFile::VirtualFile(std::string filename) {
    this->name = filename;
    this->bin  = NULL;
}

VirtualFile::VirtualFile(std::string filename, t_size size) {
    this->name = filename;
    this->bin  = new DataPacket(size);
}

VirtualFile::VirtualFile(std::string filename, DataPacket * packet) {
    this->name = filename;
    this->bin  = packet;
}

VirtualFile::VirtualFile(std::string filename, t_byte* bin, t_size size) {
    this->name = filename;
    this->bin  = NULL;
    this->setBinary(bin, size);
}

VirtualFile::~VirtualFile() {
    // NOT IMPLEMENTED
}

void VirtualFile::setFilename(std::string filename) {
    this->name = filename;
}

void VirtualFile::setBinary(t_byte * bin, t_size size) {
    if(bin != NULL)
        this->bin->~DataPacket();
        
    this->bin = new DataPacket(size);
    this->bin->set(bin);
}

void VirtualFile::setBinary(DataPacket * packet) {
    if(bin != NULL)
        this->bin->~DataPacket();
    
    this->bin = packet;
}

std::string VirtualFile::getFilename() {
    return this->name;
}

t_byte * VirtualFile::getBinary() {
    return this->bin->get();
}

DataPacket * VirtualFile::getBinPacket() {
    return this->bin;
}

t_size VirtualFile::size() {
    return this->bin->size();
}
