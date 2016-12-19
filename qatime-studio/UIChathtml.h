#ifndef UICHATHTML_H
#define UICHATHTML_H

#include <QWidget>
#include "ui_UIChathtml.h"
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebChannel/QtWebChannel>
#include <QtWebView/QtWebView>
#include "webobjectinstance.h"
#include "UIViewimage.h"
#include <vector>
#include "UIChatRoom.h"

#pragma execution_character_set("utf-8")

class UIChatRoom;
struct MyStructImg
{
	QString sendPicPath;
	QString	sMsgId;
	int		iProcessPicName;
	int		iFailPicName;
};
class UIChatHtml : public QWidget
{
	Q_OBJECT

public:
	UIChatHtml(QWidget *parent = 0);
	~UIChatHtml();

	QWebEngineView*				m_TalkView;
	QWebChannel					m_WebChannel;
	UIViewImage*				m_ViewImage;		//�鿴ͼƬ
	std::vector<MyStructImg>	m_vecHtmlProcess;	//������
private:
	Ui::UIChatHtml  ui;
	int					m_iCountName;
	UIChatRoom*			m_parent;

protected:
	void mousePressEvent(QMouseEvent *e);       //--��갴���¼� 
	bool eventFilter(QObject *target, QEvent *event);
public:
	void setParent(UIChatRoom* parent);
	void sendMsg(QString sImg, QString sName, QString sTime, QString sContent);
	void sendImageMsg(QString sImg, QString sName, QString sTime, QString sContent, QString sMsgid);
	void receiverMsg(QString sImg, QString sName, QString sTime, QString sContent);
	void receiverImageMsg(QString sImg, QString sName, QString sTime, QString sContent);
	void CenterMsg(QString sMsg);
	void initHtml();
	void ClearHtml();
	void PicProcess(double iProcess);
	void SendStatus(QString msg_id);
public slots:
	void slotJsClick(QVariant var);	// Callback from javascript
};

#endif // UICHATHTML_H
