#include "buffer.h"

Buffer::Buffer(int size, int elem_size) {
    Buffer::r_pos = 0;
    Buffer::w_pos = 0;
    Buffer::buff_size = size;
    Buffer::elem_size = elem_size;
    Buffer::buffer = (tElement*) malloc(sizeof(char) * size * elem_size);
}

Buffer::~Buffer() {
    free(Buffer::buffer);
}

tElement Buffer::next() {
    return Buffer::buffer[r_pos++ % Buffer::buff_size];
}

tElement Buffer::get(int index) {
    return index >= 0 && index < Buffer::buff_size ? Buffer::buffer[index] : NULL;
}

void Buffer::add(tElement elem) {
    Buffer::buffer[w_pos++ % Buffer::buff_size] = elem;
}

int Buffer::set(int index, tElement elem) {
    if(index >= 0 && index < Buffer::buff_size) {
        Buffer::buffer[index] = elem;
        return 0;
    }
    
    return 1;
}

tElement Buffer::remove(int index) {
    tElement elem = NULL;
    
    if(index >= 0 && index < Buffer::buff_size) {
        elem = Buffer::buffer[index];
        Buffer::buffer[index] = NULL;
    }
    
    return elem;
}
