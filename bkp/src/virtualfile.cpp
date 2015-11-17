#include "virtualfile.h"

VirtualFile(std::string filename) {
    this->name = filename;
    this->bin = NULL;
    this->size = 0;
}

VirtualFile(std::string filename, t_size size) {
    this->name = filename;
    this->bin = (t_byte*) malloc(sizeof(t_byte) * size);
    this->size = size;
}

VirtualFile(std::string filename, t_byte* bin, t_size size) {
    this->name = filename;
    this->bin = bin;
    this->size = size;
}

virtual ~VirtualFile() {
    // VAZIO
}

void setFilename(std::string filename) {
    this->name = filename;
}

void setSize(t_size size) {
    this->size = size;
}

void setBinary(t_byte * bin) {
    this->bin = bin;
}

std::string getFilename() {
    return this->name;
}

t_size getSize() {
    return this->size;
}

t_byte * getBinary() {
    return this->bin;
}

void copyBinary(t_byte * bin, t_size size) {
    
}

/*
void copyBinary(t_byte * bin, t_size size) {
    if(this->size == 0) {
        this->size = size;
        this->bin = (t_byte*) malloc(sizeof(t_byte) * size);
    }
    
    EXIT_IF(this->size != size, "[VirtualFile] ERROR: Incompatible sizes");
    
    t_byte byte;
    
    do {
        byte = *bin++;
        *this->bin++ = byte;
    } while(byte != EOF);
}

/*
void copyBinary(t_byte * bin, t_size size) {
    if(this->size == 0) {
        this->size = size;
        this->bin = (t_byte*) malloc(sizeof(t_byte) * size);
    }
    
    EXIT_IF(this->size != size, "[VirtualFile] ERROR: Incompatible sizes");
    
    for(int i = 0; i < this->size; i++)
        this->bin[i] = bin[i];
}
*/
