#include "UISeelivewnd.h"
#include <QPainter>

#define MAINWINDOW_X_MARGIN 6
#define MAINWINDOW_Y_MARGIN 6
#define MAINWINDOW_TITLE_HEIGHT 30
UISeeLiveWnd::UISeeLiveWnd(QWidget *parent)
	: QWidget(parent)
	, m_Parent(NULL)
{
	ui.setupUi(this);
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(CloseDialog()));
}

UISeeLiveWnd::~UISeeLiveWnd()
{
	if (m_Parent)
		m_Parent = NULL;
}

void UISeeLiveWnd::CloseDialog()
{
	if (m_Parent)
		m_Parent->setComeBack();
}

void UISeeLiveWnd::SetMainWnd(UIMainWindow* parent)
{
	m_Parent = parent;
}

bool UISeeLiveWnd::nativeEvent(const QByteArray &eventType, void *message, long *result)
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
				if (ui.close_pushButton )
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

// Ôö¼ÓÒõÓ°
void UISeeLiveWnd::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(2, 2, this->width() - 4, this->height() - 4);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	//	painter.fillPath(path, QBrush(Qt::white));

	QColor color(102, 204, 204);
	for (int i = 0; i < 4; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);

		for (int j = 0; j < 30; j++)
		{
			path.addRect(4 - i, j - i, this->width() - (4 - i) * 2, this->height() - (2 - i) * 2);
		}

		painter.setPen(color);
		painter.drawPath(path);
	}
}