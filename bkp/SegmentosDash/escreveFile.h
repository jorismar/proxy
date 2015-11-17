//
//  escreveFile.h
//  
//
//  Created by Josue on 24/09/15.
//
//

#ifndef ____escreveFile__
#define ____escreveFile__

#include <stdio.h>
#include "lista.h"

class EscreveFile{
    public:
        Fila * fila;
        Lista * lista;
        EscreveFile();
        ˜EscreveFile();
        void escrevefile();
};
#endif /* defined(____escreveFile__) */
