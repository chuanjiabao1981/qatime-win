#include "UITraymenu.h"

UITrayMenu::UITrayMenu(QWidget *parent)
	: QWidget(parent)
	, m_parent(NULL)
{
	ui.setupUi(this);
	ui.main_pushButton->setStyleSheet("QPushButton{border-image:url(./images/AuxiliaryPanelBack.png);}"
		"QPushButton:hover{border-image:url(./images/menuback.png);}"
		"QPushButton:pressed{border-image:url(./images/menuback.png);}");

	ui.close_pushButton->setStyleSheet("QPushButton{border-image:url(./images/AuxiliaryPanelBack.png);}"
		"QPushButton:hover{border-image:url(./images/menuback.png);}"
		"QPushButton:pressed{border-image:url(./images/menuback.png);}");

	ui.retrue_pushButton->setStyleSheet("QPushButton{border-image:url(./images/AuxiliaryPanelBack.png);}"
		"QPushButton:hover{border-image:url(./images/menuback.png);}"
		"QPushButton:pressed{border-image:url(./images/menuback.png);}");

	connect(ui.main_pushButton, SIGNAL(clicked()), this, SLOT(MainDialog()));
	connect(ui.retrue_pushButton, SIGNAL(clicked()), this, SLOT(ReturnDialog()));
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(CloseDialog()));
}

UITrayMenu::~UITrayMenu()
{

}

void UITrayMenu::SetMainWindow(LoginWindow* parent)
{
	m_parent = parent;
}

void UITrayMenu::MainDialog()
{
	this->hide();
// 	if (m_parent)
// 		m_parent->ShowMain();
}

void UITrayMenu::ReturnDialog()
{
	this->hide();
// 	if (m_parent)
// 		m_parent->ReturnAccount();
}

void UITrayMenu::CloseDialog()
{
	this->hide();
// 	if (m_parent)
// 		m_parent->CloseWindow();
}

void UITrayMenu::focusOutEvent(QFocusEvent* e)
{
	QRect rc = geometry();
	if (rc.contains(QCursor::pos()))
		return;
	hide();
}