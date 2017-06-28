#include "UIAuxiliarytoday.h"

UIAuxiliaryToday::UIAuxiliaryToday(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.enter_pushButton, SIGNAL(clicked()), this, SLOT(clickedEnter()));

	QFont font = ui.time_label->font();
	font.setPixelSize(13);
	ui.time_label->setFont(font);

	font = ui.course_label->font();
	font.setPixelSize(13);
	ui.course_label->setFont(font);

	font = ui.lesson_label->font();
	font.setPixelSize(13);
	ui.lesson_label->setFont(font);

	font = ui.status_label->font();
	font.setPixelSize(13);
	ui.status_label->setFont(font);
}

UIAuxiliaryToday::~UIAuxiliaryToday()
{

}

void UIAuxiliaryToday::AddLesson(QString lessonName, QString courseID, QString courseName, QString time, QString status)
{
	m_courseID = courseID;

	ui.course_label->setText(courseName);
	ui.lesson_label->setText("�γ�:" + lessonName);
	ui.time_label->setText(time);
	ui.status_label->setText(status);
// 	if (status == "ֱ����")
// 		ui.status_label->setStyleSheet("color:rgb(255,0,0)");

	int fontSize = ui.course_label->fontMetrics().width(courseName);//��ȡ֮ǰ���õ��ַ��������ش�С
	if (fontSize >= 190) //��label������Ƚ�
	{
		QString  tmpStr = ui.course_label->fontMetrics().elidedText(courseName, Qt::ElideRight, 190);
		ui.course_label->setText(tmpStr);
		ui.course_label->setToolTip(courseName);
	}

	fontSize = ui.lesson_label->fontMetrics().width("�γ�:" + lessonName);//��ȡ֮ǰ���õ��ַ��������ش�С
	if (fontSize >= 190) //��label������Ƚ�
	{
		QString  tmpStr = ui.lesson_label->fontMetrics().elidedText("�γ�:" + lessonName, Qt::ElideRight, 190);
		ui.lesson_label->setText(tmpStr);
		ui.lesson_label->setToolTip("�γ�:" + lessonName);
	}
}

QString UIAuxiliaryToday::GetCourseID()
{
	return m_courseID;
}

void UIAuxiliaryToday::clickedEnter()
{
	emit clickAuxiliaryToday(this);
}

void UIAuxiliaryToday::enterEvent(QEvent *e)
{
	QString str = QString::number(ui.widget_2->width()) + QString::number(ui.widget_2->height());
	qInfo(str.toUtf8().data());

//	setStyleSheet("background-color: rgb(251, 239, 193)");
	return;
}

void UIAuxiliaryToday::leaveEvent(QEvent* e)
{
//	setStyleSheet("background-color: rgb(255, 255, 255)");
	return;
}

void UIAuxiliaryToday::setStatue(QString status)
{
	ui.status_label->setText(status);
}