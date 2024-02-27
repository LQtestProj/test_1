#pragma once
#include <Windows.h>
#include <stdio.h>
#include <malloc.h> 
#include <tchar.h>





#define _INTSIZEOF(n)          ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
class Log {
private:
	char mCache[200], mAsciiCache[200];
public:
	static int Char2Wchar(LPTSTR wcharStr, LPCSTR charStr, int& sLen) {
		sLen = MultiByteToWideChar(CP_ACP, 0, charStr, strlen(charStr), NULL, 0);

		sLen = MultiByteToWideChar(CP_ACP, 0, charStr, strlen(charStr), wcharStr, sLen);
		wcharStr[sLen] = '\0';
		return sLen;
	}
	static int Wchar2Char( LPSTR charStr, LPTSTR wcharStr,  int& sLen) {
		sLen = WideCharToMultiByte(CP_ACP, 0, wcharStr, wcslen(wcharStr), NULL, 0, NULL, NULL);
		sLen = WideCharToMultiByte(CP_ACP, 0, wcharStr, wcslen(wcharStr), charStr, sLen, NULL, NULL);
	
		return sLen;
	}

	VOID Trace(LPCSTR lpszFmt, ...)
	{
		int sizeint = sizeof(int);
		int dbsize = sizeof(double);
		int slen  ;
		LPWSTR nlpsz = (LPWSTR)mAsciiCache;
		Char2Wchar(nlpsz, lpszFmt, slen);
		va_list args;
		((void)(__vcrt_assert_va_start_is_not_reference<decltype(lpszFmt)>()
			, ((void)(args = (va_list)_ADDRESSOF(lpszFmt) + _INTSIZEOF(lpszFmt)))));
		int len = _vsctprintf(nlpsz, args) + 1;
		TCHAR* lpszBuf = (TCHAR*)mCache;//栈中分配, 不需要释放
		_vstprintf_s(lpszBuf, len, nlpsz, args);
		va_end(args);
		OutputDebugString(lpszBuf);
		memset(mCache, 0, len*2);
		memset(mAsciiCache, 0, slen *2);
	}
	VOID Trace(LPCTSTR lpszFmt, ...)
	{
		int sizeint = sizeof(int); 
		int slen = wcslen(lpszFmt);
		va_list args;
		((void)(__vcrt_assert_va_start_is_not_reference<decltype(lpszFmt)>()
			, ((void)(args = (va_list)_ADDRESSOF(lpszFmt) + _INTSIZEOF(lpszFmt)))));
		int len = _vsctprintf(lpszFmt, args) + 1;
		TCHAR* lpszBuf = (TCHAR*)mCache;//栈中分配, 不需要释放
		_vstprintf_s(lpszBuf, len, lpszFmt, args);
		va_end(args);
		OutputDebugString(lpszBuf);
		memset(mCache, 0, len*2); 
	}

};

extern Log _log;