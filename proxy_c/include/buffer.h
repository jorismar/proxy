#ifndef BUFFER_H
#define BUFFER_H

#include "datapacket.h"

typedef struct {
    unsigned int r_pos;          // Posição de Leitura para next();
    unsigned int w_pos;          // Posição de Escrita para add();
    unsigned int buff_len;       // Tamanho do Buffer
    unsigned int data_len;      // Tamanho do elemento
    DataPacket *buffer;         // Buffer    
} Buffer;

Buffer new_buffer(unsigned int, unsigned int);
void add_to_buffer(Buffer*, DataPacket);
DataPacket read_next(Buffer*);
void remove_from_buffer(Buffer *, unsigned int);

#endif
