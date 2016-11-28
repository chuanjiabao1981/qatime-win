#ifndef UISEELIVEWND_H
#define UISEELIVEWND_H

#include <QWidget>
#include "ui_UISeelivewnd.h"
#include "UIMainwindow.h"
#include <windowsx.h>

class UIMainWindow;

class UISeeLiveWnd : public QWidget
{
	Q_OBJECT

public:
	UISeeLiveWnd(QWidget *parent = 0);
	~UISeeLiveWnd();

private:
	Ui::UISeeLiveWnd ui;
	UIMainWindow*			m_Parent;

protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private slots :
	void CloseDialog();									// ¹Ø±Õ¶Ô»°¿ò

public:
	void SetMainWnd(UIMainWindow* parent);
};

#endif // UISEELIVEWND_H
