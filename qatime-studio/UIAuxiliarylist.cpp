#include "UIAuxiliarylist.h"
#include <QUrl>
#include <QNetworkAccessManager>
#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMouseEvent>

UIAuxiliaryList::UIAuxiliaryList(QWidget *parent)
	: QWidget(parent)
	, m_iMsgCount(0)
	, m_pWorker(NULL)
	, m_b1v1Lesson(false)
{
	ui.setupUi(this);
	setCursor(Qt::PointingHandCursor);
	setMouseTracking(true);
	ui.number_label->setText("");

	m_pWorker = new Worker();
	connect(this, SIGNAL(sig_StartLoading()), m_pWorker, SLOT(slot_StartLoading()));

	QFont font = ui.name_label->font();
	font.setPixelSize(13);
	ui.name_label->setFont(font);

	font = ui.grade_label->font();
	font.setPixelSize(12);
	ui.grade_label->setFont(font);

	font = ui.number_label->font();
	font.setPixelSize(12);
	ui.number_label->setFont(font);
}

UIAuxiliaryList::~UIAuxiliaryList()
{

}


QLabel* UIAuxiliaryList::AddCourse(QString picUrl, QString courseName, QString grade, QString teacherName, QString chatID, QString courseID, 
	QString teacherID, QString token, QString studentName, std::string AudioPath, QString status, QString url, QString cameraUrl, bool b1v1Lesson)
{
	m_picUrl = picUrl;
	m_courseName = courseName;
	m_teacherName = teacherName;
	m_chatID = chatID;
	m_courseID = courseID;
	m_teacherID = teacherID;
	m_token = token;
	m_studentName = studentName;
	m_AudioPath = AudioPath;
	m_status = status;
	m_b1v1Lesson = b1v1Lesson;
	m_boardUrl = url;
	m_cameraUrl = cameraUrl;

	ui.name_label->setText(courseName);
	ui.grade_label->setText(grade + "|" + teacherName);


	int fontSize = ui.name_label->fontMetrics().width(courseName);//获取之前设置的字符串的像素大小
	if (fontSize >= 150) //与label自身相比较
	{
		QString  tmpStr = ui.name_label->fontMetrics().elidedText(courseName, Qt::ElideRight, 150);
		ui.name_label->setText(tmpStr);
		ui.name_label->setToolTip(courseName);
	}

	fontSize = ui.grade_label->fontMetrics().width(grade + "|" + teacherName);//获取之前设置的字符串的像素大小
	if (fontSize >= 150) //与label自身相比较
	{
		QString  tmpStr = ui.grade_label->fontMetrics().elidedText(grade + "|" + teacherName, Qt::ElideRight, 150);
		ui.grade_label->setText(tmpStr);
		ui.grade_label->setToolTip(grade + "|" + teacherName);
	}

// 	if (status == "teaching")
// 		ui.teaching_label->setVisible(true);
// 	else
	ui.teaching_label->setVisible(false);

	return ui.label;
}

void UIAuxiliaryList::mousePressEvent(QMouseEvent *e)
{
	if (Qt::LeftButton == e->button())
	{
		emit clickAuxiliary(this);
		return;
	}
}

// 是否是1v1课程
bool UIAuxiliaryList::Is1v1Lesson()
{
	return m_b1v1Lesson;
}

void UIAuxiliaryList::enterEvent(QEvent *e)
{
	setStyleSheet("background-color: rgb(251, 239, 193)");
}

void UIAuxiliaryList::leaveEvent(QEvent* e)
{
	setStyleSheet("background-color: rgb(255, 255, 255)");
}

QString	UIAuxiliaryList::PicUrl()
{
	return m_picUrl;
}
QString	UIAuxiliaryList::CourseName()
{
	return m_courseName;
}
QString	UIAuxiliaryList::CourseID()
{
	return m_courseID;
}
QString	UIAuxiliaryList::ChatID()
{
	return m_chatID;
}
QString	UIAuxiliaryList::TeacherID()
{
	return m_teacherID;
}
QString	UIAuxiliaryList::TeacherName()
{
	return m_teacherName;
}
QString	UIAuxiliaryList::Token()
{
	return m_token;
}
std::string	UIAuxiliaryList::AudioPath()
{
	return m_AudioPath;
}
QString UIAuxiliaryList::StudentName()
{
	return m_studentName;
}

QString UIAuxiliaryList::Status()
{
	return m_status;
}

void UIAuxiliaryList::AddMsgCount()
{
	m_iMsgCount++;
	QString strNum = QString::number(m_iMsgCount);
	ui.number_label->setText(strNum);
	ui.pic_label->setStyleSheet("border-image: url(:/LoginWindow/images/session.png);");
}

void UIAuxiliaryList::ClearMsgNumber()
{
	m_iMsgCount = 0;
	ui.number_label->setText("");
	ui.pic_label->setStyleSheet("border-image: url(:/LoginWindow/images/.png);");
}

int UIAuxiliaryList::UnreadMsgCount()
{
	return m_iMsgCount;
}

QString UIAuxiliaryList::BoardUrl()
{
	return m_boardUrl;
}

QString UIAuxiliaryList::CameraUrl()
{
	return m_cameraUrl;
}