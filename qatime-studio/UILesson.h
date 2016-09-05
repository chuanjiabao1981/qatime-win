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

	QString		m_sLessonID;

private slots :
	void SelectionChanged();

public:
	void	initLesson();
	void	setLessonInfo(QJsonObject &obj);
	QString GetSelectedLessonId();
};

#endif // UILESSON_H
