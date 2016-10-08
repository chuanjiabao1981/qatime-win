#ifndef UILESSONTABLE_H
#define UILESSONTABLE_H

#include <QWidget>
#include "ui_UILessontable.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "UIMainwindow.h"

class UIMainWindow;
class UILessonTable : public QWidget
{
	Q_OBJECT

public:
	UILessonTable(QWidget *parent = 0);
	~UILessonTable();

private:
	Ui::UILessonTable ui;

	UIMainWindow*					m_MainWindow;	// 主窗口

	QNetworkAccessManager			manager;
	QNetworkReply*					reply;
	QString							mRemeberToken;
	QString							m_teacherID;

	QString							m_LessonID;		// 课程ID
	QString							m_CourseID;		// 辅导班ID
	QString							m_LessonName;	// 课程名字
	QString							m_LessonTime;	// 课程时间
	QString							m_Status;		// 课程状态

	bool							m_bInit;		// 是否初始化

private:
	void init();
	void returnLesson();

private slots :
	void LessonDoubleClicked(int row, int column);
	void SelectItem();
	void CancelSelect();
public:
	void InitToken(QString sToken, QString sTeacherID);
	void RequestLesson();
	void SetMainWindow(UIMainWindow* mainWnd);
};

#endif // UILESSONTABLE_H
