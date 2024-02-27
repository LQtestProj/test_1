#pragma once
#include "pro.h"
 
#include "popMenu.h"

#define HA0 (25.0/46)
#define HANN(n,N) (HA0-((1-HA0)*cos(TWO_PI*((n)*1.0/((N)-1)))))


typedef VOID ( playerEndCallback)(VOID*);
//https://www.cnblogs.com/cheney23reg/archive/2010/08/08/1795067.html
  class  WaveHead {
	int riffhex = 0x46464952;
	//正常情况 数据大小+头大小-8
	int ChunkSize;
	int wavehex =  0x45564157;
	int fmthex =  0x20746d66;
	/*
	Subchunk1Size:
	10H：subchunk1没有附加信息，常规size，
	e.g:12H：chunk1包含附加信息，chunk size不固定
	*/
	int Subchunk1Size = 0x10;
	//0x1 PCM  size:16
	//0x3 IEEE float  size:18
	//0x2 Mircsoft ADPCM  size:18
	//0x6 ITU G.711 a-law  size:18
	short AudioFormat = 1;
	//声道数 1,2,4,.. etc
	short NumChannels;
	//采样频率 eg 44100
	int SampleRate;
	//音频数据传送速率,采样率×每次采样大小
	int ByteRate;
	//每次采样的大小 = 采样精度*声道数/8
	short BlockAlign;
	//每个声道的采样精度 16=2字节 e.g. 0xFFFF-----8=1字节 e.g. 0xFF
	short BitsPerSample;
	int datahex =  0x61746164;
	//正常情况 数据大小
	int Subchunk2Size;
	//正常 PCM未压缩数据 格式 44字节
	BYTE m_buffer[44];
public:
	static int HEAD_SIZE  ;
	WaveHead(  short numChannels, int sampleRate, short bitsPerSample, int dataSize) {
		ChunkSize = dataSize+ HEAD_SIZE -8;
		NumChannels = numChannels;
		SampleRate = sampleRate;
		BitsPerSample = bitsPerSample;
		Subchunk2Size = dataSize;
		BlockAlign = NumChannels * BitsPerSample / 8;
		ByteRate = SampleRate * BlockAlign; 
	}
	BYTE* GetHead() { 
		memset(m_buffer, 0, HEAD_SIZE);
		*(((int*)m_buffer) + 0) = riffhex;
		*(((int*)m_buffer) + 1) = ChunkSize;
		*(((int*)m_buffer) + 2) = wavehex;
		*(((int*)m_buffer) + 3) = fmthex;
		*(((int*)m_buffer) + 4) = Subchunk1Size; 
		*(((int*)m_buffer) + 5) = (NumChannels <<16)| AudioFormat ;
		*(((int*)m_buffer) + 6) = SampleRate;
		*(((int*)m_buffer) + 7) = ByteRate;
		*(((int*)m_buffer) + 8) = (BitsPerSample <<16)| BlockAlign ;
		*(((int*)m_buffer) + 9) = datahex;
		*(((int*)m_buffer) + 10) = Subchunk2Size;

		return m_buffer;
	}


};
  typedef WaveHead WaveHeadPcmFormat;


  class AudioFourierStaff {
	  int m_iFFTSampleSize = 0;
	  int m_iWidth, m_iHeight,m_ix,m_iy;
	  FType* m_fftBuffer = NULL;
	  VOID* m_drawBuffer = NULL;
	  HDC m_bitDc;	  RECT m_rtDc, m_rtWave;
	  VOID InitPalette(HWND hwnd, HDC hdc);
	  int SortEpic(FourierStruct** fsArray, int N );
  public:
	  int DataNormalization(VOID* buffer, int iblen, FType** normFloatBuffer, int perSampleBitsSize, int chans);
	  AudioFourierStaff(HWND hwnd, HDC hdc){ 
		  InitPalette(hwnd, hdc); 
	  }
	  FourierStruct* m_fourierStructCache = NULL;
	  int NoImgFFT(VOID* fftBuffer, int N, BOOLEAN isSkipEpic=FALSE);
	  VOID DrawFourier(HWND hwnd, HDC hdc, VOID* splitBuffer, int perSampleBitsSize, int chans, int size);
	  VOID DrawCoordinate(HWND hwnd, HDC hdc, int zeroX, int zeroY, int lineWidth,
		  int lineHeight, float subXtextLen, float subYtextLen, float scale);

  };
  class AudioWaveStaff {
	  HFONT m_hcoorFont;
	  HDC m_bitDc, m_coordDc;
	  RECT m_rtDc,m_rtWave;
	  HBITMAP m_nCoordbitmap, m_oCoorbitmap; 
	  VOID* m_drawBuffer = NULL;
	  VOID InitPalette(HWND hwnd, HDC hdc);
	  int m_fcoordPerBits;
  public:
	VOID   ClearDraw(const HWND& hwnd);
	  ~AudioWaveStaff() { 
		  ::DeleteObject(m_hcoorFont); 
		  ::DeleteObject(m_nCoordbitmap);
	  ::DeleteObject(m_oCoorbitmap);
	  ::DeleteDC(m_bitDc);
	  ::DeleteDC(m_coordDc);
	   
	 
	  }
	  AudioWaveStaff(HWND hwnd, HDC hdc) { InitPalette(hwnd, hdc); }
	  VOID DrawWave(HWND hwnd, HDC hdc,VOID* buffer, int size, int chans, int perBitsSize, float scale);
	  VOID  DrawCoordinate(HWND hwnd, HDC hdc, int rule, int perBits, int scaleX);
  };
class BassAudioPlayer { 
	DWORD mhBass = { 0 }; 
	BASS_CHANNELINFO m_channelInfo = { 0 };
	PCHAR m_filename=NULL;
	int m_iBufferDataLen;
	int m_iPerSampleSize,m_iSplitBufferLen=0, m_iFFTSampleSize=0;
	long m_lAnimeTime;
	BOOLEAN mIsPlaying=FALSE,mIsPause=TRUE;
	VOID* m_buffer=NULL,*m_floatBuffer,*m_splitBufferTmp=NULL,*m_fftBuffer=NULL;
	BYTE* mFileBuffer = NULL;
	VOID ConvertBufferToFloatBuffer(VOID** fbuffer); 
	int GetSplitBuffer(VOID* buffer, int irs, int sampleSize, int perSampleBitsZize, int chans);
	AudioFourierStaff *m_fStaff=NULL;
	AudioWaveStaff* m_wStaff = NULL;  
	ContextMenu mPopmenu;
public:
	BOOLEAN  IsPause() {
		return mIsPause;
	}
	int GetPerSampleSize() { return m_iPerSampleSize; }
	VOID Setting(HWND& hwnd);
	VOID PopMenu(HWND& hwnd, POINTS& pt);
	VOID Pause();
	VOID Resume();
	BOOLEAN IsEndPlay();
	VOID SetAudioFile(LPCSTR filename, int sampleFlag = 0);
	VOID Stop(); 
	VOID SetPlayerEndCallback(playerEndCallback* end);
	playerEndCallback* mPlayerEndCallback = NULL;
	static VOID Init();
	static BOOLEAN s_init; 
	VOID SetBuffer(VOID* buffer, int ibLen, int fraq, int chans, int flag=0);
	int SplitBuffer(double secStart, double secEnd, VOID** buffer, int perBitsSize=-1);
	int GetSampleBuffer(VOID** buffer);
	BassAudioPlayer(); 
	~BassAudioPlayer();
	VOID Play();
	QWORD GetPlayPostion();
	double  GetPlaySecond();
	int GetChannelNum();
	int GetFraquency();
	VOID DrawWave(HWND hwnd,HDC hdc, int bufferSampleSize);
};

VOID BASS_ASSERT(LPCSTR posStr);
 
VOID NumToString(CHAR* str, float num); 



