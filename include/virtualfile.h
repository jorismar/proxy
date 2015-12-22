#ifndef VIRTUALFILE_H
#define VIRTUALFILE_H

#include <cstdlib>
#include <cstring>
#include "types.h"
#include "datapacket.h"

class VirtualFile {
    private:
        std::string name;
        std::string type;
        std::string modifd;
        t_byte * bin;
        t_size len;
        
    public:
        VirtualFile(std::string, std::string, std::string);
        VirtualFile(std::string, std::string, std::string, t_byte*, t_size);
        virtual ~VirtualFile();
        
        void setFilename(std::string);
        void setBinary(t_byte*, t_size);

        std::string filename();
        std::string filetype();
        std::string filemodified_date();
        t_byte* binary();
        t_size  size();
};

#endif