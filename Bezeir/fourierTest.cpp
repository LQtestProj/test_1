#include "fourierTest.h"

#include "fourierMathTool.h"
#include "toolutil.h"
#include "imageContoursTool.h"
#include "fourier.h"
#include "resource.h"
#include "log.h"
#include "popMenu.h"
#define ID_FOURIER_TIMER 40002
ImageContoursTool* _imagetool = NULL;
FourierDraw** _fourierArr = NULL;
VOID EndFourierTest(const HWND& hwnd) {
	if (_imagetool != NULL) {

		int i;
		FourierDraw* fs;
		KillTimer(hwnd, ID_FOURIER_TIMER);
		if (_fourierArr == NULL) {
			return;
		}
		for (i = 0;; i++) {
			fs = _fourierArr[i];
			if (fs == NULL) {
				break;
			}
			delete fs;
		}
		delete[] _fourierArr;
		_fourierArr = NULL;
	}
}
VOID FourierTest(HWND& hwnd, CHAR* filename) {
	HMENU hmenu = ::GetMenu(hwnd);
	::EnableMenuItem(hmenu, ID_OPENIMAGEFILE, MF_ENABLED);
	::EnableMenuItem(hmenu, ID_OPEN_AUDIOFILE, MF_GRAYED);
	WCHAR wchar[MAX_PATH * 2] = { 0 };
	int len;
	Log::Char2Wchar(wchar, filename, len);
	ReadImageComplex(hwnd, wchar);
}
VOID ReadImageComplex(HWND hwnd, WCHAR* filename) {

	FourierStruct* fsArr;
	ComplexNum* slurpCom, *** testCom, ** fourierCom, ** groupCom;
	int size = 12, nth = 5// 分裂基4次方数 nth=5  4^5=1024
		, sampleSize = pow(4, nth), i, j, ptCount, groupSize; //样本轴均分2*PI周期,越大越精细 需修改nth参数修改sampleSize防止奔溃 
	FType* floatPt;
	if (_imagetool == NULL) {
		_imagetool = new ImageContoursTool;
	}


	testCom = _imagetool->ReadBitmapPointToComplex(hwnd, filename, ptCount);

	if (testCom == NULL) {
		EndFourierTest(hwnd);
		return;
	}

	for (j = 0;; j++) {
		groupCom = testCom[j];
		if (groupCom == NULL) {
			groupSize = j;
			break;
		}

	}
	_fourierArr = new FourierDraw * [groupSize + 1];
	_fourierArr[groupSize] = NULL;
	fsArr = (FourierStruct*)alloca(sizeof(FourierStruct) * sampleSize);

	floatPt = (FType*)alloca(sizeof(FType) * sampleSize * 2);
	slurpCom = (ComplexNum*)alloca(sizeof(ComplexNum) * sampleSize);
	;	for (j = 0; j < groupSize; j++) {
		size = 0;
		for (i = 0;; i++) {
			if (testCom[j][i] == NULL) {
				break;
			}
		}
		memset(fsArr, 0, sizeof(FourierStruct) * sampleSize);
		memset(floatPt, 0, sizeof(FType) * sampleSize * 2);
		size = i - 1;
		memset(slurpCom, 0, sizeof(ComplexNum) * sampleSize);
		FourierMathTool::SlurpPoint(testCom[j], slurpCom, size, sampleSize);
		FourierMathTool::ComplexToFloatArray(slurpCom, floatPt, sampleSize);
		FourierMathTool::FFT2AndFFT4(floatPt, fsArr, sampleSize, ptCount);

		_fourierArr[j] = new FourierDraw;

		_fourierArr[j]->SetEpicList(fsArr, sampleSize);

	}

	for (i = 0; i < groupSize; i++) {
		groupCom = testCom[i];
		for (j = 0;; j++) {
			if (testCom[i][j] == NULL) {
				break;
			}
			delete testCom[i][j];
		}
		delete[] testCom[i];

	}
	delete[] testCom;

	SetTimer(hwnd, ID_FOURIER_TIMER, 10, FourierTimerProc);

}

VOID FourierCommand(HWND& hwnd, WPARAM& param) {
	if (_fourierArr != NULL) {
		switch (LOWORD(param))
		{
		case ITEM_HIDDEN_FOURIERCIRCLE:
		{

			FourierDraw::Setting(hwnd);
			break;
		}
		}



	}

}
VOID FourierRButtonUp(HWND& hwnd, POINTS& pt) {
	if (_fourierArr != NULL) {
		FourierDraw::PopMenu(hwnd, pt);
	}
}
VOID CALLBACK FourierTimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime) {
	if (_fourierArr == NULL)return;
	HDC hdc = ::GetDC(hwnd);
	int i, j, scale = 3, skip, skipLimit = 5;
	Point** ptArray, * curPt, * prePt, *** _testPt = _imagetool->GetTestPtr();
	FourierDraw::DrawFourierArray(hwnd, hdc, _fourierArr);
	::ReleaseDC(hwnd, hdc);

}


