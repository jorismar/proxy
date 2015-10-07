#include "lista.h"
#include "boxftyp.h"
#include <vector>  //for std::vector
#include <array> //for std::array

class Conversoes{
	private:
		int cabecalho;
		int ftype;
		FILE *arquivo;
		Ftype fftype;
		std::vector <char> lista;
		
	public:
		Conversoes(char *);
		~Conversoes();
		int capturatamanhotipo();
		int capturacaixa();
		void iniciaAnalize();
		void segmentInitialization();
		void segmentMedia();
		int bits(char *, char );
};