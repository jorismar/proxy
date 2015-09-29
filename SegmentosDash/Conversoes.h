#include "lista.h"
#include "boxftyp.h"

class Conversoes{
	private:
		int cabecalho;
		int ftype;
		FILE *arquivo;
		Fila *fila;
		Lista *lista;
		Ftype fftype;
		
	public:
		Conversoes(char *);
		~Conversoes();
		int verificatipoSeg();
		void iniciaAnalize();
		Lista* avancarByte(int);
		void segmentInitialization();
		void segmentMedia();
		int bits(char *, char );
        Lista* encontraBox(string);
		string capturaIdentificador();
};