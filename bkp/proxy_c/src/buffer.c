#include "buffer.h"

Buffer new_buffer(unsigned int buffer_len, unsigned int data_len) {
    Buffer buff = {
        .r_pos = 0,
        .w_pos = 0,
        .buff_len = buffer_len,
        .data_len = data_len,
    };

    buff.buffer = (DataPacket*) malloc(sizeof(DataPacket) * buffer_len);
    
    return buff;
}

void add_to_buffer(Buffer* buff, DataPacket packet) {
    buff->buffer[buff->w_pos] = packet;
    buff->w_pos = buff->w_pos + 1 % buff->buff_len;
}

DataPacket read_next(Buffer* buff) {
    unsigned int i = buff->r_pos;
    
    buff->r_pos = buff->r_pos + 1 % buff->buff_len;
    
    return buff->buffer[i];
}

void remove_from_buffer(Buffer * buff, unsigned int index) {
    buff->buffer[index].length = 0;
    free(buff->buffer[index].data);
    
    buff->r_pos = buff->r_pos == index ? (buff->r_pos - 1 % buff->buff_len) : buff->r_pos;
}
