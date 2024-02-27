#include "bignum_byte.h"


#define BIG_BYTE 



const int _tenArrayLength = 30;
 BCHAR _tenPowByteArray[_tenArrayLength][32] = { {2}, {20},{200},{0xd0,0x7} ,{0x20,0x4e},{0x40,0x0d,0x3},{0x80,0x84,0x1e}
,{0,0x2d,0x31,0x1},{0,0xc2,0xeb,0xb},{0,0x94,0x35,0x77}
,{0x00,0xc8,0x17,0xa8,0x04},{0x00,0xd0,0xed,0x90,0x2e},{0x00,0x20,0x4a,0xa9,0xd1,0x01},{0x00,0x40,0xe5,0x9c,0x30,0x12},{0x00,0x80,0xf4,0x20,0xe6,0xb5},{0x00,0x00,0x8d,0x49,0xfd,0x1a,0x07},{0x00,0x00,0x82,0xdf,0xe4,0x0d,0x47},{0x00,0x00,0x14,0xbb,0xf0,0x8a,0xc6,0x02},{0x00,0x00,0xc8,0x4e,0x67,0x6d,0xc1,0x1b},{0x00,0x00,0xd0,0x13,0x09,0x46,0x8e,0x15,0x01},{0x00,0x00,0x20,0xc6,0x5a,0xbc,0x8e,0xd7,0x0a},{0x00,0x00,0x40,0xbd,0x8b,0x5b,0x93,0x6b,0x6c},{0x00,0x00,0x80,0x64,0x75,0x93,0xc1,0x33,0x3c,0x04},{0x00,0x00,0x00,0xed,0x95,0xc2,0x8f,0x05,0x5a,0x2a},{0x00,0x00,0x00,0x42,0xdb,0x99,0x9d,0x37,0x84,0xa7,0x01},{0x00,0x00,0x00,0x94,0x90,0x02,0x28,0x2c,0x2a,0x8b,0x10},{0x00,0x00,0x00,0xc8,0xa5,0x19,0x90,0xb9,0xa5,0x6f,0xa5},{0x00,0x00,0x00,0xd0,0x79,0x00,0xa1,0x3f,0x79,0x5c,0x76,0x06},{0x00,0x00,0x00,0x20,0xc2,0x04,0x4a,0x7c,0xbc,0x9c,0x9f,0x40},{0x00,0x00,0x00,0x40,0x95,0x2f,0xe4,0xda,0x5c,0x1f,0x3c,0x86,0x02}
};
 int _tenBitsLength[_tenArrayLength] = { 2,5,8,11,15,18,21,25,28,31,35,38,41,45,48,51,55,58,61,65,68,71,75,78,81,85,88,91,95 };

int Div(BCHAR* dest, BCHAR* inum1, int i1Len, BCHAR* inum2, int i2Len, BCHAR* outrem) {
	int c1sign = GETSIGN(inum1), c2sign = GETSIGN(inum2);
	int org1Len = GetUsedLength(inum1, i1Len), org2Len = GetUsedLength(inum2, i2Len);
	int maxlen = (org1Len > org2Len ? org1Len : org2Len)+1;
	int minlen = org1Len > org2Len ? org2Len : org1Len;
	int boundInc = 5, reslen, i,j;
	int inclen;
	int allocSize = maxlen + boundInc,num1bitLen,num2bitLen,shiftLen;
	BOOLEAN is1Zero = FALSE, is2Zero = FALSE;
	BCHAR ressign = 0;
	BCHAR* res, * c, * tmpAddres, * tmpSubres, * multTenres, * divAddtmpRes,
		* cnum1b, * cnum2b, *tmpMultRes;
	if (c1sign != c2sign) {
		ressign = 1;
	}

	if (is2Zero) {
		throw 1;
	}

	DESTPOINT_CHECK(dest, allocSize);
	if (is1Zero) {
		*(dest) = 0;
		return 1;
	}

	res = ALLOCA_INT(allocSize);
	c = ALLOCA_INT(allocSize);
	cnum1b = ALLOCA_INT(allocSize);
	cnum2b = ALLOCA_INT(allocSize);
	tmpAddres = ALLOCA_INT(allocSize);
	tmpSubres = ALLOCA_INT(allocSize);
	tmpMultRes = ALLOCA_INT(allocSize<<1);
	divAddtmpRes = ALLOCA_INT(allocSize);

	is1Zero = IsZero(inum1, org1Len);
	is2Zero = IsZero(inum2, org2Len);

	ZERONUM(cnum1b, allocSize);
	ZERONUM(cnum2b, allocSize);
	ZERONUM(res, allocSize);
	ZERONUM(c, maxlen);

	num1bitLen = GetNumBitLength(inum1, org1Len);
	num2bitLen = GetNumBitLength(inum2, org2Len);

	COPYNUM(cnum1b, inum1, org1Len);
	COPYNUM(cnum2b, inum2, org2Len);

	SetSign(cnum1b, 0);
	SetSign(cnum2b, 0);

	shiftLen = (num1bitLen - num2bitLen) > 16 ? (num1bitLen - (BCHAR_BIT_SIZE << 1)) : 0;
	//Shift(cnum2b, cnum2b, maxlen, shiftLen);
	while (shiftLen > 0) {
		inclen = num1bitLen / BCHAR_BIT_SIZE - num2bitLen / BCHAR_BIT_SIZE - 1;
		inclen = 0;

		for (i = 0; i < _tenArrayLength; i++) {
			j = _tenBitsLength[i];
			if (num1bitLen < (j + num2bitLen)) {
				--i;
				break;
			}
		}
		if (i < 0) {
			i = 0;
		}
		inclen = i;

		multTenres = _tenPowByteArray[inclen];
		ZERONUM(tmpMultRes, allocSize<<1);
		Multiply(tmpMultRes, cnum2b, maxlen, multTenres, maxlen);
		COPYNUM(cnum2b, tmpMultRes, maxlen);
		i = IsBigger(cnum1b, maxlen, tmpMultRes, maxlen) ? 1 : -1; 

		while (TRUE) { 
			multTenres = _tenPowByteArray[1];
			ZERONUM(tmpMultRes, allocSize<<1);
			Multiply(tmpMultRes, cnum2b, maxlen, multTenres, 1);
			if (i > 0) {
				if (!IsBigger(cnum1b, maxlen, tmpMultRes, maxlen)) {
					break;
				} 
			}
			else {
				if (IsBigger(cnum1b, maxlen, tmpMultRes, maxlen)) {
					break;
				}
			}
			inclen = inclen + i;
			COPYNUM(cnum2b, tmpMultRes, maxlen);
		} 

		ZERONUM(divAddtmpRes, allocSize);
		while (IsBigger(cnum1b, maxlen, cnum2b, maxlen)) {
			ZERONUM(tmpSubres, allocSize);
			ZERONUM(tmpAddres, allocSize);
			Sub(tmpSubres, cnum1b, allocSize, cnum2b, allocSize);
			COPYNUM(cnum1b, tmpSubres, allocSize);
			Add(tmpAddres, divAddtmpRes, maxlen, _tenPowByteArray[0], 1);
			COPYNUM(divAddtmpRes, tmpAddres, maxlen);
		}  

			ZERONUM(tmpMultRes, allocSize<<1);
			Multiply(tmpMultRes, divAddtmpRes, maxlen, _tenPowByteArray[inclen], (_tenBitsLength[inclen]>>3)+1);
			COPYNUM(divAddtmpRes, tmpMultRes, maxlen);
			 

		COPYNUM(tmpAddres, res, maxlen);
		ZERONUM(res, allocSize);
		Add(res, tmpAddres, maxlen, divAddtmpRes, maxlen);

		num1bitLen = GetNumBitLength(cnum1b, org1Len);
		num2bitLen = GetNumBitLength(inum2, org2Len);
		  
		COPYNUMANDZERO(cnum2b, allocSize, inum2, org2Len);
		shiftLen = (num1bitLen - num2bitLen) > 16 ? (num1bitLen - (BCHAR_BIT_SIZE << 1)) : 0;

		 
	}
	ZERONUM(divAddtmpRes, allocSize);
	while (IsBigger(cnum1b, maxlen, cnum2b, maxlen)) {
		ZERONUM(tmpSubres, allocSize);
		ZERONUM(tmpAddres, allocSize);
		Sub(tmpSubres, cnum1b, maxlen, cnum2b, maxlen); 
		COPYNUM(cnum1b, tmpSubres, maxlen);

		Add(tmpAddres, divAddtmpRes, maxlen, _tenPowByteArray[0], 1); 
		COPYNUM(divAddtmpRes, tmpAddres, maxlen);
	} 
	COPYNUM(tmpAddres, res, maxlen);
	ZERONUM(res, allocSize);
	Add(res, tmpAddres, maxlen, divAddtmpRes, maxlen);

	//Shift(cnum2b, cnum2b, maxlen, -shiftLen);
	COPYNUM(c, cnum1b, org1Len);
	if (outrem != NULL) {
		COPYNUM(outrem, cnum1b, org1Len);
	}

	reslen = GetUsedLength(res, maxlen);
	COPYNUM(dest, res, reslen);
	SetSign(dest, ressign);
	return reslen;


}
int Multiply(BCHAR* dest, BCHAR* inum1, int i1Len, BCHAR* inum2, int i2Len) {
	int c1sign = GETSIGN(inum1), c2sign = GETSIGN(inum2);
	int org1Len = GetUsedLength(inum1, i1Len), org2Len = GetUsedLength(inum2, i2Len);
	int maxlen = org1Len > org2Len ? org1Len : org2Len;
	int minlen = org1Len > org2Len ? org2Len : org1Len;
	int boundInc = 4, reslen, i;
	int allocSize = maxlen + minlen + boundInc;
	BOOLEAN is1Zero = FALSE, is2Zero = FALSE;
	BCHAR ressign = 0;
	BCHAR* res, * c, * tmpAddres,
		* cnum1b, * cnum2b, *one ;


	if (c1sign != c2sign) {
		ressign = 1;
	}
	is1Zero = IsZero(inum1, org1Len);
	is2Zero = IsZero(inum2, org2Len); 
	
	 DESTPOINT_CHECK(dest, allocSize);


	if (is1Zero || is2Zero) {
		*(dest+0) = 0;
		return 0;
	}

	if (!IsBigger(inum1, org1Len, inum2, org2Len)) {
		c = inum1;
		inum1 = inum2;
		inum2 = c;
		org1Len = maxlen;
		org2Len = minlen;
	}

	res = ALLOCA_INT(allocSize);
	c = ALLOCA_INT(allocSize);
	cnum1b = ALLOCA_INT(allocSize);
	cnum2b = ALLOCA_INT(allocSize);
	tmpAddres = ALLOCA_INT(allocSize);
	one = ALLOCA_INT(allocSize);

	ZERONUM(cnum1b, allocSize);
	ZERONUM(cnum2b, allocSize);
	ZERONUM(res, allocSize);
	ZERONUM(one, allocSize);

	COPYNUM(cnum1b, inum1, org1Len);
	COPYNUM(cnum2b, inum2, org2Len);

	*one = 2;
	while (!IsZero(cnum2b, org2Len)) {
		ZERONUM(c, allocSize);
		//And(c, cnum2b, one, maxlen);
		i = GETONEBIT(cnum2b,2);
		if (i!=0) {
			ZERONUM(tmpAddres, allocSize);
			Add(tmpAddres, res, allocSize - 1, cnum1b, allocSize - 1);
			COPYNUMANDZERO(res, allocSize, tmpAddres, allocSize - 1);
		}
		Shift_Left_1(cnum1b, allocSize - 2);
		Shift_Right_1(cnum2b, org2Len);

	}
	reslen = GetUsedLength(res, allocSize - 1);
	COPYNUM(dest, res, reslen);
	SetSign(dest, ressign);
	return reslen;

}
BOOLEAN IsBigger(BCHAR* cnum1, int c1len, BCHAR* cnum2, int c2len) {
	int i, j;
	BUCHAR num1, num2;
	j = c1len > c2len ? c1len : c2len;
	for (i = j - 1; i >= 0; i--) {
		num1 = 0; num2 = 0;
		if (i < c1len) {
			num1 = *(cnum1+i);
		}
		if (i < c2len) {
			num2 = *(cnum2+i);
		}
		if (i == 0) {
			num1 = num1 >> 1;
			num2 = num2 >> 1;
		}
		if (num1 != num2) {
			if (num1 > num2) {
				return TRUE;
			}
			else {
				return FALSE;
			}

		}
	}
	return TRUE;
}
int Sub(BCHAR* dest, BCHAR* inum1, int i1Len, BCHAR* inum2, int i2Len) {
	BCHAR c1sign = GETSIGN(inum1), c2sign = GETSIGN(inum2);
	int org1Len = GetUsedLength(inum1, i1Len), org2Len = GetUsedLength(inum2, i2Len);
	int maxlen = org1Len > org2Len ? org1Len : org2Len;
	int minlen = org1Len > org2Len ? org2Len : org1Len;
	int boundInc = 6, reslen, i;
	int allocSize = maxlen + boundInc;
	BOOLEAN is1Zero = FALSE, is2Zero = FALSE;
	BCHAR ressign = 0;
	BCHAR* res, * c,
		* cnum1b, * cnum2b, one[2] = { 2 };
	 

	DESTPOINT_CHECK(dest, allocSize);
	res = ALLOCA_INT(allocSize);
	c = ALLOCA_INT(allocSize);
	cnum1b = ALLOCA_INT(allocSize);
	cnum2b = ALLOCA_INT(allocSize); 

	ZERONUM(cnum1b, allocSize);
	ZERONUM(cnum2b, allocSize);
	ZERONUM(res, allocSize);
	ZERONUM(c, allocSize);

	COPYNUM(cnum1b, inum1, org1Len);
	COPYNUM(cnum2b, inum2, org2Len);
	is1Zero = IsZero(inum1, org1Len);
	is2Zero = IsZero(inum2, org2Len);
	if (is1Zero && is2Zero) {
		*(dest+0) = 0;
		return 1;
	}
	else if (is1Zero) {
		COPYNUM(dest, inum2, org2Len);
		ressign = c2sign == 0 ? 1 : 0;
		SetSign(dest, ressign);
		return 1;
	}
	else if (is2Zero) {
		COPYNUM(dest, inum1, org1Len);
		return 1;
	}


	if (c1sign != c2sign) {
		//2-(-1) 转换符号 变成2+1
		if (c1sign == 0) {
			*(cnum2b) = (*(cnum2b) >> 1) << 1;//去掉符号位
			return Add(dest, cnum1b, org1Len, cnum2b, org2Len);
		}
		else {  //-2-2 保留符号变成 -(2+2)  
			ressign = c1sign;
			*(cnum1b) = (*(cnum1b) >> 1) << 1;//去掉符号位
			reslen = Add(dest, cnum1b, org1Len, cnum2b, org2Len);
			SetSign(dest, ressign);
			return reslen;
		}
	}
	else {
		if (IsBigger(cnum1b, org1Len + 1, cnum2b, org2Len + 1)) {
			ressign = 0;
		}
		else {
			c = cnum1b;
			cnum1b = cnum2b;
			cnum2b = c;
			org1Len = maxlen;
			org2Len = minlen;
			ressign = 1;
		}
		//c1sign == 1就行 表示 -2-(-1) 转换成 1-2 然后换符号
		if (c1sign == 1 && c2sign == 1) {
			ressign = ressign == 1 ? 0 : 1;//-2-(-1) 最终符号负号 
			SetSign(cnum1b, 0);
			SetSign(cnum1b, 0);
		}

	}

	Not(res, cnum2b, maxlen);
	Add(c, res, allocSize-1, one, 1);
	ZERONUM(res, allocSize);
	reslen = Add(res, c, allocSize - 1, cnum1b, org1Len);
	for (i = 0; i < reslen - maxlen; i++) {
		*(res+(maxlen + i)) = 0;
	}
	reslen = GetUsedLength(res, allocSize - 1);
	COPYNUM(dest, res, reslen);
	SetSign(dest, ressign);
	return reslen;

}

int Add(BCHAR* dest, BCHAR* inum1, int i1Len, BCHAR* inum2, int i2Len) {
	int c1sign = GETSIGN(inum1), c2sign = GETSIGN(inum2), ressign;
	int org1Len = GetUsedLength(inum1, i1Len), org2Len = GetUsedLength(inum2, i2Len);
	int maxlen = org1Len > org2Len ? org1Len : org2Len;
	int minlen = org1Len > org2Len ? org2Len : org1Len;
	BOOLEAN is1Zero = FALSE, is2Zero = FALSE;
	int boundInc = 4;
	int allocSize = maxlen + boundInc;

	if (c1sign != c2sign) {
		return Sub(dest, inum1, org1Len, inum2, org2Len);
	}
	DESTPOINT_CHECK(dest, allocSize);
	BCHAR* res, * c, * tmpAddres, * tmpSubres,
		* cnum1b, * cnum2b, * t1, * t2, one[1] = { 2 };
	res = ALLOCA_INT(allocSize);
	c = ALLOCA_INT(allocSize);
	cnum1b = ALLOCA_INT(allocSize);
	cnum2b = ALLOCA_INT(allocSize);
	t1 = ALLOCA_INT(allocSize);
	t2 = ALLOCA_INT(allocSize);



	is1Zero = IsZero(inum1, org1Len);
	is2Zero = IsZero(inum2, org2Len);

	if (is1Zero && is2Zero) {
		*(dest) = 0;
		return 1;
	}
	if (is1Zero) {
		COPYNUM(dest, inum2, org2Len);
		return org2Len;
	}
	if (is2Zero) {
		COPYNUM(dest, inum1, org1Len);
		return org1Len;
	}


	ZERONUM(dest, allocSize);

	ZERONUM(res, allocSize);
	ZERONUM(c, allocSize);
	ZERONUM(cnum1b, allocSize);
	ZERONUM(cnum2b, allocSize);

	COPYNUM(cnum1b, inum1, org1Len);
	COPYNUM(cnum2b, inum2, org2Len);



	Xor(res, cnum1b, cnum2b, maxlen);
	And(c, cnum1b, cnum2b, maxlen);
	Shift_Left_1(c, maxlen + 1);
	while (!IsZero(c, allocSize)) {
		ZERONUM(t1, allocSize);
		ZERONUM(t2, allocSize);

		COPYNUM(t1, res, maxlen + 1);
		COPYNUM(t2, c, maxlen + 1);

		ZERONUM(res, allocSize);
		ZERONUM(c, allocSize);

		Xor(res, t1, t2, maxlen + 1);
		And(c, t1, t2, maxlen + 1);
		Shift_Left_1(c, maxlen + 1);
	}
	if (c1sign == c2sign) {
		ressign = c1sign;
	}
	//GetNumBit(5, 1);
	SetSign(res, ressign);
	COPYNUM(dest, res, allocSize);
	return GetUsedLength(dest, allocSize);

}
 
 
VOID SetSign(BCHAR* nums, int sign) {
	BUCHAR num = *(nums);  
	*(nums) = ((num >> 1) << 1) | (sign);;
} 
int GetNumBitLength(BCHAR* nums, int len) {
	int i, j, totallen;
	BCHAR num;
	BCHAR bit;
	totallen = 0;
	for (i = len - 1; i >= 0; i--) {
		num = *(nums+i);
		if (num == 0) {
			continue;
		}
		for (j = BCHAR_BIT_SIZE - 1; j >= 0; j--) {
			bit = GETNUMBIT(num, j);
			if (bit == 1) { 
					totallen = (j+1) + (i) * BCHAR_BIT_SIZE; 
				break;
			}
		}
		if (bit == 1) {
			break;
		} 
	}
	if (totallen == 1) {
		totallen=0;
	}
	return totallen;
}
VOID Shift_Right_1(BCHAR* inum, int len) {
	Shift(inum, inum, len, -1);
}
VOID Shift_Left_1(BCHAR* inum, int len) {
	Shift(inum, inum, len, 1);
}
VOID Shift(BCHAR* dest, BCHAR* inum, int len, int shift) {
	int i, j;
	BUCHAR num1, num2;
	BCHAR  tmpmask, mask, sign; 
	if (shift == 0) {
		COPYNUM(dest, inum, len);
		return;
	}

	sign = GETSIGN(inum);
	SetSign(inum, 0);
	j = shift >= 0 ? shift : -shift;
	if (j >= BCHAR_BIT_SIZE) { 
		int sl = j / BCHAR_BIT_SIZE;
		int sr = j % BCHAR_BIT_SIZE;
		if (shift < 0) {
			for (i = 0; i < len; i++) {
				if (i - sl < 0) {
					continue;
				}
				num1 = *(inum + i);
				*(dest + i) = 0;
				*(dest + (i - sl)) = num1;
			}
			if (sr != 0) {
				Shift(dest, inum, len, -sr);
			}

		}
		else
		{
			for (i = len - 1; i >= 0; i--) {
				num1 = *(inum + i);
				*(dest + i) = 0;
				*(dest + i + sl) = num1;
			}
			if (sr != 0) {
				Shift(dest, inum, len, sr);
			}
		}


		

	}
	else {
		mask = 0; 
		for (i = 0; i < j; i++) {
			if (shift < 0) {
				tmpmask = 1 << i;
				mask |= tmpmask;
			}
			else {
				tmpmask = 1 << (BCHAR_BIT_SIZE - i - 1);
				mask |= tmpmask;
			}
		}
		if (shift > 0) {
			for (i = len; i >= 1; i--) {
				num1 = *(inum + i);
				num2 = *(inum + (i - 1));
				tmpmask = ((num2 & mask)) >> (BCHAR_BIT_SIZE - shift);

				num1 = (num1 << shift) | tmpmask;
				*(dest + i) = num1;
			}
			num1 = (*inum);
			*(dest) = num1 << shift;
		}
		else {
			shift = -shift;
			for (i = 0; i < len; i++) {
				num1 = *(inum + i);
				num2 = *(inum + (i + 1));
				tmpmask = ((num2 & mask)) << (BCHAR_BIT_SIZE - shift);
				num1 = (num1 >> shift) | tmpmask;
				*(dest + i) = num1;
			}
			num1 = *(inum + len);
			*(dest + len) = num1 >> shift;

		}
	}



	SetSign(inum, sign);
}


BOOLEAN IsZero(BCHAR* nums, int len) {
	BUCHAR num;
	int i;
	for (i = 0; i < len; i++) {
		num = *(nums+i);
		if (num != 0) {
			if (i == 0) {
				num = (num >> 1) << 1;
				if (num == 0) {
					return TRUE;
				}
			}
			return FALSE;
		}
	}
	return TRUE;

}


int GetUsedLength(BCHAR* cnum, int clen) {
	BCHAR num;
	int i;
	for (i = clen - 1; i >= 0; i--) {
		num = *(cnum+i);
		if (num != 0) {
			return i + 1;
		}
	}
	return 0;
}



VOID Not(BCHAR* dest, BCHAR* inum, int ilen) {
	Operator(dest, inum, NULL, ilen, NOT);
}
VOID And(BCHAR* dest, BCHAR* inum1, BCHAR* inum2, int ilen) {
	Operator(dest, inum1, inum2, ilen, AND);
}
VOID Xor(BCHAR* dest, BCHAR* inum1, BCHAR* inum2, int ilen) {
	Operator(dest, inum1, inum2, ilen, XOR);
}
inline VOID Operator(BCHAR* dest, BCHAR* inum1, BCHAR* inum2, int ilen, int type) {
	int i, j, codepos, respos;
	BCHAR restmp, i1tmp, i2tmp;
	BCHAR signbit;
	signbit = GETSIGN(inum1);
	for (i = 0; i < ilen; i++) {
		i1tmp = *(inum1+i);
		if (type == NOT) {
			restmp = ~i1tmp;
		}
		else {
			i2tmp = *(inum2+i);
		}
		if (type == AND) {
			restmp = i1tmp & i2tmp;
		}
		if (type == XOR) {
			restmp = i1tmp ^ i2tmp;
		}
		*(dest+i) = restmp;
	}
	if (type == NOT) {
		*(dest+ilen) = ~0;
	}
	SetSign(dest, signbit);

	/*
	for (i = 0; i < bitslen; i+= BCHAR_BIT_SIZE) {
		i1tmp = inum1[codepos];
		i2tmp = inum2[codepos];
		if (i1tmp == 0 && i2tmp == 0) {
			continue;
		}
		restmp = 0;
		for (j = 0; j < BCHAR_BIT_SIZE; j++) {
			if (j == 0 && i == 0) {
				continue;
			}
			bit1 = GetIntBit(i1tmp, j);
			bit2 = GetIntBit(i2tmp, j);
			if (type == AND) {
				resbit = bit1 & bit2;
			}
			if (type == XOR) {
				resbit = bit1 ^ bit2;
			}
			restmp |= resbit << j;
		}
		dest[codepos] = restmp;
		++codepos;


	}
	*/

} 


VOID BitsToStringNum(CHAR* outstr,BCHAR* bits, int len) { 
	BCHAR* rem, * cnum1b, * tmpresdiv,  ten[2] = {20},sign;
	BUCHAR bit,num;
	int i, j,boundInc=4, allocsize = len + boundInc;

	sign = GETSIGN(bits);
	rem = ALLOCA_INT(allocsize);
	cnum1b = ALLOCA_INT(allocsize);
	tmpresdiv= ALLOCA_INT(allocsize);

	ZERONUM(rem, allocsize);
	ZERONUM(cnum1b, allocsize);
	COPYNUM(cnum1b, bits, len);

	j = 0;
	do {
			ZERONUM(tmpresdiv, allocsize);
		  Div(tmpresdiv,cnum1b, len, ten,1, rem);
		  ZERONUM(cnum1b, allocsize);
		  COPYNUM(cnum1b, tmpresdiv, len); 

		  num = *rem;// BitsToNum(rem, len);
		  num = num >> 1;
		outstr[j++] = num + '0';
		ZERONUM(rem, allocsize); 
	} while (!IsZero(cnum1b, len)); 
	if (sign != 0) {
		outstr[j++] = '-';
	}
	SwapByte((CHAR*)outstr, GetUsedLength(outstr,len));

} 
VOID SwapByte(CHAR* bs, int len) {
	CHAR bit;
	int i, j;
	j = len >> 1;
	for (i = 0; i < j; i++) {
		bit = bs[i];
		bs[i] = bs[len - i - 1];
		bs[len - i - 1] = bit;
	}


}
int StringNumToBits(BCHAR* outbyte, CHAR* str, int len) {
	int i, j, allocsize = len,tenIndex;
	CHAR numchar,*tmpstr; 
	BCHAR* res,  * tmpaddres,*tmpmultres1,* tmpmultres2,* cnum1, * tmpcnum1, * ten, * cnum2;
	BCHAR sign =str[0];
	BUCHAR num;
	tmpstr = str;
	sign = str[0];
	sign = sign == '-' ? 1 : 0; 

	cnum2 = ALLOCA_INT(2);
	res = ALLOCA_INT(allocsize);
	tmpmultres1 = ALLOCA_INT(allocsize);
	tmpmultres2 = ALLOCA_INT(allocsize);
	tmpaddres = ALLOCA_INT(allocsize);
	cnum1 = ALLOCA_INT(allocsize);
	ten = ALLOCA_INT(2);
	ZERONUM(cnum2, 2);
	ZERONUM(cnum1, allocsize);
	ZERONUM(tmpmultres1, allocsize);
	ZERONUM(tmpmultres2, allocsize);
	ZERONUM(tmpaddres, allocsize);
	ZERONUM(res, allocsize);
	ZERONUM(ten,2);
	*ten = 20;
	*cnum1 = 2; 
	while (*tmpstr++ != 0);
	--tmpstr;
	tenIndex = 0;
	for (i = 0; i < len; i++) { 
		numchar = *--tmpstr;
		if (numchar == '-' || numchar == '+') {
			continue;
		}
		num = numchar - '0';
		num = num << 1;  
		*cnum2 = num;  
		if (IsZero(cnum2,1)) {
			tenIndex++;
			continue;
		}
		ZERONUM(tmpmultres1, allocsize);
			Multiply(tmpmultres1, _tenPowByteArray[tenIndex++], allocsize - 1, cnum2, 1); 
			ZERONUM(tmpaddres, allocsize);
		 Add(tmpaddres, res, allocsize - 1, tmpmultres1, allocsize - 1); 
		 ZERONUM(tmpmultres2, allocsize);
		 Multiply(tmpmultres2,cnum1, allocsize - 1, ten, 1); 
		  COPYNUMANDZERO(cnum1, allocsize, tmpmultres2, allocsize-1);
		  COPYNUMANDZERO(res, allocsize, tmpaddres, allocsize-1);
		   
	}
	COPYNUMANDZERO(outbyte, allocsize, res, allocsize);
	SetSign(outbyte, sign);
	return GetUsedLength(outbyte, allocsize);
}

 