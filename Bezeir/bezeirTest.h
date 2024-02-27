#pragma once
#include "framework.h"
#include "Bezeir.h"

VOID CALLBACK BezeirTimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime);
VOID BezeirTest(HWND hwnd);
VOID InitBezeirPoint(HWND hwnd);
VOID EndBezeirTest(const HWND& hwnd);
VOID BezeirCommand(WPARAM&);
VOID BezeirMouseMove(POINTS& pt);
VOID BezeirRButtonUp(HWND& hwnd, POINTS& pt);
VOID BezeirMouseButtonUp(HWND& hwnd, POINTS& pt);
VOID BezeirMouseButtonDown(HWND& hwnd, POINTS& pt);