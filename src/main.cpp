#include "buffer.h"

int main() {
    Buffer* buffer = new Buffer(5, 1);
    char word[] = {'l', 'a', 'v', 'i', 'x'};
    int i;
    char c = 'd';
    
    for(i = 0; i < 5; i++)
        buffer->add(&word[i]);
    
    buffer->set(4, &c);
    
    for(i = 0; i < 5; i++)
        printf("%c", *buffer->next());
        
    printf("\n%c", *buffer->get(2));
    
    printf("\n%c\n", *buffer->remove(0));
    
    return 0;
}

