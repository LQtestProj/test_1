#include "Program.h"


HBRUSH Tear::s_defBrush = NULL;
VOID Tear::DrawDrop(HWND hwnd, HDC hdc) {
	int r = this->radius;
	int left = this->x - r, right = left + r, top = this->y - r, bottom = top + r;
	SelectObject(hdc, m_brush);

	Ellipse(hdc, left, top, right, bottom);

}

VOID Tear::Drop(HWND hwnd, HDC hdc) {
	long subtime;
	double subHeightPer = CalcAnimePer(MAX_ANIMELOOP);
	int y = WIN_HEIGHT * subHeightPer * subHeightPer * subHeightPer;
	this->y = y;
	//this->x = WIN_WIDTH >> 1;
	if (this->y >= m_iPlantHeight) {
		this->m_lAnimeStartTime = 0;
		this->m_isOnPlant = TRUE;
		this->m_isQueOver = FALSE;
		this->y = m_iPlantHeight;
		return;
	}
	DrawDrop(hwnd, hdc);

}
VOID	Tear::Rippling(HWND hwnd, HDC hdc) {

	double subHeightPer = CalcAnimePer(1000.0);
	int x, y;
	x = this->maxRipplingDown * subHeightPer;
	y = this->maxRipplingUp * subHeightPer;
	if (y == 0) {
		y = 1;
	}
	this->ripplingUp = y;
	this->ripplingDown = x;

	int left = this->x - this->ripplingDown, right = this->x + this->ripplingDown
		, top = this->y - this->ripplingUp, bottom = this->y + this->ripplingUp;

	SelectObject(hdc, s_defBrush);
	SelectObject(hdc, m_pen);
	Ellipse(hdc, left, top, right, bottom);

	if (subHeightPer == 1) {
		subHeightPer = 1;
		this->m_isQueOver = TRUE;
		this->m_isOnPlant = FALSE;
		this->m_lAnimeStartTime = 0;
		this->y = 0;
		this->x = rand() % WIN_WIDTH;
	}

}
double& Tear::CalcAnimePer(double animeTime)
{
	long subtime;
	double subHeightPer, y, x;
	if (this->m_lAnimeStartTime == 0) {
		m_lAnimeStartTime = ::GetTickCount();
	}
	subtime = ::GetTickCount() - m_lAnimeStartTime;
	subHeightPer = (subtime) / animeTime;
	if (subHeightPer >= 1) {
		subHeightPer = 1;
	}
	return subHeightPer;
}
VOID Tear::Draw(HWND hwnd, HDC hdc) {

	if (m_isQueOver && !m_isOnPlant) {
		Drop(hwnd, hdc);
	}
	else {
		Rippling(hwnd, hdc);
	}



}
Tear::Tear() {
	radius = 5 + (rand() % 10);
	this->maxRipplingUp = (int)(1.5 * radius) >> 1;
	this->maxRipplingDown = (3 * radius) >> 1;
	m_iPlantHeight = (radius << 2) + PLANT_HEIGHT;
	r = rand() % 255; g = rand() % 255; b = rand() % 255;
	m_brush = CreateSolidBrush(RGB(r, g, b));
	m_pen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	if (Tear::s_defBrush == NULL) {
		Tear::s_defBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
}

