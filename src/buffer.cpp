#include "buffer.h"

Buffer::Buffer(t_size buffer_size) {
    this->r_pos     = 0;
    this->w_pos     = 0;
    this->buff_size = buffer_size;
    this->buffer    = (VirtualFile**) malloc(sizeof(VirtualFile*) * buffer_size);
}

Buffer::~Buffer() {
    free(this->buffer);
}

int Buffer::set(t_pos index, VirtualFile * file) {
    if(!(index < this->buff_size)) return 1;
    
    this->buffer[index] = file;
    
    return 0;
}

VirtualFile * Buffer::get(t_pos index) {
    VirtualFile * file = NULL;
    
    if(index < this->buff_size)
        file = this->buffer[index];
        
    return file;
}

VirtualFile * Buffer::next() {
    VirtualFile * file = NULL;
    
    if(this->buffer[this->r_pos] != NULL) {
        file = this->buffer[this->r_pos];
        this->r_pos = ++this->r_pos % this->buff_size;
    }
    
    return file;
}

int Buffer::add(VirtualFile * file) {
    if(this->set(this->w_pos, file) == 1)
        return 1;
    
    this->w_pos = ++this->w_pos % this->buff_size;
    
    return 0;
}

void Buffer::remove(t_pos index) {
    if(index < Buffer::buff_size)
        this->buffer[index]->~VirtualFile();
}

int Buffer::size() {
    return this->buff_size;
}
