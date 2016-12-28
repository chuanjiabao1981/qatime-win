#include "UIChathtml.h"
#include <QDir>
#include <QMouseEvent>
#include <QObject>
#include "windows.h"

UIChatHtml::UIChatHtml(QWidget *parent)
	: QWidget(parent)
	, m_TalkView(NULL)
	, m_WebChannel(NULL)
	, m_iCountName(0)
	, m_ViewImage(NULL)
	, m_parent(NULL)
{
	ui.setupUi(this);
	initHtml();
}

UIChatHtml::~UIChatHtml()
{
	if (m_TalkView)
	{
		delete m_TalkView;
		m_TalkView = NULL;
	}

	if (m_ViewImage)
	{
		delete m_ViewImage;
		m_ViewImage = NULL;
	}

	if (m_parent)
		m_parent = NULL;
}

void UIChatHtml::setParent(UIChatRoom* parent)
{
	m_parent = parent;
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
	m_TalkView->page()->setWebChannel(&m_WebChannel);
	m_WebChannel.registerObject("widget", WebObjectInstance::instance());
	connect(WebObjectInstance::instance(), SIGNAL(signClick(QVariant)), this, SLOT(slotJsClick(QVariant)));

	QString strUrl = QDir::currentPath() + "/style.html";
	m_TalkView->setUrl(QUrl(strUrl));
	m_TalkView->show();
	int iHeight = this->parentWidget()->parentWidget()->parentWidget()->height()-100;
	m_TalkView->resize(318, this->parentWidget()->parentWidget()->parentWidget()->height()-100);
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
	if (m_TalkView)
		m_TalkView->page()->runJavaScript(html);
}

void UIChatHtml::mousePressEvent(QMouseEvent *e)
{
	if (Qt::LeftButton == e->button())
	{
	}
}

void UIChatHtml::sendImageMsg(QString sImg, QString sName, QString sTime, QString sImagePath, QString sMsgid)
{
	QString failPath  = "images/retry.png";
	m_iCountName++;
	int iPic = m_iCountName++;
	int iProcess = m_iCountName++;
	int iFailPic = m_iCountName++;
	QString html;
	html.append(QString("document.getElementById(\"content\").insertAdjacentHTML(\"beforeEnd\",\"<div class='new-information'>"
		"<div class='information-title'><img src='%1'><span class='information-name'>%2</span><span class='information-time'>%3</span></div>"
		"<div class='information-img'><img id='%4' onload='initclick(%5)' src='%6'><img id='%7' onload='initfailclick(%7)' src='%8'></div>"
		"<div class='information-progress'><img id='%9' onload='initclick(%9)' src='images/process.png'></div></div>\")").arg(sImg).arg(sName).arg(sTime).arg(QString::number(iPic)).arg(QString::number(iPic)).arg(sImagePath).arg(QString::number(iFailPic)).arg(failPath).arg(QString::number(iProcess)));
	m_TalkView->page()->runJavaScript(html);

	html = "document.getElementsByTagName('BODY')[0].scrollTop=document.getElementsByTagName('BODY')[0].scrollHeight;";
	m_TalkView->page()->runJavaScript(html);

	html = "";
	html.append(QString("document.getElementById(\"%1\").style.width='0px'").arg(QString::number(iProcess)));
	m_TalkView->page()->runJavaScript(html);

	html = "";
	html.append(QString("document.getElementById(\"%1\").style.top='-10px'").arg(QString::number(iProcess)));
	m_TalkView->page()->runJavaScript(html);

	html = "";
	html.append(QString("document.getElementById(\"%1\").style.width='0px'").arg(QString::number(iFailPic)));
	m_TalkView->page()->runJavaScript(html);
	html = "";
	html.append(QString("document.getElementById(\"%1\").style.height='120px'").arg(QString::number(iFailPic)));
	m_TalkView->page()->runJavaScript(html);
	html = "";
	html.append(QString("document.getElementById(\"%1\").style.left='-120px'").arg(QString::number(iFailPic)));
	m_TalkView->page()->runJavaScript(html);

	MyStructImg img;
	img.iFailPicName = iFailPic;
	img.iProcessPicName = iProcess;
	img.sMsgId = sMsgid;
	img.sendPicPath = sImagePath;
	m_vecHtmlProcess.push_back(img);
}

void UIChatHtml::receiverImageMsg(QString sImg, QString sName, QString sTime, QString sContent)
{
	m_iCountName++;
	QString html;
	html.append(QString("document.getElementById(\"content\").insertAdjacentHTML(\"beforeEnd\",\"<div class='new-information'><div class='information-title'><img src='%1'><span class='information-studentname'>%2</span><span class='information-time'>%3</span></div><div class='information-img'><img id='%4' onload='initclick(%5)' src='%6'></div></div>\")").arg(sImg).arg(sName).arg(sTime).arg(QString::number(m_iCountName)).arg(QString::number(m_iCountName)).arg(sContent));

	m_TalkView->page()->runJavaScript(html);

	html = "document.getElementsByTagName('BODY')[0].scrollTop=document.getElementsByTagName('BODY')[0].scrollHeight;";
	m_TalkView->page()->runJavaScript(html);
}

void UIChatHtml::slotJsClick(QVariant var)
{
	QString strSRC = (QString)var.toString();
	int iPos = strSRC.indexOf(":");
	if (iPos < 0)
	{
		if (m_vecHtmlProcess.size() > 0)
		{
			std::vector<MyStructImg>::iterator it;
			for (it = m_vecHtmlProcess.begin(); it != m_vecHtmlProcess.end(); it++)
			{
				MyStructImg img = *it;
				if (img.iFailPicName == strSRC.toInt())
				{
					QString html = "";
					html.append(QString("document.getElementById(\"%1\").style.width='0px'").arg(QString::number(img.iFailPicName)));
					m_TalkView->page()->runJavaScript(html);

					QString msgId;
					if (m_parent)
						m_parent->SendImage(img.sendPicPath.toStdWString(), msgId,img.sMsgId);
					return;
				}

			}
		}
		return;
	}

	if (m_ViewImage == NULL)
	{
		m_ViewImage = new UIViewImage();
		m_ViewImage->setImage(strSRC);
		return;
	}
	
	m_ViewImage->setImage(strSRC);
}

void UIChatHtml::PicProcess(double iProcess)
{
	if (m_vecHtmlProcess.size()>0)
	{
		std::vector<MyStructImg>::iterator it;
		it = m_vecHtmlProcess.begin();
		MyStructImg img = *it;

		int iPicProcess = 0;
		if (iProcess != 1)
			iPicProcess = 120 * iProcess;
		else
			m_vecHtmlProcess.erase(it);

		QString html = "";
		html.append(QString("document.getElementById(\"%1\").style.width='%2px'").arg(QString::number(img.iProcessPicName)).arg(QString::number(iPicProcess)));
		m_TalkView->page()->runJavaScript(html);
	}
}

void UIChatHtml::SendStatus(QString msg_id)
{
	if (m_vecHtmlProcess.size() > 0)
	{
		std::vector<MyStructImg>::iterator it;
		for (it = m_vecHtmlProcess.begin(); it != m_vecHtmlProcess.end(); it++)
		{
			MyStructImg img = *it;
			if (img.sMsgId == msg_id)
			{
				QString html = "";
				html.append(QString("document.getElementById(\"%1\").style.width='120px'").arg(QString::number(img.iFailPicName)));
				m_TalkView->page()->runJavaScript(html);

//				m_vecHtmlProcess.erase(it);
				return;
			}

		}
	}
}