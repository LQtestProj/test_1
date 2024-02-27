#pragma once
#include "framework.h"


class ComplexNum {

public:
	static ComplexNum* GetNewComplexNum();
	static VOID ClearComplexOperator();
	static ComplexNum** s_cache;
	static int s_iComCacheSize, s_iComMaxSize;

	FType real = 0, img = 0;
	ComplexNum(double r = 0, double i = 0);

	ComplexNum& Plus(ComplexNum& num) {
		ComplexNum nCom = *this + num;
		this->real = nCom.real;
		this->img = nCom.img;
		return *this;
	}

	ComplexNum& operator*(ComplexNum& num);
	ComplexNum& operator+(ComplexNum& num);
	ComplexNum* operator+(ComplexNum* num) {
		return &(*this + *num);
	}


	ComplexNum& operator=(ComplexNum& num);
	ComplexNum& operator-(ComplexNum& num);
	double Length();
};

