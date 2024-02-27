#pragma once
#include "framework.h"




INT_PTR CALLBACK Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

class Dlg_FourierOpt {
public :
	HWND mHwnd, mHwndParent;
	LONG mCurSliderValue = 100;
	BOOLEAN mHiddenEpic = FALSE;
	VOID ShowDlg(HWND& hwnd);
	INT_PTR  Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam); 
};

extern Dlg_FourierOpt _fourierSetting;
