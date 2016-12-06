#include "UIChathtml.h"

UIChatHtml::UIChatHtml(QWidget *parent)
	: QWidget(parent)
	, m_TalkView(NULL)
{
	ui.setupUi(this);

	m_TalkView = new QWebEngineView(ui.talk_widget);
	QString strUrl = QDir::currentPath() + "/style.html";
	m_TalkView->load(QUrl(strUrl));
	m_TalkView->show();
	m_TalkView->resize(384,245);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(sendmsg()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(sendmsg1()));
}

UIChatHtml::~UIChatHtml()
{

}

void UIChatHtml::sendmsg()
{
	QString html = QString("");
	QString msg="狐狸精";
	QString msg1 = "09-12 17:32:21";
	QString msg2 = "今天星期几？算阿斯顿阿斯顿按时阿斯顿算阿斯顿阿斯顿按时阿斯顿算阿斯顿阿斯顿按时阿斯顿算阿斯顿阿斯顿按时阿斯顿";
	
	msg.replace("[smail.png]", "<img src=qrc:/smail.png align=center border=0>");
	msg.replace("[ali.gif]", "<img src=qrc:/ali.gif align=center border=0>");
	
	html.append(QString("document.getElementById(\"content\").insertAdjacentHTML(\"beforeEnd\",\"<div class='new-information'><div class='information-title'><img src='http://192.168.1.115:8020/html/img/person-center.png'><span class='information-name'>%1</span><span class='information-time'>%2</span></div><div class='information-con'>%3</div></div>\")").arg(msg).arg(msg1).arg(msg2));

	m_TalkView->page()->runJavaScript(html);

	html = "document.getElementsByTagName('BODY')[0].scrollTop=document.getElementsByTagName('BODY')[0].scrollHeight;";
	m_TalkView->page()->runJavaScript(html);
}

void UIChatHtml::sendmsg1()
{
	QString html = QString("");
	QString msg = "狐狸精";
	QString msg1 = "09-12";
	QString msg2 = "今天星期几";

	html.append(QString("document.getElementById(\"content\").insertAdjacentHTML(\"beforeEnd\",\"<div class='logging-nomore'>%1</div>\")").arg(msg1));
	m_TalkView->page()->runJavaScript(html);
}
