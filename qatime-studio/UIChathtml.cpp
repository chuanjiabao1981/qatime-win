#include "UIChathtml.h"
#include <QDir>
#include <QMouseEvent>

UIChatHtml::UIChatHtml(QWidget *parent)
	: QWidget(parent)
	, m_TalkView(NULL)
{
	ui.setupUi(this);

	initHtml();
}

UIChatHtml::~UIChatHtml()
{

}

void UIChatHtml::sendMsg(QString sImg,QString sName,QString sTime,QString sContent)
{
	QString html;
	for (int i = 1; i < 75; i++)
	{
		QString name = "";
		name.append(QString("[em_%1]").arg(QString::number(i)));
		QString path = "";
		path.append(QString("<img src='./images/em_%1.gif' align=center border=0>").arg(QString::number(i)));
		sContent.replace(name, path);
	}
	
	html.append(QString("document.getElementById(\"content\").insertAdjacentHTML(\"beforeEnd\",\"<div class='new-information'><div class='information-title'><img src='%1'><span class='information-name'>%2</span><span class='information-time'>%3</span></div><div class='information-con'>%4</div></div>\")").arg(sImg).arg(sName).arg(sTime).arg(sContent));

	m_TalkView->page()->runJavaScript(html);

	html = "document.getElementsByTagName('BODY')[0].scrollTop=document.getElementsByTagName('BODY')[0].scrollHeight;";
	m_TalkView->page()->runJavaScript(html);
}

void UIChatHtml::receiverMsg(QString sImg, QString sName, QString sTime, QString sContent)
{
	QString html;

	for (int i = 1; i < 75;i++)
	{
		QString name = "";
		name.append(QString("[em_%1]").arg(QString::number(i)));
		QString path = "";
		path.append(QString("<img src='./images/em_%1.gif' align=center border=0>").arg(QString::number(i)));
		sContent.replace(name, path);
	}

	html.append(QString("document.getElementById(\"content\").insertAdjacentHTML(\"beforeEnd\",\"<div class='new-information'><div class='information-title'><img src='%1'><span class='information-studentname'>%2</span><span class='information-time'>%3</span></div><div class='information-con'>%4</div></div>\")").arg(sImg).arg(sName).arg(sTime).arg(sContent));

	m_TalkView->page()->runJavaScript(html);

	html = "document.getElementsByTagName('BODY')[0].scrollTop=document.getElementsByTagName('BODY')[0].scrollHeight;";
	m_TalkView->page()->runJavaScript(html);
}

void UIChatHtml::CenterMsg(QString sMsg)
{
	QString html;
	html.append(QString("document.getElementById(\"content\").insertAdjacentHTML(\"beforeEnd\",\"<div class='logging-nomore'>%1</div>\")").arg(sMsg));
	m_TalkView->page()->runJavaScript(html);

	html = "document.getElementsByTagName('BODY')[0].scrollTop=document.getElementsByTagName('BODY')[0].scrollHeight;";
	m_TalkView->page()->runJavaScript(html);
}

void UIChatHtml::initHtml()
{
	m_TalkView = new QWebEngineView(ui.talk_widget);
	QString strUrl = QDir::currentPath() + "/style.html";
	m_TalkView->setUrl(QUrl(strUrl));
	m_TalkView->show();
	m_TalkView->resize(318, 283);

//	m_TalkView->installEventFilter(this);
}
// 拖动标题做的处理
bool UIChatHtml::eventFilter(QObject *target, QEvent *event)
{
	if (target == m_TalkView)
	{
		QMouseEvent* pMe = static_cast<QMouseEvent*>(event);
		if (event->type() == QEvent::MouseButtonPress)
		{
		}
		return false;
	}
	return false;
}

void UIChatHtml::ClearHtml()
{
	QString html = "document.body.innerHTML=''";
	m_TalkView->page()->runJavaScript(html);
}

void UIChatHtml::mousePressEvent(QMouseEvent *e)
{
	if (Qt::LeftButton == e->button())
	{
	}
}