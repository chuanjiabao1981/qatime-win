#ifndef UILIVELESSONWND_H
#define UILIVELESSONWND_H

#include <QWidget>
#include "ui_UILivelessonwnd.h"
#include "UILivelessonitem.h"
#include <QScrollArea>

#pragma execution_character_set("utf-8")

struct LessonInfo
{
	QString m_time;			// �Ͽ�ʱ��
	QString m_status;		// �Ͽ�״̬
	QString m_LessonID;		// �γ�ID
	QString m_CourseID;		// ������ID
	QString m_BoardUrl;		// �װ�url
	QString m_CameraUrl;	// ����ͷurl
	QString m_name;			// �γ�����
};

class UILiveLessonWnd : public QWidget
{
	Q_OBJECT

public:
	UILiveLessonWnd(QWidget *parent = 0);
	~UILiveLessonWnd();

	QWidget*							m_mainAllView;
	QVBoxLayout*						m_VerAll;
	QSpacerItem*						m_spacerAll;

	// ��갴���϶��Ĳ���
	QPoint								m_startPos;
	QPoint								m_clickPos;
	QPoint								m_WndCurPos;

	// �Ƿ���1v1�γ�
	bool								m_b1v1Lesson;

signals:
	void sig_PullStreaming(QString, QString, QString, QString, QString,bool);
	void sig_changeLessonStatus(QString,QString);
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual bool eventFilter(QObject *target, QEvent *event);

private slots:
	void clickClose();
	void slot_dbClick(QString, QString, QString, QString, int, QString);
private:
	Ui::UILiveLessonWnd ui;
	std::vector<LessonInfo>				m_vecLessonInfo;
	std::vector<UILiveLessonItem*>		m_vecLessonItem;
public:
	void style(QScrollArea *style);
	void changeStatus(QString id);
	void DeleteItem();
	void setCourseID(QString courseid, bool b1v1Lesson);
	void addLesson(QString  id, QString courseid, QString boardUrl, QString cameraUrl, QString time, QString status, QString name);
};

#endif // UILIVELESSONWND_H
