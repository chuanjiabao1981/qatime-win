#include "UIOtherApp.h"
#include <QScrollBar>
#include <QBitmap>
#include <QPainter>
#include <shlobj.h> // SHGetFolderPathW
#include <shlwapi.h>
#pragma comment(lib,"Version.lib")
#pragma comment(lib,"Shlwapi.lib")

#define QT_USERDATA			100

//遍历windows窗口
bool GetFileVersion(const wchar_t *file_path, WORD *major_version, WORD *minor_version, WORD *build_number, WORD *revision_number)
{
	DWORD handle, len;
	UINT buf_len;
	LPTSTR buf_data;
	VS_FIXEDFILEINFO *file_info;
	len = GetFileVersionInfoSize(file_path, &handle);
	if (0 == len)
		return false;

	buf_data = (LPTSTR)malloc(len);
	if (!buf_data)
		return false;

	if (!GetFileVersionInfo(file_path, handle, len, buf_data))
	{
		free(buf_data);
		return false;
	}
	if (VerQueryValue(buf_data, L"\\", (LPVOID *)&file_info, (PUINT)&buf_len))
	{
		*major_version = HIWORD(file_info->dwFileVersionMS);
		*minor_version = LOWORD(file_info->dwFileVersionMS);
		*build_number = HIWORD(file_info->dwFileVersionLS);
		*revision_number = LOWORD(file_info->dwFileVersionLS);
		free(buf_data);
		return true;
	}
	free(buf_data);
	return false;
}
int GetNTDLLVersion()
{
	static int ret = 0;
	if (ret == 0)
	{
		wchar_t buf_dll_name[MAX_PATH] = { 0 };
		HRESULT hr = ::SHGetFolderPathW(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, buf_dll_name);
		if (SUCCEEDED(hr) && ::PathAppendW(buf_dll_name, L"ntdll.dll"))
		{
			WORD major_version, minor_version, build_number, revision_number;
			GetFileVersion(buf_dll_name, &major_version, &minor_version, &build_number, &revision_number);
			ret = major_version * 100 + minor_version;
		}
	}
	return ret;
}

BOOL CALLBACK WindowsEnumerationHandler(HWND hwnd, LPARAM param)
{
	CaptureWindowInfoList* list =
		reinterpret_cast<CaptureWindowInfoList*>(param);

	// Skip windows that are invisible, minimized, have no title, or are owned,
	// unless they have the app window style set.
	int len = GetWindowTextLength(hwnd);
	HWND owner = GetWindow(hwnd, GW_OWNER);
	LONG exstyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	if (len == 0 || IsIconic(hwnd) || !IsWindowVisible(hwnd) ||
		(owner && !(exstyle & WS_EX_APPWINDOW))
		|| (exstyle & WS_EX_LAYERED)) {
		return TRUE;
	}

	// Skip the Program Manager window and the Start button.
	const size_t kClassLength = 256;
	WCHAR class_name[kClassLength];
	int class_name_length = GetClassName(hwnd, class_name, kClassLength);

	// Skip Program Manager window and the Start button. This is the same logic
	// that's used in Win32WindowPicker in libjingle. Consider filtering other
	// windows as well (e.g. toolbars).
	if (wcscmp(class_name, L"Progman") == 0 || wcscmp(class_name, L"Button") == 0)
		return TRUE;

	if (GetNTDLLVersion() >= 602 &&
		(wcscmp(class_name, L"ApplicationFrameWindow") == 0 ||
		wcscmp(class_name, L"Windows.UI.Core.CoreWindow") == 0)) {
		return TRUE;
	}

	CaptureWindowInfo window;
	window.id = hwnd;

	const size_t kTitleLength = 500;
	WCHAR window_title[kTitleLength];
	// Truncate the title if it's longer than kTitleLength.
	GetWindowText(hwnd, window_title, kTitleLength);
	window.title = window_title;

	// Skip windows when we failed to convert the title or it is empty.
	if (window.title.empty())
		return TRUE;

	list->push_back(window);

	return TRUE;
}

UIOtherApp::UIOtherApp(QWidget *parent)
	: QDialog(parent)
	, m_CurrentIndex(0)
{
	ui.setupUi(this);

	ui.Auxiliary_tableWidget->setFocusPolicy(Qt::NoFocus);

	connect(ui.Auxiliary_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(CurrentIndex()));
	connect(ui.OK_pushBtn, SIGNAL(clicked()), this, SLOT(OK()));
	connect(ui.Cancel_pushBtn, SIGNAL(clicked()), this, SLOT(CANCEL()));

	initAppPath();
}

UIOtherApp::~UIOtherApp()
{

}

void UIOtherApp::setAppInfo(ST_NLSS_INDEVICE_INF* m_pAppWinds, int iAppCount)
{
	ui.Auxiliary_tableWidget->setRowCount(1);
	ui.Auxiliary_tableWidget->clearContents();

	for (int i = 0; i < iAppCount;i++)
	{
		ui.Auxiliary_tableWidget->insertRow(i+1);

		QString strAppName = m_pAppWinds[i].paFriendlyName;
		QTableWidgetItem *pItemNum = new QTableWidgetItem(strAppName);
		ui.Auxiliary_tableWidget->setItem(i - 1, 0, pItemNum);
	}

	ui.Auxiliary_tableWidget->removeRow(iAppCount);
}

void UIOtherApp::initAppPath()
{
	ui.Auxiliary_tableWidget->setColumnCount(1); //设置列数
	ui.Auxiliary_tableWidget->setRowCount(1);
	ui.Auxiliary_tableWidget->horizontalHeader()->setSectionsMovable(false);
	ui.Auxiliary_tableWidget->horizontalHeader()->setSectionsClickable(false);

	ui.Auxiliary_tableWidget->verticalHeader()->setDefaultSectionSize(25); //设置行高
	ui.Auxiliary_tableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
	ui.Auxiliary_tableWidget->setShowGrid(false); //设置不显示格子线
	ui.Auxiliary_tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
	ui.Auxiliary_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
	ui.Auxiliary_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
	ui.Auxiliary_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
	ui.Auxiliary_tableWidget->horizontalHeader()->resizeSection(0, 370); //设置表头第一列的宽度为370
	ui.Auxiliary_tableWidget->horizontalHeader()->setFixedHeight(30); //设置表头的高度
	ui.Auxiliary_tableWidget->setStyleSheet("selection-background-color:lightblue;");  //设置选中背景色
	ui.Auxiliary_tableWidget->horizontalHeader()->setVisible(false);

	// 设置滚动条样式
	ui.Auxiliary_tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,0%);"
		"margin:0px,0px,0px,0px;"
		"padding-top:9px;"
		"padding-bottom:9px;"
		"}"
		"QScrollBar::handle:vertical"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,25%);"
		" border-radius:4px;"
		"min-height:20;"
		"}"
		"QScrollBar::handle:vertical:hover"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,50%);"
		" border-radius:4px;"
		"min-height:20;"
		"}"
		"QScrollBar::add-line:vertical"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/3.png);"
		"subcontrol-position:bottom;"
		"}"
		"QScrollBar::sub-line:vertical"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/1.png);"
		"subcontrol-position:top;"
		"}"
		"QScrollBar::add-line:vertical:hover"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/4.png);"
		"subcontrol-position:bottom;"
		"}"
		"QScrollBar::sub-line:vertical:hover"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/2.png);"
		"subcontrol-position:top;"
		"}"
		"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
		"{"
		"background:rgba(0,0,0,10%);"
		"border-radius:4px;"
		"}"
		);
}

void UIOtherApp::CurrentIndex()
{
	m_CurrentIndex = ui.Auxiliary_tableWidget->currentRow();
}

void UIOtherApp::OK()
{
	done(2);
}

void UIOtherApp::CANCEL()
{
	done(0);
}

int UIOtherApp::getCurrentIndex()
{
	return m_CurrentIndex;
}

void UIOtherApp::resizeEvent(QResizeEvent *e)
{
	QBitmap bmp(this->size());
	bmp.fill();
	QPainter p(&bmp);
	p.setPen(Qt::NoPen);
	p.setBrush(Qt::black);
	p.drawRoundedRect(bmp.rect(), 10, 10);
	setMask(bmp);
	bmp.clear();
}

void UIOtherApp::closeEvent(QCloseEvent *)
{
	reject();
}

bool UIOtherApp::GetCaptureWindowList(CaptureWindowInfoList* windows)
{
	CaptureWindowInfoList result;
	LPARAM param = reinterpret_cast<LPARAM>(&result);
	if (!EnumWindows(&WindowsEnumerationHandler, param))
		return false;
	windows->swap(result);
	return true;
}