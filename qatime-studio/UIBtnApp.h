/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#ifndef _LS_UI_BtnApp_APP_H_
#define _LS_UI_BtnApp_APP_H_
#include <QtWidgets/QRadioButton>
#pragma execution_character_set("utf-8")

class CBtnApp : public QRadioButton
{
	Q_OBJECT

public:
	CBtnApp(QWidget *parent = 0);
	~CBtnApp();

Q_SIGNALS:
	void sig_idclicked(HWND);

public:
	void AddWndName(HWND hwnd, QString titleName);
	void clickWnd();
protected:
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
private:
	HWND		m_wnd;

};

#endif // _LS_MEDIACAPTURE_UI_BtnApp_H_