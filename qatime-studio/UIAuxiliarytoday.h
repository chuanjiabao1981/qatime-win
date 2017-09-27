#ifndef UIAUXILIARYTODAY_H
#define UIAUXILIARYTODAY_H

#include <QWidget>
#include "ui_UIAuxiliarytoday.h"

#pragma execution_character_set("utf-8")
class UIAuxiliaryToday : public QWidget
{
	Q_OBJECT

public:
	UIAuxiliaryToday(QWidget *parent = 0);
	~UIAuxiliaryToday();

	QString			m_courseID;		 // 辅导班ID
	int				m_LessonType;	 // 辅导班类型
	QString			GetCourseID();	 // 获取辅导班ID
private:
	Ui::UIAuxiliaryToday ui;

protected:
	virtual void enterEvent(QEvent *e);
	virtual void leaveEvent(QEvent* e);

signals:
	void clickAuxiliaryToday(UIAuxiliaryToday*);

private slots :
	void clickedEnter();
public:
	void AddLesson(QString lessonName, QString courseID, QString courseName, QString time, QString status);
	void setStatue(QString status);
};

#endif // UIAUXILIARYTODAY_H
