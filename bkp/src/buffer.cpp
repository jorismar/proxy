#include "buffer.h"

Buffer::Buffer(unsigned int buffer_size, unsigned int data_size) {
    this->r_pos     = 0;
    this->w_pos     = 0;
    this->buff_size = buffer_size;
    this->data_size = data_size;
    this->buffer    = (DataPacket**) malloc(sizeof(DataPacket) * buffer_size);
}

Buffer::~Buffer() {
    // VAZIO
}

int Buffer::set(unsigned int index, DataPacket * data) {
    if(index < this->buff_size) {
        this->buffer[index] = new DataPacket(data);
        return 0;
    }
    
    return 1;
}

DataPacket* Buffer::get(unsigned int index) {
    DataPacket * data;
    
    if(index < this->buff_size)
        data = this->buffer[index];
        
    return data;
}

DataPacket* Buffer::next() {
    unsigned int index = this->r_pos;
    
    this->r_pos = ++this->r_pos % this->buff_size;

    return this->get(index);
}

void Buffer::add(DataPacket *data) {
    unsigned int index = this->w_pos;
    
    this->w_pos = ++this->w_pos % this->buff_size;
    
    this->set(index, data);
}

void Buffer::remove(unsigned int index) {
    if(index < Buffer::buff_size)
        this->buffer[index]->~DataPacket();
}

int Buffer::size() {
    return this->buff_size;
}
