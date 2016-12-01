#include "UIRangewnd.h"
#include <QMouseEvent>
#include <wtypes.h>
#include <QApplication>
#include <QDesktopWidget>

UIRangeWnd::UIRangeWnd(QWidget *parent)
	: QWidget(parent)
	, m_parent(NULL)
{
	ui.setupUi(this);
	ui.title_pushButton->installEventFilter(this);
}

UIRangeWnd::~UIRangeWnd()
{
	m_parent = NULL;
}

void UIRangeWnd::SetMainWnd(RangeCapture* parent)
{
	m_parent = parent;
	connect(ui.full_pushButton, SIGNAL(clicked()), m_parent, SLOT(FullDialog()));
}

// 拖动标题做的处理
bool UIRangeWnd::eventFilter(QObject *target, QEvent *event)
{
	if (target == ui.title_pushButton)
	{
		static int i = 0;
		QMouseEvent* pMe = static_cast<QMouseEvent*>(event);
		if (event->type() == QEvent::MouseButtonPress)
		{
			m_startPos = pMe->globalPos();
			m_WndCurPos = this->pos();
			m_ParentWndPos = m_parent->pos();
		}
		else if (event->type() == QEvent::MouseMove)
		{
			m_clickPos = pMe->globalPos();
			this->move(m_WndCurPos + (m_clickPos - m_startPos));
			m_parent->move(m_ParentWndPos + (m_clickPos - m_startPos));
			
			QPoint pt = m_ParentWndPos + (m_clickPos - m_startPos);
			QString sRc;
			sRc.append(QString::number(pt.x()) + "," + QString::number(pt.y()) + "," + QString::number(m_parent->width()) + "," + QString::number(m_parent->height()));
			m_parent->ChangeRange(sRc);
		}
		else if (event->type() == QEvent::MouseButtonRelease)
		{
			SetWindowPos((HWND)m_parent->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			// 鼠标放开的时候，判断界面位置，如超出边界，则重新设置
			bool bTop = false;
			QRect rect;
			rect = m_parent->geometry();
			if (rect.left() <= 0)
			{
				if (rect.bottom() > QApplication::desktop()->height())
					bTop = true;
				m_parent->move(0, rect.top());
				move(m_parent->width(),rect.top());
				
				QString sRc;
				sRc.append("0," + QString::number(rect.top()) + "," + QString::number(m_parent->width()) + "," + QString::number(m_parent->height()));
				m_parent->ChangeRange(sRc);
			}
			
			if (bTop || rect.bottom() > QApplication::desktop()->height())
			{
				m_parent->move(rect.x(), QApplication::desktop()->height()-m_parent->height());
				move(rect.x() + m_parent->width(), QApplication::desktop()->height() - m_parent->height());

				QString sRc;
				sRc.append(QString::number(rect.x()) + "," + QString::number(QApplication::desktop()->height() - m_parent->height()) + "," + QString::number(m_parent->width()) + "," + QString::number(m_parent->height()));
				m_parent->ChangeRange(sRc);
			}
		}
		return false;
	}
	return false;
}

void UIRangeWnd::UpdateMove()
{
	if (m_parent)
	{
		QPoint pt = m_parent->pos();
		this->move(pt.x() + m_parent->size().width()-1, pt.y());
	}
}