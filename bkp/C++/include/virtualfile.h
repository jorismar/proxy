#ifndef VIRTUALFILE_H
#define VIRTUALFILE_H

#include <cstdlib>
#include <cstring>
#include "types.h"

class VirtualFile {
    private:
        std::string name;
        DataPacket * binary;
        // Substituir por DataPacket
        t_byte * bin;
        t_size size;
        
    public:
        VirtualFile(std::string);
        VirtualFile(std::string, t_size);
        VirtualFile(std::string, t_byte*, t_size);
        virtual ~VirtualFile();
        
        void setFilename(std::string);
        void setSize(t_size);
        void setBinary(t_byte*);

        std::string getFilename();
        t_size getSize();
        t_byte * getBinary();

        void copyBinary(t_byte*, t_size);
}

#endif