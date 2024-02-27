#pragma once


#include "pro.h" 
#define FPS 5
#define ELAPSE (1/FPS)*1000
VOID AudioBoomTest();
VOID AudioTestMain(HWND hwnd, CHAR* filename= "test.mp3");
VOID CALLBACK AudioTimer(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime);
VOID EndAudioTest(const HWND& hwnd);
VOID AudioCommand(HWND& hwnd, WPARAM& param);
VOID RBUTTONUP_AUDIO(HWND& hwnd, POINTS& pt);
BOOLEAN IsFileExists(CCHAR* filename, CHAR* filefullpath);