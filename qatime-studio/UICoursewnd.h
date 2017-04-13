#ifndef UICOURSEWND_H
#define UICOURSEWND_H

#include <QWidget>
#include "ui_UICoursewnd.h"
#include <QPushButton>
#include "UIWindowSet.h"

class UIWindowSet;
class UICourseWnd : public QWidget
{
	Q_OBJECT

public:
	UICourseWnd(QWidget *parent = 0);
	~UICourseWnd();

private:
	Ui::UICourseWnd ui;
	QPushButton*		m_btn;
	UIWindowSet*				 m_parent;
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void focusOutEvent(QFocusEvent *);

public:
	void setNetworkPic(const QString &szUrl);
	void AddInfo(QString pic, QString courseName, QString grade, QString prosses, QString time, QString desc);
	void style(QScrollArea *style);
	void setParentBtn(UIWindowSet* parent);
};

#endif // UICOURSEWND_H
