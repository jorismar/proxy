#include "dash.h"

Dash::Dash(){}

Dash:~Dash(){}

void Dash::iniciafragmentacao(char * name){
	this->file = gf_isom_open(name,GF_ISOM_OPEN_READ,NULL);
}

void Dash::mostradados(){
	GF_ISOFile *aux;
	PRINT("Dados do arquivo mp4");
	PRINT("Tipo "<<);




}