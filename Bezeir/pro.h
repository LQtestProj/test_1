#pragma once
#include "framework.h"
#include "complexNum.h"
#include "bass.h"
#define ID_TIMER 5
#define FPS 5
#define ELAPSE (1/FPS)*1000
#define PLANT_HEIGHT WIN_HEIGHT-150
#define MAX_ANIMELOOPTIME 2000.0


struct Point {
	int x, y, flag;
	int clr;

};
struct Vector {
	double x = 0, y = 0;
};
struct PointNode {
	PointNode* next = NULL, * curNode = NULL, * firstNode = NULL;
	TestPoint pt;
	~PointNode() {
		PointNode* pnode = this;
		if (pnode->next != NULL) {
			delete pnode->next;
		}




	}
	PointNode* AddPoint(TestPoint& pt_) {
		if (curNode == NULL) {
			curNode = this;
		}
		else {
			curNode->next = new PointNode;
			curNode = curNode->next;

		}

		curNode->pt.x = pt_.x;
		curNode->pt.y = pt_.y;
		return this;
	}
	TestPoint& GetPoint(int index) {
		PointNode* pnode = this;
		while (index-- != 0) {
			pnode = pnode->next;
		}

		return pnode->pt;
	}
	VOID DeleteNode(int index) {
		int i;
		PointNode* pnode = this, * preNode;

		while (index-- != 0) {
			preNode = pnode;
			pnode = pnode->next;
		}
		if (pnode == this) {
			curNode = NULL;
			return;
		}
		preNode->next = pnode->next;
		if (pnode->next == NULL) {
			curNode = preNode;
		}
		pnode->next = NULL;
		delete pnode;

	}
	int Size() {
		int size = 1;
		PointNode* node = this;
		while ((node = node->next) != NULL) {
			++size;
		}
		return size;
	}
};
VOID InitTestPoint(HWND hwnd);
VOID InitBezeirPoint(HWND hwnd);
VOID InitFourierPoint(HWND hwnd);
VOID DrawMain(HWND hwnd, HDC hdc);
VOID MouseMoveMain(POINTS pt);
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime);
VOID LButtonUp(HWND& hwnd, POINTS& pt);
VOID LButtonDown(HWND& hwnd, POINTS& pt);
VOID RButtonUp(HWND& hwnd, POINTS& pt);
VOID Command(HWND& hwnd, WPARAM& wp, LPARAM& lp);
