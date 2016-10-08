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

	UIMainWindow*					m_MainWindow;	// ������

	QNetworkAccessManager			manager;
	QNetworkReply*					reply;
	QString							mRemeberToken;
	QString							m_teacherID;

	QString							m_LessonID;		// �γ�ID
	QString							m_CourseID;		// ������ID
	QString							m_LessonName;	// �γ�����
	QString							m_LessonTime;	// �γ�ʱ��
	QString							m_Status;		// �γ�״̬

	bool							m_bInit;		// �Ƿ��ʼ��

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
