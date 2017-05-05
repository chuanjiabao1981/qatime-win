#ifndef UIVIDEO1V1_H
#define UIVIDEO1V1_H

#include <QWidget>
#include "ui_UIVideo1v1.h"

struct CaptureWindowInfo
{
	HWND id;
	std::wstring title;
};
typedef std::vector<CaptureWindowInfo> CaptureWindowInfoList;

class UIVideo1v1 : public QWidget
{
	Q_OBJECT

public:
	UIVideo1v1(QWidget *parent = 0);
	~UIVideo1v1();

	enum FrameType
	{
		Ft_I420 = 0,
		Ft_ARGB,
		Ft_ARGB_r,
	};
private:
	Ui::UIVideo1v1 ui;
	QTimer*							m_capturnTimer;	//图像抓取定时器
	QTimer*							m_refreshTimer;	//刷新
	QPixmap							m_pBkImage;		// 背景
	QScreen*						m_screen;
	
	HWND							capture_hwnd_;
	HBITMAP							capture_bitmap_;
	char*							capture_data_;
	int								capture_width_;
	int								capture_height_;
protected:
	virtual void paintEvent(QPaintEvent *);

signals:
	void sig_refreshWnd();
	void sig_CustomVideoData(__int64, const char*, int, int, int);
private slots:
	void slot_onCapturnTimeout();
	void slot_onRefreshTimeout();

public:
	bool GetCaptureWindowList(CaptureWindowInfoList* windows);
	void InitHBitmap(int width, int height);
	void CustomFrame();
	int	 ScreenWidth();
	int  ScreenHeight();
	void setCaptureWnd(HWND hwnd);

	void AddVideoFrame(bool capture, __int64 time, const char* data, int size, int width, int height, const std::string& json, FrameType frame_type);
};

#endif // UIVIDEO1V1_H
