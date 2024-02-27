
#include"bass_audio.h"
#include "toolutil.h"


VOID AudioWaveStaff::InitPalette(HWND hwnd, HDC hdc) {
	if (m_drawBuffer == NULL) {
		RECT winRect;
		int SPECWIDTH, SPECHEIGHT,subheight=50,subx=70;
		::GetClientRect(hwnd, &winRect);
		SPECWIDTH = winRect.right - winRect.left;
		SPECHEIGHT = (winRect.bottom - winRect.top) >> 1;
		int clrSize = 4; 
		RGBQUAD* clrData;
		//
		HBITMAP secbitmap;
		BITMAPINFO *binfo = (BITMAPINFO*)alloca(sizeof(BITMAPINFO)+sizeof(RGBQUAD) * (clrSize-1));;;
		BITMAPINFOHEADER* bh = &binfo->bmiHeader;
		int a,norWaveWidth; 
		memset(binfo, 0, sizeof(BITMAPINFO) + sizeof(RGBQUAD) * (clrSize - 1));
		RGBQUAD* pal = binfo->bmiColors;
			//(RGBQUAD*)(data + sizeof(BITMAPINFOHEADER));
		//标准化图像宽度 很重要 否则位图会偏移
		// 2*((bmWidth * bmBitsPixel + 15) / 16); 
		norWaveWidth = (SPECWIDTH - subx) >>4;
		if (norWaveWidth << 4 != (SPECWIDTH - subx)) {
			norWaveWidth = (norWaveWidth + 1) << 4;
		}

		bh->biSize = sizeof(*bh);
		bh->biWidth = norWaveWidth;
		bh->biHeight = SPECHEIGHT- subheight; // upside down (line 0=bottom)
		bh->biPlanes = 1;
		bh->biBitCount = 8;
		bh->biClrUsed = bh->biClrImportant =4;

		a = 0;
		pal[a].rgbGreen = 0xff;
		pal[a].rgbBlue = 0xff;
		pal[a].rgbRed = 0xff;

		a++;
		pal[a].rgbGreen = 255;
		pal[a].rgbBlue = 22;
		pal[a].rgbRed = 0;
		a++;
		pal[a].rgbGreen = 0;
		pal[a].rgbBlue = 0;
		pal[a].rgbRed = 255;
		secbitmap = CreateDIBSection(0, (BITMAPINFO*)bh, DIB_RGB_COLORS, &m_drawBuffer, NULL, 0);
		m_bitDc = CreateCompatibleDC(0);//创建兼容HDC
		::SelectObject(m_bitDc, secbitmap);
		BITMAP bitm;
		::GetObject(secbitmap, sizeof(BITMAP), &bitm);

		 

			

		m_coordDc = ::CreateCompatibleDC(hdc);
		m_nCoordbitmap = ::CreateCompatibleBitmap(hdc, SPECWIDTH, SPECHEIGHT);
	 
		m_rtWave.top = 0;
		m_rtWave.left = subx;
		m_rtWave.bottom = SPECHEIGHT - subheight;
		m_rtWave.right = norWaveWidth+ subx;

		m_rtDc.top = 0;
		m_rtDc.left = 0;
		m_rtDc.right= SPECWIDTH;
		m_rtDc.bottom = SPECHEIGHT;
		m_oCoorbitmap = (HBITMAP)SelectObject(m_coordDc, m_nCoordbitmap);
		m_hcoorFont =CreateFont(
			-11/*高度*/, -5/*宽度*/, 0/*不用管*/, 0/*不用管*/, 100 /*一般这个值设为400*/,
			FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
			DEFAULT_CHARSET, //这里我们使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, //这行参数不用管
			DEFAULT_QUALITY, //默认输出质量
			FF_DONTCARE, //不指定字体族*/
			L"微软雅黑" //字体名
		);



	}
}
VOID AudioWaveStaff::ClearDraw(const HWND& hwnd) {
	m_fcoordPerBits = -1;
}
VOID AudioWaveStaff::DrawWave(HWND hwnd, HDC hdc, VOID* buffer, int size, int chans, int perSampleBitsSize, float scale) {
	int i, j, N = size, irsh = -1, loop, snum, prey = -1, prex, stepY, stepX =1, sy, sx
		, iwbw = GetWidth(m_rtWave), iwbh = GetHeight(m_rtWave)
		, iwbx = m_rtWave.left, iwby = 0, normh, ilinex, iliney, icoordSplitMax = 1 << 4;;
	const int zeroX = m_rtWave.left, zeroY = m_rtWave.bottom;
	float fcoordScale,fk; 
	int prey2 = -1, iliney2, sy2,prex2, stepY2,inosignMax=(1<<(perSampleBitsSize))-1;
	//::SelectObject(m_coordDc, m_oCoorbitmap);
	memset(m_drawBuffer, 0, iwbw * iwbh);

	switch (perSampleBitsSize) {
	case 8: 
		irsh = 0;
		break;
	case 16: 
		irsh = 1;
		break;
	case 32: 
		irsh = 2;
		break;
	}
	if (perSampleBitsSize > 16) {
		icoordSplitMax = 20;
		inosignMax = 1;
	}

	if (perSampleBitsSize != m_fcoordPerBits) {
		m_fcoordPerBits = perSampleBitsSize; 
	 	DrawCoordinate(hwnd, m_coordDc, icoordSplitMax, perSampleBitsSize, stepX);
		BitBlt(hdc, 0, 0, GetWidth(m_rtDc), GetHeight(m_rtDc), m_coordDc, 0, 0, SRCCOPY);
	}
	//DrawCoordinate(hwnd, m_coordDc, icoordSplitMax, perSampleBitsSize, stepX);
	normh = iwbh - (iwbh % icoordSplitMax);
	//normh = normh-1 ;
	//分界线0点
	memset(((BYTE*)m_drawBuffer) +(normh >> 1) * iwbw, 3, iwbw);

//	if(1==0)
	for (j = 0; j < chans; j++) {
		prey = -1;
		prex = 0;
		for (i = 0; i < N/ stepX; i++) {
			switch (irsh) {
			case 0:
				sy = ((BYTE*)buffer)[i * chans + j]*1.0/ inosignMax * normh; 
				break;
			case 1: 
				sy = (32768+((SHORT*)buffer)[i * chans + j])*1.0/ inosignMax * normh; 
				break;
			case 2:
				sy = (1+((float*)buffer)[i * chans + j]) * (normh>>1); 
				break;
			}

			if (sy < 0) {
				throw 1;
			}
			stepY = (sy < prey) ? -1 : 1;
			if (prey != -1) {
				sx = prex + stepX;
				fk = (sy - prey) * 1.0 / stepX;
			}
			else {
				prex = 0;
				sx = 0;
				prey = sy;
			}
			ilinex = prex;
			iliney = prey;

			do {
				((BYTE*)m_drawBuffer)[(int)(((iliney)) * iwbw + ilinex)] = 2 - j;
				if (sy == iliney && (sx) == (ilinex)) {
					break;
				}
				prey2 = iliney;
				prex2 = ilinex;
				if (ilinex < sx) {
					++ilinex;
					iliney = prey + fk * ilinex;
				}
				else {
					iliney += stepY;;
				}
				if ((stepY < 0 && iliney < sy) || (stepY > 0 && iliney > sy)) {
					iliney = sy;
				}
				stepY2 = (prey2 > iliney) ? -1 : 1;
				iliney2 = prey2;
				do {
					((BYTE*)m_drawBuffer)[(int)(iliney2)*iwbw + prex2] = 2 - j;
					if (iliney2 == iliney) {
						break;
					}
					iliney2 += stepY2;
				} while (TRUE);


			} while (TRUE);
			prey = sy;
			prex = sx;
		}
	}
	 
	BitBlt(hdc,iwbx , 0, iwbw, iwbh, m_bitDc, 0, 0, SRCCOPY);//bitmap位图数据从左往右 从下往上 
}


VOID AudioWaveStaff::DrawCoordinate(HWND hwnd, HDC hdc,int rule,int perSampleBitsSize,int scalePerPixX) {
	int h , lineWidth, lineHeight, iorginY, ioriginX,normh;
	int inosignMax=(1<<perSampleBitsSize),inormsign;
	int fX, iclineheight, iclinewidth, itextY, idcX, i, j, inumLen, ixText, idcY, inum1, inum2, inum3
		, ifontheight = 15, ifontwidth = 40, perrule, icoordLinLength = 10,
		isplitXpiece=20,isplitXsection,normw;
	HGDIOBJ hcooordText = m_hcoorFont, holdFont;
	float fnosignscale, subXtextLen=50, prenormrule, itextnum;
	 
	h = GetHeight(m_rtWave);
	ioriginX = m_rtWave.left - 1;

	inormsign = inosignMax - (inosignMax % rule);
	normh = h - (h % rule);
	perrule = normh / rule;
	prenormrule = inormsign*1.0 / rule; 
	lineWidth = GetWidth(m_rtDc);
	lineHeight = GetHeight(m_rtDc);
	iorginY = m_rtWave.top + m_rtWave.bottom;
	normw = (lineWidth - ioriginX + 1) / scalePerPixX; 
	if (prenormrule <= 1) {
		rule = 20;
		prenormrule =2.0 / rule;
		inormsign = 2;
		inosignMax =2;
		normh = h - (h % rule);
		perrule = normh / rule;
	}
	FillRect(m_coordDc, &m_rtDc, (HBRUSH) ::GetStockObject(WHITE_BRUSH));
	MoveToEx(hdc, ioriginX, iorginY, NULL);
	::LineTo(hdc, ioriginX + (lineWidth), iorginY); 
	MoveToEx(hdc, ioriginX, iorginY, NULL);
	::LineTo(hdc, ioriginX, iorginY -lineHeight); 
	fX = ioriginX - 20;
	char ytext[20] = { 0 };
	RECT txRect;
	iclineheight = lineHeight; 
	holdFont=::SelectObject(hdc, hcooordText);
	for (i = 0; i<=rule;++i) {
		itextY = i * perrule; 
		itextnum =  i* prenormrule - (inosignMax>>1);
		if (itextY > h) {
			break;
		} 
		idcY = (iorginY - itextY ) -1;
		memset(ytext, 0, sizeof(ytext));
		NumToString(ytext, itextnum);
		txRect.top = idcY - (ifontheight >> 1);
		txRect.bottom = txRect.top + ifontheight;
		txRect.left = ioriginX - ifontwidth-15;
		txRect.right = txRect.left + ifontwidth;
		DrawTextA(hdc, ytext, -1, &txRect, DT_CENTER);

		MoveToEx(hdc, ioriginX, idcY, NULL);
		::LineTo(hdc, ioriginX - icoordLinLength, idcY);
	}
	iclinewidth = lineWidth;
	isplitXsection = normw/ isplitXpiece;
	isplitXsection = isplitXsection - (isplitXsection % 10);

	
	for (i = 0; iclinewidth > 0; iclinewidth = iclinewidth - subXtextLen, ++i) {
		if (i == 0) {
			continue;
		}
		ixText = i * isplitXsection ;
		idcX = (ioriginX + i * isplitXsection* scalePerPixX) ;
		memset(ytext, 0, sizeof(ytext));
		NumToString(ytext, ixText);
		txRect.top = iorginY +( icoordLinLength<<1);
		txRect.bottom = txRect.top + ifontheight;
		txRect.left = idcX - (ifontwidth>>1);
		txRect.right = txRect.left + ifontwidth;
		DrawTextA(hdc, ytext, -1, &txRect, DT_CENTER);

		MoveToEx(hdc, idcX, iorginY, NULL);
		::LineTo(hdc, idcX, iorginY + icoordLinLength);
	}

	 ::SelectObject(hdc, holdFont);

}
