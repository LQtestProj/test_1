#include "complexNum.h"


ComplexNum** ComplexNum::s_cache = NULL;
int ComplexNum::s_iComCacheSize = 0;

int ComplexNum::s_iComMaxSize = 256;
ComplexNum& ComplexNum::operator=(ComplexNum& num) {
	this->real = num.real;
	this->img = num.img;
	return *this;
}
ComplexNum::ComplexNum(double r, double i) {
	real = r;
	img = i;
	if (ComplexNum::s_cache == NULL) {
		int size = ComplexNum::s_iComMaxSize;
		ComplexNum::s_cache = new ComplexNum * [size];
		::ZeroMemory(ComplexNum::s_cache, sizeof(ComplexNum*) * size);

	}
}
ComplexNum& ComplexNum::operator-(ComplexNum& num) {
	ComplexNum* newCom = GetNewComplexNum();
	newCom->real = this->real - num.real;
	newCom->img = this->img - num.img;
	return *newCom;



}
ComplexNum& ComplexNum::operator+(ComplexNum& num) {
	ComplexNum* nCom = GetNewComplexNum();
	nCom->real = this->real + num.real;
	nCom->img = this->img + num.img;
	return *nCom;

}
ComplexNum& ComplexNum::operator*(ComplexNum& num) {
	ComplexNum* nCom = GetNewComplexNum();
	double r, i;
	r = this->real * num.real - this->img * num.img;
	i = this->real * num.img + this->img * num.real;
	nCom->real = r;
	nCom->img = i;

	return *nCom;
}
double ComplexNum::Length() {

	return sqrt(real * real + img * img);
}



ComplexNum* ComplexNum::GetNewComplexNum() {
	ComplexNum* newCon = ComplexNum::s_cache[ComplexNum::s_iComCacheSize++];

	if (newCon == NULL) {
		newCon = new ComplexNum;
		ComplexNum::s_cache[ComplexNum::s_iComCacheSize - 1] = newCon;

	}
	if (ComplexNum::s_iComCacheSize == s_iComMaxSize) {
		ComplexNum::s_iComCacheSize = 0;
	}
	return newCon;
}
VOID ComplexNum::ClearComplexOperator() {
	ComplexNum* num;
	for (int i = 0; i < ComplexNum::s_iComCacheSize; i++) {
		num = ComplexNum::s_cache[i];
		if (num != NULL) {
			delete num;
			ComplexNum::s_cache[i] = NULL;
		}
	}
	ComplexNum::s_iComCacheSize = 0;
}
