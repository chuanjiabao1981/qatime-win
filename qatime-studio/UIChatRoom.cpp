#include "UIChatRoom.h"
#include <QDir>
#include <QTime>
#include <QMovie>
#include "UIMessageBox.h"
UIChatRoom::UIChatRoom(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.button_talk, SIGNAL(clicked()), this, SLOT(clickTalk()));
	connect(ui.button_proclamation, SIGNAL(clicked()), this, SLOT(clickProclamation()));
	connect(ui.button_studentList, SIGNAL(clicked()), this, SLOT(clickStudentList()));
	connect(ui.button_cleanText, SIGNAL(clicked()), this, SLOT(clickCleanText()));
	connect(ui.button_brow, SIGNAL(clicked()), this, SLOT(clickBrow()));
	connect(ui.button_notes, SIGNAL(clicked()), this, SLOT(clickNotes()));
	connect(ui.button_sendMseeage, SIGNAL(clicked()), this, SLOT(clickSendMseeage()));

	initEmotion();
	this->clickTalk();
	m_borw = "";
	m_isBorw = false;
	
}

UIChatRoom::~UIChatRoom()
{

}
// 弹出聊天框
void UIChatRoom::clickTalk()
{
	ui.text_proclamation->setHidden(true);
	ui.text_talk->setHidden(false);
	ui.student_list->setHidden(true);
	ui.text_talk->moveCursor(QTextCursor::End);
}
// 弹出学生列表
void UIChatRoom::clickStudentList()
{
	ui.text_proclamation->setHidden(true);
	ui.text_talk->setHidden(true);
	ui.student_list->setHidden(false);
}
// 弹出讨论框
void UIChatRoom::clickProclamation()
{
	ui.text_proclamation->setHidden(false);
	ui.text_talk->setHidden(true);
	ui.student_list->setHidden(true);
}
// 清屏
void UIChatRoom::clickCleanText()
{
	ui.text_talk->clear();
}
// 表情按钮
void UIChatRoom::clickBrow()
{
	if (m_smallEmotionWidget->isHidden())
	{
		m_smallEmotionWidget->show();
	}
	else
	{
		m_smallEmotionWidget->setHidden(true);
	}
	
	m_smallEmotionWidget->move(60, 490);
}
// 消息记录
void UIChatRoom::clickNotes()
{

}
//加载所有表情
void UIChatRoom::initEmotion()
{
	// 初始化小表情框;
	m_smallEmotionWidget = new MyEmotionWidget(this);
	connect(m_smallEmotionWidget, SIGNAL(emitFileName(QString)), this, SLOT(setBrow(QString)));
	m_smallEmotionWidget->setRowAndColumn(10, 8);
	m_smallEmotionWidget->setEmotionSize(QSize(32, 32));
	m_smallEmotionWidget->setEmotionMovieSize(QSize(24, 24));
	m_smallEmotionWidget->setMaxRow(4);
	m_smallEmotionWidget->initTableWidget();
	QString path = "./images/em_";
	QString path1 = ".gif";
	for (int i = 1; i <= 75; i++)
	{
		m_smallEmotionWidget->addEmotionItem(path + QString::number(i) + path1, "");
		//		m_smallEmotionWidget->addEmotionItem(dir.currentPath()+"./images/em_1.gif", "");
	}
	// 初始化正常表情框;
	m_normalEmotionWidget = new MyEmotionWidget;
	m_normalEmotionWidget->setRowAndColumn(10, 14);
	m_normalEmotionWidget->setEmotionSize(QSize(32, 32));
	m_normalEmotionWidget->setEmotionMovieSize(QSize(24, 24));
	m_normalEmotionWidget->setMaxRow(6);
	m_normalEmotionWidget->initTableWidget();
	m_normalEmotionWidget->setVisible(false);
	for (int i = 1; i <= 75; i++)
	{
		//		m_normalEmotionWidget->addEmotionItem(path.arg(i + 1), "");
	}
	m_smallEmotionWidget->setVisible(false);

	// 	m_lableTitle = new QLabel;
	// 	QVBoxLayout* vLayout = new QVBoxLayout;
	// 	vLayout->addWidget(m_lableTitle);
	// 	vLayout->addWidget(m_smallEmotionWidget);
	// 	vLayout->addWidget(m_normalEmotionWidget);
	// 	this->setLayout(vLayout);

}


void UIChatRoom::stringToHtmlFilter(QString &str)
{
	//注意这几行代码的顺序不能乱，否则会造成多次替换
	str.replace("&", "&amp;");
	str.replace(">", "&gt;");
	str.replace("<", "&lt;");
	str.replace("\"", "&quot;");
	str.replace("\'", "&#39;");
	str.replace(" ", "&nbsp;");
	str.replace("\n", "<br>");
	str.replace("\r", "<br>");
}

void UIChatRoom::stringToHtml(QString &str, QColor crl)
{
	QByteArray array;
	array.append(crl.red());
	array.append(crl.green());
	array.append(crl.blue());
	QString strC(array.toHex());
	str = QString("<span style=\" color:#%1;\">%2</span>").arg(strC).arg(str);
}

void UIChatRoom::imgPathToHtml(QString &path)
{
	path = QString("<img src=\"%1\"/>").arg(path);
}

void UIChatRoom::setBrow(QString path)
{
	m_smallEmotionWidget->hide();
	m_borw = path;
	ui.textEdit->insertHtml("<img src='" + path + "'/>");  //   此处的test 即 url
	ui.textEdit->addAnimation(QUrl(path), path);  //添加一个动画.
	m_isBorw = true;
}

void UIChatRoom::clickSendMseeage()
{
	QString sendText = ui.textEdit->toPlainText();
	if (!sendText.isEmpty())
	{
		QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
		QString timeStr = time.toString("MM-dd hh:mm:ss");
		QString imgPath = "./images/username.png";
		imgPathToHtml(imgPath);
		stringToHtmlFilter(sendText);
		if (m_isBorw != false)
		{
//			imgPathToHtml(m_borw);
			ui.text_talk->append(imgPath + "范德彪：" );
			ui.text_talk->insertHtml("<img src='" + m_borw + "'/>");  //   此处的test 即 url
			ui.text_talk->addAnimation(QUrl(m_borw), m_borw);  //添加一个动画.
			ui.text_talk->insertHtml(sendText);
			m_isBorw = false;
		}
		else
		{
			ui.text_talk->append(imgPath + "范德彪：" + sendText);
		}

		//		stringToHtml(sendText, QColor(255,0,0));

		ui.text_talk->append(timeStr);
		ui.text_talk->acceptRichText();
		ui.text_talk->moveCursor(QTextCursor::End);
		ui.textEdit->clear();
	}
	else
	{
		CMessageBox::showMessage(
			QString("答疑时间"),
			QString("不能发送空数据！"),
			QString("确定"),
			QString());
	}
}
