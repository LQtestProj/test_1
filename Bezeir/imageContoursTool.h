#pragma once

#include "toolutil.h" 




#define ALLOCA_NEW_ARRAY(ST,len) (ST*)alloca(sizeof(ST)*len)
#define ALLOC_NEW(ST) ALLOCA_NEW_ARRAY(ST,1)

#define ZERO_MEM(ptr,ST) memset(ptr,0,sizeof(ST))
#define CREATE_ALLOCA(__out_ptr,ST) __out_ptr=ALLOC_NEW(ST);ZERO_MEM(__out_ptr,ST)
#define CREATE_ALLOCA_ARRAY(__out_ptr,__len,ST) (__out_ptr)=ALLOCA_NEW_ARRAY(ST,(__len)); memset((__out_ptr),0,sizeof(ST)*(__len)) 
class PixelCache {
	Point*** bitmapPointPtr;
	int bitmapHeight, bitmapWidth;

public:
	Point* GetPoint(int x, int y);
	PixelCache(Point*** bitmap, int bw, int bh);
	BOOLEAN IsSingle(Point* pt, TestPoint* testChecker);
	Point* GetPoint(Point* pt, TestPoint& testpoint, int& testx, int& testy);
	Point* GetPoint(Point* pt, TestPoint& testpoint);
	BOOLEAN IsAround(Point* testPoint, TestPoint* pointChecker);
};

class ImageContoursTool {
private:
	Point*** mTestPtArray;
	BITMAP  mBitmap;
	HBITMAP mhBitmap;
	int miBitmapWidth, miBitmapHeight, antiClockWise[8] = { 4,5,6,7,0,1,2,3 };
	TestPoint antiClockPoint[8] = {
	{0,-1},{-1,-1},{-1,0},
	{-1,1}		,{0,1}
   ,{1,1},{1,0},{1,-1} }
		, clockPoint[8] = {
			{0,-1},{1,-1},{1,0},
			{1,1},			{0,1},
			{-1,1},{-1,0},{-1,-1}
	};
	VOID ReadBitmapPix(UCHAR* pBits, Point*** lpPt);
	//返回是否有下个点
	BOOLEAN  GetCheckPoint(PixelCache& pixcache, int flagIndex, ImagePointNode* pointNode, Point* curPt, Point* mark1, Point* mark2, BOOLEAN& isInnerDraw, int& preTestIndex, Point** outtestPt);
	Point* GetHeadPoint(PixelCache& pixcache, int x, int y, Point** markPt, BOOLEAN& isInnerDraw, int& preTestIndex);
	ComplexNum*** GetComplexNum(Point*** bitmapPointPtr);
	ComplexNum*** GetLinePixelComplex(GroupNode* rootGroup);
	ImagePointNode* LinePointNode(ImagePointNode* pointNode, Point** lpStartPt, Point** lpEndPt, int& endIndex);

	int CalcGroupSize(GroupNode* groupRoot) {
		GroupNode* node = groupRoot;
		if (node == NULL)return 0;
		int size = 1;
		while ((node = node->next) != NULL) {
			++size;
		}
		return size;
	}
	int CalcPointNodeSize(ImagePointNode* pointRoot) {
		ImagePointNode* node = pointRoot;
		if (node == NULL)return 0;
		int size = 1;
		while ((node = node->nextNode) != NULL) {
			++size;
		}
		return size;
	}
	VOID CreateTestPoint(GroupNode* lineGroupRoot,  ComplexNum**** outComplex);
public:
	ComplexNum*** ReadBitmapPointToComplex(HWND hwnd, LPCWSTR filename, int& outPtSize);
	Point*** GetTestPtr() {
		return mTestPtArray;
	}
};



