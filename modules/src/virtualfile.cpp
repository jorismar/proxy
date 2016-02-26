#include "virtualfile.h"

/**************************************************************************************/

VirtualFile::VirtualFile() {
    this->name = "";
    this->type = "";
    this->modifd = "";
    this->bin  = NULL;
    this->size = 0;
}

/**************************************************************************************/

VirtualFile::VirtualFile(std::string filename, std::string type, std::string modified) {
    this->name = filename;
    this->type = type;
    this->modifd = modified;
    this->bin  = NULL;
    this->size = 0;
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
    
    this->size = size;
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
    return this->size;
}

/**************************************************************************************/

t_size Webserver::readFile(std::string path, std::string filetype) {
    FILE * pfile = fopen(path.c_str(), "rb");
	
    if(pfile == NULL) return -1;

	if(this->bin != NULL)
        this->clear();

    fseek(pfile, 0, SEEK_END);
    
    this->size = ftell(pfile);
    
    rewind(pfile);
    
    this->bin = (char*) malloc (sizeof(char) * this->size);

    this->size = fread(this->bin, 1, this->size, pfile);
    
    fclose(pfile);
    
    // NAME, TYPE AND DATE MODIFICATION SET NOT IMPLEMENTED YET.

    return this->size;
}

/**************************************************************************************/

void clear() {
    this->name = "";
    this->type = "";
    this->modifd = "";
    this->size = 0;

    if(this->bin != NULL)
        free(bin);
}

