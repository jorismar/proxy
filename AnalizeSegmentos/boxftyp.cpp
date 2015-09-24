#include "boxftyp.h"

int Ftype::getMajor_brand(){
	return thus->major_brand;
}

int Ftype::getMenor_brand(){
	return this->menor_brand;
}

int* Ftype::getCompatible_brand(){
	return this->getcompatible_brand;
}

void Ftype::setMajor_brand(int major){
	this->major_brand = major;
}

void Ftype::setMenor_brand(int menor){
	this->menor_brand = menor;
}

void Ftype::setCompatible_brand(int * compatible){
	this->compatible_brand = compatible;
}