#pragma once
#include "framework.h"
VOID FourierCommand(HWND& hwnd, WPARAM& param);

VOID FourierRButtonUp(HWND& hwnd, POINTS& pt);
VOID FourierTest(HWND& hwnd,char* filename="B.BMP");
VOID ReadImageComplex(HWND hwnd, WCHAR* filename);
VOID CALLBACK FourierTimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime);
VOID EndFourierTest(const HWND&);