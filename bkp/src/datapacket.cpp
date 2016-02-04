#include "datapacket.h"

DataPacket::DataPacket(t_size n_bytes) {
    this->length = n_bytes;
    this->data   = (t_byte*) malloc(sizeof(t_byte) * n_bytes);
}

DataPacket::DataPacket(DataPacket * datapacket) {
    this->length = datapacket->size();
    this->data   = (t_byte*) malloc(sizeof(t_byte) * this->length);

    this->set(datapacket->get());
}

DataPacket::DataPacket(t_byte * data, t_size size) {
    this->length = size;
    this->data   = (t_byte*) malloc(sizeof(t_byte) * size);

    this->set(data);
}

DataPacket::~DataPacket() {
    free(this->data);
}

t_byte* DataPacket::get() { 
    return this->data;
}

void DataPacket::set(t_byte * data) {
    for(int i = 0; i < this->length; i++)
        this->data[i] = data[i];
}

void DataPacket::setbyte(t_byte byte, t_pos pos) {
    this->data[pos] = byte;
}

/*
unsigned int DataPacket::copy(DataPacket * source, int start_pos) {
    int i, j;
    int p_len = source->size();
    t_byte * data_buff = source->get(); 
    
    for(i = 0, j = start_pos; i < this->length && j < p_len; i++, j++)
        this->data[i] = data_buff[j];
    
    
    return j;
}
*/
t_size DataPacket::size() {
    return this->length;
}

DataPacket* DataPacket::fragment(t_pos initpos, t_size len) {
    if(this->length < initpos + len) return NULL;
    
    DataPacket * packet = new DataPacket(len);
    
    for(t_pos fm = initpos, to = 0; to < len; fm++, to++)
        packet->setbyte(this->data[fm], to);
    
    return packet;
}

void DataPacket::printPkt() {
    PRINT("OK!!!");
}