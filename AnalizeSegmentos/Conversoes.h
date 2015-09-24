#include <string>
#include <string.h>
#include "lista.h"
#include "boxftyp.h"
using std::string;
/*typedef struct ftyp
{
	int major_brand;
	int menor_brand;
	int brands[];

}FtypBox;

typedef struct mvhdBox
{
	unsigned creation_timer;
	unsigned modification_timer;
	unsigned timescale;
	unsigned duration;
	int rate;
	int volume;
	int reserved;
	int matrix[9];
	unsigned pre_defined;
	unsigned next_track_ID;	

}MvhdBox;

typedef struct movieBox
{
	MvhdBox mvhd;

}MovieBox;

typedef struct trakbox
{
	int dado;

}Trakbox;

typedef struct mdat
{
	int data[];

}Mdat;

typedef struct isobox
{
	int size;
	int type;
 	FtypBox ftyp;
 	Mdat mdat;
	MovieBox moov;

}ISOBox;

typedef struct segmentInitialization
{
	int size;
	int type;
	FtypBox ftyp;
	Mdat mdat;
	MovieBox moov;

}SegmentInitialization;
*/

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
		string capturaIdentificador(Lista *);
};