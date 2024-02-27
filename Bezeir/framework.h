// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <WinUser.h>
// C 运行时头文件
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h> 
#include <mmreg.h>
#include <mmsystem.h>
#include <dsound.h>
#include <Commdlg.h>
#include <Commctrl.h>

#define WIN_WIDTH 1440
#define WIN_HEIGHT 700

VOID OnWinCreate(HWND hwnd);

class ComplexNum; 
class WaveHead;
struct TestPoint;
struct HNODE;
struct PointPathNode;


struct TestPoint {
	int x, y;
};
struct PointPathNode {
	PointPathNode* next;
	TestPoint pt;
	BOOLEAN isEnd;

};
struct FourierStruct {
	int freq;
	float amplitude, phase;
	float real, img;
};
typedef float FType;

#define TWO_PI (2*M_PI)

const double STEP = 0.01;
static const int MAX_ANIME_SIZE = (int)(1.0 / STEP + 1);


#define MEMORY_ERROR 1
#define ASSERT(b) if(!(b)){throw MEMORY_ERROR;} 

extern HINSTANCE hInst;