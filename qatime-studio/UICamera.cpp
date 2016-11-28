#include "UICamera.h"
#include <iosfwd>
#include <sstream>
#include "define.h"
#include "windows.h"

#ifdef TEST
	#define _DEBUG
#else
#endif

UICamera::UICamera(QWidget *parent)
	: QWidget(parent)
	, m_CameraTimer(NULL)
{
	ui.setupUi(this);

	m_CameraTimer = new QTimer(this);
	connect(m_CameraTimer, SIGNAL(timeout()), this, SLOT(OpenCamera()));
	m_CameraTimer->start(500);
}

UICamera::~UICamera()
{
	if (m_CameraTimer)
	{
		delete m_CameraTimer;
		m_CameraTimer = NULL;
	}
}

void UICamera::SetMainWindow(UIMainWindow* parent)
{
	m_parent = parent;
}

void UICamera::OpenCamera()
{
	m_CameraTimer->stop();

	// 启动直播摄像头视频
	TCHAR szTempPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, szTempPath);
	lstrcat(szTempPath, L"\\camera_video.exe");
	std::wstring wszCmdLine;
	std::wstringstream wstream;
	HWND hwnd = (HWND)this->winId();
	wstream << L" -parenthwnd " << hwnd
		<< std::endl;
	wszCmdLine = wstream.str();

	STARTUPINFOW	si = { 0 };
	si.cb = sizeof(si);

	PROCESS_INFORMATION	pi;
	//查找关闭进程
	//	FindAndKillProcessByName(szExecFileName);
#ifdef TEST
	CreateProcess(szTempPath, (LPWSTR)wszCmdLine.c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	return;
#else
#endif

#ifdef _DEBUG
	CreateProcess(L"E:\\QATIME\\Camera_src\\nls_demo\\bin\\camera_video.exe", (LPWSTR)wszCmdLine.c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
#else
	CreateProcess(szTempPath, (LPWSTR)wszCmdLine.c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
#endif
}