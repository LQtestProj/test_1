#include "bass_audio.h"  
#include "log.h"
#include "fourierMathTool.h"
#include "audioOptDlg.h"

#ifdef _WIN64
#pragma comment( lib, "res/bassx64.lib" ) ;
#else
#pragma comment( lib, "res/bass.lib" ) ;
#endif
#define ANIMETIME 2000
int WaveHead::HEAD_SIZE = 44;
BOOLEAN BassAudioPlayer::s_init = FALSE;
VOID BassAudioPlayer::Init() {
	if (!s_init) {
		//CoInitialize(nullptr);
		BASS_Init(-1, 44100, 0, 0, NULL);
		s_init = TRUE;

	}
}
BassAudioPlayer::~BassAudioPlayer() {
	if (m_buffer != NULL) {
		delete m_buffer;
	}
}
VOID BassAudioPlayer::PopMenu(HWND& hwnd, POINTS& pt) {
	mPopmenu.PopMenu(hwnd, pt);

}
VOID BassAudioPlayer::Setting(HWND& hwnd) {
	_audioSetting.ShowDlg(hwnd, this);
}
VOID BassAudioPlayer::Pause() {
	if (mIsPlaying) {
		BASS_ChannelPause(mhBass);
		mIsPause = TRUE;
	}
}
VOID BassAudioPlayer::Resume() {
	if (mIsPlaying) {
		BASS_ChannelPlay(mhBass, FALSE);
		mIsPause = FALSE;
	}
}
BOOLEAN BassAudioPlayer::IsEndPlay() {
	if (mhBass == 0) {
		return TRUE;
	}
	QWORD pos = ::BASS_ChannelGetPosition(mhBass, BASS_POS_BYTE);
	if (mIsPlaying && pos == this->m_iBufferDataLen) {
		return TRUE;
	}
	return FALSE;
}
int BassAudioPlayer::GetChannelNum() {
	return m_channelInfo.chans;
}
int BassAudioPlayer::GetFraquency() {
	return m_channelInfo.freq;
}
int BassAudioPlayer::SplitBuffer(double secStart, double secEnd, VOID** buffer, int perBitsSize) {

	QWORD lsecstart, lsecend;
	VOID* outBuffer;
	BYTE b;
	int iNewBufferLen, isampleLen, i, j, icpyLen, psize = perBitsSize, irsh, flag = m_channelInfo.flags,
		chans = m_channelInfo.chans, istart;
	if (perBitsSize == -1) {
		if (flag & BASS_SAMPLE_8BITS) {
			psize = 1;
		}
		else if (flag & BASS_SAMPLE_FLOAT) {
			psize = 4;
		}
		else {
			psize = 2;
		}
	}
	switch (psize) {
	case 1:irsh = -1;
		break;
	case 2:irsh = 1;
		break;
	case 4:irsh = 2;
		break;
	}
	lsecstart = BASS_ChannelSeconds2Bytes(mhBass, secStart);
	lsecend = BASS_ChannelSeconds2Bytes(mhBass, secEnd);
	iNewBufferLen = lsecend - lsecstart;

	//iNewBufferLen = iNewBufferLen <<irsh;
	(*buffer) = malloc(iNewBufferLen);
	outBuffer = *buffer;
	memset(outBuffer, 0, iNewBufferLen);
	memcpy(outBuffer, ((BYTE*)m_buffer + lsecstart), iNewBufferLen);

	return iNewBufferLen;
}
VOID BassAudioPlayer::Stop() {
	BASS_ChannelPause(mhBass);
	BASS_ASSERT("BASS_ChannelStop"); 
	if (m_wStaff != NULL) {
		m_wStaff->ClearDraw(NULL);
	}
	//m_sample = 0;

}
VOID BassAudioPlayer::Play() {
	float speed = 88200;
	if (mhBass == 0) {
		mhBass = BASS_SampleLoad(FALSE, m_filename, 0, 0, 1, 0);
		BASS_ASSERT("BASS_SampleLoad");
	}

	mIsPause = FALSE;
	mIsPlaying = TRUE;
	BASS_ChannelPlay(mhBass, TRUE);

	BASS_ASSERT("BASS_ChannelPlay");
	BASS_ASSERT("BASS_ChannelSetAttribute");
}
int BassAudioPlayer::GetSampleBuffer(VOID** buffer) {
	*buffer = malloc(m_iBufferDataLen);
	memcpy(*buffer, m_buffer, m_iBufferDataLen);
	return m_iBufferDataLen;
}
BassAudioPlayer::BassAudioPlayer() :mPopmenu(AUDIO_POPMENU) {
	BassAudioPlayer::Init();
}
VOID BassAudioPlayer::SetBuffer(VOID* buffer, int ibLen, int fraq, int chans, int flag) {

	mIsPlaying = FALSE;
	if (m_buffer != NULL && this->m_iBufferDataLen != ibLen) {
		free(m_buffer);
		m_buffer = malloc(ibLen);
	}
	if (mFileBuffer != NULL) {
		free(mFileBuffer);
	}
	if (flag & BASS_SAMPLE_FLOAT != 0) {
		m_iPerSampleSize = 32;
	}
	else if (flag & BASS_SAMPLE_8BITS != 0) {
		m_iPerSampleSize = 8;

	}
	else {
		m_iPerSampleSize = 16;
	}
	WaveHeadPcmFormat ramWav((short)chans, fraq, (short)m_iPerSampleSize, ibLen);
	int ramsize = WaveHeadPcmFormat::HEAD_SIZE + ibLen;
	mFileBuffer = (BYTE*)malloc(ramsize);


	memcpy(mFileBuffer, ramWav.GetHead(), WaveHeadPcmFormat::HEAD_SIZE);
	memcpy(mFileBuffer + WaveHeadPcmFormat::HEAD_SIZE, buffer, ibLen);


	BASS_StreamFree(mhBass);
	BASS_ASSERT("BASS_StreamFree");
	memcpy(m_buffer, buffer, ibLen);
	mhBass = BASS_StreamCreateFile(TRUE, mFileBuffer, 0, ramsize, flag);


	//mhBass = BASS_SampleCreate(ibLen , fraq, chans, 1, flag );
	BASS_ASSERT("BASS_StreamCreate");
	//BASS_StreamPutFileData(mhBass, filebuffer, ramsize);
	//BASS_StreamPutData(mhBass, m_buffer, ibLen);
	BASS_ASSERT("BASS_SampleSetData");
	BASS_ChannelGetInfo(mhBass, &m_channelInfo);
	BASS_ASSERT("BASS_SampleGetInfo");
	//BASS_SampleSetData(mhBass, buffer);
	this->m_iBufferDataLen = ibLen;
	//free(filebuffer);
}




static int POWTABLE[] = { 1,10,100,1000 };
VOID NumToString(CHAR* str, float num) {
	DWORD   j, inum1, inum2, num3, inumLen;
	int mult = 3, iaddZero, ishift = 0, tmpinum;
	BOOLEAN isNag = FALSE, isSmallNum = FALSE;
	float tmpfnum;
	if (num < 0) {
		isNag = TRUE;
		num = -num;
	}

	if (num > 0 && num < 1) {
		num += 0.00006103515625;
		num = (int)(num * POWTABLE[mult]);
		isSmallNum = TRUE;
	}
	else {
		tmpinum = num;
		if (tmpinum != num) {
			num += 0.00006103515625;
			num = (int)(num * POWTABLE[mult]);
			isSmallNum = TRUE;
		}
	}


	for (j = 0, inumLen = 1, inum2 = num; ; j++, ++inumLen) {
		inum1 = (inum2 * 52429) >> (16 + 3);
		num3 = inum2 - ((inum1 << 3) + (inum1 << 1));
		str[j] = num3 + 0x30;
		inum2 = inum1;
		if (inum2 == 0) {
			break;
		}
	}


	for (j = 0;; j++) {
		if (j == inumLen >> 1) {
			break;
		}
		inum1 = str[inumLen - j - 1];
		str[inumLen - j - 1] = str[j];
		str[j] = inum1;
	}
	if (isSmallNum) {
		ishift = 2;
		iaddZero = (mult)-inumLen;

		if (iaddZero <= 0) {
			ishift += iaddZero;
			for (j = 0; j < inumLen + iaddZero; j++) {
				inum1 = str[inumLen - j - 1];
				str[inumLen - j - 1 + ishift] = inum1;
			}
			str[1] = '.';
			if (iaddZero == 0) {
				str[0] = '0';
			}
		}
		else {
			ishift += (iaddZero);
			for (j = 0; j < inumLen; j++) {
				inum1 = str[inumLen - j - 1];
				str[inumLen - j - 1 + ishift] = inum1;
			}
			for (j = 2; j < ishift; j++) {
				str[j] = '0';
			}
			str[0] = '0';
			str[1] = '.';
		}


	}
	if (isNag) {
		for (j = 0; j < inumLen + ishift; j++) {
			inum1 = str[inumLen + ishift - j - 1];
			str[inumLen + ishift - j] = inum1;
		}
		str[0] = '-';
	}

}
HBITMAP _specbmp; 
int __x = 0; 
int BassAudioPlayer::GetSplitBuffer(VOID* buffer, int irs, int sampleSize, int perSampleBitsZize, int chans) {
	if (m_buffer == NULL)return 0;
	VOID* subbuffer;
	int N, i, j, irsh = -1, nsize = N * (perSampleBitsZize >> 3) * chans;
	int ire = irs + sampleSize * perSampleBitsZize * chans;
	if (ire > m_iBufferDataLen) {
		ire = m_iBufferDataLen;
	}
	nsize = ire - irs;
	memcpy(buffer, (BYTE*)m_buffer + irs, nsize);
	 return nsize;
 
}
VOID BassAudioPlayer::SetPlayerEndCallback(playerEndCallback* end) {
	this->mPlayerEndCallback = end;

}
double _pos;
int _x = 0, _iCatchSample = 1024;
VOID BassAudioPlayer::DrawWave(HWND hwnd, HDC hdc, int bufferSampleSize) {
	 
	if (m_fStaff == NULL) {
		m_fStaff = new AudioFourierStaff(hwnd, hdc);
		m_wStaff = new AudioWaveStaff(hwnd, hdc);
	}
	if (mIsPause)
	{
		return;
	}
	int irs, ire, irsh = -1, i, j;
	const int capSampleSize = 1024;
	int perSampleBitsSize = sizeof(BYTE) << 2, chans = m_channelInfo.chans, ifsize, flag = m_channelInfo.flags;
	int sampleLen, itotalBufferLen, N, usedSampleSize = -2;
	if (flag & BASS_SAMPLE_8BITS) {
		perSampleBitsSize = sizeof(BYTE) << 3;
	}
	else if (flag & BASS_SAMPLE_FLOAT) {
		perSampleBitsSize = sizeof(float) << 3;
	}
	else {
		perSampleBitsSize = sizeof(short) << 3;
	}
	m_iPerSampleSize = perSampleBitsSize >> 3;

	_x = this->GetPlayPostion();
	 
	irs = _x; ire = _iCatchSample + (_x);

	itotalBufferLen = m_iBufferDataLen / (m_iPerSampleSize) / chans;
 
	if (ire > itotalBufferLen) {
		ire = itotalBufferLen;
	} 

	itotalBufferLen = this->m_iBufferDataLen;

	ire = irs + capSampleSize * m_iPerSampleSize * chans;
	if (ire > m_iBufferDataLen) {
		ire = m_iBufferDataLen;
	}
	sampleLen = ire - irs;
	if (m_iSplitBufferLen < sampleLen) {
		if (m_splitBufferTmp != NULL) {
			free(m_splitBufferTmp);
		}
		m_iSplitBufferLen = sampleLen;
		m_splitBufferTmp = malloc(sampleLen);
	}
	memset(m_splitBufferTmp, 0, m_iSplitBufferLen);

	if (!IsEndPlay()) {
		sampleLen = this->GetSplitBuffer(m_splitBufferTmp, irs, capSampleSize, m_iPerSampleSize, chans);
	}
	else { 
		 
	}
	m_wStaff->DrawWave(hwnd, hdc, m_splitBufferTmp, capSampleSize, chans, perSampleBitsSize, 1.0);
	m_fStaff->DrawFourier(hwnd, hdc, m_splitBufferTmp, perSampleBitsSize, chans, capSampleSize); 
	Sleep(100);
}
VOID BassAudioPlayer::ConvertBufferToFloatBuffer(VOID** fbuffer) {
	if (this->m_iBufferDataLen == 0) {
		return;
	}
	float ftmp;



	ftmp = *((float*)m_buffer + 0);



}
VOID CALLBACK  EndOfPlayCallback(HSYNC handle, DWORD channel, DWORD data, void* pTarget) {

	BassAudioPlayer* player = (BassAudioPlayer*)pTarget;
	if (player->mPlayerEndCallback != NULL) {
		player->mPlayerEndCallback(player);
	}

}

VOID BassAudioPlayer::SetAudioFile(LPCSTR  filename, int sampleFlag) {
	if (m_filename != NULL && strcmp(filename, m_filename) == 0) {
		return;
	}
	int iBufferSize = 0;
	int strLen = strlen(filename) + 1;//补\0
	HSAMPLE hsample;
	mIsPlaying = FALSE;
	m_iPerSampleSize = sizeof(float);
	if (this->m_filename != NULL) {
		free(this->m_filename); 
		BASS_Free();
		s_init = FALSE;
		mhBass = { 0 };
		BassAudioPlayer::Init();

	}
	if (mFileBuffer != NULL) {
		free(mFileBuffer);
		mFileBuffer = NULL;
	}
	this->m_filename = (PCHAR)malloc(strLen);
	assert(this->m_filename != NULL);
	memset(this->m_filename, 0, strLen);
	memcpy(this->m_filename, filename, strLen - 1);

	mhBass =// BASS_MusicLoad(FALSE, filename,0,0,BASS_MUSIC_RAMPS | BASS_MUSIC_POSRESET | BASS_MUSIC_PRESCAN, 44100);
		BASS_StreamCreateFile(FALSE, filename, 0, 0, 0);
	BASS_ASSERT("BASS_StreamCreateFile");
	BASS_ChannelGetInfo(mhBass, &m_channelInfo);
	BASS_ChannelSetSync(mhBass, BASS_SYNC_ONETIME | BASS_SYNC_END, 0, EndOfPlayCallback, this);
	BASS_ASSERT("BASS_ChannelSetSync");

	hsample = BASS_SampleLoad(FALSE, filename, 0, 0, 1, sampleFlag);
	BASS_ASSERT("BASS_SampleLoad"); 
	BASS_ASSERT("BASS_ChannelSetSync");
	if (m_channelInfo.flags & BASS_SAMPLE_8BITS) {
		m_iPerSampleSize = sizeof(BYTE);
	}
	else if (m_channelInfo.flags & BASS_SAMPLE_FLOAT) {
		m_iPerSampleSize = sizeof(float);
	}
	else {
		m_iPerSampleSize = sizeof(short);
	}
	m_iBufferDataLen = BASS_ChannelGetLength(hsample, BASS_POS_BYTE);
	BASS_ASSERT("BASS_ChannelGetLength");
	if (m_buffer != NULL) {
		free(m_buffer);
	}
	m_buffer = malloc(m_iBufferDataLen);
	if (m_buffer != NULL) {
		memset(m_buffer, 0, m_iBufferDataLen);
		assert(m_buffer != NULL);
		BASS_SampleGetData(hsample, m_buffer);
		BASS_ASSERT("BASS_SampleGetData");
		BASS_SampleFree(hsample);
	}

	else {
		::MessageBox(NULL, L"音频文件内容过大无法缓存", L"出问题了", MB_ICONWARNING);
	}



}

QWORD BassAudioPlayer::GetPlayPostion() {
	QWORD pos = BASS_ChannelGetPosition(mhBass, BASS_POS_BYTE);
	return pos;
}
double BassAudioPlayer::GetPlaySecond() {
	QWORD pos = BASS_ChannelGetPosition(mhBass, BASS_POS_BYTE);
	return BASS_ChannelBytes2Seconds(mhBass, pos);
}
VOID BASS_ASSERT(LPCSTR posStr) {
	int err = BASS_ErrorGetCode();
	if (err == 0 || err == BASS_ERROR_NOPLAY) {
		return;
	}
	LPCSTR str1 = " bass error :%d\n";
	int len1 = strlen(str1), len2 = strlen(posStr);
	PCHAR nStr = (PCHAR)alloca(len1 + len2);
	memset(nStr, 0, len1 + len2);
	memcpy(nStr, posStr, len2);
	strcat(nStr, str1);
	_log.Trace(nStr, err);
	assert(err == 0);
}


VOID TraceBassTime(HCHANNEL channel, int bufferIndex, double& pretime) {
	float flen = bufferIndex * sizeof(float);
	double time = 0;
	int h, m, s;
	time = BASS_ChannelBytes2Seconds(channel, flen);
	time = floorf(time);
	if (pretime == time) {
		return;
	}
	pretime = time;
	h = time / 3600;
	m = (time - 3600 * h) / 60;
	s = (time - 3600 * h) - m * 60;
	_log.Trace(L"h:%d m:%d s:%d\n", h, m, s);

}

