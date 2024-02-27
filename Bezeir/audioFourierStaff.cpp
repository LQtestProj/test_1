#include "bass_audio.h"

#include "toolutil.h"
#include "framework.h"
#include "fourierMathTool.h"
int AudioFourierStaff::DataNormalization(VOID* buffer, int iblen, FType** normFloatBuffer, int perSampleBitsSize, int chans) {
	int normsamplelen = iblen, i, j, ibsize, irshchan = 0, iunsignmax = (1 << perSampleBitsSize) - 1;
	VOID* normBufferTmp;
	float fnorm;
	int bc;
	if (iblen <= 1024) {
		normsamplelen = 1024;
	}
	else if (iblen > 1024) {
		normsamplelen = 2048;
	}
	switch (chans) {
	case 1:
		irshchan = 0;
		break;
	case 2:
		irshchan = 1;
		break;

	}
	ibsize = normsamplelen * sizeof(FType) * 2;//*2= 虚数面的X和实数Y
	if (m_iFFTSampleSize < normsamplelen) {
		if (*normFloatBuffer != NULL) {
			free(m_fftBuffer);
		}
		*normFloatBuffer =(FType*) malloc(ibsize);
		m_iFFTSampleSize = normsamplelen;
	}
	normBufferTmp = *normFloatBuffer;
	memset(normBufferTmp, 0, ibsize);
	for (i = 0; i < normsamplelen; i++) {
		if (i >= iblen) {
			break;
		}
		switch (perSampleBitsSize) {
		case 8:
			fnorm = (((iunsignmax + 1) >> 1) - ((BYTE*)buffer)[i << irshchan]) * 1.0 / (iunsignmax >> 1);
			break;
		case 16:
			fnorm = (((SHORT*)buffer)[i << irshchan]) / 32767.0;
			if (fnorm < -1) {
				fnorm = -1;
			}
			//(((iunsignmax + 1) >> 1) - ((SHORT*)buffer)[i << irshchan]) *1.0/ (iunsignmax>>1);
			break;
		case 32:
			fnorm = ((float*)buffer)[i << irshchan];
			break;
		}
		((float*)normBufferTmp)[i << 1] = fnorm;
	}
	return normsamplelen;
}
VOID AudioFourierStaff::DrawFourier(HWND hwnd, HDC hdc, VOID* splitBuffer,int perSampleBitsSize,int chans, int size) {
	int zeroX;
	int  zeroY, lineWidth = GetWidth(m_rtDc), lineHeight = GetHeight(m_rtDc) , iclineheight, i, j,
		N = size, w, h, suby, ixlen = 11, ibindex,iepictotalx;
	float itextxsub = 50, itextysub = 10, scale = 800, fvalue;
	FourierStruct* fs;
	BYTE* buffer;
	BYTE clr;
	BOOLEAN isOutBound = FALSE;
	FourierStruct* fsArray = NULL;
	int ifsize, usedSampleSize; 
	FType* floatBuffer;
	ifsize = DataNormalization(splitBuffer, size, &m_fftBuffer, perSampleBitsSize, chans);
	if ( m_fourierStructCache == NULL) {
		  m_fourierStructCache = (FourierStruct*)malloc(sizeof(FourierStruct) * ifsize);
	}
	memset( m_fourierStructCache, 0, sizeof(FourierStruct) * ifsize);
	FourierMathTool::FFT2AndFFT4((FType*)m_fftBuffer,  m_fourierStructCache, ifsize, usedSampleSize);
	fsArray = m_fourierStructCache;
	zeroX = m_rtWave.left;
	zeroY = lineHeight- m_rtWave.bottom;
	w = lineWidth;
	h = lineHeight;
	buffer = (BYTE*)m_drawBuffer;
	memset(buffer, 0, w * h);
	DrawCoordinate(hwnd, hdc, zeroX, zeroY, lineWidth, lineHeight, itextxsub, itextysub, scale);

	for (i = 1; i <N ; i++) {
		fs = &(fsArray[i]);
		fvalue = fs->amplitude;
		fvalue = sqrt(fvalue) * scale - 5;
		if ((fvalue + zeroY) >= h) {
			fvalue = h - zeroY - 1;
		}
		if (fvalue < 0) {
			fvalue = 0;
		}
		clr = (i % 2) + 1;
		suby = fvalue;
		while (suby != 0) {
			for (j = 0; j < ixlen; j++) {
				iepictotalx =  zeroX + i * ixlen + j;
				if (iepictotalx >= lineWidth) {
					isOutBound = TRUE;
					break;
				}
				buffer[(int)(zeroY + suby) * w + iepictotalx] = clr;
			}
			--suby;
		}
		if (isOutBound) {
			break;
		}

	}

	BitBlt(hdc, 0, m_rtDc.top, w, h, m_bitDc, 0, 0, SRCCOPY);//bitmap位图数据从左往右 从下往上

}
VOID AudioFourierStaff::InitPalette(HWND hwnd, HDC hdc) {
	if (m_drawBuffer == NULL) {
		RECT winRect;
		int SPECWIDTH, SPECHEIGHT, subHeight = 50,iwinHalfHeight;
		::GetClientRect(hwnd, &winRect); //获取窗体大小不包括标题
		iwinHalfHeight = ((winRect.bottom - winRect.top) >> 1);
		SPECWIDTH = winRect.right - winRect.left;
		SPECHEIGHT = iwinHalfHeight;

		//m_drawBuffer = malloc(SPECHEIGHT * SPECWIDTH);
		//memset(m_drawBuffer, 1, SPECHEIGHT * SPECWIDTH);
		HBITMAP specbmp;
		BYTE data[1000] = { 0 };
		BITMAPINFOHEADER* bh = (BITMAPINFOHEADER*)data;
		int a;
		RGBQUAD* pal = (RGBQUAD*)(data + sizeof(*bh));
		bh->biSize = sizeof(*bh);
		bh->biWidth = SPECWIDTH;
		bh->biHeight = iwinHalfHeight -subHeight; // upside down (line 0=bottom)
		bh->biPlanes = 1;
		bh->biBitCount = 8;
		bh->biClrUsed = bh->biClrImportant = 4;

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
		pal[a].rgbBlue = 22;
		pal[a].rgbRed = 255;
		a++;
		pal[a].rgbGreen = 0;
		pal[a].rgbBlue = 0;
		pal[a].rgbRed = 0;
		m_bitDc = CreateCompatibleDC(0);
		specbmp = CreateDIBSection(m_bitDc, (BITMAPINFO*)bh, DIB_RGB_COLORS, &m_drawBuffer, NULL, 0);
		SelectObject(m_bitDc, specbmp);
		m_iWidth = SPECWIDTH;
		m_iHeight = SPECHEIGHT;
		m_iy = SPECHEIGHT + (subHeight << 1);
		m_ix = 0;
		m_rtDc.top = iwinHalfHeight+subHeight;
		m_rtDc.left = 0;
		m_rtDc.bottom = iwinHalfHeight<<1;
		m_rtDc.right = SPECWIDTH;

		m_rtWave.top = 0;
		m_rtWave.left = 40;
		m_rtWave.bottom = GetHeight(m_rtDc) - 20;
		m_rtWave.right = SPECWIDTH ;


	}

}
VOID AudioFourierStaff::DrawCoordinate(HWND hwnd, HDC hdc, int zeroX, int zeroY, int lineWidth,
	int lineHeight, float subXtextLen, float subYtextLen, float scale) {
	int i, j, w = GetWidth(m_rtDc), h = GetHeight(m_rtDc);
	int bottom = h-m_rtWave.bottom;
	BYTE* dbuffer = (BYTE*)m_drawBuffer;
	//zeroY = (m_iHeight + __x);
	memset(dbuffer + (bottom * w ), 3, w);
	for (i = 0; i < h; i++) {
		dbuffer[i * w + zeroX] = 3;
	}
	 
}

int AudioFourierStaff::SortEpic(FourierStruct** fsArray, int N) {
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


int AudioFourierStaff::NoImgFFT(VOID* fftBuffer, int N, BOOLEAN isSkipEpic) {
	int i, j;
	FourierStruct** fdata;
	FourierStruct* fs, * fs41, * fs43;
	int halfN = N >> 1;
	int evenLeft, evenRight;
	int k = 0, k2, k4, n, k41, k43;
	float rleft, rright, ileft = 0, iright = 0, frsum, fisum, angle, rc, is, rfs, ifs; 
	float* fout = (float*)fftBuffer;

	//4基参数
	float angle41, angle43, r41c, r43c, i41s, i43s, fourN = N >> 2;
	float r40, i40, r41, i41, r42, i42, r43, i43;
	float r41sum, i41sum, r43sum, i43sum, r41fs, i41fs, r43fs, i43fs;
	//加窗
	for (i = 0; i < N; i++) {
 	((float*)fftBuffer)[i<<1] = ((float*)fftBuffer)[i << 1] * HANN(i, N);
	}


	if (m_fourierStructCache == NULL) {
		//m_fourierStructCache = (FourierStruct**)malloc(sizeof(FourierStruct*) * N);
		memset(m_fourierStructCache, 0, sizeof(FourierStruct*) * N);
	}
	//usedEpicSize = N;
	//fdata = m_fourierStructCache; 

	//基2
	for (k = 0; k < halfN; k++) {
		k2 = k * 2;
		fs = fdata[k2];
		if (fs == NULL) {
			fs = (FourierStruct*)malloc(sizeof(FourierStruct));
		}
		memset(fs, 0, sizeof(FourierStruct));
		fdata[k2] = fs;
		fs->freq = k2;
		for (n = 0; n < halfN; n++) {
			evenLeft = n << 1;
			evenRight = (halfN + n) << 1;
			rleft = fout[evenLeft];
			rright = fout[evenRight];
			ileft = fout[evenLeft + 1];
			iright = fout[evenRight + 1];

			frsum = rleft + rright;
			fisum = ileft + iright;
			angle = (TWO_PI / N) * k2 * n;
			rc = cos(angle);
			is = sin(angle);
			//(a+ib)(c-id)
			rfs = frsum * rc + fisum * is;
			ifs = fisum * rc-frsum * is  ;

			fs->real = fs->real + rfs;
			fs->img = fs->img + ifs;

		}
		fs->amplitude = sqrt(fs->real * fs->real + fs->img * fs->img) / N;
		fs->phase = atan(fs->img / fs->real);//不能用atan 需使用atan2获得phase
		throw 1;
	}
	//基4
	for (k = 0; k < fourN; k++) {
		k4 = k * 4;
		k41 = k4 + 1;
		k43 = k4 + 3;
		fs41 = fdata[k41];
		if (fs41 == NULL) {
			fs41 = (FourierStruct*)malloc(sizeof(FourierStruct));
		}
		memset(fs41, 0, sizeof(FourierStruct));
		fdata[k41] = fs41;
		fs41->freq = k41;
		fs43 = fdata[k43];
		if (fs43 == NULL) {
			fs43 = (FourierStruct*)malloc(sizeof(FourierStruct));
		}
		memset(fs43, 0, sizeof(FourierStruct));
		fdata[k43] = fs43;
		fs43->freq = k43;
		for (n = 0; n < fourN; n++) {


			r40 = fout[(n + ((0 *  N)>> 2)) << 1];//n+0N/4
			i40 = fout[((n + ((0 * N) >> 2)) << 1)+1];
			r41 = fout[((n + ((1 * N )>> 2)) << 1)];//n+1N/4
			i41 = fout[((n + ((1 * N) >> 2)) << 1)+1];
			r42 = fout[((n + ((2 * N) >> 2)) << 1)];//n+2N/4
			i42 = fout[((n +(( 2 * N) >> 2)) << 1) + 1];
			r43 = fout[((n +(( 3 * N )>> 2)) << 1)];//n+3N/4
			i43 = fout[((n +(( 3 * N )>> 2)) << 1) + 1];
			 
			r41sum = r40 - r42 + i41 - i43;
			i41sum = i40 - i42 - r41 + r43;
			r43sum = r40 - i41 - r42 + i43;
			i43sum = i40 - i42 + r41 - r43;

			angle41 = (TWO_PI / N) * k41 * n;
			r41c = cos(angle41);
			i41s = sin(angle41);

			angle43 = (TWO_PI / N) *k43* n;
			r43c = cos(angle43);
			i43s = sin(angle43);
			 
			r41fs = r41sum * r41c + i41sum * i41s;
			i41fs = i41sum * r41c - r41sum * i41s;

			r43fs = r43sum * r43c + i43sum * i43s;
			i43fs = i43sum * r43c - r43sum * i43s;
			 
			fs41->real = fs41->real + r41fs;
			fs41->img = fs41->img + i41fs;
			fs43->real = fs43->real + r43fs;
			fs43->img = fs43->img + i43fs;
		}
		fs41->amplitude = sqrt(fs41->real * fs41->real + fs41->img * fs41->img) / N;
		fs41->phase = atan(fs41->img / fs41->real);
		fs43->amplitude = sqrt(fs43->real * fs43->real + fs43->img * fs43->img) / N;
		fs43->phase = atan(fs43->img / fs43->real);
	}
	//共轭 排除混叠
	for (i = N - 1; i >= halfN; i--) {
		fs = fdata[i];
		fs->freq = fs->freq - N;
	}


	if (isSkipEpic) {

		return SortEpic(fdata, N);
	}
	else {
		return N;
	}


}