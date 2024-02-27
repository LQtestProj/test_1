
#include "fourierMathTool.h"




 


//分裂基FFT 先基2 再基4 未优化-
ComplexNum** FourierMathTool::FFT4Radix(ComplexNum comArray[], int N) {
	ComplexNum** outList = new ComplexNum * [N];
	ComplexNum* c1, * c2, * sumCom, * c3, * c4, sum1, sum2;
	ComplexNum cc1, cc2, cc3, cc4;
	int halfN = N >> 1, fourN = N >> 2;
	int evenLeft, evenRight, quarterLeft, quarterRight;
	int k = 0, k2, k4, n, k41, k43;
	ComplexNum zeroNum;
	zeroNum.real = 0;
	zeroNum.img = 1;
	for (int i = 0; i < N; i++) {
		outList[i] = new ComplexNum;
	}
	//基2
	for (k = 0; k < halfN; k++) {
		k2 = k * 2;
		for (n = 0; n < halfN; n++) {
			Euler e;
			evenLeft = n;
			evenRight = halfN + n;
			c1 = &comArray[evenLeft];
			c2 = &comArray[evenRight];
			sumCom = &(*c1 + *c2);
			e.MultiEuler(N, k2, n);
			e.MultiEuler(*sumCom);
			outList[k2]->Plus(e.GetComplexNum());
		}
	}
	//基4
	for (k = 0; k < fourN; k++) {
		k4 = k * 4;
		k41 = k4 + 1;
		k43 = k4 + 3;
		for (n = 0; n < fourN; n++) {
			Euler e1, e2;
			evenLeft = n;
			evenRight = n + 1 * (N >> 2);
			quarterLeft = n + 2 * (N >> 2);
			quarterRight = n + (3 * (N >> 2));

			cc1 = comArray[evenLeft];
			cc2 = comArray[evenRight];
			cc3 = comArray[quarterLeft];
			cc4 = comArray[quarterRight];

			sum1 = cc1 - (cc2 * zeroNum) - cc3 + (cc4 * zeroNum);
			sum2 = cc1 + (cc2 * zeroNum) - cc3 - (cc4 * zeroNum);
			e1.MultiEuler(N, k41, n);
			e2.MultiEuler(N, k43, n);

			e1.MultiEuler(sum1);
			e2.MultiEuler(sum2);

			outList[k41]->Plus(e1.GetComplexNum());
			outList[k43]->Plus(e2.GetComplexNum());
		}


	}
	//for (int i = 0; i < N; i++) { c1 = outList[i]; TRACE(L"index:%d,real:%f,img:%f\n",i,c1->real,c1->img); }

	ComplexNum::ClearComplexOperator();
	return outList;
}


VOID FourierMathTool::ComplexToFloatArray(ComplexNum  comNumArray[], FType* res, int comsize) {

	int i, j;
	
	for (i = 0; i < comsize; i++) {
		ComplexNum& com = comNumArray[i];
		res[i * 2] = com.real;
		res[i * 2 + 1] = com.img;
	}


}

VOID FourierMathTool::FFT2AndFFT4(FType* fftBuffer, FourierStruct* fsArr, int N, int& usedEpic, BOOLEAN isSkipEpic) {
	int i, j;
	FourierStruct* fdata;
	int halfN = N >> 1;
	int evenLeft, evenRight;
	int k = 0, k2, k4, n, k41, k43;
	FType rleft, rright, ileft = 0, iright = 0, frsum, fisum, angle, rc, is, rfs, ifs;
	FType* fout = (FType*)fftBuffer;

	//4基参数
	FType angle41, angle43, r41c, r43c, i41s, i43s, fourN = N >> 2;
	FType r40, i40, r41, i41, r42, i42, r43, i43;
	FType r41sum, i41sum, r43sum, i43sum, r41fs, i41fs, r43fs, i43fs;
	//加窗
	//for (i = 0; i < N; i++) {
	//	((FType*)fftBuffer)[i << 1] = ((FType*)fftBuffer)[i << 1] * HANN(i, N);
	//}
	fdata = fsArr;


	//基2
	for (k = 0; k < halfN; k++) {
		k2 = k * 2;
		FourierStruct& fs = fdata[k2];
		fs.freq = k2;
		for (n = 0; n < halfN; n++) {
			evenLeft = n << 1;
			evenRight = (halfN + n) << 1;
			rleft = fout[evenLeft];
			rright = fout[evenRight];
			ileft = fout[evenLeft + 1];
			iright = fout[evenRight + 1];

			frsum = rleft + rright;
			fisum = ileft + iright;
			angle = (TWO_PI / N) * k2 * n;
			rc = cos(angle);
			is = sin(angle);
			//(a+ib)(c-id)
			rfs = frsum * rc + fisum * is;
			ifs = fisum * rc - frsum * is;

			fs.real = fs.real + rfs;
			fs.img = fs.img + ifs;

		}
		fs.amplitude = sqrt(fs.real * fs.real + fs.img * fs.img) / N;
		fs.phase = atan2(fs.img, fs.real);
	}
	//基4
	for (k = 0; k < fourN; k++) {
		k4 = k * 4;
		k41 = k4 + 1;
		k43 = k4 + 3;
		FourierStruct& fs41 = fdata[k41],
			& fs43 = fdata[k43];

		fs41.freq = k41;
		fs43.freq = k43;
		for (n = 0; n < fourN; n++) {


			r40 = fout[(n + ((0 * N) >> 2)) << 1];//n+0N/4
			i40 = fout[((n + ((0 * N) >> 2)) << 1) + 1];
			r41 = fout[((n + ((1 * N) >> 2)) << 1)];//n+1N/4
			i41 = fout[((n + ((1 * N) >> 2)) << 1) + 1];
			r42 = fout[((n + ((2 * N) >> 2)) << 1)];//n+2N/4
			i42 = fout[((n + ((2 * N) >> 2)) << 1) + 1];
			r43 = fout[((n + ((3 * N) >> 2)) << 1)];//n+3N/4
			i43 = fout[((n + ((3 * N) >> 2)) << 1) + 1];

			r41sum = r40 - r42 + i41 - i43;
			i41sum = i40 - i42 - r41 + r43;
			r43sum = r40 - i41 - r42 + i43;
			i43sum = i40 - i42 + r41 - r43;

			angle41 = (TWO_PI / N) * k41 * n;
			r41c = cos(angle41);
			i41s = sin(angle41);

			angle43 = (TWO_PI / N) * k43 * n;
			r43c = cos(angle43);
			i43s = sin(angle43);

			r41fs = r41sum * r41c + i41sum * i41s;
			i41fs = i41sum * r41c - r41sum * i41s;

			r43fs = r43sum * r43c + i43sum * i43s;
			i43fs = i43sum * r43c - r43sum * i43s;

			fs41.real = fs41.real + r41fs;
			fs41.img = fs41.img + i41fs;
			fs43.real = fs43.real + r43fs;
			fs43.img = fs43.img + i43fs;
		}
		fs41.amplitude = sqrt(fs41.real * fs41.real + fs41.img * fs41.img) / N;
		fs41.phase = atan2(fs41.img, fs41.real);
		fs43.amplitude = sqrt(fs43.real * fs43.real + fs43.img * fs43.img) / N;
		fs43.phase = atan2(fs43.img, fs43.real);
	}
	//共轭 排除混叠
	for (i = N - 1; i >= halfN; i--) {
		FourierStruct& fs = fdata[i];
		fs.freq = fs.freq - N;
	}


	if (isSkipEpic) {
		//	usedEpic= SortEpic(fdata, N);

	}
	else {
		usedEpic = N;
	}


}



//num2 > num1
FType  FourierMathTool::Slurp(FType num1, FType num2, FType t) {

	return (num2 - num1) * t + num1;

}
VOID  FourierMathTool::SlurpComplex(ComplexNum* num1, ComplexNum* num2, FType t, ComplexNum& newComplex) {

	newComplex.real = Slurp(num1->real, num2->real, t);
	newComplex.img = Slurp(num1->img, num2->img, t);

}
VOID FourierMathTool:: SlurpPoint(ComplexNum** comList,ComplexNum* outSlurpArray, int size, int sampleSize) {

	//样本基数=样本数直接返回输入复数
	if (size == sampleSize) {
		for (int cp = 0; cp < size; cp++) {
			ComplexNum* comp = comList[cp];
			outSlurpArray[cp] = *comp;
		}
		return ;
	} 
	ComplexNum *com1,  *com2 = NULL,  newCom ;
	FType per = size * (1.0 / sampleSize), t, lerpPos = 0;
	int index1 = 0, index2 = 0, lastIndex = 0,i;
	if (size > sampleSize) {
		//样本基数大于样本轴数,则降低图像精度,可能出现更长的直线
		per = (size * 1.0) / sampleSize;
		lastIndex = size - 1;
		for (  i = 0; i < sampleSize && index1 != lastIndex; i++) {
			index1 = lerpPos;
			if (index1 >= lastIndex) {
				index1 = lastIndex;
			} 
			memcpy(&newCom, comList[index1], sizeof(ComplexNum));
		 
			outSlurpArray[i] = newCom;

			lerpPos += per;
		}

	}
	else {
		//插值用来获得满足样本轴的数量,sampleSize越多图像越精细.
		for (  i = 0; i < sampleSize; i++) {
			lerpPos = per * i;
			index1 = floor(lerpPos);

			index2 = index1 + 1;
			if (index2 >= size) {
				index2 = 0;
			}
			t = lerpPos - index1;
			com1 = comList[index1];
			com2 = comList[index2];
			SlurpComplex(com1, com2, t, newCom);
			outSlurpArray[i] = newCom;
			//	TRACE(L"index:%d,x:%f,y:%f\n",i, newCom->real, newCom->img);
		}

	}
	 

}

FType FourierMathTool:: Lerp(FType p0, FType p1, FType t) {
	return (1 - t) * p0 + t * p1;

}






Euler::Euler() {}
VOID Euler::MultiEuler(double sample, double a, BOOLEAN isConjugate) {
	angle = a;
	complex.real = sample * cos(a);
	complex.img = sample * sin(a);
	if (isConjugate) {
		complex.img = -complex.img;
	}
}

Euler* Euler::operator+(Euler& c) {

	complex = (complex + c.complex);

	return this;
}
Euler* Euler::operator*(Euler& c) {
	ComplexNum tnum = this->complex;

	return this;
}
