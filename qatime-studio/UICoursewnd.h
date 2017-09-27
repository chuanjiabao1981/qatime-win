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
	void AddInfo(QString pic, QString courseName, QString grade, QString prosses, QString time, QString desc, QString mCourseTarget, QString mFitPeople, QString mCourseProcesses, QString mName, QString mClassName);
	void style(QScrollArea *style);
	void setParentBtn(UIWindowSet* parent);
	void AddCourseTag(QJsonArray mJsonTags);	// 添加课程标签
	void CleanCourseInfo();						// 清除课程里的一些信息
};

#endif // UICOURSEWND_H
