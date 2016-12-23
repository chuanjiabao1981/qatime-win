#include "UIBulletset.h"

UIBulletSet::UIBulletSet(QWidget *parent)
	: QWidget(parent)
	, m_parent(NULL)
{
	ui.setupUi(this);
	setFocusPolicy(Qt::ClickFocus);

	connect(ui.font1_pushButton, SIGNAL(clicked()), this, SLOT(Font1()));
	connect(ui.font2_pushButton, SIGNAL(clicked()), this, SLOT(Font2()));
	connect(ui.font3_pushButton, SIGNAL(clicked()), this, SLOT(Font3()));
	connect(ui.font4_pushButton, SIGNAL(clicked()), this, SLOT(Font4()));

	connect(ui.color1_pushButton, SIGNAL(clicked()), this, SLOT(Color1()));
	connect(ui.color2_pushButton, SIGNAL(clicked()), this, SLOT(Color2()));
	connect(ui.color3_pushButton, SIGNAL(clicked()), this, SLOT(Color3()));
	connect(ui.color4_pushButton, SIGNAL(clicked()), this, SLOT(Color4()));
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(closeDialog()));

	ui.font1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font1_nor.png);background-color: rgb(255,255,255);}");
	ui.font2_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font2_sel.png);background-color: rgb(255,255,255);}");
	ui.font3_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font3_nor.png);background-color: rgb(255,255,255);}");
	ui.font4_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font4_nor.png);background-color: rgb(255,255,255);}");

	ui.color1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color1_sel.png);background-color: rgb(255,255,255);}");
	ui.color2_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color2_nor.png);background-color: rgb(255,255,255);}");
	ui.color3_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color3_nor.png);background-color: rgb(255,255,255);}");
	ui.color4_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color4_nor.png);background-color: rgb(255,255,255);}");

	ui.label->setStyleSheet("border-image:url(./images/AuxiliaryPanelBack.png);");
}

UIBulletSet::~UIBulletSet()
{

}

void UIBulletSet::setMainWindow(UIBulletScreen* parent)
{
	m_parent = parent;
}

void UIBulletSet::focusOutEvent(QFocusEvent* e)
{
//	this->hide();
}

void UIBulletSet::closeDialog()
{
	this->hide();
}

void UIBulletSet::Font1()
{
	ui.font1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font1_sel.png);background-color: rgb(255,255,255);}");
	ui.font2_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font2_nor.png);background-color: rgb(255,255,255);}");
	ui.font3_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font3_nor.png);background-color: rgb(255,255,255);}");
	ui.font4_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font4_nor.png);background-color: rgb(255,255,255);}");
	if (m_parent)
		m_parent->SetFontSize(9);
}

void UIBulletSet::Font2()
{
	ui.font1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font1_nor.png);background-color: rgb(255,255,255);}");
	ui.font2_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font2_sel.png);background-color: rgb(255,255,255);}");
	ui.font3_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font3_nor.png);background-color: rgb(255,255,255);}");
	ui.font4_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font4_nor.png);background-color: rgb(255,255,255);}");
	if (m_parent)
		m_parent->SetFontSize(10);
}

void UIBulletSet::Font3()
{
	ui.font1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font1_nor.png);background-color: rgb(255,255,255);}");
	ui.font2_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font2_nor.png);background-color: rgb(255,255,255);}");
	ui.font3_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font3_sel.png);background-color: rgb(255,255,255);}");
	ui.font4_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font4_nor.png);background-color: rgb(255,255,255);}");
	if (m_parent)
		m_parent->SetFontSize(12);
}

void UIBulletSet::Font4()
{
	ui.font1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font1_nor.png);background-color: rgb(255,255,255);}");
	ui.font2_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font2_nor.png);background-color: rgb(255,255,255);}");
	ui.font3_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font3_nor.png);background-color: rgb(255,255,255);}");
	ui.font4_pushButton->setStyleSheet("QPushButton{border-image:url(./images/font4_sel.png);background-color: rgb(255,255,255);}");
	if (m_parent)
		m_parent->SetFontSize(14);
}

void UIBulletSet::Color1()
{
	ui.color1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color1_sel.png);background-color: rgb(255,255,255);}");
	ui.color2_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color2_nor.png);background-color: rgb(255,255,255);}");
	ui.color3_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color3_nor.png);background-color: rgb(255,255,255);}");
	ui.color4_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color4_nor.png);background-color: rgb(255,255,255);}");
	if (m_parent)
		m_parent->SetContentColor(QColor(0,0,0));
}

void UIBulletSet::Color2()
{
	ui.color1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color1_nor.png);background-color: rgb(255,255,255);}");
	ui.color2_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color2_sel.png);background-color: rgb(255,255,255);}");
	ui.color3_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color3_nor.png);background-color: rgb(255,255,255);}");
	ui.color4_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color4_nor.png);background-color: rgb(255,255,255);}");
	if (m_parent)
		m_parent->SetContentColor(QColor(255, 255, 255));
}

void UIBulletSet::Color3()
{
	ui.color1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color1_nor.png);background-color: rgb(255,255,255);}");
	ui.color2_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color2_nor.png);background-color: rgb(255,255,255);}");
	ui.color3_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color3_sel.png);background-color: rgb(255,255,255);}");
	ui.color4_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color4_nor.png);background-color: rgb(255,255,255);}");
	if (m_parent)
		m_parent->SetContentColor(QColor(255, 84, 0));
}

void UIBulletSet::Color4()
{
	ui.color1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color1_nor.png);background-color: rgb(255,255,255);}");
	ui.color2_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color2_nor.png);background-color: rgb(255,255,255);}");
	ui.color3_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color3_nor.png);background-color: rgb(255,255,255);}");
	ui.color4_pushButton->setStyleSheet("QPushButton{border-image:url(./images/color4_sel.png);background-color: rgb(255,255,255);}");
	if (m_parent)
		m_parent->SetContentColor(QColor(68, 224, 90));
}