/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#include "UIBtnApp.h"
#include <QtGui/QPainter>
#include <QtGui/QEvent.h>

CBtnApp::CBtnApp(QWidget *parent /*= 0*/)
//	: QPushButton(parent)
	: QRadioButton(parent)
{
	setStyleSheet("border-image:url(./images/alpha.png);"
				  "text-align:left;");
	setMaximumWidth(250);
}

CBtnApp::~CBtnApp()
{
}

void CBtnApp::mouseDoubleClickEvent(QMouseEvent* event)
{
	emit sig_idclicked(m_wnd);
}

void CBtnApp::enterEvent(QEvent *e)
{
	
}

void CBtnApp::leaveEvent(QEvent *e)
{

}

void CBtnApp::AddWndName(HWND hwnd, QString titleName)
{
	QFont font;
	font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
	font.setPixelSize(13);

	setText(titleName);
	setFont(font);
	m_wnd = hwnd;
}

void CBtnApp::clickWnd()
{
	emit sig_idclicked(m_wnd);
}