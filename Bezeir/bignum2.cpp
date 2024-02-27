#include "bignum.h"
 
#define ALLOCA_INT(len) (UINT*)alloca(len*sizeof(int))
#define AND 1
#define XOR 2
#define NOT 3

BignumCache _cAdd,_cSub,_cMult,_cDiv;
int Div(UINT* dest, UINT* inum1, int i1Len, UINT* inum2, int i2Len, UINT* outrem) {
	int c1sign = GETSIGN(inum1), c2sign = GETSIGN(inum2);
	int org1Len = GetUsedLength(inum1, i1Len), org2Len = GetUsedLength(inum2, i2Len);
	int maxlen = org1Len > org2Len ? org1Len : org2Len;
	int minlen = org1Len > org2Len ? org2Len : org1Len;
	int boundInc = 4, reslen, i;
	int allocSize = maxlen  + boundInc;
	BOOLEAN is1Zero = FALSE, is2Zero = FALSE;
	BYTE ressign = 0;
	UINT* res, * c , * tmpAddres, * tmpSubres,
		* cnum1b, * cnum2b , one[1] = { 2 };

	_cDiv.AllocCache(allocSize);
	_cDiv.ZeroCache();
	res = _cDiv.Get(0);
	c = _cDiv.Get(1);
	tmpAddres = _cDiv.Get(2);
	tmpSubres = _cDiv.Get(3);
	cnum1b = _cDiv.Get(4);
	cnum2b = _cDiv.Get(5);




	if (c1sign != c2sign) {
		ressign = 1;
	}

	is1Zero = IsZero(inum1, org1Len);
	is2Zero = IsZero(inum2, org2Len);
	if (is2Zero) {
		throw 1;
	}
	if (is1Zero) {
		dest[0] = 0;
		return 1;
	}
	ZeroInt(cnum1b, allocSize);
	ZeroInt(cnum2b, allocSize);
	ZeroInt(res, allocSize);
	ZeroInt(c, maxlen);

	IntCopy(cnum1b, inum1, org1Len);
	IntCopy(cnum2b, inum2, org2Len);

	SetSign(cnum1b, 0);
	SetSign(cnum2b, 0);
	while (IsBigger(cnum1b, maxlen, cnum2b, maxlen)) {
		ZeroInt(tmpSubres, allocSize);
		ZeroInt(tmpAddres, allocSize);
		 Sub(tmpSubres,cnum1b, maxlen, cnum2b, maxlen);
		 ZeroInt(cnum1b, allocSize); 
		 IntCopy(cnum1b, tmpSubres, maxlen);

		 Add(tmpAddres,res, maxlen, one, 1);
		 ZeroInt(res, allocSize);
		 IntCopy(res, tmpAddres, maxlen);  
	} 
	IntCopy(c,cnum1b, org1Len);  
	if (outrem != NULL) {
		IntCopy(outrem, cnum1b, org1Len);  
	}

	reslen = GetUsedLength(res, maxlen);
	IntCopy(dest, res, reslen);
	SetSign(dest, ressign);
	return reslen;


}
int Multiply(UINT* dest, UINT* inum1, int i1Len, UINT* inum2, int i2Len) {
	int c1sign = GETSIGN(inum1), c2sign = GETSIGN(inum2);
	int org1Len = GetUsedLength(inum1, i1Len), org2Len = GetUsedLength(inum2, i2Len);
	int maxlen = org1Len > org2Len ? org1Len : org2Len;
	int minlen = org1Len > org2Len ? org2Len : org1Len;
	int boundInc = 4, reslen, i;
	int allocSize = maxlen+minlen + boundInc;
	BOOLEAN is1Zero = FALSE, is2Zero = FALSE;
	BYTE ressign = 0; 
	UINT* res, * c, * tmpAddres, * tmpSubres,
		* cnum1b, * cnum2b, one[1] = { 2 };

	_cMult.AllocCache(allocSize);
	_cMult.ZeroCache();
	res = _cMult.Get(0);
	c = _cMult.Get(1);
	tmpAddres = _cMult.Get(2);
	tmpSubres = _cMult.Get(3);
	cnum1b = _cMult.Get(4);
	cnum2b = _cMult.Get(5);



	if (c1sign != c2sign) {
		ressign = 1;
	}
	is1Zero = IsZero(inum1, org1Len);
	is2Zero = IsZero(inum2, org2Len);
 
	if (is1Zero|| is2Zero) {
		dest[0] = 0;
		return 1;
	}

	ZeroInt(cnum1b, allocSize);
	ZeroInt(cnum2b, allocSize);
	ZeroInt(res, allocSize);

	IntCopy(cnum1b, inum1, org1Len);
	IntCopy(cnum2b, inum2, org2Len);


	while (!IsZero(cnum2b, org2Len)) {
		ZeroInt(c, allocSize);
		And(c, cnum2b, one, maxlen);
		if (!IsZero(c, maxlen)) {
			ZeroInt(tmpAddres, allocSize);
			Add(tmpAddres,res, allocSize - 1, cnum1b, allocSize - 1);
			ZeroInt(res, allocSize);
			IntCopy(res, tmpAddres, allocSize-1);  
		}
		Shift_Left_1(cnum1b, allocSize - 2);
		Shift_Right_1(cnum2b, org2Len);

	}
	reslen = GetUsedLength(res, allocSize - 1);
	IntCopy(dest, res, reslen);
	SetSign(dest, ressign);
	return reslen;

}
BOOLEAN IsBigger(UINT * cnum1, int c1len, UINT * cnum2, int c2len) {
	int i,j;
	UINT num1, num2;
	j = c1len > c2len ? c1len : c2len;
	for (i = j-1; i >=0; i--) {
		num1 = 0; num2 = 0;
		if (i < c1len) {
			num1 = cnum1[i];
		}
		if (i < c2len) {
			num2 = cnum2[i];
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
int Sub(UINT* dest, UINT* inum1, int i1Len, UINT* inum2, int i2Len) {
	int c1sign = GETSIGN(inum1), c2sign = GETSIGN(inum2);
	int org1Len = GetUsedLength(inum1, i1Len), org2Len = GetUsedLength(inum2, i2Len);
	int maxlen = org1Len > org2Len ? org1Len : org2Len;
	int minlen = org1Len > org2Len ? org2Len : org1Len;
	int boundInc = 4, reslen,i;
	int allocSize = maxlen + boundInc;
	BOOLEAN is1Zero = FALSE, is2Zero = FALSE;
	BYTE ressign=0; 
	UINT* res, * c, * tmpAddres, * tmpSubres,
		* cnum1b, * cnum2b, one[1] = { 2 };

	_cSub.AllocCache(allocSize);
	_cSub.ZeroCache();
	res = _cSub.Get(0);
	c = _cSub.Get(1);
	tmpAddres = _cSub.Get(2);
	tmpSubres = _cSub.Get(3);
	cnum1b = _cSub.Get(4);
	cnum2b = _cSub.Get(5);


	ZeroInt(cnum1b, allocSize);
	ZeroInt(cnum2b, allocSize);
	ZeroInt(res, allocSize);
	ZeroInt(c, allocSize);

	IntCopy(cnum1b, inum1, org1Len);
	IntCopy(cnum2b, inum2, org2Len);
	is1Zero = IsZero(inum1, org1Len);
	is2Zero = IsZero(inum2, org2Len);
	if (is1Zero && is2Zero) {
		dest[0] = 0;
		return 1;
	}else if (is1Zero) {
		IntCopy(dest, inum2, org2Len);
		ressign = c2sign == 0 ? 1 : 0;
		SetSign(dest, ressign);
		return 1;
	}
	else if (is2Zero) {
		IntCopy(dest, inum1, org1Len);
		return 1;
	}


	if (c1sign != c2sign) {
		//2-(-1) 转换符号 变成2+1
		if (c1sign == 0) {
			cnum2b[0] = (cnum2b[0]>>1)<<1;//去掉符号位
			return Add(dest, cnum1b, org1Len, cnum2b, org2Len);
		}
		else {  //-2-2 保留符号变成 -(2+2)  
			ressign = c1sign;
			cnum1b[0] = (cnum1b[0]>>1)<<1;//去掉符号位
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
			SetSign(cnum1b,0);
			SetSign(cnum1b, 0); 
		}

	} 
	    
	Not(res, cnum2b, maxlen);
	Add(c,res, maxlen + 1, one, 1);
	ZeroInt(res, allocSize);
	reslen=Add(res,c, allocSize - 1, cnum1b, org1Len);
	for (i = 0; i < reslen- maxlen; i++) {
		res[maxlen + i] = 0;
	}
	reslen = GetUsedLength(res, allocSize - 1);
	IntCopy(dest, res, reslen);
	SetSign(dest, ressign);
	return reslen;

}


 
int Add(UINT* dest, UINT* inum1, int i1Len, UINT* inum2, int i2Len) {
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

	UINT* res, * c, * tmpAddres, * tmpSubres,
		* cnum1b, * cnum2b,*t1,*t2, one[1] = { 2 };

	_cAdd.AllocCache(allocSize);
	_cAdd.ZeroCache();
	res = _cAdd.Get(0);
	c = _cAdd.Get(1);
	tmpAddres = _cAdd.Get(2);
	tmpSubres = _cAdd.Get(3);
	cnum1b = _cAdd.Get(4);
	cnum2b = _cAdd.Get(5);
	t1 = _cAdd.Get(6);
	t2 = _cAdd.Get(7);

	is1Zero = IsZero(inum1, org1Len);
	is2Zero = IsZero(inum2, org2Len);

	if (is1Zero && is2Zero) {
		dest[0] = 0;
		return 1;
	}
	if (is1Zero) {
		IntCopy(dest, inum2, org2Len);
		return org2Len;
	}
	if (is2Zero) { 
		IntCopy(dest, inum1, org1Len);
		return org1Len;
	}


	ZeroInt(dest, allocSize);

	ZeroInt(res, allocSize);
	ZeroInt(c, allocSize);
	ZeroInt(cnum1b, allocSize);
	ZeroInt(cnum2b, allocSize);

	IntCopy(cnum1b, inum1, org1Len);
	IntCopy(cnum2b, inum2, org2Len);



	Xor(res, cnum1b, cnum2b, maxlen);
	And(c, cnum1b, cnum2b, maxlen);
	Shift_Left_1(c, maxlen + 1);
	while (!IsZero(c, allocSize)) {
		ZeroInt(t1, allocSize);
		ZeroInt(t2, allocSize);

		IntCopy(t1, res, maxlen + 1);
		IntCopy(t2, c, maxlen + 1);

		ZeroInt(res, allocSize);
		ZeroInt(c, allocSize);

		Xor(res, t1, t2, maxlen + 1);
		And(c, t1, t2, maxlen + 1);
		Shift_Left_1(c, maxlen + 1);
	}
	if (c1sign == c2sign) {
		ressign = c1sign;
	}
	SetSign(res, ressign);
	IntCopy(dest, res, allocSize);
	return GetUsedLength(dest, allocSize);

}

BYTE GetIntBit(UINT* nums, int numpos, int shiftpos) {
	return GetIntBit(nums[numpos], shiftpos);
}
BYTE GetIntBit(UINT num, int pos) {
	return (num & (1 << (pos))) >> (pos);
}
VOID IntCopy(UINT* dest, UINT* src, int len) {
	memcpy(dest, src, len * sizeof(UINT));
}
VOID ZeroInt(UINT* nums, int len) {
	 memset(nums, 0, sizeof(UINT) *  len);
}
VOID SetNumBit(UINT* nums, int index, int pos, BYTE b) {
	nums[index] = SetNumBit(nums[index], pos, b);
}
UINT SetNumBit(UINT num, int pos, BYTE b) {
	UINT res = num & (1 << pos);
	if (res == num) {
		res = num ^ (1 << pos);
	}
	res = res | (b << pos);
	return res;
}
VOID SetSign(UINT* nums, int sign) {
	UINT num = nums[0];
	num = (num >> 1) << 1;
	num = num | (sign);
	nums[0] = num;
}
BYTE GETSIGN(UINT* nums) {
	return GetIntBit(nums[0], 0);
}
const int INT_BIT_SIZE = sizeof(int) * 8;
int GetIntBitLength(UINT* nums, int len) {
	int i, j, totallen;
	UINT num;
	BYTE bit;
	totallen = len * INT_BIT_SIZE;
	for (i = len - 1; i > 1; i--) {
		num = nums[i];
		for (j = INT_BIT_SIZE - 1; j >= 0; j--) {
			bit = GetIntBit(num, j);
			if (bit == 1) {
				totallen = i * INT_BIT_SIZE - (INT_BIT_SIZE - (j + 1));
				break;
			}
		}
		if (bit == 1) {
			break;
		}

	}

	return totallen;
}
VOID Shift_Right_1(UINT* inum, int len) {
	Shift(inum, inum, len, -1);
}
VOID Shift_Left_1(UINT* inum, int len) {
	Shift(inum, inum, len, 1);
}
VOID Shift(UINT* dest, UINT* inum, int len, int shift) {
	int i, j, maskloop;
	UINT num1, num2, tmpmask, mask;
	BYTE sign;
	sign = GETSIGN(inum);
	SetSign(inum, 0);
	maskloop = shift < 0 ? -shift : shift;
	mask = 0;
	for (i = 0; i < maskloop; i++) {
		if (shift < 0) {
			tmpmask = 1 << i;
			mask |= tmpmask;
		}
		else {
			tmpmask = 1 << (INT_BIT_SIZE - i - 1);
			mask |= tmpmask;
		}
	}
	if (shift > 0) {
		for (i = len; i >= 1; i--) {
			num1 = inum[i];
			num2 = inum[i - 1];
			tmpmask = (num2 & mask) >> (INT_BIT_SIZE - shift );
			num1 = num1 << shift;
			num1 = num1 | tmpmask;
			dest[i] = num1;
		}
		dest[0] = inum[0] << shift;
	}
	else {
		shift = -shift;
		for (i = 0; i < len; i++) {
			num1 = inum[i];
			num2 = inum[i + 1];
			tmpmask = (num2 & mask) << (INT_BIT_SIZE - shift );
			num1 = num1 >> shift;
			num1 = num1 | tmpmask;
			dest[i] = num1;
		}
		dest[len] = inum[len] >> shift;

	}
	SetSign(inum, sign);
}


BOOLEAN IsZero(UINT* nums, int len) {
	UINT num;
	int i;
	for (i = 0; i < len; i++) {
		num = nums[i];
		if (i == 0) {
			num = (num >> 1) << 1;
		}
		if (num != 0) {
			return FALSE;
		}
	}
	return TRUE;

}


int GetUsedLength(UINT* cnum, int clen) {
	UINT num;
	int i;
	for (i = clen - 1; i >= 0; i--) {
		num = cnum[i];
		if (num != 0) {
			return i + 1;
		}
	}
	return 0;
}



VOID Not(UINT* dest, UINT* inum, int ilen) {
	Operator(dest, inum, NULL, ilen, NOT);
}
VOID And(UINT* dest, UINT* inum1, UINT* inum2, int ilen) {
	Operator(dest, inum1, inum2, ilen, AND);
}
VOID Xor(UINT* dest, UINT* inum1, UINT* inum2, int ilen) {
	Operator(dest, inum1, inum2, ilen, XOR);
}
VOID Operator(UINT* dest, UINT* inum1, UINT* inum2, int ilen, int type) {
	int i, j, codepos, respos, bitslen = ilen * INT_BIT_SIZE;
	UINT restmp, i1tmp, i2tmp;
	BYTE signbit;
	signbit = GETSIGN(inum1);
	for (i = 0; i < ilen; i++) {
		i1tmp = inum1[i];
		if (type == NOT) {
			restmp = ~i1tmp;
		}
		else {
			i2tmp = inum2[i];
		}
		if (type == AND) {
			restmp = i1tmp & i2tmp;
		}
		if (type == XOR) {
			restmp = i1tmp ^ i2tmp;
		}
		dest[i] = restmp;
	}
	if (type == NOT) {
		dest[ilen] = ~0;
	}
	SetSign(dest, signbit);

	/*
	for (i = 0; i < bitslen; i+= INT_BIT_SIZE) {
		i1tmp = inum1[codepos];
		i2tmp = inum2[codepos];
		if (i1tmp == 0 && i2tmp == 0) {
			continue;
		}
		restmp = 0;
		for (j = 0; j < INT_BIT_SIZE; j++) {
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
VOID BignumCache::ClearArray() {
	if (m_ilen == 0) {
		return;
	}
	int i;
	UINT* nums;
	for (i = 0; i < m_iArrayLen; i++) {
		nums = m_numsArrayCache[i];
		free(nums);
	}
	memset(m_numsArrayCache, 0, m_iArrayLen * sizeof(UINT*));
}
VOID BignumCache::AllocCache(int len) {
	if (len <= m_ilen) {
		return;
	}
	int i;
	ClearArray();
	if (m_ilen == 0) {
		m_numsArrayCache = (UINT**)malloc(m_iArrayLen * sizeof(UINT*));
		memset(m_numsArrayCache, 0, m_iArrayLen * sizeof(UINT*));
	}
	for (i = 0; i < m_iArrayLen; i++) {
		m_numsArrayCache[i] = (UINT*)malloc(sizeof(UINT) * len);
		memset(m_numsArrayCache[i], 0, sizeof(UINT) * len);
	}

	m_ilen = len;
}
UINT* BignumCache::Get(int index) {
	return m_numsArrayCache[index];
}
VOID BignumCache::ZeroCache() {
	int i;
	for (i = 0; i < m_iArrayLen; i++) { 
		memset(m_numsArrayCache[i], 0, sizeof(UINT) * m_ilen);
	}
}

