#ifndef UILESSONLIST_H
#define UILESSONLIST_H

#include <QWidget>
#include "ui_UILessonlist.h"
#include "UILessonbuddy.h"
#include <QPushButton>
#include "UIWindowSet.h"

class UIWindowSet;
class UILessonList : public QWidget
{
	Q_OBJECT

public:
	UILessonList(QWidget *parent = 0);
	~UILessonList();

private:
	Ui::UILessonList ui;

	std::vector<UILessonBuddy*>	 m_vecBuddy;
	QSpacerItem*				 m_spacer;
	QPushButton*				 m_btn;
	UIWindowSet*				 m_parent;
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void focusOutEvent(QFocusEvent *);

public:
	void AddLesson(QString number, QString strTime, QString strName, QString strStatus);
	void DeleteLesson();
	void style(QScrollArea *style);
	void setParentBtn(UIWindowSet* parent);
};

#endif // UILESSONLIST_H
