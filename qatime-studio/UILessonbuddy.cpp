#include "UILessonbuddy.h"
#include <QScrollBar>
#include "define.h"

UILessonBuddy::UILessonBuddy(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QFont font = ui.number_label->font();
	font.setPixelSize(13);
	ui.number_label->setFont(font);

	font = ui.time_label->font();
	font.setPixelSize(13);
	ui.time_label->setFont(font);

	font = ui.name_label->font();
	font.setPixelSize(13);
	ui.name_label->setFont(font);

	font = ui.status_label->font();
	font.setPixelSize(13);
	ui.status_label->setFont(font);
}

UILessonBuddy::~UILessonBuddy()
{

}

void UILessonBuddy::AddLesson(QString number, QString strTime, QString strName, QString strStatus)
{
	if (ChinaLessonStatus(strStatus) =="ÒÑ½áÊø")
	{
		ui.time_label->setStyleSheet("color:rgb(153,153,153)");
		ui.name_label->setStyleSheet("color:rgb(153,153,153)");
		ui.status_label->setStyleSheet("color:rgb(153,153,153)");
	}
	else
	{
		ui.time_label->setStyleSheet("color:rgb(102,102,102)");
		ui.name_label->setStyleSheet("color:rgb(102,102,102)");
		ui.status_label->setStyleSheet("color:rgb(102,102,102)");
	}

	ui.number_label->setText(number);
	ui.time_label->setText(strTime);
	ui.name_label->setText(strName);
	ui.status_label->setText(ChinaLessonStatus(strStatus));
}

QString UILessonBuddy::ChinaLessonStatus(QString mStatus)
{
	QString strChinaStatus;
	if (mStatus == "init")
	{
		strChinaStatus = MSG_LESSON_STATUS_INIT;
	}
	else if (mStatus == "ready")
	{
		strChinaStatus = MSG_LESSON_STATUS_READY;
	}
	else if (mStatus == "teaching")
	{
		strChinaStatus = MSG_LESSON_STATUS_THEACHING;
	}
	else if (mStatus == "paused")
	{
		strChinaStatus = MSG_LESSON_STATUS_PAUSED;
	}
	else if (mStatus == "closed")
	{
		strChinaStatus = MSG_LESSON_STATUS_CLOSED;
	}
	else if (mStatus == "finished")
	{
		strChinaStatus = MSG_LESSON_STATUS_FINISHED;
	}
	else if (mStatus == "billing")
	{
		strChinaStatus = MSG_LESSON_STATUS_BILLING;
	}
	else if (mStatus == "completed")
	{
		strChinaStatus = MSG_LESSON_STATUS_COMPLETED;
	}
	else if (mStatus == "missed")
	{
		strChinaStatus = MSG_LESSON_STATUS_MISSED;
	}

	return strChinaStatus;
}