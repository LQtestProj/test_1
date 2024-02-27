#pragma once
#ifndef _CONTEXTMENU_H_
#define _CONTEXTMENU_H_
#include "framework.h"

#define ITEM_MENU_DELETE_POINT	40001 
#define ITEM_HIDDEN_FOURIERCIRCLE	50001 
#define ITEM_AUDIO_OPTION	60001 
#define BEZEIER_POPMENU 1
#define FOURIER_POPMENU 2
#define AUDIO_POPMENU 3
class ContextMenu
{
public:
	ContextMenu(const int&);
	~ContextMenu();

	void PopMenu(HWND& hwnd, int nX, int nY);
	void PopMenu(HWND& hwnd, const POINTS& pt);
	void EnableMenu(int nItem, bool nEnable);
private:
	HMENU m_hPopMenu;
};
#endif
