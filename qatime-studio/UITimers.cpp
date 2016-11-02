#include "UITimers.h"

UITimers::UITimers(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
// 	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: red;border-radius: 10px; }"
// 		"QPushButton{border: 2px groove gray; border - style: outset;}"
// 		"QPushButton:pressed{border-style: inset; }");
//	ui.Live_pushBtn->setIcon(QIcon("./images/starimage.png"));
	connect(ui.Live_pushBtn, SIGNAL(clicked()), this, SLOT(slot_startOrStopLiveStream()));
}
UITimers::~UITimers()
{

}
//¿ªÊ¼Ö±²¥
void UITimers::slot_startOrStopLiveStream()
{
	emit m_Parent->slot_startOrStopLiveStream();
}
void UITimers::setLivePushBtnText(bool bStart)
{
	if (bStart)
		ui.Live_pushBtn->setStyleSheet("QPushButton{border-image: url(./images/starimage.png); }");
	else
		ui.Live_pushBtn->setStyleSheet("QPushButton{border-image: url(./images/endLive.png); }"); 
}
void UITimers::setTimeLabelVisible(bool b)
{
	ui.time_label->setVisible(b);
}
void UITimers::setTimeLabelText(QString text)
{
	ui.time_label->setText(text);
}

void UITimers::setParent(UIMenuTool* p)
{
	m_Parent = p;
}

void UITimers::setImage()
{
	ui.Live_pushBtn->setStyleSheet("QPushButton{border-image: url(./images/Continue.png); }");
}