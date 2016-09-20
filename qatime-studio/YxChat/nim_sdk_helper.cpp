/** @file nim_sdk_helper.cpp
  * @brief SDK库辅助方法
  * @copyright (c) 2015-2016, NetEase Inc. All rights reserved
  * @author towik, Oleg
  * @date 2015/09/08
  */

#include "nim_sdk_helper.h"
#include <QDir>
#include <QString>

namespace nim
{

static const std::wstring kSdkNimDll_d = L"nim_d.dll";
static const std::wstring kSdkNimDll = L"nim.dll";
HINSTANCE SDKFunction::instance_nim_ = NULL;
std::map<std::string, void*> SDKFunction::function_map;

bool SDKFunction::LoadSdkDll()
{

	QDir dir;
	QString strDir = dir.currentPath();
	strDir.append("/nim.dll");

	LPCWSTR str = (LPCWSTR)strDir.utf16();
	instance_nim_ = ::LoadLibraryW(str);
	if (instance_nim_ == NULL)
	{
		//QLOG_ERR(L"sdk nim load fail {0} {1}") << dir << GetLastError();
		return false;
	}

	return true;
}

void SDKFunction::UnLoadSdkDll()
{
	assert(instance_nim_);
	if (instance_nim_)
	{
		//QLOG_APP(L"client cleanup");

		::FreeLibrary(instance_nim_);
		instance_nim_ = NULL;
		//QLOG_APP(L"free nim library");
	}
}

}