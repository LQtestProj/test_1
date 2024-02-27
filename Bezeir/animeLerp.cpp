#include "animeLerp.h"
#include "toolutil.h"
#include "fourierMathTool.h"
#include "fourier.h"
#include "log.h"



HBITMAP BezeirDraw::s_nbitmap = NULL;
HBITMAP BezeirDraw::s_obitmap = NULL;
RECT BezeirDraw::s_rt;
HDC BezeirDraw::s_bitDc;
HPEN BezeirDraw::s_LinePen;
HBRUSH BezeirDraw::s_circleBrush = NULL;
int BezeirDraw::s_Radius = 12;
long AnimeLerpArray::MAX_ANIME_LOOPTIME = 2000; 
VOID AnimeLerp::Draw(HWND hwnd, HDC hdc) {

	MoveToEx(hdc, pt1.x, pt1.y, NULL);
	LineTo(hdc, pt2.x, pt2.y);
}
VOID AnimeLerp::DrawLineAnime(HWND hwnd, HDC hdc) {

	t = CalcAnimePer(MAX_ANIMELOOPTIME);
	if (t >= 1) {
		t = 1;
	}
	int x =FourierMathTool::  Lerp(pt1.x, pt2.x, t);
	int y = FourierMathTool::Lerp(pt1.y, pt2.y, t);
	TestPoint nPt = { x,y };
	 
	DrawPoint(hwnd, hdc, nPt);
	MoveToEx(hdc, pt1.x, pt1.y, NULL);
	LineTo(hdc, nPt.x, nPt.y);

}
VOID AnimeLerpArray::ClearArray(BOOLEAN needClearMem) {

	for (int i = 0; i < this->m_iSize; i++) { 
		delete this->m_array[i];
		this->m_array[i] = NULL;
	} 
	for (int i = 0; i < this->m_iPtSize; i++) { 
		delete this->m_pointArray[i];
		this->m_pointArray[i] = NULL;
	}
	if (needClearMem) {
		delete[] this->m_pointArray;
		this->m_pointArray = NULL;
		delete[] this->m_array;
		this->m_array = NULL;
	}
	this->m_iSize = 0;
	m_iPtSize = 0;
}
FType AnimeLerp::CalcAnimePer(const FType& animeTime)
{
	long subtime;
	FType subHeightPer, y, x;
	if (this->m_lAnimeStartTime == 0) {
		m_lAnimeStartTime = ::GetTickCount();
	}
	subtime = ::GetTickCount() - m_lAnimeStartTime;
	//m_lAnimetime = ::GetTickCount();
	subHeightPer = (subtime) / animeTime;
	if (subHeightPer >= 1) {
		subHeightPer = 1;
		m_lAnimeStartTime = 0;
	}
	return subHeightPer;
}

AnimeLerpArray::AnimeLerpArray(int size) {

	this->m_array = new AnimeLerp*[size];
	this->m_pointArray = new TestPoint * [size];
	for (int i = 0; i < size; i++) {
		this->m_array[i] = NULL;
		this->m_pointArray[i] = NULL;
	}

}
VOID AnimeLerpArray::AddPoint(TestPoint pt1) {
	this->m_pointArray[m_iPtSize++] = new TestPoint(pt1);
}
VOID AnimeLerpArray::Add(TestPoint pt1, TestPoint pt2) {

	this->m_array[m_iSize++] = new AnimeLerp(pt1, pt2);

}
int r = 0;
VOID AnimeLerpArray::DrawBezeirDot(HWND& hwnd, HDC& hdc, TestPoint& pt) {
	COLORREF clr = RGB(r+=10, 255, 12);
	::DrawPixel(hwnd, hdc, pt, clr);

}


VOID AnimeLerpArray::DrawDot(HWND hwnd, HDC hdc) {
	TestPoint*curPt = m_pointArray[0],*prePt=NULL;
	 
	FType &animeTime = this->m_lAnimeTime;
	FType animePer =FourierDraw::CalcAnimePer(AnimeLerpArray::MAX_ANIME_LOOPTIME, this->m_lAnimeTime);
	 
	int size = animePer * m_iPtSize; 
	HPEN oldpen, npen = ::CreatePen(PS_DASH, 1, RGB(255,66, 0));
	oldpen = (HPEN)::SelectObject(hdc, npen);
	for (int i = 0; i < size; i++) { 
		curPt = m_pointArray[i]; 
		if (prePt == NULL) {
			prePt = curPt;
			::MoveToEx(hdc, curPt->x, curPt->y, NULL);
			continue;
		}
		::LineTo(hdc, curPt->x, curPt->y); 
	}
	::SelectObject(hdc, oldpen);
	::DeleteObject(npen); 
}
VOID AnimeLerpArray::Draw(HWND hwnd, HDC hdc) {
	AnimeLerp* noDraw = NULL,*curDraw=NULL;
	
	curDraw = m_array[0];
	 DrawPoint(hwnd, hdc, curDraw->pt1);
	FType animeTime = this->m_lAnimeTime;
	FType animePer =FourierDraw:: CalcAnimePer(MAX_ANIMELOOPTIME, animeTime);
	this->m_lAnimeTime = animeTime;
	int size = animePer * m_iSize;

	for (int i = 0; i < size; i++) {
		curDraw = m_array[i]; 
			curDraw->Draw(hwnd, hdc);
	}

	DrawPoint(hwnd, hdc, curDraw->pt2);

}
BezeirDraw::BezeirDraw() :BezeirDraw(NULL,MAX_ANIME_SIZE) {

}
BezeirDraw::BezeirDraw(PointNode* nList, int animeSize):mMenu(BEZEIER_POPMENU) {
	 
	m_ptNodeList = nList;

	if (nList != NULL) {

		CalcPoint();
	}

}

VOID BezeirDraw::DrawNodePoint(HWND& hwnd, HDC& hdc,TestPoint &pt,int ptIndex) {

	if (s_circleBrush == NULL) {

		LOGBRUSH lb;
		lb.lbStyle = BS_SOLID;
		lb.lbColor = RGB(122, 33, 11);
		lb.lbHatch = HS_BDIAGONAL;
		s_circleBrush = CreateBrushIndirect(&lb);
	}
	HBRUSH oldBrush;
	if (ptIndex == mSelectedPoint) {
		HBRUSH selBrush; 
		LOGBRUSH lb;
		lb.lbStyle = BS_SOLID;
		lb.lbColor = RGB(233, 00, 11);
		lb.lbHatch = HS_BDIAGONAL;;
		selBrush = CreateBrushIndirect(&lb); 
		oldBrush = (HBRUSH)::SelectObject(hdc, selBrush);
		DrawCircle(hwnd, hdc, pt.x, pt.y, s_Radius);
		::DeleteObject(selBrush);

	}
	else { 
		oldBrush = (HBRUSH)::SelectObject(hdc, s_circleBrush);
		DrawCircle(hwnd, hdc, pt.x, pt.y, s_Radius);
	}

	::SelectObject(hdc, oldBrush);
}
VOID BezeirDraw::Draw(HWND hwnd, HDC hdc) {

	if (BezeirDraw::s_nbitmap == NULL) {
		BezeirDraw::s_bitDc = CreateCompatibleDC(hdc);
		BezeirDraw::s_LinePen = (HPEN)::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		::GetClientRect(hwnd, &BezeirDraw::s_rt);
		BezeirDraw::s_nbitmap = CreateCompatibleBitmap(hdc, BezeirDraw::s_rt.right - BezeirDraw::s_rt.left,
			BezeirDraw::s_rt.bottom - BezeirDraw::s_rt.top);//hdc 
	}

	BezeirDraw::s_obitmap = (HBITMAP)::SelectObject(BezeirDraw::s_bitDc, BezeirDraw::s_nbitmap);
	::FillRect(BezeirDraw::s_bitDc, &BezeirDraw::s_rt, (HBRUSH)::GetStockObject(WHITE_BRUSH));
	HDC& bitDc = BezeirDraw::s_bitDc;
	HBITMAP& hbitmap = BezeirDraw::s_obitmap;
	int w = BezeirDraw::s_rt.right - BezeirDraw::s_rt.left, h = BezeirDraw::s_rt.bottom - BezeirDraw::s_rt.top;

	if (m_ptNodeList == NULL) {
		BitBlt(hdc, 0, 0, w, h, bitDc, 0, 0, SRCCOPY);
		SelectObject(bitDc, hbitmap);
		return;
	}
	int size = m_ptNodeList->Size(),i; 
	TestPoint  *prePt=NULL, *curPt= NULL;
	for (i = 0; i < size;  DrawNodePoint(hwnd, bitDc,* curPt,i), i++) {
		curPt = &(m_ptNodeList->GetPoint(i));
		if (prePt == NULL) {
			prePt = curPt;
			continue;
		}
		DrawLine(hwnd, bitDc, *prePt,*curPt);
		prePt = curPt;
	} 
	if (size < 3) {


	}else{ 
		m_animeArr.DrawDot(hwnd, bitDc);
	}



	BitBlt(hdc, 0, 0, w, h, bitDc, 0, 0, SRCCOPY);
	SelectObject(bitDc, hbitmap);
}
VOID BezeirDraw::RecalPoint(TestPoint& pt1, TestPoint& pt2, double t, TestPoint& pt) {
	int x1 =FourierMathTool:: Lerp(pt1.x, pt2.x, t);
	int y1 = FourierMathTool::Lerp(pt1.y, pt2.y, t);
	pt.x = x1; pt.y = y1;

}

VOID BezeirDraw::LoopCalcPoint(PointNode* ptNodeList, double t) {


	TestPoint prePt(ptNodeList->GetPoint(0));
	TestPoint npt = {};

	int nodeSize = ptNodeList->Size() - 1;
	PointNode* newNode = NULL, * rootNode = new PointNode;
	for (int i = 0; i < nodeSize; i++) {
		TestPoint& pt1 = ptNodeList->GetPoint(i), & pt2 = ptNodeList->GetPoint(i + 1);
		RecalPoint(pt1, pt2, t, npt);

		rootNode->AddPoint(npt);
	}
	if (rootNode->Size() != 1) {
		LoopCalcPoint(rootNode, t);
	}
	else {
		m_animeArr.AddPoint(rootNode->pt);
	}
	delete rootNode;
}
VOID BezeirDraw::CalcPoint() { 
	m_animeArr.ClearArray();
	for (double t = STEP; t <= 1; t += STEP) { 
		LoopCalcPoint(m_ptNodeList, t); 
	}

}

int BezeirDraw::PointHitTest(POINTS& pt) {
	if (m_ptNodeList == NULL)return -1;
	int i,len=m_ptNodeList->Size(),radDb= s_Radius * s_Radius,cRes,x,y;
	
	
	for (i = 0; i < len; i++) {
		TestPoint& tpt=m_ptNodeList->GetPoint(i);
		x = pt.x - tpt.x;
		y = pt.y - tpt.y;
		cRes = x * x + y * y;
		if (cRes < radDb) {
			_log.Trace("hit point:%d\n", i);
			return i;
		}

	}

	return -1;
}

VOID  BezeirDraw::DeleteSelectedPoint() {
	if (m_ptNodeList == NULL) {
		return;
	}
	int size = m_ptNodeList->Size();
	if (size == 1) {
		delete m_ptNodeList;
		m_ptNodeList = NULL;
	}
	else {
		m_ptNodeList->DeleteNode(mSelectedPoint);
		CalcPoint();
	}
	mSelectedPoint = -1;
}
VOID  BezeirDraw::PopMenu(HWND& hwnd, POINTS& pt) {
	mSelectedPoint= this->PointHitTest(pt);
	if (mSelectedPoint != -1) {
		mMenu.PopMenu(hwnd, pt);
	}

}
VOID BezeirDraw::MouseDown(POINTS& pt) { 
	this->mButtonDown = TRUE; 
	mSelectedPoint = PointHitTest(pt);
}
VOID BezeirDraw::MouseUp(POINTS& pt) {
	if (this->mButtonDown) {
		if (mSelectedPoint == -1) {
			if (m_ptNodeList == NULL) {
				m_ptNodeList = new PointNode;
			} 
			m_ptNodeList->AddPoint(TestPoint{ pt.x,pt.y });

		}
		else {

		}
		CalcPoint();
		mSelectedPoint = -1;
	}

	this->mButtonDown = FALSE;

}
VOID BezeirDraw::MouseMove(POINTS& pt) {
	if (m_ptNodeList == NULL) {
		return;
	}
	if (this->mButtonDown) {
		if (mSelectedPoint != -1) {
			Point pt2 = { pt.x,pt.y }; 
			m_ptNodeList->GetPoint(mSelectedPoint).x = pt.x;
			m_ptNodeList->GetPoint(mSelectedPoint).y = pt.y;

			CalcPoint();
		} 
	}

}