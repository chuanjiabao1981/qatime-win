#include "UIHoverwindow.h"
#include <QDesktopWidget>

UIHoverWindow::UIHoverWindow(QWidget *parent)
	: QWidget(parent)
	, m_Parent(NULL)
	, m_iCount(0)
{
	ui.setupUi(this);
}

UIHoverWindow::~UIHoverWindow()
{
	if (m_Parent)
		m_Parent = NULL;
}

void UIHoverWindow::setParentWindow(UIMainWindow* parent)
{
	m_Parent = parent;
}

// 鼠标进入弹出窗口
void UIHoverWindow::enterEvent(QEvent *)
{
	this->hide();

	if (m_Parent)
	{
		QDesktopWidget *dsk = QApplication::desktop();
		m_Parent->move(dsk->width() - m_Parent->width(), m_Parent->pos().y());
		m_Parent->show();
	}

	ui.timer_label->setText("00:00:00");
	ui.chatNum_label->setText("0");
	m_iCount = 0;
}

void UIHoverWindow::mouseReleaseEvent(QMouseEvent *event)
{
	// 屏蔽点击弹出窗口
// 	this->hide();
// 
// 	if (m_Parent)
// 		m_Parent->show();
// 
// 	ui.timer_label->setText("00:00:00");
// 	ui.chatNum_label->setText("0");
// 	m_iCount = 0;
}

void UIHoverWindow::SetLiveTimer(QString sTimer)
{
	ui.timer_label->setText(sTimer);
}

// 更新聊天个数
void UIHoverWindow::UpdateChatNumber()
{
	m_iCount++;
	QString sNumber = QString::number(m_iCount);
	ui.chatNum_label->setText(sNumber);
}

// 重置聊天个数
void UIHoverWindow::SetNumber()
{
	m_iCount = 0;
	QString sNumber = QString::number(m_iCount);
	ui.chatNum_label->setText(sNumber);
}