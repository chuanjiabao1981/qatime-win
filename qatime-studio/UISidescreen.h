#ifndef UISIDESCREEN_H
#define UISIDESCREEN_H

#include <QWidget>
#include "ui_UISidescreen.h"
#include "uimainwindow.h"

class UIMainWindow;
class UISideScreen : public QWidget
{
	Q_OBJECT

public:
	UISideScreen(QWidget *parent = 0);
	~UISideScreen();

private:
	Ui::UISideScreen ui;
	UIMainWindow*		m_parent;

signals:
	void emit_HideSideScreen();					// ���ظ���
	void emit_SwichScreen();					// �л���Ļ

private slots:
	void slot_swichScreen();
	void slot_packScreen();
public:
	void SetMainWindow(UIMainWindow* hwnd);
	void ChangeTip(QString Change);
};

#endif // UISIDESCREEN_H
