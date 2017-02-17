#include "app_dump.h"
#include <time.h>

#pragma comment(lib, "dbghelp.lib")

BOOL CALLBACK MyMiniDumpCallback(PVOID, const PMINIDUMP_CALLBACK_INPUT input, PMINIDUMP_CALLBACK_OUTPUT output)
{
	if(input == NULL || output == NULL)
		return FALSE;

	BOOL ret = FALSE;
	switch(input->CallbackType)
	{
	case IncludeModuleCallback:
	case IncludeThreadCallback:
	case ThreadCallback:
	case ThreadExCallback:
		ret = TRUE;
		break;
	case ModuleCallback:
		{
			if( !(output->ModuleWriteFlags & ModuleReferencedByMemory) )
			{
				output->ModuleWriteFlags &= ~ModuleWriteModule;
			}
			ret = TRUE;
		}
		break;
	default:
		break;
	}

	return ret;
}

void WriteDump(EXCEPTION_POINTERS* exp, const WCHAR *pwPath)
{
    HANDLE h = ::CreateFile(pwPath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	MINIDUMP_EXCEPTION_INFORMATION info;
	info.ThreadId = ::GetCurrentThreadId();
	info.ExceptionPointers = exp;
	info.ClientPointers = NULL;

	MINIDUMP_CALLBACK_INFORMATION mci; 
	mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)MyMiniDumpCallback; 
	mci.CallbackParam = 0; 

	MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory); 

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), h, mdt, &info, NULL, &mci);
	::CloseHandle(h);
}

LONG WINAPI MyUnhandledExceptionFilter(EXCEPTION_POINTERS* exp)
{
    time_t tNowTime;
    tm *pLocalTime;
    time(&tNowTime);
    pLocalTime = localtime(&tNowTime);

    WCHAR awcBuf[MAX_PATH] = { 0 };
    ::GetCurrentDirectory(MAX_PATH, awcBuf); 
    wnsprintf(awcBuf, MAX_PATH, L"%s\\qatime_%04d%02d%02d_%02d%02d%02d.dmp", awcBuf, pLocalTime->tm_year + 1900, pLocalTime->tm_mon + 1,
        pLocalTime->tm_mday, pLocalTime->tm_hour, pLocalTime->tm_min, pLocalTime->tm_sec);

    WriteDump(exp, awcBuf);
	return EXCEPTION_EXECUTE_HANDLER;
}