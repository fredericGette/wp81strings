// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here

#include <windows.h>
#include <stdlib.h>

extern "C" {
	WINBASEAPI
		VOID
		WINAPI
		DebugBreak(
			VOID
		);

	WINBASEAPI
		VOID
		WINAPI
		OutputDebugStringA(
			_In_opt_ LPCSTR lpOutputString
		);

	WINBASEAPI
		VOID
		WINAPI
		Sleep(
			_In_ DWORD dwMilliseconds
		);

	WINBASEAPI
		HANDLE
		WINAPI
		CreateFileA(
			_In_ LPCSTR lpFileName,
			_In_ DWORD dwDesiredAccess,
			_In_ DWORD dwShareMode,
			_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			_In_ DWORD dwCreationDisposition,
			_In_ DWORD dwFlagsAndAttributes,
			_In_opt_ HANDLE hTemplateFile
		);
}