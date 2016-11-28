#ifndef UIHOVERWINDOW_H
#define UIHOVERWINDOW_H

#include <QWidget>
#include "ui_UIHoverwindow.h"
#include "UIMainwindow.h"

class UIMainWindow;

class UIHoverWindow : public QWidget
{
	Q_OBJECT

public:
	UIHoverWindow(QWidget *parent = 0);
	~UIHoverWindow();

private:
	Ui::UIHoverWindow ui;
	UIMainWindow*     m_Parent;
	int				  m_iCount;

protected:
	virtual void enterEvent(QEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *event);

public:
	void setParentWindow(UIMainWindow* parent);
	void SetLiveTimer(QString sTimer);
	void UpdateChatNumber();					  // �����������
	void SetNumber();
};

#endif // UIHOVERWINDOW_H
