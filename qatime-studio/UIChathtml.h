#ifndef UICHATHTML_H
#define UICHATHTML_H

#include <QWidget>
#include "ui_UIChathtml.h"
#include <QtWebEngineWidgets>

#pragma execution_character_set("utf-8")

class UIChatHtml : public QWidget
{
	Q_OBJECT

public:
	UIChatHtml(QWidget *parent = 0);
	~UIChatHtml();

private:
	Ui::UIChatHtml  ui;
	QWebEngineView*		m_TalkView;

private slots :
	void sendmsg();		
	void sendmsg1();
};

#endif // UICHATHTML_H
