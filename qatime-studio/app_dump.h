#ifndef	__APP_DUMP_H__
#define __APP_DUMP_H__

#include <windows.h>
#include <imagehlp.h>
#include <stdlib.h>
#include <wchar.h>
#include "atlstr.h"

LONG WINAPI MyUnhandledExceptionFilter(EXCEPTION_POINTERS* exp);
#endif