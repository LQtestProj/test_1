#pragma once

#include <memory.h>  
#include <malloc.h>

#define CHAR char
#define VOID void
#define BYTE unsigned char
#define BOOLEAN char
#define FALSE 0
#define TRUE 1
#define UINT unsigned int
#define NULL 0
class Bignum {

public:
	Bignum(BYTE* b,int len);
	BYTE* bits;
	BYTE sign;
	int bitsize;

};


CHAR* BitsToStringNum(BYTE* str, int len);
BYTE* StringNumToBits(CHAR* str, int len);
BYTE* Add(BYTE* cnum1, int c1len, BYTE* cnum2, int c2len);
BYTE* Sub(BYTE* cnum1, int c1len, BYTE* cnum2, int c2len);
BYTE* Multiply(BYTE* cnum1, int c1len, BYTE* cnum2, int c2len);
BYTE* Div(BYTE* cnum1, int c1len, BYTE* cnum2, int c2len,BYTE* outrem=NULL);
BOOLEAN IsZero(BYTE* cnum, int clen);
int BitLen(BYTE* cnum, int clen);
BOOLEAN IsBigger(BYTE* cnum1, int c1len, BYTE* cnum2, int c2len);
int RemoveTailBit(BYTE* cnum, int clen);

VOID SwapByte(BYTE* bs, int len);
int ArrayLen(BYTE* arr);
VOID NumToBits(UINT num, CHAR* outstr);
UINT BitsToNum(BYTE* num, int len);
VOID Not(BYTE* dest, BYTE* cnum, int cLen);
VOID Xor(BYTE* dest, BYTE* cnum1, BYTE* cnum2, int cLen);
VOID And(BYTE* dest, BYTE* cnum1, BYTE* cnum2, int cLen);
VOID SHIFT_LEFT_1(BYTE* cnum, int cLen);
VOID SHIFT_RIGHT_1(BYTE* cnum, int cLen);
VOID SHIFT(BYTE* dest, int shift, BYTE* cnum, int cLen);




class BignumCache {
	UINT** m_numsArrayCache;
	int m_ilen,m_iArrayLen;
public:
	BignumCache() :m_ilen(0), m_iArrayLen(10){}
	VOID ClearArray();
	VOID AllocCache(int ilen);
	VOID ZeroCache();
	UINT* Get(int index);


};
VOID ZeroInt(UINT* nums, int len);
VOID IntCopy(UINT* dest, UINT* src, int len);
BOOLEAN IsBigger(UINT* cnum1, int c1len, UINT* cnum2, int c2len);
BOOLEAN IsZero(UINT* nums, int len);
BYTE GetIntBit(UINT* nums, int numpos, int shiftpos);
BYTE GetIntBit(UINT num, int pos);
BYTE GETSIGN(UINT* nums);
VOID SetSign(UINT* nums, int sign);
int GetUsedLength(UINT* cnum, int clen);

VOID Not(UINT* dest, UINT* inum, int ilen);
VOID Xor(UINT* dest, UINT* inum1, UINT* inum2, int ilen);
VOID And(UINT* dest, UINT* inum1, UINT* inum2, int ilen);
VOID Operator(UINT* dest, UINT* inum1, UINT* inum2, int ilen, int type);
VOID Shift_Left_1(UINT* inum, int len);
VOID Shift_Right_1(UINT* inum, int len);
VOID Shift(UINT* dest, UINT* inum, int len, int shift);
UINT SetNumBit(UINT num, int pos, BYTE b);
VOID SetNumBit(UINT* nums, int index, int pos, BYTE b);

int Add(UINT* dest, UINT* inum1, int i1Len, UINT* inum2, int i2Len);
int Sub(UINT* dest, UINT* inum1, int i1Len, UINT* inum2, int i2Len);
int Multiply(UINT* dest, UINT* inum1, int i1Len, UINT* inum2, int i2Len);
int Div(UINT* dest, UINT* inum1, int i1Len, UINT* inum2, int i2Len, UINT* outrem=NULL);







