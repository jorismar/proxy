/**
 * \file 	virtualfile.h
 * 
 * \author 	Jorismar Barbosa Meira <jorismar.barbosa@lavid.ufpb.br>
 */

#ifndef VIRTUALFILE_H
#define VIRTUALFILE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "util.h"

/**
 * \brief This class implements a virtual file for on-the-fly management.
 * 
 * \headerfile virtualfile.h
 */

class VirtualFile {
    private:
    /**************************************************************************************/

        std::string name;       // Name of file.
        std::string type;       // Type of file (Type or http content-type).
        std::string modifd;     // Modification date.
        t_byte * bin;           // Binary data.
        t_size len;             // Binary data length.

    /**************************************************************************************/

    public:
        /**
         * \brief   Constructor.
         */
        VirtualFile();

        /**
         * \brief   Constructor.
         *
         * \param   filename    Name of file.
         * \param   type        Type of file.
         * \param   modified    Modified date.
         */
        VirtualFile(std::string filename, std::string type, std::string modified);

        /**
         * \brief   Constructor.
         *
         * \param   filename    Name of file.
         * \param   type        Type of file.
         * \param   modified    Modified date.
         * \param   bin         Array of data bytes.
         * \param   size        Size of array of the data bytes.
         */
        VirtualFile(std::string filename, std::string type, std::string modified, t_byte* bin, t_size size);

        /**
         * \brief   Destructor.
         */
        virtual ~VirtualFile();

        /**
         * \brief   Set the name of file.
         *
         * \param   filename    New name of file.
         */
        void setFilename(std::string filename);

        /**
         * \brief   Set the type of file.
         *
         * \param   type    New type of file.
         */
        void setFiletype(std::string type);

        /**
         * \brief   Set the modified date of file.
         *
         * \param   type    New modified date of file.
         */
        void setModifiedDate(std::string modified);

        /**
         * \brief   Set the binary data and size of file.
         *
         * \param   type    New file binary data and size.
         */
        void setBinary(t_byte* bin, t_size size);

        /**
         * \brief   Get the current name of file.
         *
         * \return  Return the current name of file.
         */
        std::string filename();

        /**
         * \brief   Get the current type of file.
         *
         * \return  Return the current type of file.
         */
        std::string filetype();

        /**
         * \brief   Get the current modified date of file.
         *
         * \return  Return the current modified date of file.
         */
        std::string fileModifiedDate();

        /**
         * \brief   Get the binary of file.
         *
         * \return  Return the binary of file.
         */
        t_byte* binary();

        /**
         * \brief   Get the size of file.
         *
         * \return  Return the size of file.
         */
        t_size  size();
};

#endif