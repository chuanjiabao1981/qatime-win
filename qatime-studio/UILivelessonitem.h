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

	int			m_index;		//	����,����������һ�ڿγ�
	QString		m_id;
	QString		m_chatid;
	QString		m_boardUrl;
	QString		m_cameraUrl;
	QString		m_status;
	QString		m_name;

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent*);
	virtual void enterEvent(QEvent *e);
	virtual void leaveEvent(QEvent* e);

signals:
	void sig_doubleclick(QString,QString,QString,QString,int,QString);// �γ�id,������id,�װ�����,����ͷ����,λ������,�γ�����
private:
	Ui::UILiveLessonItem ui;

public:
	void AddLesson(QString id, QString chatid, QString boardUrl, QString cameraUrl, QString time, QString status, QString name,int index);
};

#endif // UILIVELESSONITEM_H
