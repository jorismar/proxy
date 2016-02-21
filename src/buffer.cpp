#include "buffer.h"

/******************************************************************************************/

Buffer::Buffer(t_size size) {
    this->r_pos     = 0;
    this->w_pos     = 0;
    this->buff_size = size;
    this->buffer    = (VirtualFile**) malloc(sizeof(VirtualFile*) * size);
    
    memset(this->buffer, 0, size);
}

/******************************************************************************************/

Buffer::~Buffer() {
    free(this->buffer);
}

/******************************************************************************************/

void Buffer::set(t_pos index, VirtualFile * file) {
    if(index < this->buff_size) 
        this->buffer[index] = file;
}

/******************************************************************************************/

VirtualFile * Buffer::get(t_pos index) {
    VirtualFile * file = NULL;
    
    if(index < this->buff_size)
        file = this->buffer[index];
        
    return file;
}

/******************************************************************************************/

VirtualFile * Buffer::next() {
    VirtualFile * file = this->buffer[this->r_pos];
    
    this->r_pos = ++this->r_pos % this->buff_size;

    return file;
}

/******************************************************************************************/

void Buffer::add(VirtualFile * file) {
    this->buffer[this->w_pos] = file;
    
    this->w_pos = ++this->w_pos % this->buff_size;
}

/******************************************************************************************/

void Buffer::remove(t_pos index) {
    if(index < Buffer::buff_size) {
        this->buffer[index]->~VirtualFile();
        this->buffer[index] = NULL;
    }
}

/******************************************************************************************/

t_size Buffer::size() {
    return this->buff_size;
}

/******************************************************************************************/

int Buffer::setNextIndicator(t_pos pos) {
    if(pos < this->buff_size) {
        this->r_pos = pos;
        return this->r_pos;
    }
    
    return -1;
}

/******************************************************************************************/

int Buffer::setAddIndicator(t_pos pos) {
    if(pos < this->buff_size) {
        this->w_pos = pos;
        return this->w_pos;
    }
    
    return -1;
}
