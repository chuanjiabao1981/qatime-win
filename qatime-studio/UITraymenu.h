#ifndef UITRAYMENU_H
#define UITRAYMENU_H

#include <QWidget>
#include "ui_UITraymenu.h"
#include "loginwindow.h"

class LoginWindow;
class UITrayMenu : public QWidget
{
	Q_OBJECT

public:
	UITrayMenu(QWidget *parent = 0);
	~UITrayMenu();

private:
	Ui::UITrayMenu ui;
	LoginWindow*	m_parent;

protected:
	virtual void focusOutEvent(QFocusEvent *);

private slots :
	void MainDialog();									// ÏÔÊ¾Ö÷´°¿Ú
	void CloseDialog();									// ¹Ø±Õ
	void ReturnDialog();								// ÇÐ»»ÕËºÅ

public:
	void SetMainWindow(LoginWindow* parent);
};

#endif // UITRAYMENU_H
