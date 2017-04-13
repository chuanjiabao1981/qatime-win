#ifndef UIRATIO_H
#define UIRATIO_H

#include <QWidget>
#include "ui_UIRatio.h"
#include "UIWindowSet.h"

#pragma execution_character_set("utf-8")
class UIWindowSet;
class UIRatio : public QWidget
{
	Q_OBJECT

public:
	UIRatio(QWidget *parent = 0);
	~UIRatio();

private:
	Ui::UIRatio ui;
	UIWindowSet*	m_Parent;

	// 鼠标按下拖动的操作
	QPoint								m_startPos;
	QPoint								m_clickPos;
	QPoint								m_WndCurPos;
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual bool eventFilter(QObject *target, QEvent *event);
	virtual void focusOutEvent(QFocusEvent *);

private slots:
	void RatioChanged(int index);
	void clickClose();

public:
	void setVideoChange(UIWindowSet* Parent);
};

#endif // UIRATIO_H
