#include "UIHoverwindow.h"

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

// �����뵯������,��Ϊ���
void UIHoverWindow::enterEvent(QEvent *)
{
// 	this->hide();
// 
// 	if (m_Parent)
// 		m_Parent->show();
}

void UIHoverWindow::mouseReleaseEvent(QMouseEvent *event)
{
	this->hide();

	if (m_Parent)
		m_Parent->show();

	ui.timer_label->setText("00:00:00");
	ui.chatNum_label->setText("0");
	m_iCount = 0;
}

void UIHoverWindow::SetLiveTimer(QString sTimer)
{
	ui.timer_label->setText(sTimer);
}

// �����������
void UIHoverWindow::UpdateChatNumber()
{
	m_iCount++;
	QString sNumber = QString::number(m_iCount);
	ui.chatNum_label->setText(sNumber);
}

// �����������
void UIHoverWindow::SetNumber()
{
	m_iCount = 0;
	QString sNumber = QString::number(m_iCount);
	ui.chatNum_label->setText(sNumber);
}