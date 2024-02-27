#include "audioOptDlg.h"
#include "resource.h"
#include "log.h"
#include "bass_audio.h"

Dlg_AudioOpt _audioSetting;
INT_PTR CALLBACK SettingAudio(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
return	_audioSetting.Setting(hDlg, message, wParam, lParam);

}
VOID Dlg_AudioOpt::ShowDlg(HWND& hwnd,VOID* user) {
    _curClz = user;
	mHwndParent = hwnd;
	::DialogBox(hInst, MAKEINTRESOURCE(IDD_AUDIO_SETTING_DLG), hwnd,  SettingAudio);
}

INT_PTR  Dlg_AudioOpt::Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    mHwnd = hDlg;
    BassAudioPlayer* player = (BassAudioPlayer*)_curClz;
    switch (message)
    {
    case WM_INITDIALOG:
    {

        HWND hBtnPlay = ::GetDlgItem(hDlg, IDC_BTN_AUDIO_PLAY);
        if (player->IsPause()) { 
                ::SendMessage(hBtnPlay, WM_SETTEXT, 0,(LPARAM)L"²¥·Å"); 
        }
        else { 
            ::SendMessage(hBtnPlay, WM_SETTEXT, 0, (LPARAM)L"ÔÝÍ£");
        }
     
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BTN_AUDIO_PLAY) {
            HWND hBtnPlay = ::GetDlgItem(hDlg, IDC_BTN_AUDIO_PLAY);


            if (!player->IsPause()) {
                player->Pause();
                ::SendMessage(hBtnPlay, WM_SETTEXT, 0, (LPARAM)L"²¥·Å");
            }
            else {
                player->Resume();
                ::SendMessage(hBtnPlay, WM_SETTEXT, 0, (LPARAM)L"ÔÝÍ£");
            } 
        }
        else  if (LOWORD(wParam) == IDCANCEL_AUDIO)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;

}



