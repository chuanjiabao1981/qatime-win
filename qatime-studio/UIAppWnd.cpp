#include "UIAppWnd.h"
#include <QPainter>
#include <windows.h>
#include <windowsx.h>
#include <QScrollBar>
#include <QScrollArea>

#define MAINWINDOW_X_MARGIN 0
#define MAINWINDOW_Y_MARGIN 0
#define MAINWINDOW_TITLE_HEIGHT 30

UIAppWnd::UIAppWnd(QWidget *parent)
	: QWidget(parent)
	, m_spacer(NULL)
{
	ui.setupUi(this);
	setAutoFillBackground(true);
	QPalette p = palette();
	p.setColor(QPalette::Window, QColor(255, 255, 255));
	setPalette(p);

	ui.close_pushButton->setStyleSheet("border-image: url(./images/btn_off.png);");
	ui.ok_pushButton->setStyleSheet("border-image: url(./images/ok1.png);");
	ui.cancel_pushButton->setStyleSheet("border-image: url(./images/cancel.png);");
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(clickClose()));
	connect(ui.cancel_pushButton, SIGNAL(clicked()), this, SLOT(clickCancel()));
	connect(ui.ok_pushButton, SIGNAL(clicked()), this, SLOT(clickOK()));

	style(ui.scrollArea);
}

UIAppWnd::~UIAppWnd()
{

}

void UIAppWnd::clickClose()
{
	hide();
}

void UIAppWnd::clickCancel()
{
	hide();
}

void UIAppWnd::clickOK()
{
	if (m_vecBtnApp.size() > 0)
	{
		std::vector<CBtnApp*>::iterator it;
		for (it = m_vecBtnApp.begin(); it != m_vecBtnApp.end(); it++)
		{
			CBtnApp* layout = *it;
			if (layout->isChecked())
			{
				layout->clickWnd();
				hide();
				return;
			}
		}
	}
	else
		hide();
}

void UIAppWnd::style(QScrollArea *style)
{
	if (style)
	{
		// 设置滚动条样式
		style->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
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
}

void UIAppWnd::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	QPainter painter(this);

	QColor color(145, 145, 145);

	path.addRect(0, 0, this->width() - 1, this->height() - 1);
	painter.setPen(color);
	painter.drawPath(path);
}

bool UIAppWnd::nativeEvent(const QByteArray &eventType, void *message, long *result)
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

void UIAppWnd::AddAppWnd(QString title, HWND id)
{
	CBtnApp* titleName = new CBtnApp(this);
	titleName->AddWndName(id, title);
	connect(titleName, SIGNAL(sig_idclicked(HWND)), this, SLOT(slot_click(HWND)));
	ui.verticalLayout->addWidget(titleName);

	if (m_spacer == NULL)
	{
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		ui.verticalLayout->addSpacerItem(m_spacer);
	}
	else
	{
		ui.verticalLayout->removeItem(m_spacer);
		m_spacer = NULL;
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		ui.verticalLayout->addSpacerItem(m_spacer);
	}
		
	m_vecBtnApp.push_back(titleName);
	setFocus();
}

void UIAppWnd::slot_click(HWND hwnd)
{
	if (hwnd)
	{
		emit sig_selectWnd(hwnd);
	}
}

void UIAppWnd::deleteWnd()
{
	if (m_vecBtnApp.size() > 0)
	{
		std::vector<CBtnApp*>::iterator it;
		for (it = m_vecBtnApp.begin(); it != m_vecBtnApp.end(); it++)
		{
			CBtnApp* layout = *it;
			disconnect(layout, SIGNAL(sig_idclicked(HWND)), this, SLOT(slot_click(HWND)));
			if (layout)
			{
				delete layout;
				layout = NULL;
			}
		}
	}

	m_vecBtnApp.clear();
}