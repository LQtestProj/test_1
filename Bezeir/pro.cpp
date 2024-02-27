#include "pro.h"

#include "animeLerp.h"
#include "fourier.h" 
#include "toolutil.h"
#include "huffman.h"  
#include "list.h"
#include "audioTest.h"
#include "bezeirTest.h"
#include <map>
#include "resource.h"
#include "fourierTest.h"
#include "log.h"
#define POINT_LEN 3  


#pragma warning(disable:4996)

 
ComplexNum** _testCom;
VOID* _drawBuffer; int _drawLength;
HWND _mainWnd;
VOID OnWinCreate(HWND hwnd) { 
	int i,t;  
	_mainWnd = hwnd;  
}


  
 


VOID LButtonUp(HWND& hwnd, POINTS& pt) { 
	BezeirMouseButtonUp(hwnd, pt);
}
VOID LButtonDown(HWND& hwnd, POINTS& pt) {

	BezeirMouseButtonDown(hwnd, pt);
}
VOID Command(HWND& hwnd,WPARAM& wp, LPARAM& lp) {

	WORD uId = LOWORD(wp); 
	switch (uId) {
	case ID_OPENIMAGEFILE: {
		OPENFILENAMEA ofn = { 0 };
		CHAR strFile[MAX_PATH]; 
		// 对 OPENFILENAME 进行初始化 
		memset(strFile, 0, sizeof(char) * MAX_PATH);
		ofn.lStructSize = sizeof(OPENFILENAME);
		 ofn.lpstrFilter = "BMP(*.bmp)\0*.bmp\0PNG(*.png)\0*.png*;\0\0";     //(*.png)\0 * .png*; \0一组选择
		ofn.lpstrFile = strFile;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_FILEMUSTEXIST;
		if (::GetOpenFileNameA(&ofn)) {
			EndFourierTest(_mainWnd);
			FourierTest(_mainWnd, ofn.lpstrFile);
		}
		break;
	}
	case ID_OPEN_AUDIOFILE: { 
		OPENFILENAMEA ofn = { 0 };
		 CHAR strFile[MAX_PATH];
		// 对 OPENFILENAME 进行初始化 
		memset(strFile, 0, sizeof(char) * MAX_PATH);
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFilter =  "MP3(*.mp3)\0*.mp3*;\0WAV(*.wav)\0*.wav\0\0";
		ofn.lpstrFile = strFile;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_FILEMUSTEXIST;
		if (::GetOpenFileNameA(&ofn)) { 
			AudioTestMain(_mainWnd,ofn.lpstrFile);
		}
		break;
	}
	case ID_AUDIO_BUFFER_TEST:
		EndFourierTest(_mainWnd);
		EndBezeirTest(_mainWnd);
	//	EndAudioTest(NULL);
		AudioTestMain(_mainWnd);
		break;
	case ID_FOURIER_TEST:
		//EndFourierTest(NULL);
		EndBezeirTest(_mainWnd);
		EndAudioTest(_mainWnd);
		FourierTest(_mainWnd);
	
		break;
	case ID_BEZEIR_TEST:
		EndFourierTest(_mainWnd);
		//EndBezeirTest(NULL);
		EndAudioTest(_mainWnd);
		BezeirTest(_mainWnd);
		break;
	default: 
		BezeirCommand(wp);
		FourierCommand(_mainWnd, wp);
		AudioCommand(hwnd, wp);
		break;
	}

}
VOID RButtonUp(HWND& hwnd, POINTS& pt) {
	BezeirRButtonUp(hwnd, pt); 
	 FourierRButtonUp(  hwnd, pt);
	 RBUTTONUP_AUDIO(hwnd, pt);
}
VOID MouseMoveMain(POINTS pt) {
	BezeirMouseMove(pt);
}
int _timeCount = 0;
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime) {
	 
	switch (iTimerID) {
	case ID_TIMER:
		break; 

	}  
//	

} 


#define _INTSIZEOF(n)          ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))


