#ifndef UIRANGEWND_H
#define UIRANGEWND_H

#include <QWidget>
#include "ui_UIRangewnd.h"
#include "Rangecapture.h"

class RangeCapture;
class UIRangeWnd : public QWidget
{
	Q_OBJECT

public:
	UIRangeWnd(QWidget *parent = 0);
	~UIRangeWnd();

private:
	Ui::UIRangeWnd ui;
	RangeCapture*		m_parent;

	// 鼠标按下拖动的操作
	QPoint							m_startPos;
	QPoint							m_clickPos;
	QPoint							m_WndCurPos;	// 当前窗口
	QPoint							m_ParentWndPos; // 父窗口

protected:
	virtual bool eventFilter(QObject *target, QEvent *event);

public:
	void SetMainWnd(RangeCapture* parent);
	void UpdateMove();
};

#endif // UIRANGEWND_H
