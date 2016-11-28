#include "UIScreentip.h"
#include <windows.h>
#include <windowsx.h>

#define MAINWINDOW_X_MARGIN 6
#define MAINWINDOW_Y_MARGIN 6
#define MAINWINDOW_TITLE_HEIGHT 49
UIScreenTip::UIScreenTip(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.back_label->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid #cccccc");
	ui.tip_checkBox->setStyleSheet("QCheckBox::indicator{width: 65px;height: 13px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/Screen_noTip.png);}"
		"QCheckBox::indicator:checked{image: url(./images/Screen_Tip.png);}");

	connect(ui.ok_pushButton, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.tip_checkBox, SIGNAL(stateChanged(int)), this, SLOT(TipStatus(int)));
	SetWindowPos((HWND)this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

UIScreenTip::~UIScreenTip()
{

}

void UIScreenTip::CloseDialog()
{
	hide();
}

void UIScreenTip::TipStatus(int i)
{
	TCHAR szTempPath[MAX_PATH] = { 0 };

	GetCurrentDirectory(MAX_PATH, szTempPath);
	lstrcat(szTempPath, L"\\config.ini");

	if (i == 0)
	{
		WritePrivateProfileString(L"CONFIG_PATH", L"SCREENTIP", L"0", szTempPath);
	}
	else
	{
		WritePrivateProfileString(L"CONFIG_PATH", L"SCREENTIP", L"1", szTempPath);
	}
}

bool UIScreenTip::IsTip()
{
	TCHAR szTempPath[MAX_PATH] = { 0 };

	GetCurrentDirectory(MAX_PATH, szTempPath);
	lstrcat(szTempPath, L"\\config.ini");

	int screenTip = GetPrivateProfileInt(L"CONFIG_PATH", L"SCREENTIP", 0, szTempPath);

	return (bool)screenTip;
}
bool UIScreenTip::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	if ("windows_generic_MSG" == eventType)
	{
		PMSG pMsg = static_cast<PMSG>(message);
		switch (pMsg->message)
		{
		case WM_NCHITTEST:
		{
			int x = GET_X_LPARAM(pMsg->lParam) - this->frameGeometry().x();
			int y = GET_Y_LPARAM(pMsg->lParam) - this->frameGeometry().y();

			int xflag = (x <= MAINWINDOW_X_MARGIN) ? -1 : ((x < this->width() - MAINWINDOW_X_MARGIN) ? 0 : 1);
			int yflag = (y <= MAINWINDOW_Y_MARGIN) ? -1 : ((y < this->height() - MAINWINDOW_Y_MARGIN) ? 0 : 1);

			if (-1 == xflag && -1 == yflag)
			{
				*result = HTTOPLEFT;
			}
			else if (-1 == xflag && 0 == yflag)
			{
				*result = HTLEFT;
			}
			else if (-1 == xflag && 1 == yflag)
			{
				*result = HTBOTTOMLEFT;
			}
			else if (0 == xflag && -1 == yflag)
			{
				*result = HTTOP;
			}
			else if (0 == xflag && 0 == yflag)
			{
				*result = HTCLIENT;
			}
			else if (0 == xflag && 1 == yflag)
			{
				*result = HTBOTTOM;
			}
			else if (1 == xflag && -1 == yflag)
			{
				*result = HTTOPRIGHT;
			}
			else if (1 == xflag && 0 == yflag)
			{
				*result = HTRIGHT;
			}
			else if (1 == xflag && 1 == yflag)
			{
				*result = HTBOTTOMRIGHT;
			}
			if (0 == xflag && y > MAINWINDOW_Y_MARGIN && y <= MAINWINDOW_TITLE_HEIGHT)
			{
				if (ui.close_pushButton)
				{
					QRect Rect = ui.close_pushButton->geometry();
					if (x > Rect.left() && x < Rect.right() && y > Rect.top() && y < Rect.bottom())
					{
						return false;
					}
				}
				*result = HTCAPTION;
			}
			return true;
		}
		break;
		default:
			return false;
			break;
		}
	}
	return false;
}
