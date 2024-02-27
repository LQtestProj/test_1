#include "imageContoursTool.h"


#include "log.h"


VOID ImageContoursTool::ReadBitmapPix(UCHAR* pBits, Point*** lpPt) {


	int bIndex = 0, iBlack = 0x00, iWhite = 0xff;
	unsigned char t, r, g, b;
	int step = mBitmap.bmBitsPixel >> 3, i, j, calcIndex,clr;
	if (step == 0) {
		step = 1;
	}
	//读取点
	for (i = 0; i < miBitmapHeight; i++) {
		for (j = 0; j < miBitmapWidth; j++) {
			calcIndex = mBitmap.bmWidthBytes * i + j * step;
			if (step == 1) {
				clr = pBits[calcIndex]; 
			}
			else { 
				t = pBits[calcIndex + 3]; r = pBits[calcIndex + 2]; g = pBits[calcIndex + 1]; b = pBits[calcIndex + 0];
				clr= (t << 24) | (r << 16) | (g << 8) | b;
			}
			if (clr != 0x00ffffff) {
				clr = 0;
			}
			lpPt[i][j]->x = j;
			lpPt[i][j]->y = i;
			lpPt[i][j]->flag = 0;
			lpPt[i][j]->clr = clr;
		}
	}

}
//返回是否有下个点
BOOLEAN  ImageContoursTool::GetCheckPoint(PixelCache& pixcache, int flagIndex, ImagePointNode* pointNode, Point* curPt, Point* mark1, Point* mark2, BOOLEAN& isInnerDraw, int& preTestIndex, Point** outtestPt) {
	int testIndex, m, testx, testy;
	Point* testPt, * inboundTestPoint, * inboundTest2Point;
	TestPoint subPtTmp;
	BOOLEAN noNeedCalc, hasNext = TRUE;
	*outtestPt = NULL;
	testIndex = -1;
	noNeedCalc = TRUE;
	for (m = 0; m < 8; m++) {
		testIndex = (m + preTestIndex + 1) % 8;
		//testIndex = preTestIndex + m; 
		subPtTmp = antiClockPoint[testIndex];
		testPt = pixcache.GetPoint(curPt, subPtTmp, testx, testy);
		if (testPt == NULL)continue;
		if (testPt == mark1) {
			if (mark2 == NULL || pointNode->point == mark2) {
				hasNext = FALSE;
				break;
			}
		}

		if (testPt->clr == 0) {
			if (!pixcache.IsAround(testPt, antiClockPoint))
			{
				if (isInnerDraw) {
					if (testIndex == 1) {
						inboundTestPoint = pixcache.GetPoint(testx, testy + 1);
						inboundTest2Point = pixcache.GetPoint(testx + 1, testy);
						if (inboundTest2Point->flag == flagIndex) {
							m = -1;
							preTestIndex = 1;
							continue;
						}
						if (inboundTestPoint->clr == 0) {
							testPt = inboundTestPoint;
							preTestIndex = 6;
							noNeedCalc = FALSE;
						}
					}
					else
						if (testIndex == 3) {
							inboundTestPoint = pixcache.GetPoint(testx + 1, testy);
							inboundTest2Point = pixcache.GetPoint(testx, testy - 1);
							if (inboundTest2Point->flag == flagIndex) {
								m = -1;
								preTestIndex = 3;
								continue;
							}
							if (inboundTestPoint->clr == 0) {
								testPt = inboundTestPoint;
								preTestIndex = 0;
								noNeedCalc = FALSE;
							}
						}
						else
							if (testIndex == 5) {
								inboundTestPoint = pixcache.GetPoint(testx, testy - 1);
								inboundTest2Point = pixcache.GetPoint(testx - 1, testy);
								if (inboundTest2Point->flag == flagIndex) {
									m = -1;
									preTestIndex = 5;
									continue;
								}
								if (inboundTestPoint->clr == 0) {
									testPt = inboundTestPoint;
									preTestIndex = 2;
									noNeedCalc = FALSE;
								}
							}
							else
								if (testIndex == 7) {
									inboundTestPoint = pixcache.GetPoint(testx - 1, testy);
									inboundTest2Point = pixcache.GetPoint(testx, testy + 1);
									if (inboundTest2Point->flag == flagIndex) {
										m = -1;
										preTestIndex = 7;
										continue;
									}
									if (inboundTestPoint->clr == 0) {
										testPt = inboundTestPoint;
										preTestIndex = 4;
										noNeedCalc = FALSE;
									}

								}
								else {
									preTestIndex = antiClockWise[testIndex];
								}
					if (testPt == mark1) {
						hasNext = FALSE;
						break;
					}
				}
				else {
					if (testIndex == 1) {
						inboundTestPoint = pixcache.GetPoint(testx, testy + 1);
						inboundTest2Point = pixcache.GetPoint(testx + 1, testy);
						if (inboundTest2Point->flag == flagIndex) {
							m = -1;
							preTestIndex = 1;
							continue;
						}
						if (inboundTestPoint->clr == 0) {
							testPt = inboundTestPoint;
							preTestIndex = 6;
							noNeedCalc = FALSE;
						}
					}
					else if (testIndex == 3) {
						inboundTestPoint = pixcache.GetPoint(testx + 1, testy);
						inboundTest2Point = pixcache.GetPoint(testx, testy - 1);
						if (inboundTest2Point->flag == flagIndex) {
							m = -1;
							preTestIndex = 3;
							continue;
						}
						if (inboundTestPoint->clr == 0) {
							testPt = inboundTestPoint;
							preTestIndex = 0;
							noNeedCalc = FALSE;
						}
					}
					else if (testIndex == 5) {
						inboundTestPoint = pixcache.GetPoint(testx, testy - 1);
						inboundTest2Point = pixcache.GetPoint(testx - 1, testy);
						if (inboundTest2Point->flag == flagIndex) {
							m = -1;
							preTestIndex = 5;
							continue;
						}
						if (inboundTestPoint->clr == 0) {
							testPt = inboundTestPoint;
							preTestIndex = 2;
							noNeedCalc = FALSE;
						}
					}
					else if (testIndex == 7) {
						inboundTestPoint = pixcache.GetPoint(testx - 1, testy);
						inboundTest2Point = pixcache.GetPoint(testx, testy + 1);
						if (inboundTest2Point->flag == flagIndex) {
							m = -1;
							preTestIndex = 7;
							continue;
						}
						if (inboundTestPoint->clr == 0) {
							testPt = inboundTestPoint;
							preTestIndex = 4;
							noNeedCalc = FALSE;
						}
					}

				}
				if (noNeedCalc) {
					preTestIndex = antiClockWise[testIndex];
				}
				break;
			}
			else {
				testPt->flag = -1;
			}

		}


		testPt = NULL;
	}
	if (testPt != NULL) {
		testPt->flag = flagIndex;
	}

	*outtestPt = testPt;

	return hasNext;

}
Point* ImageContoursTool::GetHeadPoint(PixelCache& pixcache, int x, int y, Point** markPt, BOOLEAN& isInnerDraw, int& preTestIndex) {
	BOOLEAN isAround = FALSE;
	int m;
	TestPoint subPtTmp;
	Point* pt1 = pixcache.GetPoint(x, y), * testPt, * testPt2, ** markPt2 = markPt;
	preTestIndex = -1;
	isInnerDraw = FALSE;
	(*markPt2) = NULL;
	if (pt1->flag != 0) {
		return NULL;
	}
	if (pt1->clr != 0) {
		return NULL;
	}
	if (pixcache.IsSingle(pt1, antiClockPoint)) {
		return NULL;
	}
	testPt = pixcache.GetPoint(pt1, antiClockPoint[0]);
	if (testPt != NULL) {
		if (testPt->clr != 0) {
			testPt2 = pixcache.GetPoint(pt1, antiClockPoint[1]);
			if (testPt2 != NULL) {
				if (testPt2->clr == 0)
				{
					testPt2 = pixcache.GetPoint(pt1, antiClockPoint[7]);
					if (testPt2 != NULL) {
						if (testPt2->clr == 0) {
							return NULL;
						}
					}
				}
				else {

				}
			}
		}
		else {
			isInnerDraw = TRUE;
		}

		if (isInnerDraw) {
			testPt = pixcache.GetPoint(pt1, antiClockPoint[2]);
			if (testPt->clr != 0) {
				preTestIndex = 1;
			}
			else {
				testPt = pixcache.GetPoint(pt1, antiClockPoint[6]);
				if (testPt->clr != 0) {
					preTestIndex = 5;
				}
				else {
					testPt = pixcache.GetPoint(pt1, antiClockPoint[3]);
					if (testPt->clr != 0) {
						preTestIndex = 2;
					}
					else {
						testPt = pixcache.GetPoint(pt1, antiClockPoint[5]);
						if (testPt->clr != 0) {
							preTestIndex = 4;
						}
						else {
							isAround = TRUE;
						}

					}
				}
			}
			if (isAround) {
				pt1->flag = -1;
				preTestIndex = -1;
				return NULL;
			}
		}
		else {
			for (m = 1; m < 4; m++) {
				subPtTmp = clockPoint[m];
				testPt = pixcache.GetPoint(pt1, subPtTmp);
				if (testPt == NULL)continue;
				if (testPt->clr == 0) {
					(*markPt2) = testPt;
					break;
				}
			}

		}
		if (FALSE) {
			testPt2 = pixcache.GetPoint(testPt, antiClockPoint[2]);
			if (testPt2->clr == 0) {
				testPt2 = pixcache.GetPoint(testPt, antiClockPoint[6]);
				if (testPt2->clr != 0) {
					isAround = TRUE;
				}
			}
			else {
				isAround = TRUE;
			}
			if (isAround) {
				return NULL;
			}

		}

	}

	return pt1;
}
#define LINEX 1
#define LINEY 2
#define LINESINGLE 4

ComplexNum*** ImageContoursTool::GetLinePixelComplex(GroupNode* rootGroup) {
	GroupNode* groupNode,* newGroupLineNode,* groupLineRootNode,* groupLineNode;
	ImagePointNode* pointNode,* newPointLineNode,* pointLineNode,*curPointRoot;
	int nodeindex = 0,endIndex,ptIndex;
	BOOLEAN isFinished;
	Point* startPt, * endPt,*lastPt;
	groupLineRootNode = newGroupLineNode = groupNode = (groupLineNode = NULL);
	pointNode = newPointLineNode = pointLineNode = NULL;
	groupNode = rootGroup;
do{
	ptIndex = 0;
	endIndex = 0;
	++nodeindex;
	pointNode = groupNode->ptGroup;
	curPointRoot = pointNode;
	CREATE_ALLOCA(newGroupLineNode, GroupNode);
	CREATE_ALLOCA(newPointLineNode, ImagePointNode);
	if (groupLineRootNode == NULL) {
		groupLineRootNode = newGroupLineNode;
	}
	else {
		groupLineNode->next = newGroupLineNode;
	}
	newGroupLineNode->ptGroup = newPointLineNode;
	groupLineNode = newGroupLineNode;
	pointLineNode = newPointLineNode; 
	isFinished = FALSE; 
	while (endIndex != (LINEX | LINEY)) {
		++ptIndex;
	
		pointNode = LinePointNode(pointNode, &startPt, &endPt, endIndex);
		if (startPt == NULL || endPt == NULL) {
			startPt = (endPt == NULL ? startPt : endPt);
			pointLineNode->point = startPt;
		}
		else { 
			pointLineNode->point = startPt;
			CREATE_ALLOCA(newPointLineNode, ImagePointNode);

			newPointLineNode->point = endPt;
			pointLineNode->nextNode = newPointLineNode;
			newPointLineNode->preNode = pointLineNode;

			CREATE_ALLOCA(pointLineNode, ImagePointNode);
			newPointLineNode->nextNode = pointLineNode;
			pointLineNode->preNode = newPointLineNode;
		}
		if (pointNode==NULL||startPt==NULL||endPt==NULL) {
			_log.Trace("null point \n");
		}
		if (endIndex == (LINEX | LINEY)) {
	 		if (pointLineNode->preNode->point == pointLineNode->preNode->preNode->point) {
	 			pointLineNode->preNode->preNode->nextNode = NULL;
	 		}
			//endPt = lastPt;  
			pointLineNode->point = curPointRoot->point;   
		} 

	}




} while ((groupNode = groupNode->next) != NULL);

	


	return NULL;
}

VOID ImageContoursTool::CreateTestPoint(GroupNode* groupLineRootNode,ComplexNum**** outComplex) {
	int r = 2, gsize = CalcGroupSize(groupLineRootNode) + 1, psize, gindex, pindex, nodeindex,ptIndex;
	GroupNode* groupNode= groupLineRootNode;
	ImagePointNode* pointNode;
	Point* prePt,* curPt;
	ComplexNum*** complexData,complexGroup;
//	(mTestPtArray) = new  Point * *[gsize];
	complexData = new ComplexNum * *[gsize];
	//mTestPtArray[gsize-1 ] = NULL;
	complexData[gsize-1 ] = NULL;
	gindex = 0;
	nodeindex = 0;
	ptIndex = 0;
	do {
		++nodeindex;
		pointNode = groupNode->ptGroup;
		prePt = NULL;
		psize = CalcPointNodeSize(pointNode) + 1;
		//(mTestPtArray)[gindex] = new Point * [psize];
	//	(mTestPtArray)[gindex][psize - 1] = NULL;
		(complexData)[gindex] = new ComplexNum * [psize];
		(complexData)[gindex][psize - 1] = NULL;
	//	_log.Trace("nodeindex:%d,line point x1:%d,\ty1:%d \n", nodeindex, pointNode->point->x, pointNode->point->y);
		pindex = 0;
		do {
			curPt = pointNode->point;
			++ptIndex;
		//	_log.Trace("nodeindex:%d,line point x1:%d,\ty1:%d \n", nodeindex, curPt->x, curPt->y);
			//(mTestPtArray)[gindex][pindex] = NULL;
			(complexData)[gindex][pindex] = NULL;
			if (curPt == NULL) {
				pointNode->preNode->nextNode = NULL;
				continue;
			}
		//	(mTestPtArray)[gindex][pindex] = new Point;
			(complexData)[gindex][pindex] = new ComplexNum(curPt->x,curPt->y);
		//	memcpy((mTestPtArray)[gindex][pindex], curPt, sizeof(Point));
			++pindex;
			if (prePt == NULL) {
				prePt = curPt;
				continue;
			}
			//	::DrawLine(hwnd, hdc, prePt->x, prePt->y, curPt->x, curPt->y);

			prePt = curPt;
		} while ((pointNode = pointNode->nextNode) != NULL);

		++gindex;
	} while ((groupNode = groupNode->next) != NULL);


	*outComplex = complexData;

}
#define DEBUG_LINE
ImagePointNode* ImageContoursTool::LinePointNode(ImagePointNode* pointNode,Point** lpStartPt, Point** lpEndPt,int &endIndex) {
	BOOLEAN isSinglePoint, isLineY, isLineX;
	Point* curPt, * startPt, * prePt, * endPt;
	int direct, preDirect;
	*lpStartPt = NULL;
	*lpEndPt = NULL;
	if (pointNode == NULL) {
		return NULL;
	}
	prePt = NULL;
	startPt = NULL;
	endPt = NULL;
	isLineX= (isLineY = FALSE);
	endIndex = 0;
	while (TRUE) {
		curPt = pointNode->point;
		isSinglePoint = TRUE;
#ifdef DEBUG_LINE
		if (curPt->x == 83 && curPt->y == 163) {
			curPt = curPt;
		}
#endif
		if (prePt == NULL) {
			prePt = curPt; 
		}
		else {
			if (!isLineY) {
				if (curPt->x == prePt->x) {
					direct = curPt->y - prePt->y;
					if (startPt == NULL) {
						preDirect = direct;
						startPt = prePt;
					}
					isLineX = TRUE;
					if (direct != preDirect) {
						endIndex = LINEX;
						isSinglePoint = FALSE;
						isLineX = (isLineY = FALSE);
					}

				}
				else {
					if (isLineX) {
						endIndex = LINEX;
						isSinglePoint = FALSE;
						isLineX = (isLineY = FALSE);
					}
				}

			}
			if (!isLineX && isSinglePoint) {
				if (curPt->y == prePt->y) {
					direct = curPt->x - prePt->x;
					if (startPt == NULL) {
						preDirect = direct;
						startPt = prePt;
					}
					isLineY = TRUE;
					if (preDirect != direct) {
						endIndex = LINEY;
						isSinglePoint = FALSE;
						isLineX = (isLineY = FALSE);
					}
				}
				else {
					if (isLineY) {
						endIndex = LINEY;
						isSinglePoint = FALSE;
						isLineX = (isLineY = FALSE);
					}
				}


			}

			if (endIndex == LINEX || endIndex == LINEY) {
				endPt = prePt;
				if (pointNode->nextNode == NULL) {
					endIndex = LINEX | LINEY;

				}
			}
			if (!(isLineX || isLineY) && isSinglePoint) {
				endIndex = LINESINGLE;
				startPt = prePt;
				endPt = curPt;
			} 
	
		}
		pointNode = pointNode->nextNode;
		if (pointNode == NULL&& endIndex==0) {
			endIndex = LINEX | LINEY;
			if (startPt != NULL) {
				endPt = prePt;
			}
			else {
				startPt = curPt;
			}
		}
		if (endIndex != 0) {
			*lpStartPt = startPt;
			*lpEndPt = endPt;
			break;
		}
		prePt = curPt;

	}
	
	return pointNode;



}
ComplexNum*** ImageContoursTool::GetComplexNum(Point*** bitmapPointPtr) {

	PixelCache pixcache(bitmapPointPtr, miBitmapWidth, miBitmapHeight);
	Point* curPt, * markPt1, * markPt2, * nextPt;
	BOOLEAN isInnerDraw, hasNext, noNeedCalc;
	int i, j, m, preTestIndex, flagIndex, testIndex,nodeindex;
	ImagePointNode* newPointNode, * rootPointNode, * pointNode;
	GroupNode* rootGroup=NULL, * newGroupNode, * groupnode;
	ComplexNum*** outCom;
	flagIndex = 0;

	for (i = 0; i < miBitmapHeight; i++) {
		for (j = 0; j < miBitmapWidth; j++) {
			curPt = GetHeadPoint(pixcache, j, i, &markPt2, isInnerDraw, preTestIndex);
			if (curPt == NULL) {
				continue;
			} 

			++flagIndex;
		//	_log.Trace("test flag:%d,point x:%d,y:%d\n", flagIndex, curPt->x, curPt->y);
			CREATE_ALLOCA(newPointNode, ImagePointNode);
			CREATE_ALLOCA(newGroupNode, GroupNode);
			newPointNode->point = curPt;
			newGroupNode->ptGroup = newPointNode;
			if (rootGroup == NULL) {
				rootGroup = newGroupNode;
				rootPointNode = newPointNode;
			}
			else {
				groupnode->next = newGroupNode;
			}
			groupnode = newGroupNode;
			pointNode = newPointNode;
			hasNext = TRUE;
			curPt->flag = flagIndex;
			markPt1 = curPt;
			while (hasNext) {
				hasNext = GetCheckPoint(pixcache, flagIndex, pointNode, curPt, markPt1, markPt2, isInnerDraw, preTestIndex, &nextPt);
			//	_log.Trace("test flag:%d,point x:%d,y:%d\n", flagIndex, nextPt->x, nextPt->y);
				CREATE_ALLOCA(newPointNode, ImagePointNode);
				newPointNode->preNode = pointNode;
				pointNode->nextNode = newPointNode;
				newPointNode->point = nextPt;
				pointNode->nextNode = newPointNode;
				pointNode = newPointNode;
				curPt = nextPt;
			}






		}
	}

	CreateTestPoint(rootGroup,&outCom);
	return outCom;
	return GetLinePixelComplex(rootGroup);
} 
////////////////////////////////////////////
//需要修改stack保留值 和 heap保留值 默认值
	ComplexNum*** ImageContoursTool::ReadBitmapPointToComplex(HWND hwnd, LPCWSTR filename, int& outPtSize) {

		Point*** bitmapPointPtr = NULL,*newPt;
		UCHAR* pBits = NULL;
		ComplexNum*** resultComplex;
		mhBitmap = (HBITMAP)::LoadImageW(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS | LR_LOADFROMFILE);
		int bitRawsize, nBytes, i, j;
		if (mhBitmap == NULL) {
			::MessageBox(hwnd, L"图像文件b.bmp不存在", NULL, MB_OK);
			//::DestroyWindow(hwnd);
			return NULL;
		}
		//Point ptarr[272 *300] = { 0 };
		GetObject(mhBitmap, sizeof(BITMAP), (LPVOID)&mBitmap);
		miBitmapHeight = mBitmap.bmHeight; miBitmapWidth = mBitmap.bmWidth;
		if (miBitmapHeight* miBitmapWidth > 112000 ) {
			::MessageBox(hwnd, L"图像过大 需高<280,宽<400", NULL, MB_OK);
		//	::DestroyWindow(hwnd);
			return NULL;
		}
		bitRawsize = miBitmapHeight * mBitmap.bmWidthBytes + 1;//bmWidthBytes=颜色字节数 TRGB=32位 4字节
		CREATE_ALLOCA_ARRAY(pBits, bitRawsize, UCHAR);
		nBytes = GetBitmapBits(mhBitmap, bitRawsize, pBits);
		CREATE_ALLOCA_ARRAY(bitmapPointPtr, miBitmapHeight, Point**);
		for ( i = 0; i < miBitmapHeight; i++) {
			//(bitmapPointPtr)[i] =   new Point * [bitmap.bmWidth];
			CREATE_ALLOCA_ARRAY(bitmapPointPtr[i], miBitmapWidth, Point*);
			for ( j = 0; j < miBitmapWidth; j++) { 
			 	CREATE_ALLOCA(newPt, Point); 
				(bitmapPointPtr)[i][j] = newPt; 
			}

			//_log.Trace("i:%d,width:%d,sum:%d\n", i, miBitmapWidth,i*miBitmapWidth*sizeof(Point));
		}
		ReadBitmapPix(pBits, bitmapPointPtr);
		resultComplex = GetComplexNum(bitmapPointPtr);

		return resultComplex;
	}







Point* PixelCache::GetPoint(int x, int y) {
	return bitmapPointPtr[y][x];
}
PixelCache::PixelCache(Point*** bitmap, int bw, int bh) {
	bitmapPointPtr = bitmap;
	bitmapHeight = bh;
	bitmapWidth = bw;
}
BOOLEAN PixelCache::IsSingle(Point* pt, TestPoint* testChecker) {
	BOOLEAN isSingle = TRUE;
	int i, testx, testy;
	TestPoint testpoint;
	Point* checkPoint;
	for (i = 0; i < 9; i++) {
		testpoint = testChecker[i];
		testx = testpoint.x + pt->x;
		testy = testpoint.y + pt->y;
		if (testx < 0 || testy < 0 || testx >= bitmapWidth || testy >= bitmapHeight) {
			continue;
		}
		checkPoint = bitmapPointPtr[testy][testx];
		if (checkPoint->clr != 0) {
			isSingle = FALSE;
			break;
		}
	}
	return isSingle;
}
Point* PixelCache::GetPoint(Point* pt, TestPoint& testpoint, int& testx, int& testy) {
	int i;
	Point* retPt;
	testx = testpoint.x + pt->x;
	testy = testpoint.y + pt->y;
	if (testx < 0 || testy < 0 || testx >= bitmapWidth || testy >= bitmapHeight) {
		return NULL;
	}
	retPt = bitmapPointPtr[testy][testx];
	return retPt;
}
Point* PixelCache::GetPoint(Point* pt, TestPoint& testpoint) {
	int i, testx, testy;
	return GetPoint(pt, testpoint, testx, testy);

}
BOOLEAN  PixelCache::IsAround(Point* testPoint, TestPoint* pointChecker) {
	int i, j, testx, testy, count;
	BOOLEAN isAround = TRUE;
	Point* tmpPoint;
	TestPoint chkPt;
	count = 0;
	for (i = 0; i < 9; i++) {
		chkPt = pointChecker[i];
		testx = chkPt.x + testPoint->x;
		testy = chkPt.y + testPoint->y;
		if (testx < 0 || testy < 0 || testx >= bitmapWidth || testy >= bitmapHeight) {
			continue;
		}
		tmpPoint = bitmapPointPtr[testy][testx];
		if (tmpPoint->clr != 0) {
			isAround = FALSE;
			break;
		}


	}
	return isAround;
}







