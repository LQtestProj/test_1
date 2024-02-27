#include "popMenu.h"

 

ContextMenu::ContextMenu(const int& popid)
{
	m_hPopMenu = CreatePopupMenu();
	if (m_hPopMenu!=NULL)
	{
		if (popid == BEZEIER_POPMENU) {
			::InsertMenu(m_hPopMenu, (-1), MF_BYPOSITION, ITEM_MENU_DELETE_POINT, TEXT("删除点"));
		}
		if (popid == FOURIER_POPMENU) { 
			::InsertMenu(m_hPopMenu, (-1), MF_BYPOSITION, ITEM_HIDDEN_FOURIERCIRCLE, TEXT("EPIC设置"));
		}
		if (popid == AUDIO_POPMENU) {
			::InsertMenu(m_hPopMenu, (-1), MF_BYPOSITION, ITEM_AUDIO_OPTION, TEXT("Audio选项"));
		}
		HBITMAP Hbitmap = NULL;

	}
}

ContextMenu::~ContextMenu()
{
	if (m_hPopMenu)
	{
		DestroyMenu(m_hPopMenu); 
	}
}

void ContextMenu::PopMenu(HWND& hwnd, int nX, int nY)
{
	if (m_hPopMenu && hwnd)
	{
		POINT pt = { nX,nY };
		::ClientToScreen(hwnd, &pt);
		TrackPopupMenu(m_hPopMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
	}
}

void ContextMenu::PopMenu(HWND& hwnd, const POINTS& pt)
{
	PopMenu(hwnd, pt.x, pt.y);
}