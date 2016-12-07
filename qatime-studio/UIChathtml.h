#ifndef UICHATHTML_H
#define UICHATHTML_H

#include <QWidget>
#include "ui_UIChathtml.h"
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebView/QtWebView>

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

protected:
	void mousePressEvent(QMouseEvent *e);       //--鼠标按下事件 
	bool eventFilter(QObject *target, QEvent *event);
public:
	void sendMsg(QString sImg, QString sName, QString sTime, QString sContent);
	void receiverMsg(QString sImg, QString sName, QString sTime, QString sContent);
	void CenterMsg(QString sMsg);
	void initHtml();
	void ClearHtml();
};

#endif // UICHATHTML_H
