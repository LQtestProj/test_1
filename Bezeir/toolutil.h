#pragma once

#include "pro.h" 
 
  struct ImagePointNode;
  struct GroupNode;


struct ImagePointNode {
	ImagePointNode* preNode, * nextNode;
	Point* point;
};
struct GroupNode {
	GroupNode* next;
	ImagePointNode* ptGroup;
	int ptSize;
};

 
UINT NumBitReflex(UINT num, int bitlen);
VOID NumToBits(USHORT num, int bitlen, CHAR * outbits);
int Char2Wchar(wchar_t* wcharStr, const char* charStr); 
double FloatCeil(double num,int ceilsize, int nsize);
VOID FloatToBits(double num, int nsize,CHAR* outbuffer);

ComplexNum** ReadBitmapPoint(HWND hwnd, int& outPtSize);
VOID ReadBitmapPix(HWND hwnd, BITMAP& bitmap,HBITMAP &hbitmap,  UCHAR* longBits, Point**** longtmpPt);

 
VOID DeleteCache(VOID** dest, int len); 
VOID SwapString(CHAR * outstr, CHAR * str, int len);

VOID DrawLine(HWND hwnd, HDC hdc, TestPoint& pt1, TestPoint& pt2);
VOID DrawPixel(HWND hwnd, HDC hdc, TestPoint& pt, COLORREF  clr=0);
VOID DrawPoint(HWND hwnd, HDC hdc, TestPoint& pt);
VOID DrawCircle(HWND hwnd, HDC hdc, int x, int y, FType radius);
VOID DrawLine(HWND hwnd, HDC hdc, int x1, int y1, int x2, int y2);

int Add(int num1, int num2);
int GetWidth(RECT& rt);
int GetHeight(RECT& rt);

