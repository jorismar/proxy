#include "datapacket.h"

DataPacket::DataPacket(unsigned int size) {
    this->sz   = size;
    this->data = (t_byte*) malloc(sizeof(t_byte) * size);
}

DataPacket::DataPacket(DataPacket * datapacket) {
    this->sz   = datapacket->size();
    this->data = datapacket->get();
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

unsigned int DataPacket::size() {
    return this->sz;
}