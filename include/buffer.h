#ifndef BUFFER_H
#define BUFFER_H

#include "datapacket.h"

class Buffer {
    private:
        unsigned int r_pos;          // Posição de Leitura para next();
        unsigned int w_pos;          // Posição de Escrita para add();
        unsigned int buff_size;      // Tamanho do Buffer
        unsigned int data_size;      // Tamanho do elemento
        DataPacket **buffer;         // Buffer
        
    public:
        Buffer(unsigned int, unsigned int);
        virtual ~Buffer();
        
        int set(unsigned int, DataPacket*);
        DataPacket* get(unsigned int);

        DataPacket* next();
        
        void add(DataPacket*);      
        void remove(unsigned int);
        
        //int occupiedLength();
        int size();
};

#endif
