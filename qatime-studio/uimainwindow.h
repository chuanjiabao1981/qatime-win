#ifndef UIMAINWINDOW_H
#define UIMAINWINDOW_H

#include <QWidget>
#include "ui_uimainwindow.h"

class UIMainWindow : public QWidget
{
	Q_OBJECT

public:
	UIMainWindow(QWidget *parent = 0);
	~UIMainWindow();

private:
	Ui::UIMainWindow ui;

private slots :
	void MinDialog();			// 最小化对话框
	void CloseDialog();			// 关闭对话框
};

#endif // UIMAINWINDOW_H
