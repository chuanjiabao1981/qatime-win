#ifndef UILIVELESSONITEM_H
#define UILIVELESSONITEM_H

#include <QWidget>
#include "ui_UILivelessonitem.h"

class UILiveLessonItem : public QWidget
{
	Q_OBJECT

public:
	UILiveLessonItem(QWidget *parent = 0);
	~UILiveLessonItem();

	int			m_index;		//	索引,用他查找上一节课程
	QString		m_id;
	QString		m_chatid;
	QString		m_boardUrl;
	QString		m_cameraUrl;
	QString		m_status;
	QString		m_name;
	bool		m_bEverytime;	// 是否为即时直播

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent*);
	virtual void enterEvent(QEvent *e);
	virtual void leaveEvent(QEvent* e);

signals:
	void sig_doubleclick(QString,QString,QString,QString,int,QString,bool);// 课程id,辅导班id,白板推流,摄像头推流,位置索引,课程名称,是否为即时直播

private slots:
	void clicked();
private:
	Ui::UILiveLessonItem ui;

public:
	void AddLesson(QString id, QString chatid, QString boardUrl, QString cameraUrl, QString time, QString status, QString name,int index);
	void AddAuToDefineLesson(QString id, QString chatid, QString boardUrl, QString cameraUrl, QString time, QString status, QString name, int index, int mLessonType);
};

#endif // UILIVELESSONITEM_H
