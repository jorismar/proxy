#ifndef SEGMENT_H
#define SEGMENT_H

typedef struct segment
{	
	struct Ftype;
}Segmento;

typedef struct ftyp
{
	int manjor_brand;
	int menor_brand;
	char marcas[];
}Ftyp;

#endif