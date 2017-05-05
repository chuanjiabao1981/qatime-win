#ifndef UIAPPWNDTOOL_H
#define UIAPPWNDTOOL_H

#include <QWidget>
#include "ui_UIAppWndTool.h"

class UIAppWndTool : public QWidget
{
	Q_OBJECT

public:
	UIAppWndTool(QWidget *parent = 0);
	~UIAppWndTool();

private:
	Ui::UIAppWndTool ui;

signals:
	void sig_shiftWnd();
	void sig_CloseWnd();

private slots:
	void clickShift();
	void clickClose();
};

#endif // UIAPPWNDTOOL_H
