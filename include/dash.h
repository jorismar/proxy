#include <gpac/isomedia.h>

class Dash
{
public:
	GF_ISOFile *file;
	GF_ISObox *caixa;
	GF_BitStream *stream;	
	dash();
	~dash();
	void fragmentacao();
	void mostradados();
	
};