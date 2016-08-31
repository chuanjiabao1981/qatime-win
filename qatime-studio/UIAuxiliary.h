#ifndef UIAUXILIARY_H
#define UIAUXILIARY_H

#include <QWidget>
#include "ui_UIAuxiliary.h"
#include <stdbool.h>

#pragma execution_character_set("utf-8")

class UIAuxiliary : public QWidget
{
	Q_OBJECT

public:
	UIAuxiliary(QWidget *parent = 0);
	~UIAuxiliary();

private:
	Ui::UIAuxiliary ui;
	QString mLessonID;
	bool bFirstInit;

private slots :
	void setLessonID();

public:
	void setAuxiliaryInfo(QJsonObject &obj);
	void initCourses();
	QString getLessonID();
	bool IsInit();
};

#endif // UIAUXILIARY_H
