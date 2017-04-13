#ifndef UILESSONBUDDY_H
#define UILESSONBUDDY_H

#include <QWidget>
#include "ui_UILessonbuddy.h"
#include <QScrollArea>

#pragma execution_character_set("utf-8")

class UILessonBuddy : public QWidget
{
	Q_OBJECT

public:
	UILessonBuddy(QWidget *parent = 0);
	~UILessonBuddy();

private:
	Ui::UILessonBuddy ui;

public:
	void	AddLesson(QString number, QString strTime, QString strName, QString strStatus);
	void	style(QScrollArea *style);
	QString ChinaLessonStatus(QString status);
};

#endif // UILESSONBUDDY_H
