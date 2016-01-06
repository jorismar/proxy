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
    //else file = new VirtualFile();
        
    return file;
}

VirtualFile * Buffer::next() {
    VirtualFile * file = NULL;
    t_pos index = this->r_pos;
    
    if(this->buffer[index] != NULL) {
        this->r_pos = ++this->r_pos % this->buff_size;
        file = this->buffer[index];
    }

    return file;
}

void Buffer::add(VirtualFile * file) {
    this->set(this->w_pos, file);
    
    this->w_pos = ++this->w_pos % this->buff_size;
}

void Buffer::remove(t_pos index) {
    if(index < Buffer::buff_size)
        this->buffer[index]->~VirtualFile();
}

int Buffer::size() {
    return this->buff_size;
}
