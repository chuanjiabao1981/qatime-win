#include "Rangecapture.h"
#include <QPainter>
#include <wtypes.h>
#include <windowsx.h>
#include <QTimer>
#include <QBitmap>

#define MAINWINDOW_X_MARGIN 6
#define MAINWINDOW_Y_MARGIN 6
#define MAINWINDOW_TITLE_HEIGHT 49

RangeCapture::RangeCapture(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
//  	setWindowFlags(Qt::Window);
//  	setWindowOpacity(0.3);

// 	m_Timer = new QTimer(this);
// 	connect(m_Timer, SIGNAL(timeout()), this, SLOT(setPos()));
// 	m_Timer->start(1000);
//	setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

RangeCapture::~RangeCapture()
{

}

void RangeCapture::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(2, 2, this->width() - 4, this->height() - 4);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::white));

	QColor color(0, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);

		for (int j = 0; j < 4; j++)
		{
			path.addRect(3 - i, j - i, this->width() - (3 - i) * 2, this->height() - (2 - i) * 2);
		}

		painter.setPen(color);
		painter.drawPath(path);
	}
}

bool RangeCapture::nativeEvent(const QByteArray &eventType, void *message, long *result)
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

void RangeCapture::setPos()
{
	static int iCount = 0;
	QRect rect = this->geometry();
	if (iCount % 2 == 0)
	{
		move(rect.left() + 10, rect.top());
	}
	else
		move(rect.left() - 10, rect.top());
}