#include <cstdio>
#include <cstdlib>

typedef char * tElement;

class Buffer {
    private:
        int r_pos;          // Posição de Leitura para next();
        int w_pos;          // Posição de Escrita para add();
        int buff_size;      // Tamanho do Buffer
        int elem_size;      // Tamanho do elemento
        tElement * buffer;  // Tamanho do buffer
        
    public:
        /**
         * Construtor
         * 
         * @param size      tamanho do buffer
         * @param elem_size tamanho do elemento
         */
        Buffer(int size, int elem_size);

        /**
         * Destrutor
         */
        virtual ~Buffer();
        
        /**
        * Lê o próximo elemento do buffer
        *
        * @return retorna o próximo elemento do buffer
        */
        tElement next();

        /**
         * Recupera um elemento específico do buffer
         *
         * @param index Indice da posição do buffer
         * @return      Elemento da posição solicitada ou NULL se
         *              o index for inválido
         */
        tElement get(int index);

        /**
         * Adiciona um novo elemento no Buffer
         *
         * @param elem elemento a ser adicionado
         */
        void add(tElement elem);        

        /**
         * Atribui um novo elemento numa posição especifica do buffer
         *
         * @param index Indice da posição do buffer
         * @param elem  Elemento a ser adicionado
         * @return      0 se bem sucedido ou 1 se o index for inválido
         */
        int set(int index, tElement elem);
        
        
        /**
         * Remove um elemento do buffer
         *
         * @param index Indice da posição do buffer
         * @return      Elemento removido ou NULL se o index for inválido
         */
        tElement remove(int index);
};