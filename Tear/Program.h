#pragma once

#include "framework.h"
#define ID_TIMER 5
#define FPS 60
#define ELAPSE (1/FPS)*1000
#define PLANT_HEIGHT WIN_HEIGHT-150
#define MAX_ANIMELOOP 2000.0
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime);
void Pix(HWND& hWnd, HDC& hDC);
VOID InitParam(HWND& hwnd);
void Ellipse(HWND& hWnd, HDC& hDC);//Õ÷‘≤ 
void TRACE(LPCTSTR lpszFmt, ...);
VOID TearDrop(HWND hwnd, HDC hdc);
class Tear
{
	
private:
	HBRUSH  m_brush;
	HPEN m_pen;
	BYTE r,g, b;
	BOOLEAN m_isOnPlant = FALSE;
	BOOLEAN m_isQueOver = TRUE;
	VOID Drop(HWND hwnd, HDC hdc);
	VOID DrawDrop(HWND hwnd, HDC hdc);
	VOID Rippling(HWND hwnd, HDC hdc);
	double& CalcAnimePer(double animeTime);
public: 
	Tear();
	int x = 0, y = 0, weight = 10, radius = 10, ripplingUp=0, ripplingDown=0,maxRipplingUp,maxRipplingDown;
	int m_iPlantHeight;
	long m_lAnimeStartTime = 0;
	VOID Draw(HWND hwnd, HDC hdc); 
	static
		HBRUSH s_defBrush;
	
};
struct TearNode {
	TearNode* next=NULL;
	Tear* tear = NULL;


};
