#ifndef UICAMERA_H
#define UICAMERA_H

#include <QWidget>
#include "ui_UICamera.h"
#include "UIMainwindow.h"

class UIMainWindow;
class UICamera : public QWidget
{
	Q_OBJECT

public:
	UICamera(QWidget *parent = 0);
	~UICamera();

private:
	Ui::UICamera ui;
	UIMainWindow*    m_parent;

public:
	void SetMainWindow(UIMainWindow* hwnd);
};

#endif // UICAMERA_H
