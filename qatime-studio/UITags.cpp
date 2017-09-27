#include "UITags.h"
#include <QMouseEvent>
#include <windows.h>

UITags::UITags(QWidget *parent)
	: QWidget(parent)
	, m_room(NULL)
	, m_bSelect(false)
	, m_boardStream("")
	, m_cameraStream("")
	, m_bModle(false)
	, m_mLessonType(1)
{
	ui.setupUi(this);
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	ui.name_label->setCursor(Qt::PointingHandCursor);

	QFont font = ui.name_label->font();
	font.setPixelSize(13);
	ui.name_label->setFont(font);

	font = ui.ellipsis_label->font();
	font.setPixelSize(13);
	ui.ellipsis_label->setFont(font);
}

UITags::~UITags()
{

}


void UITags::SetCourseNameAndID(QString sName, QString id, QString chatID, QString board, QString camera)
{
	m_boardUrl = board;
	m_cameraUrl = camera;
	m_name = sName;
	m_id = id;
	m_chatID = chatID;
	ui.name_label->setToolTip(m_name);
}

QString UITags::ChatID()
{
	return m_chatID; ////QString::number(36309071);
}

QString UITags::CourseID()
{
	return m_id;
}

void UITags::paintEvent(QPaintEvent *event)
{
 	ui.name_label->setText(m_name);                                       //设置label上的字符串

	int fontSize = fontMetrics().width(m_name);//获取之前设置的字符串的像素大小
	if (fontSize >= ui.name_label->width()) //与label自身相比较
	{
		ui.ellipsis_label->setVisible(true);
	}
	else
	{
		ui.ellipsis_label->setVisible(false);
	}

// 	if (m_bSelect)
// 	{
// 		ui.widget->setStyleSheet("background-color: rgb(0, 63, 86);border-radius:10px;");
// 		ui.name_label->setStyleSheet("color: rgb(255, 255, 255)");
// 		ui.ellipsis_label->setStyleSheet("color: rgb(255, 255, 255)");
// 		ui.close_pushButton->setStyleSheet("border-image: url(:/LoginWindow/images/select_close.png)");
// 
// 		if (m_bModle)
// 			ui.pic_label->setStyleSheet("border-image: url(./images/tags_video_hover.png);");
// 		else
// 			ui.pic_label->setStyleSheet("border-image: url(./images/tags_chat_hover.png);");
// 	}
// 	else
// 	{
// 		ui.widget->setStyleSheet("background-color: rgb(255, 255, 255);border-radius:10px;");		 
// 		ui.name_label->setStyleSheet("color: rgb(0, 0, 0)");
// 		ui.ellipsis_label->setStyleSheet("color: rgb(0, 0, 0)");
// 		ui.close_pushButton->setStyleSheet("border-image: url(:/LoginWindow/images/noselect_close.png)");
// 
// 		if (m_bModle)
// 			ui.pic_label->setStyleSheet("border-image: url(./images/tags_video_nor.png);");
// 		else
// 			ui.pic_label->setStyleSheet("border-image: url(./images/tags_chat_nor.png);");
// 	}
}

void UITags::CloseDialog()
{
	emit DeleteTag(this);
}

void UITags::SetRoom(UIChatRoom* room)
{
	m_room = room;
}

void UITags::setStyle(bool bSel)
{
	m_bSelect = bSel;

	if (m_bSelect)
	{
		ui.widget->setStyleSheet("background-color: rgb(0, 63, 86);border-radius:10px;");
		ui.name_label->setStyleSheet("color: rgb(255, 255, 255)");
		ui.ellipsis_label->setStyleSheet("color: rgb(255, 255, 255)");
		ui.close_pushButton->setStyleSheet("border-image: url(:/LoginWindow/images/select_close.png)");

		if (m_bModle)
			ui.pic_label->setStyleSheet("border-image: url(./images/tags_video_hover.png);");
		else
			ui.pic_label->setStyleSheet("border-image: url(./images/tags_chat_hover.png);");
	}
	else
	{
		ui.widget->setStyleSheet("background-color: rgb(255, 255, 255);border-radius:10px;");		 
		ui.name_label->setStyleSheet("color: rgb(0, 0, 0)");
		ui.ellipsis_label->setStyleSheet("color: rgb(0, 0, 0)");
		ui.close_pushButton->setStyleSheet("border-image: url(:/LoginWindow/images/noselect_close.png)");

		if (m_bModle)
			ui.pic_label->setStyleSheet("border-image: url(./images/tags_video_nor.png);");
		else
			ui.pic_label->setStyleSheet("border-image: url(./images/tags_chat_nor.png);");
	}
}

bool UITags::IsSelect()
{
	return m_bSelect;
}

void UITags::mousePressEvent(QMouseEvent *e)
{
	if (Qt::LeftButton == e->button())
	{
		if (IsSelect())
			return;

		emit clickTag(this);
	}
}

UIChatRoom* UITags::GetRoom()
{
	return m_room;
}

void UITags::setBoardStream(QString strBoard)
{
	m_boardStream = strBoard;
}

void UITags::setCameraStream(QString strCamera)
{
	m_cameraStream = strCamera;
}

QString UITags::BoardStream()
{
	return m_boardUrl;
}

QString UITags::CameraStream()
{
	return m_cameraUrl;
}

bool UITags::IsModle()
{
	return m_bModle;
}

void UITags::setModle(bool bModle)
{
	m_bModle = bModle;

	if (m_bSelect)
	{
		ui.widget->setStyleSheet("background-color: rgb(0, 63, 86);border-radius:10px;");
		ui.name_label->setStyleSheet("color: rgb(255, 255, 255)");
		ui.ellipsis_label->setStyleSheet("color: rgb(255, 255, 255)");
		ui.close_pushButton->setStyleSheet("border-image: url(:/LoginWindow/images/select_close.png)");

		if (m_bModle)
			ui.pic_label->setStyleSheet("border-image: url(./images/tags_video_hover.png);");
		else
			ui.pic_label->setStyleSheet("border-image: url(./images/tags_chat_hover.png);");
	}
	else
	{
		ui.widget->setStyleSheet("background-color: rgb(255, 255, 255);border-radius:10px;");
		ui.name_label->setStyleSheet("color: rgb(0, 0, 0)");
		ui.ellipsis_label->setStyleSheet("color: rgb(0, 0, 0)");
		ui.close_pushButton->setStyleSheet("border-image: url(:/LoginWindow/images/noselect_close.png)");

		if (m_bModle)
			ui.pic_label->setStyleSheet("border-image: url(./images/tags_video_nor.png);");
		else
			ui.pic_label->setStyleSheet("border-image: url(./images/tags_chat_nor.png);");
	}
}

void UITags::setLessonType(int mLessonType)
{
	m_mLessonType = mLessonType;
}

int UITags::GetLessonType()
{
	return m_mLessonType;
}