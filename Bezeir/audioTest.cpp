#include "audioTest.h"
#include "bass_audio.h"
#include "toolutil.h"
#include "log.h"
#include "resource.h"
#include "audioOptDlg.h"
#define ID_TIMER_AUDIOWAVE 100

BassAudioPlayer* _player=NULL;
BOOLEAN _isCurrentTest = FALSE;
const DWORD DELAY_PLAY = 30* 1000;
const int MAX_PLAYRAND = 3;
DWORD _delayReplayTimer = 0;
int _playtime = 0,_rdnPlayTimeCount=0;
VOID CALLBACK Callback(VOID* user) {
	BassAudioPlayer* player = (BassAudioPlayer*)user;

}
VOID RBUTTONUP_AUDIO(HWND& hwnd, POINTS& pt) {
	if (_isCurrentTest) {
		_player->PopMenu(hwnd, pt);
	}
}

VOID AudioTestMain(HWND hwnd,CHAR* filename) {
	HMENU hmenu = ::GetMenu(hwnd); 
	::EnableMenuItem(hmenu, ID_OPENIMAGEFILE, MF_GRAYED);
	::EnableMenuItem(hmenu, ID_OPEN_AUDIOFILE, MF_ENABLED);
	//AudioBoomTest();
	CHAR filefullpath[MAX_PATH + 1] = { 0 };


	if (!IsFileExists(filename, filefullpath)) {
		::MessageBoxA(NULL, "not exists test mp3 file", "error", MB_ICONERROR);
		return;
	}
	if (_player == NULL) {
		_player = new BassAudioPlayer();
	}
	playerEndCallback*  call = (playerEndCallback*)Callback;
	_player->SetAudioFile(filefullpath);
	VOID* buffer;
	int blen=_player->GetSampleBuffer(&buffer),fraq=_player->GetFraquency(),chans=_player->GetChannelNum();
	_player->SetBuffer(buffer, blen, fraq, chans, 0);

	 _player->Play();
	_isCurrentTest = TRUE;
	 SetTimer(hwnd, ID_TIMER_AUDIOWAVE, ELAPSE, AudioTimer);
}


int GetLastSpalshIndex(CCHAR* name) {
	int i, lastindex = -1;
	CHAR c;
	for (i = 0;; i++) {
		c = name[i];
		if (c == '\\') {
			lastindex = i;
		}
		if (c == 0)break;
	}
	return lastindex;
}
//次处无法使用 (.\) 来表示当前目录 可以使用(..\)来表示父目录
BOOLEAN IsFileExists(CCHAR* filename,CHAR *filefullpath) {
	int lastindex = GetLastSpalshIndex(filename);
	HANDLE h;
	if (lastindex != -1) { 
			h = ::CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			memcpy(filefullpath, filename,strlen(filename));
	}
	else {
		//训练PAGE转换
		int wlen= MAX_PATH * sizeof(WCHAR),alen;
		WCHAR path[MAX_PATH + 1] = { 0 };
		CHAR ascPath[MAX_PATH + 1] = { 0 };
		::GetModuleFileName(NULL, path, MAX_PATH);
		Log::Wchar2Char(ascPath, path, alen);
		lastindex = GetLastSpalshIndex(ascPath);
		memset(path + lastindex + 1, 0, sizeof(WCHAR) * (MAX_PATH - lastindex - 1));
		LPTSTR filepath=(LPTSTR)alloca(wlen);
		memset(filepath, 0, wlen);
		Log::Char2Wchar(filepath, filename, wlen);
		memcpy(path + lastindex + 1, filepath, wlen*sizeof(WCHAR));
		h = ::CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		Log::Wchar2Char(filefullpath, path, alen);
	}

	if (h == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	else {
		::CloseHandle(h);
		return TRUE;
	}
}

VOID EndAudioTest(const HWND& hwnd) {
	if (_player != NULL) {
		KillTimer(hwnd, ID_TIMER_AUDIOWAVE);
		_player->Stop();
	}
	_isCurrentTest = FALSE;
}
VOID CALLBACK AudioTimer(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime) {
	if (_player == NULL)return;
	HDC hdc = ::GetDC(hwnd);
	_player->DrawWave(hwnd, hdc, sizeof(BYTE));
	 ::ReleaseDC(hwnd, hdc);
	 if (_player->IsEndPlay()) {
		 _player->Play();
	 }
}

VOID AudioCommand(HWND& hwnd, WPARAM& param) {

	if (_isCurrentTest) {
		switch (LOWORD(param))
		{
		case ITEM_AUDIO_OPTION:
		{
			_player->Setting(hwnd); 
			break;
		}
		}



	}
} 
 


