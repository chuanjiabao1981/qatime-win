#include "UIVideo1v1.h"
#include <QTimer>
#include <QScreen>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include "windows.h"
#include <shlobj.h> 
#include <shlwapi.h>


#pragma comment(lib,"Version.lib")
#pragma comment(lib,"Shlwapi.lib")

#include "stdint.h"
#include "libyuv.h"

void sleepTime(int secs)
{
	QTime dieTime = QTime::currentTime().addMSecs(secs);

	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 20);
}

void MixClr(const uint8_t* src_clr, uint8_t* dest_clr, uint8_t a)
{
	uint16_t temp = ((uint16_t)*src_clr) * a + ((uint16_t)*dest_clr) * (255 - a);
	*dest_clr = temp / 255;
}

void I420CopyEx(const uint8* src_y, int src_stride_y, const uint8* src_ya, int src_stride_ya,
	const uint8* src_u, int src_stride_u, const uint8* src_ua, int src_stride_ua,
	const uint8* src_v, int src_stride_v, const uint8* src_va, int src_stride_va,
	uint8* dst_y, int dst_stride_y,
	uint8* dst_u, int dst_stride_u,
	uint8* dst_v, int dst_stride_v,
	int width, int height)
{
	if (src_stride_ya == 0 && src_stride_ua == 0 && src_stride_va == 0)
	{
		libyuv::I420Copy(src_y, src_stride_y,
			src_u, src_stride_u,
			src_v, src_stride_v,
			dst_y, dst_stride_y,
			dst_u, dst_stride_u,
			dst_v, dst_stride_v,
			width, height);
	}
	else
	{
		for (int ih = 0; ih < height; ++ih)
		{
			bool uv = ih % 2 == 0;
			for (int iw = 0; iw < width / 2; ++iw)
			{
				MixClr(src_y++, dst_y++, *src_ya++);
				MixClr(src_y++, dst_y++, *src_ya++);
				if (uv)
				{
					MixClr(src_u++, dst_u++, *src_ua++);
					MixClr(src_v++, dst_v++, *src_va++);
				}
			}
			src_y += (src_stride_y - width);
			src_ya += (src_stride_ya - width);
			dst_y += (dst_stride_y - width);
			if (uv)
			{
				src_u += (src_stride_u - width / 2);
				src_v += (src_stride_v - width / 2);
				src_ua += (src_stride_ua - width / 2);
				src_va += (src_stride_va - width / 2);
				dst_u += (dst_stride_u - width / 2);
				dst_v += (dst_stride_v - width / 2);
			}
		}
	}
}

UIVideo1v1::UIVideo1v1(QWidget *parent)
	: QWidget(parent)
	, m_capturnTimer(NULL)
	, m_refreshTimer(NULL)
	, m_screen(NULL)
	, capture_hwnd_(NULL)
	, pdata_(NULL)
{
	ui.setupUi(this);
	m_screen = QGuiApplication::primaryScreen();

	// 计时器 改变直播计时时间
	m_capturnTimer = new QTimer(this);
	connect(m_capturnTimer, SIGNAL(timeout()), this, SLOT(slot_onCapturnTimeout()));
	m_capturnTimer->start(200);

	m_refreshTimer = new QTimer(this);
	m_refreshTimer->start(1000 / 25);
	connect(m_refreshTimer, SIGNAL(timeout()), this, SLOT(slot_onRefreshTimeout()));
}

UIVideo1v1::~UIVideo1v1()
{
	if (m_capturnTimer)
	{
		m_capturnTimer->stop();
		delete m_capturnTimer;
	}
}

// 通过timer时间控制传输帧率，目前是200ms发送一张图，相当于每秒发送5张图，也就是帧率为5
void UIVideo1v1::slot_onCapturnTimeout()
{
	CustomFrame();
}


void UIVideo1v1::paintEvent(QPaintEvent *)
{
	
	if (this->isVisible())
	{
		QPainter p(this);
		p.setPen(QColor(0x8099be));
		QImage qimage;
		qimage = QImage((uchar*)capture_data_, capture_width_, capture_height_, QImage::Format_RGB32);
 		QImage q = qimage.mirrored(false, true);

		p.drawImage(rect(), q);//SmoothTransformation
	}
}

void UIVideo1v1::slot_onRefreshTimeout()
{
	update();
}

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

BOOL CALLBACK WindowsEnumerationHandler(HWND hwnd, LPARAM param) {
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

	if (wcscmp(window_title, L"UIAuxiliaryWnd") == 0 ||
		wcscmp(window_title, L"UIAppWnd") == 0 ||
		wcscmp(window_title, L"UIWindowSet") == 0 ||
		wcscmp(window_title, L"UIMainWindow") == 0 || 
		wcscmp(window_title, L"StudentWindow") == 0)
	{
		return TRUE;
	}	
		
	QString strTitle = QString::fromStdWString(window_title);
	if (strTitle.indexOf("Microsoft PowerPoint") >= 0 || 
		strTitle.indexOf("Microsoft Word") >= 0 ||
		strTitle.indexOf("Microsoft Excel") >= 0 ||
		strTitle.indexOf("- WPS") >= 0)
	{
		list->push_back(window);
	}

	return TRUE;
}

bool UIVideo1v1::GetCaptureWindowList(CaptureWindowInfoList* windows)
{
	CaptureWindowInfoList result;
	LPARAM param = reinterpret_cast<LPARAM>(&result);
	if (!EnumWindows(&WindowsEnumerationHandler, param))
		return false;
	windows->swap(result);
	return true;
}

void UIVideo1v1::InitHBitmap(int width, int height)
{
	if (capture_width_ != width || capture_height_ != height)
	{
		if (capture_bitmap_)
		{
			DeleteObject(capture_bitmap_);
			capture_bitmap_ = nullptr;
		}
		capture_width_ = width;
		capture_height_ = height;
		capture_width_ -= capture_width_ % 2;
		capture_height_ -= capture_height_ % 2;
		HDC	hdc = ::GetDC(capture_hwnd_);
		HDC hMemDC = CreateCompatibleDC(hdc);
		BITMAPINFO bmi;
		::ZeroMemory(&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = capture_width_;
		bmi.bmiHeader.biHeight = capture_height_;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = capture_width_*capture_height_ * 4;
		capture_bitmap_ = ::CreateDIBSection(hMemDC, &bmi, DIB_RGB_COLORS,
			(void**)&capture_data_, NULL, 0);
		DeleteDC(hMemDC);
		::ReleaseDC(capture_hwnd_, hdc);
	}
	else if (capture_data_ != nullptr)
	{
		ZeroMemory(capture_data_, capture_width_*capture_height_ * 4);
	}
}

void UIVideo1v1::CustomFrame()
{
	//nbase::NAutoLock auto_lock(&lock_);
	if (IsWindow(capture_hwnd_))
	{
		// 把屏幕设备描述表拷贝到内存设备描述表中
		HDC w_dc = GetDC(capture_hwnd_);
		if (w_dc)
		{
			RECT rcDlg;
			::GetClientRect(capture_hwnd_, &rcDlg);
			
			int width = rcDlg.right - rcDlg.left;
			int height = rcDlg.bottom - rcDlg.top;
			InitHBitmap(width, height);
			if (capture_bitmap_ == nullptr)
				return;

			HDC mem_dc = CreateCompatibleDC(w_dc);
			HBITMAP old_hbitmap = (HBITMAP)SelectObject(mem_dc, capture_bitmap_);
			
			BitBlt(mem_dc, 0, 0, capture_width_, capture_height_, w_dc, 0, 0, SRCCOPY /*| CAPTUREBLT*/);
			//鼠标
			if (1)
			{
				CURSORINFO pci;
				pci.cbSize = sizeof(CURSORINFO);
				GetCursorInfo(&pci);
				POINT ptCursor = pci.ptScreenPos;
				ICONINFO IconInfo = { 0 };
				if (GetIconInfo(pci.hCursor, &IconInfo))
				{
					ptCursor.x -= IconInfo.xHotspot;
					ptCursor.y -= IconInfo.yHotspot;
					if (NULL != IconInfo.hbmMask)
						DeleteObject(IconInfo.hbmMask);
					if (NULL != IconInfo.hbmColor)
						DeleteObject(IconInfo.hbmColor);
				}
				if (capture_hwnd_ != nullptr)
				{
					ScreenToClient(capture_hwnd_, &ptCursor);
				}
				DrawIconEx(mem_dc, ptCursor.x, ptCursor.y, pci.hCursor, 0, 0, 0, NULL, DI_NORMAL | DI_COMPAT);
			}
			SelectObject(mem_dc, old_hbitmap);
			DeleteDC(mem_dc);
			ReleaseDC(capture_hwnd_, w_dc);
			__int64 cur_timestamp = QDateTime::currentMSecsSinceEpoch();// 毫秒
			int wxh = capture_width_ * capture_height_;
// 			QImage qimage;
// 			qimage = QImage((uchar*)capture_data_, capture_width_, capture_height_, QImage::Format_RGB32);
// 			QImage q=qimage.mirrored(false, true);
// 			q.save("123.png");
			AddVideoFrame(true, cur_timestamp, capture_data_, wxh * 4, capture_width_, capture_height_, "", Ft_ARGB_r);
		}
	}
}

// 当前抓取的窗口宽度
int UIVideo1v1::ScreenWidth()
{
	return capture_width_;
}

// 当前抓取的窗口高度
int UIVideo1v1::ScreenHeight()
{
	return capture_height_;
}

// 设置抓取的窗口
void UIVideo1v1::setCaptureWnd(HWND hwnd)
{
	m_capturnTimer->stop();
	capture_hwnd_ = hwnd;
	m_capturnTimer->start(80);

	// 延时100毫秒首次刷新下窗口
	sleepTime(100);
	static int iCount = 0;
	if (iCount == 0)
	{
		emit sig_refreshWnd();
		iCount++;
	}
}

void UIVideo1v1::AddVideoFrame(bool capture, __int64 time, const char* data, int size, int width, int height, const std::string& json, FrameType frame_type)
{
	__int64 cur_timestamp = time; // 毫秒
	const char* src_buffer = data;
	std::string ret_data;
	if (frame_type != Ft_I420)
	{
		int byte_width = width * 4;
		width -= width % 2;
		height -= height % 2;
		int wxh = width * height;
		ret_data.append(wxh * 3 / 2, (char)0);
		uint8_t* des_y = (uint8_t*)ret_data.c_str();
		uint8_t* des_u = des_y + wxh;
		uint8_t* des_v = des_u + wxh / 4;
		const uint8_t* src_argb = (const uint8_t*)data;
		if (frame_type == Ft_ARGB_r)
		{
			src_argb = (const uint8_t*)data + size - byte_width;
			byte_width = -byte_width;
		}
		libyuv::ARGBToI420(src_argb, byte_width,
			des_y, width,
			des_u, width / 2,
			des_v, width / 2,
			width, height);
		src_buffer = ret_data.c_str();
		size = wxh * 3 / 2;
	}
	
	int src_w = width;
	int src_h = height;

	// 缩放1280*720
	width = 1280;
	height = 720;
	std::string ret_data_tmp;
	ret_data_tmp.append(width * height * 3 / 2, (char)0);
	const char* data_buffer = ret_data_tmp.c_str();

	if (width != src_w || height != src_h)
	{
		uint8_t* src_y = (uint8_t*)src_buffer;
		uint8_t* src_u = src_y + src_w * src_h;
		uint8_t* src_v = src_u + src_w * src_h / 4;
		uint8_t* des_y = (uint8_t*)ret_data_tmp.c_str();
		uint8_t* des_u = des_y + width * height;
		uint8_t* des_v = des_u + width * height / 4;
		libyuv::FilterMode filter_mode = libyuv::kFilterBox;
		libyuv::I420Scale(src_y, src_w,
			src_u, src_w / 2,
			src_v, src_w / 2,
			src_w, src_h,
			des_y, width,
			des_u, width / 2,
			des_v, width / 2,
			width, height,
			filter_mode);
	}

	memcpy((char*)data_buffer, ret_data_tmp.c_str(), ret_data_tmp.size());
	width = 1280;
	height = 720;
	size = width*height * 3 / 2;
	emit sig_CustomVideoData(0, data_buffer, size, width, height);
}

void UIVideo1v1::mousePressEvent(QMouseEvent *event)
{
	setFocus();
}