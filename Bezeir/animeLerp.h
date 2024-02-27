#pragma once
#include "pro.h" 
#include "popMenu.h"
class AnimeLerp {
private:
	long m_lAnimeStartTime=0;
public:
	AnimeLerp(TestPoint p1, TestPoint p2) :pt1(p1), pt2(p2) {}
	TestPoint pt1, pt2;
	double t=0;
	VOID DrawLineAnime(HWND hwnd, HDC hdc);
	VOID Draw(HWND hwnd, HDC hdc);
	FType CalcAnimePer(const FType& animeTime);

};

class AnimeLerpArray {
private:
	static long MAX_ANIME_LOOPTIME;
	int m_iSize=0,m_iPtSize=0;
	AnimeLerp** m_array;
	TestPoint** m_pointArray;
	FType m_lAnimeTime=0;
	VOID DrawBezeirDot(HWND& hwnd, HDC& hdc, TestPoint& pt);
public:
	~AnimeLerpArray() {
		ClearArray(TRUE);
	}
	AnimeLerpArray(int size= MAX_ANIME_SIZE);
	VOID Add(TestPoint pt1, TestPoint pt2);
	VOID AddPoint(TestPoint pt1);
	VOID Draw(HWND hwnd, HDC hdc);
	VOID DrawDot(HWND hwnd, HDC hdc);
	VOID ClearArray(BOOLEAN needClearMem=FALSE);
};



class BezeirDraw {
private:
	AnimeLerpArray  m_animeArr ; 
	BOOLEAN mButtonDown;
	PointNode* m_ptNodeList=NULL;
	  VOID DrawNodePoint(HWND&, HDC&, TestPoint& pt, int ptIndex);
	static HBRUSH s_circleBrush;
	static int s_Radius;
	int mSelectedPoint = -1;
	int PointHitTest(POINTS& pt);
	ContextMenu mMenu ;
public:
	~BezeirDraw() {
		if (m_ptNodeList != NULL) {
			delete m_ptNodeList; 
		}
	}
	static HDC s_bitDc;
	static RECT s_rt;
	static HBITMAP s_nbitmap, s_obitmap;
	static HPEN s_LinePen;
	BezeirDraw();
	BezeirDraw(PointNode* nList,int animeSize);
	VOID	RecalPoint(TestPoint& pt1, TestPoint& pt2, double t, TestPoint& pt);
	VOID	LoopCalcPoint(PointNode* ptNodeList, double t);
	VOID	CalcPoint( );
	VOID Draw(HWND hwnd, HDC hdc);
	VOID MouseMove(POINTS& pt);
	VOID MouseDown(POINTS& pt);
	VOID MouseUp(POINTS& pt); 
	VOID PopMenu(HWND&, POINTS& pt);
	VOID DeleteSelectedPoint();
};


