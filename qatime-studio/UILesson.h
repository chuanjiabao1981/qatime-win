#ifndef UILESSON_H
#define UILESSON_H

#include <QWidget>
#include "ui_UILesson.h"

#pragma execution_character_set("utf-8")

class UILesson : public QWidget
{
	Q_OBJECT

public:
	UILesson(QWidget *parent = 0);
	~UILesson();

private:
	Ui::UILesson ui;

private slots :
	void Test();

public:
	void initLesson();
	void setLessonInfo(QJsonObject &obj);
};

#endif // UILESSON_H
