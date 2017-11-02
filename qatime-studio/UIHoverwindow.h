#ifndef UIHOVERWINDOW_H
#define UIHOVERWINDOW_H

#include <QWidget>
#include "ui_UIHoverwindow.h"
#include "UIMainNewWindow.h"

class UIMainNewWindow;

class UIHoverWindow : public QWidget
{
	Q_OBJECT

public:
	UIHoverWindow(QWidget *parent = 0);
	~UIHoverWindow();

private:
	Ui::UIHoverWindow ui;
	UIMainNewWindow*  m_Parent;
	int				  m_iCount;

protected:
	virtual void enterEvent(QEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *event);

public:
	void setParentWindow(UIMainNewWindow* parent);
	void SetLiveTimer(QString sTimer);
	void UpdateChatNumber();					  // 更新聊天个数
	void SetNumber();
};

#endif // UIHOVERWINDOW_H
