#ifndef UIAUXILIARYLIST_H
#define UIAUXILIARYLIST_H

#include <QWidget>
#include "ui_UIAuxiliarylist.h"
#include "UIWorkThread.h"

class UIAuxiliaryList : public QWidget
{
	Q_OBJECT

public:
	UIAuxiliaryList(QWidget *parent = 0);
	~UIAuxiliaryList();

private:
	Ui::UIAuxiliaryList ui;
	QString				m_picUrl;
	QString				m_courseName;
	QString				m_teacherName;
	QString				m_chatID;
	QString				m_courseID;
	QString				m_teacherID;
	QString				m_token;
	QString				m_studentName;
	std::string			m_AudioPath;
	QString				m_status;
	int					m_iMsgCount;	// ��Ϣ����
	QString				m_boardUrl;
	QString				m_cameraUrl;

	Worker*				m_pWorker;
	int					m_LessonType;	// ����������

public:
	QString				PicUrl();
	QString				CourseName();
	QString				CourseID();
	QString				ChatID();
	QString				TeacherID();
	QString				TeacherName();
	QString				Token();
	std::string			AudioPath();
	QString				StudentName();
	QString				Status();
	int					UnreadMsgCount();
	int					GetLessonType();
	QString				BoardUrl();
	QString				CameraUrl();

signals:
	void clickAuxiliary(UIAuxiliaryList*);
	void sig_StartLoading();

protected:
	virtual void mousePressEvent(QMouseEvent *e);       //--��갴���¼� 
	virtual void enterEvent(QEvent *e);
	virtual void leaveEvent(QEvent* e);

public:
	// Ĭ����������Ϊֱ����(1-ֱ���� 2-1V1 3-ר����) add by zbc 20170802
	QLabel* AddCourse(QString picUrl, QString courseName, QString grade, QString teacherName, QString chatID, QString courseID, QString teacherID, QString token,
		QString studentName, std::string AudioPath, QString status, QString url, QString cameraUrl, int mLessonType = 1); // �½�������

	void AddMsgCount();			// �ı���Ϣ����
	void ClearMsgNumber();		// �����Ϣ��
};

#endif // UIAUXILIARYLIST_H
