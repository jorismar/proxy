/******************************************************************************************
 * \file 	buffer.h
 * 
 * \author 	Jorismar Barbosa Meira <jorismar.barbosa@lavid.ufpb.br>
 ******************************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

#include <cstdio>
#include <cstdlib>
#include "virtualfile.h"

/******************************************************************************************
 * \brief   This class implements a buffer for storing the objects of VirtualFile type and
 *          can be used in circular or linear mode.
 * 
 * \headerfile buffer.h
 ******************************************************************************************/

class Buffer {
    private:
    /**************************************************************************************/

        t_pos r_pos;          // Position of the read indicator used for next() method
        t_pos w_pos;          // Position of the write indicator used for add() method
        t_pos buff_size;      // Buffer size
        VirtualFile **buffer; // Buffer bytes object
        
    /**************************************************************************************/
    public:
        /**********************************************************************************
         * \brief   Constructor.
         *
         * \param   size    Size of buffer
         **********************************************************************************/
        Buffer(t_size size);

        /**********************************************************************************
         * \brief   Destructor.
         **********************************************************************************/
        virtual ~Buffer();

        /**********************************************************************************
         * \brief   Set a pointer of VirtualFile object in the specific position.
         *
         * /param   index   Index position on the buffer.
         * /param   file    Pointer of a VirtualFile object.
         **********************************************************************************/
        void set(t_pos index, VirtualFile * file);

        /**********************************************************************************
         * \brief   Get a pointer of VirtualFile object from the specific position of
         *          buffer.
         *
         * /param   index   Index position on the buffer.
         *
         * /return  Return the requested object or NULL pointer if the position is invalid. 
         **********************************************************************************/
        VirtualFile* get(t_pos index);

        /**********************************************************************************
         * \brief   Get the next object of indicator position. For default 0 is the start
         *          postion and after the final position is reached, the indicator will
         *          return to the initial position, forming a circular buffer.
         *
         * \return  Return the next pointer of VirtualFile object from the buffer. If a
         *          erro occurs, NULL is returned.
         **********************************************************************************/
        VirtualFile* next();

        /**********************************************************************************
         * \brief   Add an pointer of VirtualFile object in the buffer. For default 0 is
         *          the startpostion and after the final position is reached, the indicator
         *          will return to the initial position, forming a circular buffer.
         *
         * \param   file    Pointer of a VirtualFile object.
         **********************************************************************************/
        void add(VirtualFile* file);      

        /**********************************************************************************
         * \brief   Remove a VirtualFile object from the buffer.
         *
         * \param   index   Index position on the buffer.
         **********************************************************************************/
        void remove(t_pos index);

        /**********************************************************************************
         * \brief   Get the buffer size.
         *
         * \return  Return the current buffer size.
         **********************************************************************************/
        t_size size();

        /**********************************************************************************
         * \brief   Set the position of next() method indicator.
         *
         * \param   pos     New position read indicator for next() method.
         *
         * \return  Return the set position if sucessful, or -1 if a erro occurs.
         **********************************************************************************/
        int setNextIndicator(t_pos pos);

        /**********************************************************************************
         * \brief   Set the position of add() method indicator.
         *
         * \param   pos     New position write indicator for add() method.
         *
         * \return  Return the set position if sucessful, or -1 if a erro occurs.
         **********************************************************************************/
        int setAddIndicator(t_pos pos);
};

#endif
