
#include "bignum.h"


#define ASSERT(v) if(v){throw -1;}

#define ZERO '0'
#define ONE '1'



Bignum::Bignum(BYTE * b, int len) {
	this->bits = (BYTE*)malloc(len * sizeof(BYTE));
	memset(this->bits, 0, len * sizeof(BYTE));
	memcpy(this->bits, b, len * sizeof(BYTE));
	this->bitsize = len;
}



BYTE* Sub(BYTE * cnum1, int c1len, BYTE * cnum2, int c2len) {
	int orgc1Len = BitLen(cnum1, c1len), orgc2Len = BitLen(cnum2, c2len);
	int maxlen = orgc1Len > orgc2Len ? orgc1Len : orgc2Len;
	int minlen = orgc1Len > orgc2Len ? orgc2Len : orgc1Len;
	int boundinc = 4, allocSize = maxlen + boundinc,i; 
	BYTE* res = (BYTE*)alloca(allocSize), * c = (BYTE*)alloca(allocSize),
		* cnum1b = (BYTE*)alloca(allocSize), * cnum2b = (BYTE*)alloca(allocSize)
		, addone[3] = { ZERO,ONE };
	int c1sign = cnum1[0], c2sign = cnum2[0], ressign=ZERO;//默认相减后无符号
	 
	memset(res, 0, allocSize); 
	if (orgc1Len == 0 && orgc2Len == 0) {
		res[0] = ZERO;
		res[1] = ZERO;
		return res;
	}
	else if (orgc1Len == 0) {
		memcpy(cnum2, res, orgc2Len);
		if (c2sign == ONE) {
			ressign = ZERO;
		}
		else {
			ressign = ONE;
		}
		res[0] = ressign;
		return res;
	}
	else if (orgc2Len == 0) {
		memcpy(cnum1, res, orgc1Len);
		return res;
	}

	memset(cnum1b, 0, allocSize);
	memset(cnum2b, 0, allocSize);
	memcpy(cnum1b, cnum1, c1len);
	memcpy(cnum2b, cnum2, c2len);
	if (c1sign != c2sign) {
		//2-(-1) 转换符号 变成2+1
		if (c2sign == ONE) {
			cnum2b[0] = ZERO;
			return Add(cnum1, c1len, cnum2b, c2len);
		}
		else {  //-2-2 保留符号变成 -(2+2)  
			cnum1b[0] = ZERO;
			res = Add(cnum1b, c1len, cnum2, c2len);
			res[0] = ONE;
			return res;
		}
	}
	else {
		if (IsBigger(cnum1b,  orgc1Len+1, cnum2b, orgc2Len+1)) {
			ressign = ZERO;
		}
		else {
			c = cnum1b;
			cnum1b = cnum2b;
			cnum2b = c;
			orgc1Len = maxlen;
			orgc2Len = minlen;
			ressign = ONE;
		}
		//c1sign == 1就行 表示 -2-(-1) 转换成 1-2 然后换符号
		if (c1sign == 1 && c2sign == 1) {
			ressign = ressign == ONE ? ZERO : ONE;//-2-(-1) 最终符号负号 
			cnum1b[0] = ZERO;
			cnum2b[0] = ZERO;
		}
	}


	Not(res, cnum2b, allocSize-1);
	c=Add(res, allocSize-1, addone, sizeof(addone));
	res = Add(c, allocSize-1, cnum1b, c1len);
	 
	for (i = 0; i < boundinc; i++) {
		res[allocSize-i-1] = ZERO; 
	}
	res[0] = ressign;
	RemoveTailBit(res,allocSize);
	free(c);
	return res;
}

BYTE* Multiply(BYTE* cnum1, int c1len, BYTE* cnum2, int c2len) { 
	int orgc1Len = BitLen(cnum1, c1len), orgc2Len = BitLen(cnum2, c2len);
	int maxlen = orgc1Len > orgc2Len ? orgc1Len : orgc2Len;
	int minlen = orgc1Len > orgc2Len ? orgc2Len : orgc1Len;
	int boundinc = 4, allocSize = c1len+ c2len + boundinc, i;
	BYTE* res = (BYTE*)malloc(allocSize), * c = (BYTE*)alloca(allocSize),
		* cnum1b = (BYTE*)alloca(allocSize), * cnum2b = (BYTE*)alloca(allocSize)
		, addone[3] = { ZERO,ONE },*tmpres;
	int c1sign = cnum1[0], c2sign = cnum2[0], ressign = ZERO;//默认相减后无符号
	if (c1sign!=c2sign) {
		ressign = ONE;
	}
	memset(res, 0, allocSize);
	 
	memset(cnum1b, 0, allocSize);
	memset(cnum2b, 0, allocSize);
	memcpy(cnum1b, cnum1, c1len);
	memcpy(cnum2b, cnum2, c2len);
	res[0] = ZERO;
	res[1] = ZERO;
	while (!IsZero(cnum2b, c2len)) { 
		memset(c, 0, allocSize);
		And(c, cnum2b, addone, maxlen);
		if (!IsZero(c, maxlen)) {
			tmpres = Add(res, allocSize - 1, cnum1b, allocSize-1);
			memset(res, 0, allocSize);
			memcpy(res, tmpres, ArrayLen(tmpres));
			free(tmpres);
		}
		SHIFT_LEFT_1(cnum1b, allocSize - 2);
		SHIFT_RIGHT_1(cnum2b, allocSize - 2);

	}
	res[0] = ressign;
	return res;
}
BYTE* StringNumToBits(CHAR* str, int len) {
	int i, j, allocsize = len * 4 + 1;;
	BYTE bit,num;
	BYTE* res, * tmpres,*tmpaddres, * cnum1,*tmpcnum1 ,*ten,*cnum2;
	BYTE sign = str[0];
	CHAR  bitOf10[6] = "00101";
	i = 0;
	if (sign == '+' || sign == '-') {
		i= 1;
	}
	cnum2= (BYTE*)alloca(33);
	res =(BYTE*) alloca(allocsize);
	cnum1 = (BYTE*)alloca(allocsize);
	ten = (BYTE*)alloca(sizeof(bitOf10)+1);
	memset(res, 0, allocsize);
	memset(cnum1, 0, allocsize);
	memset(ten, 0, allocsize);
	//memcpy(cnum1, bitOf10, sizeof(bitOf10));
	memcpy(ten, bitOf10, sizeof(bitOf10)+1);
	res[0] = ZERO;
	res[1] = ZERO;
	cnum1[0] = ZERO;
	cnum1[1] = ONE;
	for (;i<len;i++) {
		memset(cnum2, 0, 33);
		bit = str[len-i-1];
		num = bit - '0';
		NumToBits(num, (CHAR*)cnum2);
		tmpres = Multiply(cnum1, allocsize - 1, cnum2,32);
		tmpaddres=Add(res, allocsize - 1, tmpres, allocsize - 1);
		tmpcnum1 = Multiply(cnum1, allocsize - 1, (BYTE*)bitOf10, sizeof(bitOf10));
		memset(res, 0, allocsize);
		memset(cnum1, 0, allocsize);
		memcpy(cnum1, tmpcnum1, ArrayLen(tmpcnum1 ));
		memcpy(res, tmpaddres, ArrayLen(tmpaddres));
		free(tmpres);
		free(tmpcnum1);
		free(tmpaddres);
		
	} 
	return res;
}
VOID SHIFT_RIGHT_1(BYTE* cnum, int cLen) {
	SHIFT(cnum, -1, cnum, cLen);
}
BOOLEAN IsBigger(BYTE* cnum1, int c1len, BYTE* cnum2, int c2len) {
	int i,j;
	BYTE bit1, bit2;
	if (c1len > c2len) {
		return	TRUE;
	}
	for (i = 0; i < c1len; i++) {
		j = c1len - i - 1;
		bit1 = cnum1[j];
		bit2 = cnum2[j];
		if (bit1 == 0) {
			bit1 = ZERO;
		}
		if (bit2 == 0) {
			bit2 = ZERO;
		}
		if (bit1 != bit2) {
			if (bit1 == ONE) {
				return TRUE;
			}
			else {
				return FALSE;
			}
		}

	}
	return TRUE;
}
int BitLen(BYTE* cnum, int clen) {
	int i, j;
	BYTE bit;
	for (i =0; i <clen-1; i++) {
		j = clen - i - 1;
		bit = cnum[j];
		if (bit == ONE) {
			return j+1;
		}
	}
	return 0;

}

BYTE* Add(BYTE * cnum1, int c1len, BYTE * cnum2, int c2len) {
	int c1sign = cnum1[0];
	int c2sign = cnum2[0];
	int ressign;
	int orgc1Len = BitLen(cnum1, c1len), orgc2Len = BitLen(cnum2, c2len);
	int maxlen = orgc1Len > orgc2Len ? orgc1Len : orgc2Len;
	int minlen = orgc1Len > orgc2Len ? orgc2Len : orgc1Len;
	int boundInc = 4;
	int allocSize = maxlen + boundInc;
	if (c1sign != c2sign) {
		return Sub(cnum1, c1len, cnum2, c2len);
	}
	BYTE* res = (BYTE*)malloc(allocSize),*c= (BYTE*)alloca(allocSize ),
		* t1 = (BYTE*)alloca(allocSize ), *t2 = (BYTE*)alloca(allocSize ),
		*cnum1b = (BYTE*)alloca(allocSize ),*cnum2b = (BYTE*)alloca(allocSize );
	ASSERT(res == NULL);
	ASSERT(c == NULL);

	memset(res, 0, allocSize );
	if (orgc1Len ==0 && orgc2Len == 0) {
		res[0] = ZERO;
		res[1] = ZERO;
		return res;
	}
	else if (orgc1Len == 0) {
		memcpy(res,cnum2,  orgc2Len);
		return res;
	}
	else if (orgc2Len == 0) {
		memcpy(res,cnum1, orgc1Len);
		return res;
	}
	maxlen += 1;
	memset(c, 0, allocSize);
	memset(t1 ,0, allocSize);
	memset(t2, 0, allocSize);
	memset(cnum1b, 0, allocSize );
	memset(cnum2b, 0, allocSize );
	memcpy(cnum1b, cnum1, orgc1Len);
	memcpy(cnum2b, cnum2, orgc2Len);

	Xor(res, cnum1b, cnum2b, maxlen);
	And(c, cnum1b, cnum2b, maxlen);
	SHIFT_LEFT_1(c, maxlen+1);
	while (!IsZero(c, allocSize)) {
		memcpy(t1, res, allocSize-1);
		memcpy(t2, c, allocSize-1);

		memset(c, 0, allocSize);
		memset(res, 0, allocSize);

		Xor(res, t1, t2, maxlen+1);
		And(c, t1, t2, maxlen + 1);
		SHIFT_LEFT_1(c, maxlen +1);
	}
	if (c1sign == c2sign) {
		ressign = c1sign;
	} 
	res[0] = ressign;
	RemoveTailBit(res, allocSize);
	return res;
} 
BYTE* Div(BYTE* cnum1, int c1len, BYTE* cnum2, int c2len, BYTE* outrem  ) {
	int orgc1Len = BitLen(cnum1, c1len), orgc2Len = BitLen(cnum2, c2len);
	int maxlen = orgc1Len > orgc2Len ? orgc1Len : orgc2Len;
	int minlen = orgc1Len > orgc2Len ? orgc2Len : orgc1Len;
	int boundinc = 4, allocSize = maxlen + boundinc, i;
	BYTE* res = (BYTE*)malloc(allocSize), * c = (BYTE*)alloca(allocSize),
		* cnum1b = (BYTE*)alloca(allocSize), * cnum2b = (BYTE*)alloca(allocSize)
		, addone[3] = { ZERO,ONE }, * tmpres,*tmpsubres;
	int c1sign = cnum1[0], c2sign = cnum2[0], ressign = ZERO;//默认相减后无符号
	if (c1sign != c2sign) {
		ressign = ONE;
	}
	memset(res, 0, allocSize); 
	memset(c, 0, allocSize);
	memset(cnum1b, 0, allocSize);
	memset(cnum2b, 0, allocSize);
	memcpy(cnum1b, cnum1, orgc1Len);
	memcpy(cnum2b, cnum2, orgc2Len);
	cnum2b[0] = ZERO;
	cnum1b[0] = ZERO;
	res[0] = ZERO;
	res[1] = ZERO;
	while (IsBigger(cnum1b, maxlen, cnum2b, maxlen)) {
		tmpsubres = Sub(cnum1b, maxlen, cnum2b, maxlen);
		memset(cnum1b, 0, maxlen);
		memcpy(cnum1b, tmpsubres, ArrayLen(tmpsubres));
		tmpres = Add(res, maxlen, addone, sizeof(addone));
		memset(res, 0, maxlen);
		memcpy(res, tmpres, ArrayLen(tmpres));

		free(tmpsubres);
		free(tmpres);
	}
	memset(c, 0, maxlen);
	memcpy(c, cnum1b, ArrayLen(cnum1b));
	if (outrem != NULL) {
		memcpy(outrem, cnum1b, ArrayLen(cnum1b));
		outrem[0] = ZERO;
	}
	c[0] = ressign;
	res[0] = ressign;
	return res;
}

int RemoveTailBit(BYTE * cnum, int clen) {
	int i, j;
	BYTE bit;
	for (i = 0; i < clen-2; i++) {
		j = clen - i - 1;
		bit = cnum[j];
		if (bit == ZERO) {
			cnum[j] = 0;
		}
		if (bit == ONE) {
			break;
		}
	}
	return j+1;
}
BOOLEAN IsZero(BYTE* cnum, int clen) {
	int i, j;
	BYTE bit;
	for (i = 1; i < clen; i++) {
		bit = cnum[i];
		if (bit == ONE) {
			return FALSE;
		}
	}
	return TRUE;

}

VOID Not(BYTE* dest, BYTE* cnum, int cLen) {
	int i, j;
	BYTE bit;
	dest[0] = ZERO;
	for (i = 1; i < cLen; i++) {
		bit = cnum[i];
		if (bit == 0) {
			bit = ZERO;
		}
		if (bit == ZERO) {
			bit = ONE;
		}
		else {
			bit = ZERO;
		}
		dest[i] = bit;
	}


}
VOID Xor(BYTE* dest, BYTE* cnum1, BYTE* cnum2, int cLen) {
	int i, j;
	BYTE bit1,bit2,bitres;
	for (i = 1; i < cLen; i++) {
		bit1 = cnum1[i];
		bit2 = cnum2[i];
		if (bit2 == 0) {
			bit2 = ZERO;
		}
		if (bit1 == 0) {
			bit1 = ZERO;
		}
		if (bit1 == bit2) {
			bitres =ZERO;
		}
		else {
			bitres = ONE;
		}
		dest[i] = bitres;
	}
	dest[0] = cnum1[0];
}

VOID And	(BYTE* dest, BYTE* cnum1, BYTE* cnum2, int cLen) { 
	int i, j;
	BYTE bit1, bit2, bitres;
	for (i = 1; i < cLen; i++) {
		bit1 = cnum1[i];
		bit2 = cnum2[i];
		if (bit2 == 0) {
			bit2 = ZERO;
		}
		if (bit1 == 0) {
			bit1 = ZERO;
		}
		if (bit1 == bit2) {
			bitres = bit1;
		}
		else {
			bitres = ZERO;
		}
		dest[i] = bitres;
	}
	dest[0]= cnum1[0];
}

VOID SHIFT_LEFT_1( BYTE* cnum, int cLen) {
	SHIFT(cnum, 1, cnum, cLen);
}


VOID SHIFT(BYTE* dest, int shift, BYTE* cnum, int cLen) {
	int i, j;
	BYTE bit;
	if (shift >= 0) { 
		for (i = 0; i < cLen-1; i++) {
			j = cLen - i - 1;
			bit = cnum[j];
			if (bit == 0) {
				bit = ZERO;
			}
			dest[j] = ZERO;
			if (j + shift == 0) {
				continue;
			}
			dest[j + shift] = bit;
		}
		for (i = 0; i < shift; i++) {
			dest[i+1] = ZERO;
		}
		dest[0] = cnum[0];
		/*
		for (i = 0; i < cLen; i++) {
			bit = cnum[j];
			if (bit == 0) {
				bit = ZERO;
			}
			dest[i] = bit;
		}
		for (i = 0; i < shift; i++) {
			dest[i + cLen] = ZERO;
		}
		*/
	}
	else {
		for (i = 1; i < cLen; i++) {
			bit = cnum[i];
			j = i + shift;
			dest[i] = bit;
			if (j == 0) {
				continue;
			}
			dest[j] = bit;
		}
		dest[0] = cnum[0];
	}
	 

}


CHAR* BitsToStringNum(BYTE* str, int len) { 
	CHAR* outstr;
	BYTE* rem,*cnum1b,*tmpresdiv,*ten=(BYTE*)"00101";
	BYTE bit;
	int i, j,allocsize=len+1,num;
	outstr = (CHAR*)malloc(allocsize);
	rem = (BYTE*)alloca(allocsize);
	cnum1b= (BYTE*)alloca(allocsize);
	memset(outstr, 0, allocsize);
	memset(rem, 0, allocsize);
	memset(cnum1b, 0, allocsize);
	memcpy(cnum1b, str, len);
	j = 0;
	do { 
		tmpresdiv = Div(cnum1b, len, ten, ArrayLen(ten), rem);
		memset(cnum1b, 0, allocsize);
		memcpy(cnum1b, tmpresdiv, len);
		num = BitsToNum(rem, len);
		outstr[j++] = num + '0';
		memset(rem, 0, allocsize);
		free(tmpresdiv);
	} while (!IsZero(cnum1b,len));

	SwapByte((BYTE*)outstr, ArrayLen((BYTE*)outstr));
	return outstr;

}

VOID SwapByte(BYTE* bs, int len) {
	BYTE bit;
	int i, j;
	j = len >> 1;
	for (i = 0; i < j; i++) {
		bit = bs[i];
		bs[i] = bs[len - i - 1];
		bs[len - i - 1] = bit;
	}


}
UINT BitsToNum(BYTE* num, int len) {
	UINT i, j,mask=0x1,res, bit;
	res = 0;
	for (i = 0; i < len-1; i++) {
		j = len - i - 1;
		bit = num[j];
		if (bit == ONE) {
			bit = 1;
		}
		else {
			bit = 0;
		}
		bit = bit << (j - 1);
		res |= bit;
	} 
	return res;
}
VOID NumToBits(UINT num, CHAR* outstr) {
	int i, j,mask=0x1,loop=sizeof(int)*8;
	BYTE bit;
	UINT tmp=num;
	j = 1;
	do {
		bit = tmp & mask;
		outstr[j++] = (bit + '0');
		tmp = tmp >> 1;
	} while (tmp != 0);
	outstr[0] = ZERO;
}


int ArrayLen(BYTE* arr) {
	int i;
	BYTE* b = arr;
	i = 0;
	while (*(b++) != 0) {
		++i;
	}
	return i;
}

