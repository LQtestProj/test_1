#pragma once
#include "framework.h" 
#include "complexNum.h"



class Euler {

	ComplexNum complex;
	double angle = 0;

public:
	Euler();
	VOID MultiEuler(double sample, double a, BOOLEAN isConjugate = TRUE);
	Euler* operator*(Euler& c);
	Euler* operator+(Euler& c);
	double GetReal() { return complex.real; }
	double GetImg() { return complex.img; }
	ComplexNum& GetComplexNum() { return complex; }
	VOID MultiEuler(int N, int k, int n, BOOLEAN isConjugate = TRUE) {
		double angle = (TWO_PI / N) * k * n;
		double c = cos(angle)
			, s = -sin(angle);
		complex.real = c;
		complex.img = s;
	}
	VOID MultiEuler(ComplexNum& num) {
		ComplexNum* nCom = &(this->complex * num);
		this->complex.real = nCom->real;
		this->complex.img = nCom->img;

	}
};
class FourierMathTool {


public: 
	static 	FType Lerp(FType p0, FType p1, FType t);
	static VOID SlurpPoint(ComplexNum** comList, ComplexNum* outSlurpArray, int size, int sampleSize);
	static VOID  SlurpComplex(ComplexNum* num1, ComplexNum* num2, FType t, ComplexNum& newComplex);
	static FType  Slurp(FType num1, FType num2, FType t);
	static VOID FFT2AndFFT4(FType* fftBuffer, FourierStruct* fsArr, int N, int& usedEpic, BOOLEAN isSkipEpic = FALSE);
	static ComplexNum** FFT4Radix(ComplexNum comArray[], int N);
	static VOID ComplexToFloatArray(ComplexNum comNumArray[], FType* res, int comsize);



};


