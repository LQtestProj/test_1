#pragma once

#include "pro.h"


  struct HNODE {
	  int parentIndex, leftIndex, rightIndex,index;
	BYTE ascii;
	int frequency;//weight
} ;

VOID HuffTest();
int HuffBuffCreate(VOID** obuffer,LPCSTR str);
int HuffNodeSort(HNODE  hroot[]);
VOID HuffRemake(HNODE newNode[], HNODE srcNode[], int len);
VOID ShowHuffCode(HNODE* hnode, int ilen, CHAR c);
int GetHuffCodeArray(HNODE* hnode, int ilen,BYTE* str,int bytesLen, UINT code[]);
USHORT GetHuffCode(HNODE* hnode, int ilen, CHAR c);
VOID HuffDecode(HNODE* root, int inodeLen, UINT huffCodes[], int itotalBits, BYTE decodeBuffer[]);