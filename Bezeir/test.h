#pragma once
#include "bass_audio.h"



VOID TraceBassTime(HCHANNEL channel, int bufferIndex, double& pretime);

VOID SampleLoadTest(LPCSTR filname);
VOID TestPlayer();

DWORD CALLBACK StreamProc(HSTREAM handle, VOID* buffer, DWORD length, VOID* user);