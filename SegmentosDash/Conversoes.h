#include "boxftyp.h"
#include <vector>  //for std::vector
#include <array> //for std::array

typedef struct{
	int version;
	int size;
}Isobox;

typedef struct{
	std::string major_brand;
	int menor_version;
	std::vector <std::string> compatible_brand;
}Ftyp;

typedef struct{
	std::string major_brand;
	int menor_version;
	std::vector <char> compatible_brand;
}Styp;

typedef struct{
	std::string major_brand;
	int menor_version;
	std::vector <std::string> compatible_brand;
}Unknown;

typedef struct{
	int version;
	int size;
	int flags;
	int reference_id;
	int timescale;
	int earliest_presentation_time;
	int first_offset;
}Sdix;

class Conversoes{
	private:
		int cabecalho;
		int ftype;
		FILE *arquivo;
		Ftype fftype;
		std::vector <char> lista;
		Isobox isobox;
		Ftyp ftyp;
		Styp styp;
		Sdix sdix;
		
	public:
		Conversoes(char *);
		~Conversoes();
		int capturacaixa();
		void iniciaAnalize();
		void segmentInitialization();
		void segmentMedia();
		int bits(char *, char );
};
