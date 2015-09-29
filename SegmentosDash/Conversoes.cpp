#include <iostream>
#include <cstdlib>
#include <limits>
#include <cctype>
#include "Conversoes.h"
#include <vector>  //for std::vector
#include <array> //for std::array
using namespace std;

Conversoes::Conversoes(char *path){
	FILE *file = fopen(path,"rb");
	char byte;
	Fila *filabits = cria();
	this->arquivo = file;
	if(file==NULL)cout<<"Arquivo nao encontrado!\n";
	while(!feof(file)){
		byte=fgetc(file);
		insere_fila(filabits,byte);
	}
	this->fila = filabits;
	this->lista = filabits->inicio;
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
/* O k ƒ o numero de bytes que v‹o ser pulados*/
Lista* Conversoes::avancarByte(int k){
	Lista *aux;
	aux = this->fila->inicio;
	int i = 0;
	while(i<k){
		aux = aux->prox;
		i++;
		remove(this->fila);
	}
	return aux;
}

void Conversoes::iniciaAnalize(){
	Lista *aux;
    char ar[9];
	int cont = 0;
	aux = this->fila->inicio;
    bits(ar,aux->dado);
    ar[8]= '\0';
    
	switch(verificatipoSeg()){
		case 1:
			segmentInitialization();
			break;
		case 2:
			segmentMedia();
			break;
		default:
			cout<<"Porra nenhuma!\n";
			break;
	}		
}
int Conversoes::verificatipoSeg(){
	Lista *aux;
    char ar[9];
	aux = avancarByte(4);
	string ftype = "ftyp";
	string stype = "styp";
	if(capturaIdentificador() == ftype){
		cout<<"\nSegmento de Inicializacao\n";
		return 1;
	}else{
		cout<<"\nSegmento de Midia\n";
		return 2;
	}
}

void Conversoes::segmentInitialization(){
	Lista *aux = this->fila->inicio;
    int i = 0;
	string identificador;
	char byte;
	/*aux = avancarByte(4);
    
	identificador = capturaIdentificador();
	cout<<"Major brand: "<<identificador<<"\n";
    this->fftype.major_brand = identificador;
	aux = avancarByte(4);
    this->fftype.menor_brand = capturaIdentificador();
	cout<<"Menor brand: "<<capturaIdentificador()<<"\n";
	aux = avancarByte(4);
    this->fftype.compatible_brand[0] = capturaIdentificador();
	cout<<"Marcas compativeis: "<<capturaIdentificador()<<",";
    this->fftype.compatible_brand[1] = capturaIdentificador();
    
    cout<<"Box Ftyp \n";
    cout<<this->fftype.menor_brand<<"\n";
    cout<<this->fftype.major_brand<<"\n";
    */
    aux = encontraBox("moov");
    
    cout<<aux->dado;
}

void Conversoes::segmentMedia(){
    Lista *aux = this->fila->inicio;
    string identificador;
    int quatroby = 0;
    char byte;
    char ar[9];
    /*cout<<"Tipo: "<<capturaIdentificador()<<"\n";
    aux = avancarByte(4);
    identificador = capturaIdentificador();
    this->fftype.major_brand = identificador;
    cout<<"Major brand: "<<identificador<<"\n";
    aux = avancarByte(4);
    this->fftype.menor_brand = capturaIdentificador();
    cout<<"Menor brand: "<<capturaIdentificador()<<"\n";
    aux = avancarByte(4);
    this->fftype.compatible_brand[0] = capturaIdentificador();
    aux = avancarByte(4);
    this->fftype.compatible_brand[1] = capturaIdentificador();
    string moof = "moof";*/
    aux = encontraBox("moov");
    
    cout<<aux->dado;
    
}

string Conversoes::capturaIdentificador(){
    Lista *lista =  this->fila->inicio;
	int tamanho = 4,cont = 0;
	char array[5];
	while(cont<4){
		array[cont] = lista->dado;
		lista = lista->prox;
		cont++;
		//remove(this->fila);
	}
	array[4]= '\0';
	string x = array;
	return x;
}

Lista* Conversoes::encontraBox(string box){
    Lista *aux;
    string identificador;
    int i = 0;
    aux = this->fila->inicio;
    while (aux->prox != NULL) {
        identificador = capturaIdentificador();
        if(identificador == box){
            cout<<"Caixa "<<identificador<<" encontrada\n";
            break;
        }
        aux = avancarByte(4);
        i++;
    }
    cout<<i;
    return aux;
}

int main(){
	char path[] = "videos/i_1442577345.m4s";
    Conversoes con(path);
    con.iniciaAnalize();
}