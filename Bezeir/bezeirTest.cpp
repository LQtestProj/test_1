#include "bezeirTest.h"
#include "pro.h"
#include "animeLerp.h"
#include "popMenu.h"
#define ID_BEZEIER_TIMER 40001
BezeirDraw* _bezeir=NULL; 
VOID InitBezeirPoint(HWND hwnd) {
	_bezeir = new BezeirDraw(NULL, MAX_ANIME_SIZE); 
}
VOID BezeirTest(HWND hwnd) {

	HMENU hmenu = ::GetMenu(hwnd);
	::EnableMenuItem(hmenu, ID_OPENIMAGEFILE, MF_GRAYED);
	::EnableMenuItem(hmenu, ID_OPEN_AUDIOFILE, MF_GRAYED);
	InitBezeirPoint(hwnd);

	SetTimer(hwnd, ID_BEZEIER_TIMER, 10, BezeirTimerProc);
}
VOID EndBezeirTest(const HWND& hwnd) {
	if (_bezeir != NULL) {
		KillTimer(hwnd, ID_BEZEIER_TIMER);
		delete _bezeir;
		_bezeir = NULL; 
	}
}
VOID CALLBACK BezeirTimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime) {
	HDC hdc = ::GetDC(hwnd); 
	 _bezeir->Draw(hwnd, hdc);

	::ReleaseDC(hwnd, hdc);
}
VOID BezeirRButtonUp(HWND& hwnd, POINTS& pt) {
	if (_bezeir != NULL) {

		_bezeir->PopMenu(hwnd, pt);
	}
}

VOID BezeirCommand(WPARAM& param) {
	if (_bezeir != NULL) {
		switch (LOWORD(param))
		{
		case ITEM_MENU_DELETE_POINT:
		{
			_bezeir->DeleteSelectedPoint();
			break;
		}
		}

	}
}

VOID BezeirMouseButtonDown(HWND& hwnd, POINTS& pt) {
	if (_bezeir != NULL) {
		_bezeir->MouseDown(pt);
	} 
}
VOID BezeirMouseButtonUp(HWND& hwnd, POINTS& pt) {
	if (_bezeir != NULL) {
		_bezeir->MouseUp(pt);
	} 
}
VOID BezeirMouseMove (POINTS& pt) {
	if (_bezeir != NULL) {
		_bezeir->MouseMove(pt);
	}
}

