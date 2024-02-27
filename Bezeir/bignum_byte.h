#pragma once

#include <memory.h>  
#include <malloc.h>

#define BUCHAR unsigned char
#define CHAR char
#define BINT int
#define BCHAR  char
#define VOID void
#define BYTE unsigned char
#define BOOLEAN char
#define FALSE 0
#define TRUE 1
#define UINT unsigned int
#define NULL 0 

#define AND 1
#define XOR 2
#define NOT 3 

const int BCHAR_SIZE = sizeof(BCHAR);
const int BCHAR_BIT_SIZE = BCHAR_SIZE * 8;

#define CALC_SIZE(s) ((s)*BCHAR_SIZE)

#define ALLOCA_INT(len) (BCHAR*)alloca(CALC_SIZE(len))

#define ZERONUM(dst,s) memset(dst,0,CALC_SIZE(s))
#define  COPYNUM(dest,src,len) ZERONUM(dest,len+2); memcpy(dest,src,CALC_SIZE(len))
#define  COPYNUMANDZERO(dest,zdestlen,src,len) ZERONUM(dest,zdestlen); memcpy(dest,src,CALC_SIZE(len))
#define GETONEBIT(nums,pos) (*nums&pos) 
#define  GETSIGN(nums)  GETONEBIT(nums,1) 
#define GETNUMBIT(num,shift) ((BUCHAR)num & (1 << (shift))) >> (shift) 
#define DESTPOINT_CHECK(dest,len) int l=len-1;CHAR* tmppt = dest + CALC_SIZE(l); while (*tmppt-- == 0&& (l-->0)); if (++tmppt != dest) { throw 1; }



VOID SwapByte(CHAR* bs, int len);
int StringNumToBits(BCHAR* outbyte, CHAR* str, int len);
VOID BitsToStringNum(CHAR* outstr, BCHAR* bits, int len);
BOOLEAN IsBigger(BCHAR* cnum1, int c1len, BCHAR* cnum2, int c2len);
BOOLEAN IsZero(BCHAR* nums, int len); 
VOID SetSign(BCHAR* nums, int sign);
int GetUsedLength(BCHAR* cnum, int clen);
int GetNumBitLength(BCHAR* nums, int len);

VOID Not(BCHAR* dest, BCHAR* inum, int ilen);
VOID Xor(BCHAR* dest, BCHAR* inum1, BCHAR* inum2, int ilen);
VOID And(BCHAR* dest, BCHAR* inum1, BCHAR* inum2, int ilen);
inline VOID Operator(BCHAR* dest, BCHAR* inum1, BCHAR* inum2, int ilen, int type);
VOID Shift_Left_1(BCHAR* inum, int len);
VOID Shift_Right_1(BCHAR* inum, int len);
VOID Shift(BCHAR* dest, BCHAR* inum, int len, int shift);
BCHAR SetNumBit(BCHAR num, int pos, BYTE b);
VOID SetNumBit(BCHAR* nums, int index, int pos, BYTE b);

int Add(BCHAR* dest, BCHAR* inum1, int i1Len, BCHAR* inum2, int i2Len);
int Sub(BCHAR* dest, BCHAR* inum1, int i1Len, BCHAR* inum2, int i2Len);
int Multiply(BCHAR* dest, BCHAR* inum1, int i1Len, BCHAR* inum2, int i2Len);
int Div(BCHAR* dest, BCHAR* inum1, int i1Len, BCHAR* inum2, int i2Len, BCHAR* outrem = NULL);





