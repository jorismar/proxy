#include <iostream>
#include <cstdlib>
#include <limits>
#include <cctype>
#include <stdio.h>
#include "conversoes.h"
using namespace std;

Conversoes::Conversoes(char *path){
	FILE *file = fopen(path,"rb");
	char byte;
	std::vector <char> vect;
	this->arquivo = file;
	if(file==NULL)cout<<"Arquivo nao encontrado!\n";
	while(!feof(file)){
		byte=fgetc(file);
		vect.push_back(byte);
	}
	this->lista = vect;
}
Conversoes::~Conversoes(){}

int Conversoes::bits(char *ar,char j){
	int i;
    char bits[CHAR_BIT + 1];
    int value = j;

    for (i = CHAR_BIT - 1; i >= 0; i -= 1) {
        bits[i] = '0' + (value & 0x01);
        value >>= 1;
    }

    bits[CHAR_BIT] = 0;

    ar[0] = '\0';
    strcat(ar,bits);
    return 0;
}
int Conversoes::capturatamanhotipo(){
	int i = 0;
	char *end;
	char str[5];
	while(i<4){
		this->lista[i] = str[i];
		i++;	
	}
	str[4] = '\0';
	return (int)strtol(str,&end,10);
}
void Conversoes::iniciaAnalize(){
	FILE * arquivo;
	arquivo = fopen("test.mp4","wb");
	if(arquivo==NULL)cout<<"Erro arquivo de escrita\n";
	int i = 0;
	int cont = 0;
	char str[5];
	std::string  identificador;
		if(capturatamanhotipo() == 0){
			int version = 0;
			fwrite(version,sizeof(int),sizeof(version),arquivo);
			cont = 4;
			while(i<4){
				str[i] = this->lista[cont];
				i++;
				cont++;
			}
			str[4] = '\0';
			if(!strcmp(str,"styp")){
				cout<<"Segmento de Media\n";
				std::string major_brand;
				while(i<4){

				}
			}else if(!strcmp(str,"ftyp")){
				cout<<"Segmento de Inicializacao\n";
			}else{
				cout<<"Desconhecido\n";
			}
		}else if(capturatamanhotipo() == 1){
			int version = 1;
		}else{

			cout<<"Tem algo errado";
		}
}

void Conversoes::segmentInitialization(){
	
}

void Conversoes::segmentMedia(){

}


int main(){
	char path[] = "videos/v_1442508901_125235964.mp4";
    Conversoes con(path);
    con.iniciaAnalize();
}