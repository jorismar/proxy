#include "datapacket.h"

DataPacket::DataPacket(unsigned int size) {
    this->sz   = size;
    this->data = (t_byte*) malloc(sizeof(t_byte) * size);
}

DataPacket::DataPacket(DataPacket * datapacket) {
    this->sz   = datapacket->size();
    this->data = (t_byte*) malloc(sizeof(t_byte) * this->sz);

    this->copy(datapacket, 0);
}

DataPacket::~DataPacket() {
    // VAZIO
}

t_byte* DataPacket::get() {
    return this->data;
}

void DataPacket::set(t_byte * data) {
    for(int i = 0; i < this->sz; i++)
        this->data[i] = data[i];
}

unsigned int DataPacket::copy(DataPacket * packet, int start_pos) {
    int i, j;
    int p_len = packet->size();
    t_byte * data_buff = packet->get(); 
    
    for(i = 0, j = start_pos; i < this->sz && j < p_len; i++, j++) {
        this->data[i] = data_buff[j];
    }
    
    return j;
}

unsigned int DataPacket::size() {
    return this->sz;
}
