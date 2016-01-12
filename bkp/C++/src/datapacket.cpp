#include "datapacket.h"

DataPacket::DataPacket(t_size n_bytes) {
    this->length = n_bytes;
    this->data = (t_byte*) malloc(sizeof(t_byte) * n_bytes);
}

DataPacket::DataPacket(DataPacket * datapacket) {
    this->length = datapacket->size();
    this->data   = (t_byte*) malloc(sizeof(t_byte) * this->sz);

    this->copy(datapacket, 0);
}

DataPacket::~DataPacket() {
    // VAZIO
}

t_byte* DataPacket::get() {
    return this->data;
}

void DataPacket::set(t_byte * data) {
    for(int i = 0; i < this->length; i++)
        this->data[i] = data[i];
}

// NAO PERMITIR ESPAÇO VAZIO DENTRO DO PACKET, MUDAR PARA COPIA POR RANGE
unsigned int DataPacket::copy(DataPacket * source, int start_pos) {
    int i, j;
    int p_len = source->size();
    t_byte * data_buff = source->get(); 
    
    for(i = 0, j = start_pos; i < this->length && j < p_len; i++, j++) {
        this->data[i] = data_buff[j];
    }
    
    return j;
}

unsigned int DataPacket::size() {
    return this->length;
}
