#ifndef UILIVELESSONWND_H
#define UILIVELESSONWND_H

#include <QWidget>
#include "ui_UILivelessonwnd.h"
#include "UILivelessonitem.h"
#include <QScrollArea>

#pragma execution_character_set("utf-8")

struct LessonInfo
{
	QString m_time;			// 上课时间
	QString m_status;		// 上课状态
	QString m_LessonID;		// 课程ID
	QString m_CourseID;		// 辅导班ID
	QString m_BoardUrl;		// 白板url
	QString m_CameraUrl;	// 摄像头url
	QString m_name;			// 课程名字
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

	// 鼠标按下拖动的操作
	QPoint								m_startPos;
	QPoint								m_clickPos;
	QPoint								m_WndCurPos;

	// 课程类型
	int									m_CurrentLessonType;
	QString								m_homePage;
	QString								m_TeacherToken;
	QString								m_BoardURL;
	QString								m_CameraURL;
	QString								m_CourseID;

signals:
	void sig_PullStreaming(QString, QString, QString, QString, QString,int, bool);
	void sig_changeLessonStatus(QString,QString);
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual bool eventFilter(QObject *target, QEvent *event);
	virtual void focusOutEvent(QFocusEvent *);

private slots:
	void clickClose();
	void slot_dbClick(QString, QString, QString, QString, int, QString, bool = false);
private:
	Ui::UILiveLessonWnd ui;
	std::vector<LessonInfo>				m_vecLessonInfo;
	std::vector<UILiveLessonItem*>		m_vecLessonItem;
public:
	void style(QScrollArea *style);
	void changeStatus(QString id);
	void DeleteItem();
	void setCourseID(QString courseid, int mLessonType);
	void addLesson(QString  id, QString courseid, QString boardUrl, QString cameraUrl, QString time, QString status, QString name);
	void AddEveryTimeLesson();		// 添加即时直播
	void SetURLBasicInfo(QString mHomePage, QString mTeacherToken, QString mBoardURL, QString mCameraURL);
};

#endif // UILIVELESSONWND_H
