#include "Program.h"

#define MAX_TEAR 20
TearNode* _tearNode = NULL;
long m_lAnimetime = 0;
double m_dbMaxAnimeStamp = 2000;
int _tearCount = 0;
void InitParam(HWND& hwnd) {
	SetTimer(hwnd, ID_TIMER, ELAPSE, TimerProc);

}


VOID TearDrop(HWND hwnd, HDC hdc) {
	TearNode* node = _tearNode;
	Tear* tear;
	if (node == NULL || node->tear == NULL) {
		return;
	}
	do {

		node->tear->Draw(hwnd, hdc);
	} while ((node = node->next) != NULL);

}
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime) {

	Tear* newTear;
	if (_tearNode == NULL) {

		_tearNode = new TearNode;
		newTear = new Tear;
		newTear->x = rand() % WIN_WIDTH;
		_tearNode->tear = newTear;

		m_lAnimetime = ::GetTickCount();
	}
	else {
		if (_tearCount < MAX_TEAR) {
			long subtime = ::GetTickCount() - m_lAnimetime;
			TearNode* newHead;
			if (subtime > 500) {
				m_lAnimetime = ::GetTickCount();
				newHead = new TearNode;
				newTear = new Tear;
				newTear->x = rand() % WIN_WIDTH;
				newHead->next = _tearNode;
				newHead->tear = newTear;
				_tearNode = newHead;
				++_tearCount;
			}
		}
	}
	InvalidateRect(hwnd, NULL, TRUE);
}
void Ellipse(HWND& hWnd, HDC& hDC)//椭圆  
{
	HBRUSH  newBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hDC, newBrush);
	DeleteObject(newBrush);
	ReleaseDC(hWnd, hDC);
}

void TRACE(LPCTSTR lpszFmt, ...)
{
	va_list args;
	va_start(args, lpszFmt);
	int len = _vsctprintf(lpszFmt, args) + 1;
	TCHAR* lpszBuf = (TCHAR*)_alloca(len * sizeof(TCHAR));//栈中分配, 不需要释放
	_vstprintf_s(lpszBuf, len, lpszFmt, args);
	va_end(args);
	OutputDebugString(lpszBuf);
}








