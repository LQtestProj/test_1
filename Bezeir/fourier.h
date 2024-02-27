#pragma once

#include "pro.h" 
#include "fourierMathTool.h"
#include "popMenu.h"
#define DELT 0.1
 


enum AnimeState {
	NORMAL, FIRST_MATCH, OVERFLOW_RANGE
};


class EpicCircle {
public:
	ComplexNum complexNum;
	int fraq;
	FType len, phase,x,y;
	VOID SetEpicInfo(FourierStruct &fs,int f) {
		len = fs.amplitude;
		phase = fs.phase;
		fraq = f;
		complexNum.real = fs.real;
		complexNum.img = fs.img;
	}
	VOID SetEpicInfo(ComplexNum& num,int f,int N) {
		complexNum = num;
		len = num.Length() /N;
		phase = atan2(num.img, num.real);
		fraq = f;
	}

};
class FourierDraw {
private:
	static FType s_epicShowPercent;
	FType m_lAnimeTime = 0; 
	int m_N = 0;
	PointPathNode* mRootPath=NULL;
	static BOOLEAN s_isHiddenEpic, s_needClear;
	static float sCos, sSin;
	static HDC s_bitDc;
	static RECT s_rt;
	static HBITMAP s_nbitmap , s_obitmap ;
	static HPEN s_LinePen;
	static HBRUSH s_nullBrush;
	static ContextMenu _popmenu;
public:
	~FourierDraw() {
		PointPathNode* node = mRootPath,*dnode;
		int i;
		EpicCircle* c;
		if (node != NULL) {
			for (; node!=NULL; dnode = node, node = node->next, delete dnode);
			for (i = 0; i < m_iEpicSize; i++) {
				c = m_epicArray[i];
				delete c;


			}
			delete[] m_epicArray;

		}
		

	};
	static VOID SetEpicShowPercent(FType per);
	static VOID EpicHidden(BOOLEAN hidden);
	static  VOID Setting(HWND&);
	static VOID EpicShowStatusChange(const HWND&);
	static VOID PopMenu(HWND&,POINTS&);
	static
		FType CalcAnimePer(FType animeTime, FType& curAnime);
	static VOID DrawFourierArray(HWND hwnd ,HDC hdc,FourierDraw** fourierArr);
	static int SortEpic(FourierStruct** fsArray, int N);
	static VOID DrawEpicCircle(HWND&, HDC& hdc, FType x, FType y,FType radius);
	VOID SkipLittleEpic(EpicCircle** comList, int N);
	VOID SortEpic(EpicCircle** comList,int epicsize);
	VOID DrawFraquencyDomain(HWND hwnd, HDC hdc);
	VOID Draw(HWND hwnd, HDC hdc, double x, double y);
VOID DrawFourier(HWND hwnd, HDC hdc, int x, int y, AnimeState rangeState);
VOID ReadEpicList(double* xn, int size); 
VOID SetEpicList(FourierStruct* fsArr, int size);
VOID SetEpicList(ComplexNum** comList, int size);
EpicCircle** m_epicArray = NULL; 
int m_sampleSize = 0,m_iEpicSize=0;
};

