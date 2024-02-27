
#include "fourier.h"
#include "toolutil.h"
#include "log.h"
#include <map>
#include "fourierOptDlg.h"
using std::map;
using std::pair;
#define FOURIER_LOOPTIME 10000
#define SCALE_CIRCLE 3

#define HA0 (25.0/46)
#define HANN(n,N) (HA0-((1-HA0)*cos(TWO_PI*((n)*1.0/((N)-1)))))
HBITMAP FourierDraw::s_nbitmap = NULL;
HBITMAP FourierDraw::s_obitmap = NULL;
RECT FourierDraw::s_rt;
HDC FourierDraw::s_bitDc;
HPEN FourierDraw::s_LinePen;
ContextMenu FourierDraw::_popmenu(FOURIER_POPMENU);
HBRUSH FourierDraw::s_nullBrush=NULL;
BOOLEAN FourierDraw::s_isHiddenEpic = FALSE;
BOOLEAN FourierDraw::s_needClear = FALSE;

  FType  FourierDraw::s_epicShowPercent=1;
int** jj = NULL;
map<int, FType> _cosMathTable, _sinMathTable;


VOID  FourierDraw::SetEpicShowPercent(FType per) {
	s_epicShowPercent = per;
	s_needClear = TRUE;
}
VOID FourierDraw::Setting(HWND& hwnd) {
	_fourierSetting.ShowDlg(hwnd);
}
FType GetCos(FType angle) {
	angle = angle - (int)(angle / TWO_PI) * TWO_PI;
	int iangle = angle * 1000;
	FType res = _cosMathTable[(iangle)]; 
	if (res == 0) {
		res = cos(angle);
		pair<int, FType> pair(iangle, res);
		_cosMathTable.insert(pair);
	} 
	return res;
}
FType GetSin(FType angle) {
	angle = angle - (int)(angle / TWO_PI) * TWO_PI;
	int iangle = angle * 1000;
	FType res = _sinMathTable[(iangle)]; 
	if (res == 0) {
		res = sin(angle);
		pair<int, FType> pair(iangle, res);
		_sinMathTable.insert(pair);
	} 
	return res;
}

int _pIndex = 0;

//画路径
VOID FourierDraw::DrawFourier(HWND hwnd, HDC hdc, int x, int y,AnimeState rangeState) {
//	TRACE(L"f draw-x:%d,y:%d\n",x,y);
	TestPoint pt = { x + (_pIndex),y };
	if (mRootPath == NULL) {
		if (x == 0 && y == 0) {
			return;
		}
		mRootPath = new PointPathNode; 
		memset(mRootPath, 0, sizeof(PointPathNode));
		mRootPath->pt = pt;
		return;
	}
	HPEN  oldPen;
	PointPathNode* node = mRootPath,*newNode,*endNode= node;
	TestPoint prePt = node->pt, curPt = node->pt,*endPt=NULL;
	oldPen = (HPEN) ::SelectObject(hdc, s_LinePen);
	if (rangeState ==AnimeState::OVERFLOW_RANGE) {
		mRootPath->isEnd = TRUE;
	}  
	if (!mRootPath->isEnd) {
		newNode = new PointPathNode;
		memset(newNode, 0, sizeof(PointPathNode));
	}
	::MoveToEx(hdc, prePt.x, prePt.y, NULL);
	while ((node = node->next) != NULL) {
		//::SetPixel(hdc, prePt.x, prePt.y,RGB(0,0,255));
		curPt = node->pt;
		::LineTo(hdc, curPt.x, curPt.y);
		//prePt = curPt; 
		endNode = node;
	}
	if (!mRootPath->isEnd) {
		 endNode->next = newNode;
		 newNode->pt = pt;
		::LineTo(hdc, pt.x, pt.y);
	}
	
	//::Sleep(100);
	::SelectObject(hdc, oldPen); 
}
double _maxAngle=0, _minAngle=0; 

VOID FourierDraw::PopMenu(HWND& hwnd, POINTS& pt) {
	_popmenu.PopMenu(hwnd, pt);
  }
FType FourierDraw::CalcAnimePer(FType animeTime, FType& curAnime) {

	long subtime;
	FType subHeightPer, y, x;
	if (curAnime == 0) {
		curAnime = ::GetTickCount();
	}
	subtime = ::GetTickCount() - curAnime;
	//m_lAnimetime = ::GetTickCount();
	subHeightPer = (subtime) / animeTime;
	if (subHeightPer >= 1) {
		subHeightPer = 1;
		curAnime = 0;
	}
	return subHeightPer;

}

VOID FourierDraw::DrawFourierArray(HWND hwnd, HDC hdc, FourierDraw** fourierArr) {

	FourierDraw* curFourier  ;
	int fsize,i,j,w,h;	
	BOOLEAN reset=FALSE;
	if (FourierDraw::s_nbitmap == NULL) {
		 s_bitDc = CreateCompatibleDC(hdc);
		 s_LinePen = (HPEN)::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		::GetClientRect(hwnd, &FourierDraw::s_rt);
		FourierDraw::s_nbitmap = CreateCompatibleBitmap(hdc, FourierDraw::s_rt.right - FourierDraw::s_rt.left,
			FourierDraw::s_rt.bottom - FourierDraw::s_rt.top);//hdc 
	}

	w = s_rt.right - s_rt.left;
	h = s_rt.bottom - s_rt.top;
	if (s_needClear) {
		s_needClear = FALSE; 
		::FillRect(hdc, &s_rt, (HBRUSH)::GetStockObject(WHITE_BRUSH));
		for (i = 0; ; i++) {
			curFourier = fourierArr[i];
			if (curFourier == NULL) {
				break;
			}
			if (curFourier->mRootPath != NULL) {  
				PointPathNode* node = curFourier-> mRootPath, * dnode;
				for (; node != NULL; dnode = node, node = node->next, delete dnode);
				curFourier->mRootPath = NULL;
			}
			curFourier->m_lAnimeTime = 0;
		}
	}
	s_obitmap = (HBITMAP)::SelectObject(s_bitDc, s_nbitmap);
	::FillRect(s_bitDc, &s_rt, (HBRUSH)::GetStockObject(WHITE_BRUSH));
	for (i = 0;; i++) {
		curFourier = fourierArr[i];
		if (curFourier == NULL) {
			fsize = i;
			break;
		}
	}

	EpicCircle* circle = NULL;
	FType am, phase, angle, preY, preX, x  , y  ;
	FType animePer  ,step= 0.00390625;
	int  fraq, N, 
		overflowSize = 5;//闭合参数.数值越小动画开口越大
	AnimeState rangeState = AnimeState::NORMAL;
	 for (i = 0; i < fsize; i++) {
		curFourier = fourierArr[i];
		curFourier->m_lAnimeTime +=( step);
		animePer = curFourier->m_lAnimeTime; // CalcAnimePer(FOURIER_LOOPTIME, curFourier->m_lAnimeTime);
		if (rangeState == AnimeState::FIRST_MATCH) {
			if(overflowSize--==0)
				rangeState = AnimeState::OVERFLOW_RANGE;
		}
		if (animePer > 1) {
			curFourier->m_lAnimeTime = 0;
			animePer = 1;
			if(rangeState!=AnimeState::OVERFLOW_RANGE)
				rangeState = AnimeState::FIRST_MATCH ;
		}
		x = 50; y = 150;
		N = curFourier->m_iEpicSize;
		N = N * s_epicShowPercent;
		if (N <=1) {
			N = 2;
		}
		for (j = 0; j < N ; j++) {
			circle = curFourier->m_epicArray[j];
			preX = x;
			preY = y;
			am = circle->len * SCALE_CIRCLE;

			fraq = circle->fraq;
			phase = circle->phase;
			angle = (TWO_PI)*fraq * animePer + phase; 
			x += am * cos(angle);
			y += am * sin(angle);
			//排除小圆
			if (j != N - 1 && circle->len < 0.5) {
				continue;
			}
			if (!s_isHiddenEpic) {
				DrawEpicCircle(hwnd, s_bitDc, preX, preY, am);
				DrawLine(hwnd, s_bitDc, preX, preY, x, y);
			} 

		}
		 curFourier->DrawFourier(hwnd, s_bitDc, x, y, rangeState);


	}



	BitBlt(hdc, 0, 0, w, h, s_bitDc, 0, 0, SRCCOPY);
	SelectObject(s_bitDc, s_obitmap);




  }
VOID FourierDraw::EpicHidden(BOOLEAN hidden) {
	s_isHiddenEpic = hidden;

}
VOID FourierDraw::EpicShowStatusChange(const HWND& hwnd) {
	s_isHiddenEpic = !s_isHiddenEpic;
  }

VOID  FourierDraw::DrawEpicCircle(HWND& hwnd, HDC& hdc, FType x, FType y, FType radius) {

	if (s_nullBrush == NULL) { 

		s_nullBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}  


	HBRUSH   oldBrush = (HBRUSH)::SelectObject(hdc, s_nullBrush);
	::DrawCircle(hwnd, hdc, x, y, radius);

	::SelectObject(hdc, oldBrush);

}
VOID FourierDraw::Draw(HWND hwnd, HDC hdc,double x, double y) {
	if (mRootPath == NULL) { 
		s_bitDc = CreateCompatibleDC(hdc);
		::GetClientRect(hwnd, &s_rt);

		s_nbitmap= CreateCompatibleBitmap(hdc, s_rt.right - s_rt.left, s_rt.bottom - s_rt.top);//hdc


	}
	s_obitmap=(HBITMAP)::SelectObject(s_bitDc, s_nbitmap);
	 ::FillRect(s_bitDc, &s_rt, (HBRUSH)::GetStockObject(WHITE_BRUSH));
	int w = s_rt.right - s_rt.left, h = s_rt.bottom - s_rt.top;
	double animePer = CalcAnimePer(FOURIER_LOOPTIME, ( this->m_lAnimeTime) );
//	TRACE(L"an:%f\n",animePer);
	//this->Draw(hwnd, hdc, m_epicArray,0,200,200, animePer);

	EpicCircle* circle = NULL;
	double am,phase,angle, preY, preX;
	int  fraq, N = m_iEpicSize; 
	//N = 200;
	if (N > m_iEpicSize) {
		N = m_iEpicSize;
	}
		for (int i = 0; i < N; i++) {
			circle = this->m_epicArray[i];
			preX = x;
			preY = y;
			am = circle->len*SCALE_CIRCLE;
		
			fraq = circle->fraq;
			phase = circle->phase;
			angle = (TWO_PI )* fraq* animePer +phase;
			x += am* cos(angle);
			y += am * sin(angle);
			if (i != N - 1 && circle->len < 0.5) {
				//continue;
			}
			if (i == 0){
				if (animePer == 1) {
				//	delete this->m_pointNode;
				//	this->m_pointNode = new PointNode;
			}
				continue;
			}
			if (!s_isHiddenEpic) {
				DrawEpicCircle(hwnd, s_bitDc, preX, preY, am);
				DrawLine(hwnd, s_bitDc, preX, preY, x, y);
			}

		} 
		DrawFourier(hwnd, s_bitDc, x, y, AnimeState::NORMAL);
		 BitBlt(hdc, 0,0, w, h, s_bitDc, 0, 0, SRCCOPY);
		 SelectObject(s_bitDc, s_obitmap);
}


VOID FourierDraw::DrawFraquencyDomain(HWND hwnd, HDC hdc) {

	if (this->m_iEpicSize == 0) {
		return;
	}
	int zeroX = 200, zeroY = 800,lineWidth= 800,lineHeight= 400;
	RECT winRect;
	int N = this->m_iEpicSize;
	EpicCircle* epic = NULL;
	int iY = 0,fX=0,half=this->m_N>>1,fraq,scale=5;
	double* fq1=new double[half],*fq2=new double[half],am1,am2;

	::GetWindowRect(hwnd, &winRect);
	zeroX = (winRect.right - winRect.left) >> 1;
	zeroY = (winRect.bottom - winRect.top)  >>1;
	zeroY = (zeroY >> 1) + zeroY;
	MoveToEx(hdc, zeroX, zeroY, NULL);
	::LineTo(hdc, zeroX + (lineWidth >> 1), zeroY);
	MoveToEx(hdc, zeroX, zeroY, NULL);
	::LineTo(hdc, zeroX - (lineWidth >> 1), zeroY);
	MoveToEx(hdc, zeroX, zeroY, NULL);
	::LineTo(hdc, zeroX , zeroY+ (lineHeight>>1));
	MoveToEx(hdc, zeroX, zeroY, NULL);
	::LineTo(hdc, zeroX, zeroY- (lineHeight>>1));

	for (int i = 0; i < N; i++) {
		epic = m_epicArray[i];
		fraq = epic->fraq;
		am1 = epic->len;
		if (fraq >= 0) {
			fq1[fraq] = am1* scale;
		}
		else {
			fq2[-fraq] = am1* scale;
		}
	}
	HPEN rPen = ::CreatePen(PS_SOLID,1,RGB(0, 220, 255)),oldPen;
	oldPen=(HPEN)::SelectObject(hdc, rPen);
	
	for (int i = 0; i <half; i++) {
		am1 = fq1[i];
		fX = zeroX + i;
		iY = zeroY - am1;
		if (i == 0) {
			::MoveToEx(hdc, fX, iY, NULL);
			continue;
		}
		::LineTo(hdc, fX, iY);
	}
	for (int i = 0; i < half; i++) {
		am1 = fq2[i];
		fX = zeroX - i;
		iY = zeroY - am1;
		if (i == 0) {
			::MoveToEx(hdc, fX, iY, NULL);
			continue;
		}
		::LineTo(hdc, fX, iY);
	}
	 
	::SelectObject(hdc, oldPen);
	::DeleteObject(rPen);
	delete fq1;
	delete fq2;
}

int FourierDraw::SortEpic(FourierStruct** fsArray, int N) {
	FourierStruct* tmpfs, * e1, * e2;
	int i, j;
	int end = N >> 2 - 1;
	int nIndex = -1, usedEpicSize;
	float   skipam = 0.001953125; //最小直径下限 越大舍弃的epic越多
	N = N >> 1;
	//删除过小直径的圆 
	usedEpicSize = N;
	for (i = 0; i < N; i++) {
		tmpfs = fsArray[i];
		if (nIndex != -1) {
			if (tmpfs->amplitude >= skipam) {
				if (fsArray[nIndex] == NULL) {
					fsArray[nIndex] = tmpfs;
					fsArray[i] = NULL;
				}
				++nIndex;
			}
		}
		if (tmpfs->amplitude < skipam) {
			--usedEpicSize;
			if (nIndex == -1) {
				nIndex = i;
			}
			fsArray[i] = NULL;
		}
	}



	for (int i = 0; i < usedEpicSize; i++) {
		for (int j = i + 1; j < usedEpicSize; j++) {
			e1 = fsArray[i];
			e2 = fsArray[j];
			if (e1 == NULL && e2 == NULL) {
				continue;
			}
			if (e1 != NULL && e2 == NULL) {
				continue;
			}
			if (e1 == NULL && e2 != NULL) {
				fsArray[i] = e2;
				fsArray[j] = e1;
				continue;
			}
			if (e1->amplitude < e2->amplitude) {
				fsArray[i] = e2;
				fsArray[j] = e1;
			}
		}
	}
	return usedEpicSize;
}

VOID FourierDraw::SetEpicList(FourierStruct* fsArr, int size) {

	EpicCircle** epicList = new EpicCircle * [size];
	EpicCircle* epic1 = NULL, * epic2 = NULL;
	FourierStruct  *fs1,* fs2;
	int freq1 = 0, freq2 = 0;
	int halfSize = size >> 1;
	m_iEpicSize = 0;
	m_N = size;
	m_sampleSize = size;
	for (int i = 0; i < halfSize; i++) {
		epic1 = new EpicCircle;
		epic2 = new EpicCircle;
		freq1 = i;
		freq2 = (size - i - 1);
		FourierStruct& fs1 = fsArr[freq1], & fs2 = fsArr[freq2]; 

		epic1->SetEpicInfo(fs1, freq1);
		epic2->SetEpicInfo(fs2, freq2 - size);
		epicList[m_iEpicSize++] = epic1;
		epicList[m_iEpicSize++] = epic2;
	 	}
	//delete comList;
	this->m_epicArray = epicList;
	if (1) {
		 SkipLittleEpic(m_epicArray, m_iEpicSize);
	}
	
	SortEpic(m_epicArray, m_iEpicSize);


}
VOID FourierDraw::SetEpicList(ComplexNum** comList, int size) {

	EpicCircle** epicList = new EpicCircle * [size];
	EpicCircle* epic1 = NULL, *epic2=NULL;
	ComplexNum  com1,com2;
	int freq1 = 0, freq2=0;
	int halfSize = size >> 1;
	m_N = size;
	m_sampleSize = size;
	m_iEpicSize = 0;
	// C(k)与C(N - k)互为共轭
	for (int i = 0; i < halfSize; i++) {
		epic1 = new EpicCircle;
		epic2 = new EpicCircle;
		freq1 = i;
		freq2 = (size - i-1);
		com1 = *comList[freq1];
		com2 = *comList[freq2];
		epic1->SetEpicInfo(com1, freq1, size);
		epic2->SetEpicInfo(com2, freq2-size, size);
		epicList[m_iEpicSize++] = epic1;
		epicList[m_iEpicSize++] = epic2;
			}
	//delete comList;
	this->m_epicArray = epicList;

	SkipLittleEpic(m_epicArray, m_iEpicSize);
	SortEpic(m_epicArray, m_iEpicSize);
}
// 删除直接过小的圆
VOID FourierDraw::SkipLittleEpic(EpicCircle** comList, int N) {
	EpicCircle* tmp = NULL;
	int end = N >> 2 - 1;
	int nIndex = -1;
	double   skipam =0.1; //最小直径下限 越大舍弃的epic越多
	//删除过小直径的圆
	for (int i = 0; i < N; i++) {
		tmp = comList[i];

		if (nIndex != -1) {
			if (tmp->len >= skipam) {
				if (comList[nIndex] == NULL) {
					comList[nIndex] = tmp;
					comList[i] = NULL;
				}
		
			++nIndex;
			}
		}
		if (tmp->len < skipam) {
			--m_iEpicSize;
			if(nIndex==-1){
				nIndex = i;
			}
			delete tmp;
			comList[i] = NULL;
		}
	}


}
//优化epic顺序 看起来不乱 根据 傅里叶 c1*w^1+c2*w^2=c2*w^2+c1*w^1
VOID FourierDraw::SortEpic(EpicCircle** comList, int size) {
	EpicCircle* tmp = NULL,*e1,*e2;

	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			e1 = comList[i];
			e2 = comList[j];
			if (e1 == NULL && e2 == NULL) {
				continue;
			}
			if (e1 != NULL && e2 == NULL) {
				continue;
			}
			if (e1 == NULL && e2 != NULL) {
				comList[i] = e2;
				comList[j] = e1;
				continue;
			}
			if (e1->len < e2->len) {
				comList[i] = e2;
				comList[j] = e1;
			}
		}
	}

}

//无用 慢DFT
VOID FourierDraw::ReadEpicList(double* xn, int size) {

	ComplexNum** comList = new ComplexNum * [size];
	double angle, fx, fy; 
	int N = size; 
	for (int k = 0; k < N; k++) { 
		Euler ce1, ce2;
		for (int n = 0; n < N; n++) {
			angle = 2 * M_PI * k * ((n*1.0) / N);
			ce2.MultiEuler(xn[n],angle);
			ce1 = *(ce1 + ce2);
		}
		comList[k] = new ComplexNum;
		comList[k]->real = ce1.GetComplexNum().real;
		comList[k]->img = ce1.GetComplexNum().img;
		}

	SetEpicList(comList, size);

}
