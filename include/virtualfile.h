#ifndef VIRTUALFILE_H
#define VIRTUALFILE_H

#include <cstdlib>
#include <cstring>
#include "types.h"
#include "datapacket.h"

class VirtualFile {
    private:
        std::string name;
        DataPacket * bin;
        
    public:
        VirtualFile(std::string);
        VirtualFile(std::string, t_size);
        VirtualFile(std::string, DataPacket*);
        VirtualFile(std::string, t_byte*, t_size);
        virtual ~VirtualFile();
        
        void setFilename(std::string);
        void setBinary(t_byte*, t_size);
        void setBinary(DataPacket*);

        std::string getFilename();
        t_byte * getBinary();
        DataPacket * getBinPacket();

        t_size size();
};

#endif