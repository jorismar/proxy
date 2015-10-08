#include <iostream>
#include <cstdlib>
#include <limits.h>
#include <cctype>
#include <stdio.h>
#include <string.h>
#include "conversoes.h"
#include <bitset>
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

void Conversoes::iniciaAnalize(){
	//FILE * arquivo;
	//arquivo = fopen("test.mp4","wb");
	//if(arquivo==NULL)cout<<"Erro arquivo de escrita\n";
	int i = 0;
	int cont = 0;
	char str[5];
	//Verifica tamanho e versao da caixa os primeiros quatro bytes
	this->isobox.version = this->lista[i];
	if(isobox.version == 0){
		cout<<"Campos serao de 32 bits\n";
		// Depois do identificador ler 4 bytes
		i++;
		while(cont<3){
			cont++;
			if((int)this->lista[i] == 0){
				i++;
			}else{
				this->isobox.size = (int)this->lista[i];
			}
		}
		i++;
		cont = 0;
		while(cont <4){
			str[cont] = this->lista[i];
			cont++;
			i++;
		}
		str[4] = '\0';
		cont = 0;
		if(strcmp(str,"ftyp") ==  0){
			while(cont<4){
				str[cont] = this->lista[i];
				i++;
				cont++;
			}
			str[4] = '\0';
			cont= 0;
			this->ftyp.major_brand = str;
		}else if(strcmp(str,"styp") == 0){
				cont = 0;
				while(cont<4){
					str[cont] = this->lista[i];
					cont++;
					i++;
				}
				str[4] = '\0';
				this->styp.major_brand = str;
				this->styp.menor_version = (int)this->lista[i];
				//Captura marcas compativeis
				cont =0;
				//i++;
				cout<<"GGG"<<this->lista[i];
				while(cont<4){             
					this->styp.compatible_brand.push_back(this->lista[i]);
					cont++;
					i++;
				}
				cont = 0;
				while(cont<4){
					  this->styp.compatible_brand.push_back(this->lista[i]);
					cont++;
					i++;
				}
				/*cout<<"Primeiro caractere da caixa sdix "<<(int)this->lista[24];
				cout<<"\nSegundo caractere da caixa sdix "<<(int)this->lista[25];
				cout<<"\nTerceiro caractere da caixa sdix "<<(int)this->lista[26];
				cout<<"\nQuarto caractere da caixa sdix "<<(int)this->lista[27];*/
				
				cout<<"\n\n";
				cont = 0;
				this->sdix.version = (int)this->lista[i];
				if(sdix.version == 0){
					cout<<"A caixa sdix possui campos de 32bits\n";
					cout<<i;
					
				}
				if(sdix.version == 1){
					cout<<"A caixa sdix possui campos de 64bits\n";
					cont = 0;
					while(cont<3){
						cont++;
						if((int)this->lista[i] == 0){
							i++;
						}else{
							this->sdix.size = (int)this->lista[i];
						}
					}
				cont = 0;
				
				}
				cout<<"Marcas adicionadas "<<this->styp.compatible_brand[0]<<" "<<this->styp.compatible_brand[1];
				
		}else{
			cont = 0;
			i++;
			cout<<"Nao leva styp\n";
		}
	//cout<<"Tamanho da caixa iso "<<this->isobox.size<<" bytes\n";		
	}else if(isobox.version == 1){
		cout<<"Campos serão de 64 bits\n";
		//Depois do identificador ler 8 bytes
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
