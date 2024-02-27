#include "fourierOptDlg.h"
#include "resource.h"
#include "log.h"
#include "fourier.h"
#ifndef GWL_WNDPROC 
#define GWL_WNDPROC GWLP_WNDPROC
#endif


Dlg_FourierOpt _fourierSetting;

typedef VOID (*wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam));
WNDPROC  SWnd,CWnd;
VOID Dlg_FourierOpt::ShowDlg(HWND& hwnd) {
	mHwndParent = hwnd; 
	::DialogBox(hInst, MAKEINTRESOURCE(IDD_FOURIER_SETTING), hwnd, ::Setting);

}

INT_PTR CALLBACK Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) { 
	return _fourierSetting.Setting(hDlg, message, wParam, lParam); 
}
int _msgindex = 0;
LRESULT CALLBACK CheckWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_LBUTTONUP:
    { 
        _fourierSetting.mHiddenEpic=!::SendMessage(hWnd, BM_GETCHECK, 0, 0)== BST_CHECKED?TRUE:FALSE;
         FourierDraw::EpicHidden(_fourierSetting.mHiddenEpic);
        break;
    }
    }


     return ::CallWindowProc(CWnd, hWnd, message, wParam, lParam);
}
    LRESULT CALLBACK SliderWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_LBUTTONUP:
    { 
        LONG value = ::SendMessage(hWnd, TBM_GETPOS, 0, 0);
        _fourierSetting.mCurSliderValue = value;
        FourierDraw::SetEpicShowPercent(_fourierSetting.mCurSliderValue / 100.0);
        break;
    }
    case TBM_GETPOS:
         break;
    }
     return ::CallWindowProc(SWnd, hWnd, message, wParam, lParam);
}
INT_PTR Dlg_FourierOpt::Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	mHwnd = hDlg;

    switch (message)
    {
    case WM_VSCROLL:
    case WM_HSCROLL: 
    {
    
        int nScrollCode = (int)LOWORD(wParam);
        HWND hwndScrollBar = (HWND)lParam;
        int pos = SendMessage(hwndScrollBar, TBM_GETPOS, 0, 0);
        switch (nScrollCode) {
        case SB_THUMBTRACK: {
            _log.Trace("scrolling_%d\n",pos);
        }
                          break;
        case SB_ENDSCROLL: 
            _fourierSetting.mCurSliderValue = pos;
            FourierDraw::SetEpicShowPercent(_fourierSetting.mCurSliderValue / 100.0);
            _log.Trace("end scroll_%d\n", pos);
            break;
        }
    }
 

        break;
    case WM_INITDIALOG:
    {
        HWND hSlider = ::GetDlgItem(hDlg, IDC_SLIDER_EPIC_SHOW);
        HWND hStatus = ::GetDlgItem(hDlg, IDC_CHECK_EPIC_STATUS);
        LONG value = ::SendMessage(hSlider, TBM_SETPOS, 1, mCurSliderValue);
        int status = mHiddenEpic? BST_CHECKED:BST_UNCHECKED;
        ::CheckDlgButton(hDlg, IDC_CHECK_EPIC_STATUS, status);
         CWnd = (WNDPROC)::SetWindowLong(hStatus, GWL_WNDPROC, (long)CheckWndProc);
        
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOKSETTING) {
            HWND hSlider = ::GetDlgItem(hDlg, IDC_SLIDER_EPIC_SHOW);
            HWND hStatus = ::GetDlgItem(hDlg, IDC_CHECK_EPIC_STATUS);
            LONG value = ::SendMessage(hSlider, TBM_GETPOS, 0, 0);
            mCurSliderValue = value;
            mHiddenEpic=::IsDlgButtonChecked(hDlg, IDC_CHECK_EPIC_STATUS) == BST_CHECKED ? TRUE : FALSE;
            EndDialog(hDlg, LOWORD(wParam));

        }else  if (LOWORD(wParam) == IDCANCELSETTING)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;




}
