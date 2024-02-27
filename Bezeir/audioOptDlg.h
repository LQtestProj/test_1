#pragma once
#include "framework.h"




class Dlg_AudioOpt {

public:
	HWND mHwnd, mHwndParent;
	VOID* _curClz;
	VOID ShowDlg(HWND& hwnd, VOID* user);

	INT_PTR  Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

};

extern Dlg_AudioOpt _audioSetting;