
#include "toolutil.h"

#include "log.h"
#include "imageContoursTool.h"
#include "fourierMathTool.h"
#include "fourier.h"
int _pointCount = 0;
Point** _bitmapPt;


Point*** _testPt;
int _timerIndex = 0, _nextindex = 1, _drawIndex[999] = { 0 };


VOID ReadBitmapPix(HWND hwnd, BITMAP& bitmap, HBITMAP& hbitmap, UCHAR* pBits, Point**** longtmpPt) {


	int bIndex = 0, iBlack = 0x00, iWhite = 0xff;
	BOOLEAN isIn = FALSE, isOut = FALSE;
	unsigned char t, r, g, b;
	int step = bitmap.bmBitsPixel >> 3, xx = 0,
		ptSize = bitmap.bmHeight * bitmap.bmWidth + 1;
	*longtmpPt = new Point * *[bitmap.bmHeight];
	if (step == 0) {
		step = 1;
	}
	//读取点
	for (int i = 0; i < bitmap.bmHeight; i++) {
		(*longtmpPt)[i] = new Point * [bitmap.bmWidth];
		for (int j = 0; j < bitmap.bmWidth; j++) {
			xx = bitmap.bmWidthBytes * i + j * step;
			t = pBits[xx + 3]; r = pBits[xx + 2]; g = pBits[xx + 1]; b = pBits[xx + 0];
			(*longtmpPt)[i][j] = new Point;
			(*longtmpPt)[i][j]->x = j;
			(*longtmpPt)[i][j]->y = i;
			(*longtmpPt)[i][j]->flag = 0;
			(*longtmpPt)[i][j]->clr = (t << 24) | (r << 16) | (g << 8) | b;
		}
		isIn = FALSE;
		isOut = FALSE;
	}

}
int CalcGroupSize(GroupNode* groupRoot) {
	GroupNode* node = groupRoot;
	if (node == NULL)return 0;
	int size = 1;
	while ((node = node->next) != NULL) {
		++size;
	}
	return size;
}
int CalcPointNodeSize(ImagePointNode* pointRoot) {
	ImagePointNode* node = pointRoot;
	if (node == NULL)return 0;
	int size = 1;
	while ((node = node->nextNode) != NULL) {
		++size;
	}
	return size;
}


UINT NumBitReflex(UINT num, int bitlen) {
	UINT res = 0, tmp;
	int i, j, mask = 0x1, shift;
	for (i = 0; i < bitlen; i++) {
		tmp = num & mask;
		shift = (bitlen - (i << 1) - 1);
		if (shift >= 0) {
			tmp = tmp << shift;
		}
		else {
			tmp = tmp >> (-shift);
		}
		res |= tmp;
		mask = mask << 1;
	}
	return res;
}
VOID DeleteCache(VOID** dest, int len) {
	VOID* d = NULL;
	for (int i = 0; i < len; i++) {
		d = dest[i];
		try {
			delete d;
		}
		catch (...) {
			_log.Trace(L"delete err");
			break;
		}
	}
	delete[] dest;
}



VOID SwapString(CHAR* outstr, CHAR* str, int len) {
	int i, j;
	j = len >> 1;
	CHAR c;
	for (i = 0; i < len; i++) {
		c = str[i];
		outstr[len - i - 1] = c;
		/*
				outstr[i] = outstr[len - i - 1];

		*/

	}

}
VOID NumToBits(USHORT num, int bitlen, CHAR* outbits) {
	int i, j, bitmask = 0x1;
	USHORT us;
	for (i = 0; i < bitlen; i++) {
		bitmask = bitlen - i - 1;
		us = num & (1 << bitmask);
		us = us >> bitmask;
		outbits[i] = (us + 48);
	}



}



VOID DrawLine(HWND hwnd, HDC hdc, int x1, int y1, int x2, int y2) {

	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);

}

HBRUSH _brush = NULL, _nullBrush = NULL;


VOID DrawCircle(HWND hwnd, HDC hdc, int x, int y, FType radius) {

	int top = y - radius, right = x + radius, bottom = y + radius, left = x - radius;
	::Ellipse(hdc, left, top, right, bottom);
}


VOID DrawPoint(HWND hwnd, HDC hdc, TestPoint& pt) {
	if (_brush == NULL) {
		_brush = CreateSolidBrush(RGB(255, 0, 0));
	}
	HBRUSH  oldBrush;
	int r = 2;
	int left = pt.x - r, right = pt.x + r, top = pt.y - r, bottom = pt.y + r;
	oldBrush = (HBRUSH) ::SelectObject(hdc, _brush);

	//::SetPixel(hdc, pt.x, pt.y, RGB(255, 0, 0));

	Ellipse(hdc, left, top, right, bottom);
	SelectObject(hdc, oldBrush);

}

VOID DrawPixel(HWND hwnd, HDC hdc, TestPoint& pt, COLORREF  clr) {

	::SetPixel(hdc, pt.x, pt.y, (COLORREF)clr);
}

VOID DrawLine(HWND hwnd, HDC hdc, TestPoint& pt1, TestPoint& pt2) {

	MoveToEx(hdc, pt1.x, pt1.y, NULL);
	LineTo(hdc, pt2.x, pt2.y);
}



int GetWidth(RECT& rt) {
	return rt.right - rt.left;
}
int GetHeight(RECT& rt) {
	return rt.bottom - rt.top;
}


int Add(int num1, int num2) {
	int res, s, t1, t2;
	res = (num1 ^ num2);
	s = ((num1 & num2) << 1);
	while (s != 0) {
		t1 = res;
		t2 = s;
		res = (t1 ^ t2);
		s = (t1 & t2) << 1;;
	}
	return res;
}

double FloatCeil(double num, int ceilsize, int nsize) {
	ULONGLONG lnum = *(ULONGLONG*)&num, bnum, res, expont, bitmask, fraction, tmp, maxfrac, denominator, numerator;
	int dwbitssize = sizeof(ULONGLONG) << 3, i, j, expsub = 1023, maxfracpos;
	BYTE sign = lnum & (1 << (dwbitssize - 1)), bit;
	int esize = 11, signIndex = esize + 1, msize = 52;
	double dbfrac, dbexp, dbres;
	bnum = lnum;
	expont = 0;
	bnum = bnum >> msize;
	expont = 0x7FF0000000000000LL & lnum;
	sign = 0x8000000000000000LL & lnum;
	fraction = 0xFFFFFFFFFFFFFLL & lnum;
	expsub = (expont >> msize) - expsub;

	maxfracpos = 0;
	if (fraction != 0) {
		maxfrac = fraction & 0xFFFFFF;
		if (maxfrac == 0) {
			maxfracpos += 6 * 4;
			maxfrac = fraction >> maxfracpos;
		}
		maxfrac = (fraction >> maxfracpos) & 0xFFF;
		if (maxfrac == 0) {
			maxfracpos += 3 * 4;
			maxfrac = fraction >> maxfracpos;
		}
		maxfrac = (fraction >> maxfracpos) & 0xFF;
		if (maxfrac == 0) {
			maxfracpos += 2 * 4;
			maxfrac = fraction >> maxfracpos;
		}
		maxfrac = (fraction >> maxfracpos) & 0xF;
		if (maxfrac == 0) {
			maxfracpos += 1 * 4;
			maxfrac = fraction >> maxfracpos;
		}

		if ((maxfrac & 0x3) == 0) {
			maxfracpos += 2;
			maxfrac = maxfrac >> maxfracpos;
		}
		if ((maxfrac & 0x1) == 0) {
			maxfracpos += 1;
			//maxfrac = maxfrac >> maxfracpos;
		}
		maxfracpos = msize - maxfracpos;
	}
	denominator = 0;


	if (expsub > maxfracpos) {
		dbexp = 0;
		for (i = 0; i < maxfracpos; i++) {
			bit = ((1LL << (msize - i - 1)) & lnum) >> (msize - i - 1);
			if (bit == 1) {
				dbfrac = 1;
				j = (maxfracpos - i - 1 + (expsub - maxfracpos));
				for (; j > 0; j--) {
					dbfrac *= 2;
				}
				dbexp += dbfrac;
			}

		}
		dbfrac = 1;
		for (j = 0; j < expsub; j++) {
			dbfrac *= 2;
		}
		dbexp += dbfrac;

		dbres = (sign == 0 ? 1 : -1) * 1 * dbexp;
	}
	else {
		for (i = 0; i < maxfracpos; i++) {
			bit = ((1LL << (msize - i - 1)) & lnum) >> (msize - i - 1);
			denominator += (bit * (1LL << (maxfracpos - i - 1)));
		}
		denominator += (1LL << maxfracpos);
		numerator = 1LL << maxfracpos;
		dbfrac = denominator * 1.0 / numerator;

		dbexp = 1LL << expsub;

		dbres = (sign == 0 ? 1 : -1) * dbexp * dbfrac;
	}

	tmp |= sign << (signIndex - 1);
	tmp = tmp << (msize);
	return *(double*)&tmp;
}
VOID FloatToBits(double num, int ibitssize, CHAR* outbuffer) {
	ULONG lnum = *(ULONG*)&num;
	int dwbitssize = sizeof(ULONG) << 3;
	BYTE sign = lnum & (1 << (dwbitssize - 1));



}
