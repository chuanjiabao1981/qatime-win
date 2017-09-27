#include "UILivelessonitem.h"

UILiveLessonItem::UILiveLessonItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.pushButton->setStyleSheet("border-image: url(./images/startLive.png);");
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(clicked()));
	m_bEverytime = false;		//初始化即时直播为false
}

UILiveLessonItem::~UILiveLessonItem()
{

}

void UILiveLessonItem::clicked()
{
	if (m_bEverytime == true)
	{
		emit sig_doubleclick(m_id, m_chatid, m_boardUrl, m_cameraUrl, m_index, m_name, true);
	}
	else
	{
		emit sig_doubleclick(m_id, m_chatid, m_boardUrl, m_cameraUrl, m_index, m_name, false);
	}
	
}

void UILiveLessonItem::AddLesson(QString id, QString chatid, QString boardUrl, QString cameraUrl, QString time, QString status, QString name, int index)
{
	m_name = name;
	m_index = index;
	m_id = id;
	m_chatid = chatid;
	m_boardUrl = boardUrl;
	m_cameraUrl = cameraUrl;
	m_status = status;

	
	ui.time_label->setText(time);
	ui.status_label->setText(status);

	ui.name_label->setText(name);
	int fontSize = ui.name_label->fontMetrics().width(name);//获取之前设置的字符串的像素大小
	if (fontSize >= ui.name_label->width()) //与label自身相比较
	{
		QString  tmpStr = ui.name_label->fontMetrics().elidedText(name, Qt::ElideRight, ui.name_label->width());
		ui.name_label->setText(tmpStr);
		ui.name_label->setToolTip(name);
	}
}

void UILiveLessonItem::AddAuToDefineLesson(QString id, QString chatid, QString boardUrl, QString cameraUrl, QString time, QString status, QString name, int index, int mLessonType)
{
	m_name = name;
	m_index = index;
	m_id = id;
	m_chatid = chatid;
	m_boardUrl = boardUrl;
	m_cameraUrl = cameraUrl;
	m_status = status;
	m_bEverytime = true;	// 更改即时直播状态
	ui.time_label->setText(time);
	ui.time_label->setMaximumWidth(250);
	ui.name_label->setMinimumWidth(1);
	ui.status_label->setText(status);

	ui.name_label->setText(name);
	int fontSize = ui.name_label->fontMetrics().width(name);//获取之前设置的字符串的像素大小
	if (fontSize >= ui.name_label->width()) //与label自身相比较
	{
		QString  tmpStr = ui.name_label->fontMetrics().elidedText(name, Qt::ElideRight, ui.name_label->width());
		ui.name_label->setText(tmpStr);
		ui.name_label->setToolTip(name);
	}
}

void UILiveLessonItem::mouseDoubleClickEvent(QMouseEvent*)
{
//	emit sig_doubleclick(m_id,m_chatid,m_boardUrl,m_cameraUrl,m_index,m_name);
}

void UILiveLessonItem::enterEvent(QEvent *e)
{
//	setStyleSheet("background-color: rgb(251, 239, 193)");
}

void UILiveLessonItem::leaveEvent(QEvent* e)
{
//	setStyleSheet("background-color: rgb(255, 255, 255)");
}