#include "Rangecapture.h"
#include <QPainter>
#include <wtypes.h>
#include <windowsx.h>
#include <QTimer>
#include <QBitmap>
#include "qmath.h"
#include <QResizeEvent>
#include <QDesktopWidget>

#define MAINWINDOW_X_MARGIN 6
#define MAINWINDOW_Y_MARGIN 6
#define MAINWINDOW_TITLE_HEIGHT 1
#define RECT_HEIGHT 832
#define RECT_WIDTH  832

RangeCapture::RangeCapture(QWidget *parent)
	: QWidget(parent)
	, m_RangeWnd(NULL)
	, m_VideoWnd(NULL)
	, m_bMaxDialog(false)
	, m_parent(NULL)
{
	ui.setupUi(this);

	setAttribute(Qt::WA_TranslucentBackground);
	ui.rect_pushButton->setHidden(true);
	connect(ui.rect_pushButton, SIGNAL(clicked()), this, SLOT(RectDialog()));

	QDesktopWidget* desktopWidget = QApplication::desktop();
	m_ScreenWidth = desktopWidget->width();
	m_ScreenHeight = desktopWidget->height();

	m_RangeLeft = 0;
	m_RangeTop = 0;

	if (m_ScreenHeight*0.92 > 832)
	{
		m_RangeWidth = RECT_WIDTH;
		m_RangeHeight = RECT_HEIGHT;
	}
	else
	{
		m_RangeWidth = m_ScreenHeight*0.92;
		m_RangeHeight = m_ScreenHeight*0.92;
	}
}

RangeCapture::~RangeCapture()
{
	if (m_RangeWnd)
	{
		delete m_RangeWnd;
		m_RangeWnd = NULL;
	}
}

void RangeCapture::setMainWindow(UIMainWindow* parent)
{
	m_parent = parent;
}

void RangeCapture::paintEvent(QPaintEvent *event)
{
	int iWidth = this->size().width();
	int iHeight = this->size().height();

	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(iWidth / 2, iHeight / 2, this->width() - iWidth, this->height() - iHeight);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::white));

	QColor color(0, 0, 0, 0);
	for (int i = 0; i < iWidth / 2; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);
		path.addRect(iWidth / 2 - i, iHeight / 2 - i, this->width() - (iWidth / 2 - i) * 2, this->height() - (iHeight / 2 - i) * 2);
		color.setAlpha(0);
		painter.setPen(color);
		painter.drawPath(path);
	}

	if (!m_bMaxDialog)
	{
		path.addRect(2, 2, this->width() - 4, this->height() - 4);
		QColor color1(87, 207, 248);
		for (int i = 0; i < 4; i++)
		{
			QPainterPath path;
			path.setFillRule(Qt::WindingFill);

			for (int j = 0; j < 4; j++)
			{
				path.addRect(3 - i, j - i, this->width() - (3 - i) * 2, this->height() - (2 - i) * 2);
			}

			painter.setPen(color1);
			painter.drawPath(path);
		}
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

void RangeCapture::setShowOrHide(bool bShow)
{
	setVisible(bShow);
	if (m_RangeWnd == NULL)
	{
		m_RangeWnd = new UIRangeWnd();
		m_RangeWnd->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
		m_RangeWnd->SetMainWnd(this);
		m_RangeWnd->resize(21, 150);
		QRect rc = this->geometry();
		QPoint pt = this->pos();
		m_RangeWnd->move(pt.x()+rc.width()-1, pt.y());
		SetWindowPos((HWND)m_RangeWnd->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	
	m_RangeWnd->setVisible(bShow);	
}

void RangeCapture::setVideoWnd(HWND hWnd)
{
	m_VideoWnd = hWnd;
}

void RangeCapture::resizeEvent(QResizeEvent *e)
{
	int w, h;
	if ( e != NULL)
	{
		w = e->size().width() - e->oldSize().width();
		h = e->size().height() - e->oldSize().height();

		if (m_RangeWnd)
		{
			QPoint pt = m_RangeWnd->pos();
			m_RangeWnd->move(pt.x()+w, pt.y());

			// Ë«ÖØ±£ÏÕ
			m_RangeWnd->UpdateMove();
		}

		QString sRc;
		sRc += QString::number(this->pos().x());
		sRc += ",";
		sRc += QString::number(this->pos().y());
		sRc += ",";
		sRc += QString::number(width());
		sRc += ",";
		sRc += QString::number(height());
		ChangeRange(sRc);
	}
}

void RangeCapture::ToFullDialog(bool bCheck)
{
	if (!bCheck)
	{
		m_RangeLeft = this->pos().x();
		m_RangeTop = this->pos().y();
		m_RangeWidth = this->width();
		m_RangeHeight = this->height();
	}

	move(0, 0);
	resize(m_ScreenWidth - 1, m_ScreenHeight);
	ui.rect_pushButton->setHidden(false);
	m_bMaxDialog = true;
	m_parent->clickFullRadio(); 
}
void RangeCapture::FullDialog()
{
	ToFullDialog();
}

void RangeCapture::ToRectDialog()
{
	move(m_RangeLeft, m_RangeTop);
	resize(m_RangeWidth, m_RangeHeight);
	ui.rect_pushButton->setHidden(true);
	m_bMaxDialog = false;
	m_parent->clickRectRadio(); 
}
void RangeCapture::RectDialog()
{
	ToRectDialog();
}

void RangeCapture::ChangeRange(QString sRc)
{
	COPYDATASTRUCT sendData;
	char result[MAX_PATH];
	QByteArray chPath = sRc.toLatin1();

	strcpy(result, chPath.data());

	ZeroMemory(&sendData, sizeof(sendData));
	sendData.lpData = result;
	sendData.cbData = MAX_PATH;
 
	if (IsWindow(m_VideoWnd))
		::SendMessage(m_VideoWnd, WM_COPYDATA, 3, (LPARAM)&sendData);
}

bool RangeCapture::IsMax()
{
	return m_bMaxDialog;
}

void RangeCapture::SetCurRect()
{
	m_RangeLeft = this->pos().x();
	m_RangeTop = this->pos().y();
	m_RangeWidth = this->width();
	m_RangeHeight = this->height();
}