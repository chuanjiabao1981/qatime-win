#include "UIChatRoom.h"
#include <QDir>
#include <QTime>
#include <QMovie>
#include "UIMessageBox.h"
#include <QScrollBar>
#include <QClipboard>
#include <QJsonArray>
#include "member.h"
#include <QScrollBar>
#include <QProcessEnvironment>
#include "zoom_image.h"

#include "YxChat/nim_sdk_helper.h"
#include "YxChat/session_callback.h"
#include "YxChat/nim_cpp_client.h"
#include "define.h"
#include <QMouseEvent>
#include <QToolTip>
#include <QNetworkRequest>

#ifdef TEST
	#define _DEBUG
#else
#endif
typedef bool(*nim_client_init)(const char *app_data_dir, const char *app_install_dir, const char *json_extension);
typedef void(*nim_client_cleanup)(const char *json_extension);
typedef void(*nim_client_login)(const char *app_token, const char *account, const char *password, const char *json_extension, nim_json_transport_cb_func cb, const void* user_data);
typedef void(*nim_client_relogin)(const char *json_extension);
typedef void(*nim_client_logout)(NIMLogoutType logout_type, const char *json_extension, nim_json_transport_cb_func cb, const void* user_data);
typedef void(*nim_client_kick_other_client)(const char *json_extension);
typedef void(*nim_client_reg_auto_relogin_cb)(const char *json_extension, nim_json_transport_cb_func cb, const void* user_data);
typedef void(*nim_client_reg_kickout_cb)(const char *json_extension, nim_json_transport_cb_func cb, const void* user_data);
typedef void(*nim_client_reg_disconnect_cb)(const char *json_extension, nim_json_transport_cb_func cb, const void* user_data);
typedef void(*nim_client_reg_multispot_login_notify_cb)(const char *json_extension, nim_json_transport_cb_func cb, const void *user_data);
typedef void(*nim_client_reg_kickout_other_client_cb)(const char *json_extension, nim_json_transport_cb_func cb, const void *user_data);
typedef void(*nim_client_reg_sync_multiport_push_config_cb)(const char *json_extension, nim_client_multiport_push_config_cb_func cb, const void *user_data);
typedef void(*nim_client_set_multiport_push_config)(const char *switch_content, const char *json_extension, nim_client_multiport_push_config_cb_func cb, const void *user_data);
typedef void(*nim_client_get_multiport_push_config)(const char *json_extension, nim_client_multiport_push_config_cb_func cb, const void *user_data);


QColor timeColor(153, 153, 153);
QColor contentColor(102, 102, 102);
QColor nameColor(85, 170, 255);
QColor selfColor(190,11,11);

UIChatRoom::UIChatRoom(QWidget *parent)
	: QWidget(parent)
	, m_CurChatID("")
	, m_bLogin(false)
	, m_farst_msg_time(0)
	, kMsgLogNumberShow(20)
	, m_switchTime(false)
	, m_RecordTime(QDateTime::currentDateTime())
	, m_parent(NULL)
	, m_ChatHtml(NULL)
	, m_studentNum(0)
	, m_proclamationHeight(0)
	, m_LoadImgTimer(NULL)
{
	ui.setupUi(this);
	setAutoFillBackground(true);
	QPalette p = palette();
	p.setColor(QPalette::Window, QColor("white"));
	setPalette(p);

	connect(ui.button_talk, SIGNAL(clicked()), this, SLOT(clickTalk()));
	connect(ui.button_proclamation, SIGNAL(clicked()), this, SLOT(clickProclamation()));
	connect(ui.button_studentList, SIGNAL(clicked()), this, SLOT(clickStudentList()));
	connect(ui.button_cleanText, SIGNAL(clicked()), this, SLOT(clickCleanText()));
	connect(ui.button_brow, SIGNAL(clicked()), this, SLOT(clickBrow()));
	connect(ui.button_notes, SIGNAL(clicked()), this, SLOT(clickNotes()));	
	connect(ui.toolButton_3, SIGNAL(clicked()), this, SLOT(closeNotes()));
	connect(ui.timeShow, SIGNAL(Clicked()), this, SLOT(chickChoseTime()));
	connect(ui.button_sendMseeage, SIGNAL(clicked()), this, SLOT(clickSendMseeage()));
	connect(ui.timeWidget, SIGNAL(clicked(QDate)), this, SLOT(choseTime(QDate)));
	connect(ui.toolButton_2, SIGNAL(clicked()), this, SLOT(forwardTime()));
	connect(ui.toolButton_1, SIGNAL(clicked()), this, SLOT(afterTime()));
	connect(ui.button_sendMseeage_2, SIGNAL(clicked()), this, SLOT(announce()));
	connect(ui.button_sendMseeage_3, SIGNAL(clicked()), this, SLOT(putTalk()));			
	connect(ui.button_sendMseeage_cancel, SIGNAL(clicked()), this, SLOT(putTalkCancel()));
	connect(ui.student_list, SIGNAL(signalChickChage(int, QString, QString)), this, SLOT(chickChage(int, QString, QString)));	
	connect(ui.textEdit_2, SIGNAL(textChanged()), this, SLOT(proclamationTextChage()));
	connect(ui.talkRecord, SIGNAL(colseCalendar()), this, SLOT(colseCalendar()));
	connect(ui.text_talk, SIGNAL(colseBrow()), this, SLOT(colseBrow()));
	connect(ui.textEdit, SIGNAL(colseBrow()), this, SLOT(colseBrow()));
	connect(ui.timeWidget, SIGNAL(currentPageChanged(int, int)), this, SLOT(calendaCurrentPageChanged(int, int)));
	connect(ui.pic_pushButton, SIGNAL(clicked()), this, SLOT(clickPic()));

	QScrollBar* TalkRecordScrollBar;
	TalkRecordScrollBar = (QScrollBar*)ui.talkRecord->verticalScrollBar();
	if (TalkRecordScrollBar)
		connect((QObject*)TalkRecordScrollBar, SIGNAL(valueChanged(int)), this, SLOT(RecordMoved(int)));

	initEmotion();
	this->clickTalk();
	m_isBorw = false;

	initSDK();
	
	QDate cdate = QDate::currentDate();   //获取今天的日期
	QTextCharFormat format;
	format.setBackground(Qt::green);  //设置格式，颜色自选
	ui.timeWidget->setDateTextFormat(cdate, format);//设置当前日期始终高亮！
	ui.timeWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
	ui.timeWidget->setGridVisible(true);	

	// 消息编辑框粘贴过滤代码
	ui.textEdit->setContextMenuPolicy(Qt::NoContextMenu);
	ui.textEdit->installEventFilter(this);
	ui.textEdit->setStyleSheet("border: 1px solid #57cff8;");

	style(ui.text_proclamation);
	style(ui.talkRecord);
	style(ui.text_talk);

	ui.student_list->setStyleSheet("border-image: url(:/LoginWindow/images/nochat.png);");

 	m_ChatHtml = new UIChatHtml(ui.text_talk);
	m_ChatHtml->setParent(this);
 	m_ChatHtml->show();

	m_LoadImgTimer = new QTimer(this);
	connect(m_LoadImgTimer, SIGNAL(timeout()), this, SLOT(LoadImgTimeout()));
}

UIChatRoom::~UIChatRoom()
{
	m_StudentInfo.clear();
	if (m_ChatHtml)
	{
		delete m_ChatHtml;
		m_ChatHtml = NULL;
	}

	if (m_LoadImgTimer)
	{
		m_LoadImgTimer->stop();
		delete m_LoadImgTimer;
		m_LoadImgTimer = NULL;
	}
}

// 消息编辑框粘贴过滤代码
bool UIChatRoom::eventFilter(QObject *target, QEvent *event)
{
	if (target == ui.textEdit) {
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			if (keyEvent->matches(QKeySequence::Paste))
			{
				qDebug() << "Ctrl + V";
				QClipboard *board = QApplication::clipboard();
				QString strClip = board->text();
				strClip.replace("￼", "");
				ui.textEdit->insertPlainText(strClip);
				return true;
			}
			else if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			{
				emit ui.button_sendMseeage->clicked();
				return true;
			}
		}
		if (event->type() == QEvent::MouseButtonRelease) {
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			if (mouseEvent->button() == Qt::MidButton) {
				qDebug() << "Mouse MidButton Release";
				QClipboard *board = QApplication::clipboard();
				QString strClip = board->text();
				strClip.replace("￼", "");
				ui.textEdit->insertPlainText(strClip);
				return true;
			}
		}
		if (event->type() == QEvent::FocusOut)
		{
			// 失去焦点（鼠标不在窗体内，则隐藏窗体）
		}
	}
	return QWidget::eventFilter(target, event);
}

void UIChatRoom::mousePressEvent(QMouseEvent *event)
{
	if ((event->button() == Qt::LeftButton))
	{		
		m_smallEmotionWidget->setHidden(true);					
	}
}

// 弹出聊天框
void UIChatRoom::clickTalk()
{	
	QPalette   pal;
	pal.setColor(QPalette::ButtonText, QColor(153, 153, 153));
	ui.button_studentList->setPalette(pal);
	ui.button_proclamation->setPalette(pal);
	ui.button_talk->setStyleSheet("border-image: url(:/LoginWindow/images/chatBtn.png);color:#58cff9;");
 	ui.button_studentList->setStyleSheet("border-image: url(:/LoginWindow/images/AuxiliaryPanelBack.png);");
 	ui.button_proclamation->setStyleSheet("border-image: url(:/LoginWindow/images/AuxiliaryPanelBack.png);");

	ui.button_brow->setHidden(false);
	ui.button_cleanText->setHidden(false);
	ui.button_notes->setHidden(false);
	ui.button_sendMseeage->setHidden(false);
	ui.textEdit->setHidden(false);
	ui.proclamationWidget->setHidden(true);
	ui.text_talk->setHidden(false);
	ui.label->setHidden(false);
	ui.student_list->setHidden(true);
	ui.msgRecord->setHidden(true);	
	ui.text_talk->moveCursor(QTextCursor::End);
	ui.pic_pushButton->setHidden(false);
}
// 弹出学生列表
void UIChatRoom::clickStudentList()
{
	QPalette   pal;
	pal.setColor(QPalette::ButtonText, QColor(153, 153, 153));
	ui.button_talk->setPalette(pal);
	ui.button_proclamation->setPalette(pal);
	ui.button_studentList->setStyleSheet("border-image: url(:/LoginWindow/images/chatBtn.png);color:#58cff9;");
	ui.button_talk->setStyleSheet("border-image: url(:/LoginWindow/images/AuxiliaryPanelBack.png);color:rgb(153,153,153)");
	ui.button_proclamation->setStyleSheet("border-image: url(:/LoginWindow/images/AuxiliaryPanelBack.png);");

	ui.button_brow->setHidden(true);
	ui.button_cleanText->setHidden(true);
	ui.button_notes->setHidden(true);
	ui.button_sendMseeage->setHidden(true);
	ui.textEdit->setHidden(true);
	ui.proclamationWidget->setHidden(true);
	ui.text_talk->setHidden(true);
	ui.student_list->setHidden(false);
	ui.msgRecord->setHidden(true);
	ui.student_list->initMenu();
	ui.pic_pushButton->setHidden(true);
}
// 弹出公告框
void UIChatRoom::clickProclamation()
{
	QPalette   pal;
	pal.setColor(QPalette::ButtonText, QColor(153, 153, 153));
	ui.button_talk->setPalette(pal);
	ui.button_studentList->setPalette(pal);
	ui.button_proclamation->setStyleSheet("border-image: url(:/LoginWindow/images/chatBtn.png);color:#58cff9;");
	ui.button_talk->setStyleSheet("border-image: url(:/LoginWindow/images/AuxiliaryPanelBack.png);color:rgb(153,153,153)");
	ui.button_studentList->setStyleSheet("border-image: url(:/LoginWindow/images/AuxiliaryPanelBack.png);");

 	ui.button_sendMseeage_cancel->hide();
 	ui.proclamationWidget->setHidden(false);
 	ui.button_sendMseeage_3->hide();
 	ui.textEdit_2->hide();
	ui.text_talk->setHidden(true);
	ui.student_list->setHidden(true);
	ui.msgRecord->setHidden(true);	
	ui.button_brow->setHidden(true);
	ui.button_cleanText->setHidden(true);
	ui.button_notes->setHidden(true);
	ui.button_sendMseeage->setHidden(true);
	ui.textEdit->setHidden(true);
	ui.label->setHidden(true);
	ui.pic_pushButton->setHidden(true);
 	
	ui.text_proclamation->show();
	ui.text_proclamation->setGeometry(QRect(0, 40, 280, m_proclamationHeight));

	if (!m_CurCourseID.isEmpty())
		ui.button_sendMseeage_2->show();
	else
		ui.button_sendMseeage_2->hide();
}
// 清屏
void UIChatRoom::clickCleanText()
{
//	ui.text_talk->clear();
	m_ChatHtml->ClearHtml();
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

	m_smallEmotionWidget->move(0, this->size().height()-210);
}
// 消息记录
void UIChatRoom::clickNotes()
{
	if (strcmp(m_CurChatID.c_str(), "") == 0)
	{
		QToolTip::showText(QCursor::pos(), "请选择直播间！");
		//CMessageBox::showMessage(QString("答疑时间"),QString("请选择直播间！"),QString("确定"),QString());
		return;
	}
	// 消息记录日期
	
	QDateTime date = QDateTime::currentDateTime();
	QString dtstr = date.toString("yyyy-MM-dd");
	QDate cdate = QDate::currentDate();
	ui.timeWidget->setSelectedDate(cdate);
	ui.timeShow->setText(dtstr);

	ui.proclamationWidget->setHidden(true);
	ui.text_talk->setHidden(true);	
	ui.student_list->setHidden(true);
	ui.button_brow->setHidden(true);
	ui.button_cleanText->setHidden(true);
	ui.button_notes->setHidden(true);
	ui.button_sendMseeage->setHidden(true);
	ui.textEdit->setHidden(true);
	ui.label->setHidden(true);
	ui.msgRecord->setHidden(false);	
	ui.timeWidget->hide();
	ui.talkRecord->clear();		// 清除消息记录

	m_farst_msg_time = 0;
	nim::MsgLog::QueryMsgOnlineAsync(m_CurChatID, nim::kNIMSessionTypeTeam, kMsgLogNumberShow, 0, m_farst_msg_time, 0, false, true);
}

void UIChatRoom::closeNotes()
{
	clickTalk();	
	ui.button_brow->setHidden(false);
	ui.button_cleanText->setHidden(false);
	ui.button_notes->setHidden(false);
	ui.button_sendMseeage->setHidden(false);
	ui.textEdit->setHidden(false);
	ui.msgRecord->setHidden(true);
}

void UIChatRoom::QueryMsgOnlineCb(nim::NIMResCode code, const std::string& id, nim::NIMSessionType type, const nim::QueryMsglogResult& result)
{
}

//加载所有表情
void UIChatRoom::initEmotion()
{
	// 初始化小表情框;
	m_smallEmotionWidget = new MyEmotionWidget(this);
	connect(m_smallEmotionWidget, SIGNAL(emitFileName(QString)), this, SLOT(setBrow(QString)));
	m_smallEmotionWidget->setRowAndColumn(10, 9);
	m_smallEmotionWidget->setEmotionSize(QSize(33, 33));
	m_smallEmotionWidget->setEmotionMovieSize(QSize(24, 24));
	m_smallEmotionWidget->setMaxRow(4);
	m_smallEmotionWidget->initTableWidget();
	QString path = "./images/em_";
	QString path1 = ".gif";
	for (int i = 1; i <= 75; i++)
	{
		m_smallEmotionWidget->addEmotionItem(path + QString::number(i) + path1, "");
	}
	m_smallEmotionWidget->setVisible(false);
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

void UIChatRoom::stringToHtmlPos(QString &str, QColor crl)
{
	QByteArray array;
	array.append(crl.red());
	array.append(crl.green());
	array.append(crl.blue());
	QString strC(array.toHex());
//	str = QString("<span style=\" color:#%1; padding-left:70px;\">%2</span>").arg(strC).arg(str); 
	str = QString("<span style=\" color:#%1; line - height:50px; height:50px;\">%2</span>").arg(strC).arg(str);
}

void UIChatRoom::imgPathToHtml(QString &path)
{
	path = QString("<img src=\"%1\"/>").arg(path);
}

void UIChatRoom::setBrow(QString path)
{
	m_smallEmotionWidget->hide();
	m_borw.append(path);
	ui.textEdit->insertHtml("<img src='" + path + "'/>");  //   此处的test 即 url
	ui.textEdit->addAnimation(QUrl(path), path);  //添加一个动画.
	m_isBorw = true;
}

// 发送消息按钮
void UIChatRoom::clickSendMseeage()
{
	if (strcmp(m_CurChatID.c_str(),"") == 0)
	{
		QToolTip::showText(QCursor::pos(), "请选择直播间！");
		return;
	}

	QString tempText = ui.textEdit->toPlainText();
	QStringList textList = tempText.split("￼");
	QString sendText, sendMsg;
	for (int i = 0; i < textList.size(); i++)
	{
		if (i == textList.size() - 1)
		{
			sendText.append(textList.at(i));
			sendMsg.append(textList.at(i));
			break;
		}
		sendText.append(textList.at(i));
		sendMsg.append(textList.at(i));
		sendMsg.append("[" + m_borw.at(i).split("/").at(2).split(".").at(0) + "]");
	}
	if (textList.size() >= 2 || !sendText.isEmpty())
	{
		QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间

		// 跨天处理
		stepMsgDays(time);

		QString contect;
		QString timeStr = time.toString("hh:mm:ss");
		if (m_isBorw != false)
		{
			QString name = m_TeachterName;
			name += "(我) ";
			
			// 本人头像
			QString image = m_parent->TeacherPhotoPixmap();
			for (int i = 0; i < textList.size(); i++)
			{
				QString string = textList.at(i);
				contect += string;
				if (i <= textList.size() - 2)
				{
					QString borw;
					borw.append("[" + m_borw.at(i).split("/").at(2).split(".").at(0) + "]");
					contect += borw;
				}						
			}
			m_isBorw = false;
			m_ChatHtml->sendMsg(image, name, timeStr, contect);
		}
		else
		{
			QString qName = m_TeachterName;
			qName += "(我) ";

			// 本人头像
 			QString image = m_parent->TeacherPhotoPixmap();
			m_ChatHtml->sendMsg(image,qName, timeStr, sendText);
		}
		ui.textEdit->clear();
	}
	else
	{
		QToolTip::showText(QCursor::pos(), "不能发送空数据！");
		return;
	}

	// 发往服务器
	nim::IMMessage msg;
	PackageMsg(msg);
	msg.type_ = nim::kNIMMessageTypeText;
	msg.content_ = sendMsg.toStdString();

	nim::MessageSetting setting;
	std::string json_msg = nim::Talk::CreateTextMessage(msg.receiver_accid_, msg.session_type_, msg.client_msg_id_, msg.content_, setting, msg.timetag_);
	nim::Talk::SendMsg(json_msg);
	m_borw.clear();

	
	ui.textEdit->setFocus();
}

void UIChatRoom::chickChoseTime()
{
	if (ui.timeWidget->isHidden())
	{
		ui.timeWidget->raise();
		ui.timeWidget->show();	
		pPreMonthButton1 = ui.timeWidget->findChild<QToolButton*>(QLatin1String("qt_calendar_yearbutton"));
		pPreMonthButton1->move(100, 0);
		pPreMonthButton = ui.timeWidget->findChild<QToolButton*>(QLatin1String("qt_calendar_monthbutton"));
		pPreMonthButton->move(150, 0);
	}
	else
	{
		ui.timeWidget->setHidden(true);
	}
	
}

// 选择时间
void UIChatRoom::choseTime(QDate date)
{	
	QDate dateNow= QDate::currentDate();
	if (dateNow<date)
	{
		//TODO提示，消息记录只能查看之前的消息
		QToolTip::showText(QCursor::pos(),"没有今天之后的消息记录！");
	}
	else
	{
		QString dtstr = date.toString("yyyy-MM-dd");
		ui.timeShow->setText(dtstr);
		ui.timeWidget->hide();

		QueryRecord(dtstr);
	}
	
}

void UIChatRoom::forwardTime()
{
	QDate date = ui.timeWidget->selectedDate().addDays(1);	
	ui.timeWidget->setSelectedDate(date);
	QString dtstr = date.toString("yyyy-MM-dd");
	ui.timeShow->setText(dtstr);

	QueryRecord(dtstr);
}

void UIChatRoom::afterTime()
{
	QDate date = ui.timeWidget->selectedDate().addDays(-1);
	ui.timeWidget->setSelectedDate(date);
	QString dtstr = date.toString("yyyy-MM-dd");
	ui.timeShow->setText(dtstr);

	QueryRecord(dtstr);
}

void UIChatRoom::QueryRecord(QString dtstr)
{
	// 获取时间
	m_switchTime = true;
	ui.talkRecord->clear();
	QDateTime time;
	dtstr += " 00:00:01";
	time = QDateTime::fromString(dtstr, "yyyy-MM-dd hh:mm:ss");

	// 和当前时间做比较,如选择的时间比当前时间晚，则直接返回
	QDateTime curTime = QDateTime::currentDateTime();
	if (curTime < time)
		return;

	dtstr.replace("00:00:01", "23:59:59");
	time = QDateTime::fromString(dtstr, "yyyy-MM-dd hh:mm:ss");
	m_farst_msg_time = time.toMSecsSinceEpoch();
	nim::MsgLog::QueryMsgOnlineAsync(m_CurChatID, nim::kNIMSessionTypeTeam, kMsgLogNumberShow, 0, m_farst_msg_time, 0, false, true);
}

// 点击【发布公告】按钮
void UIChatRoom::announce()
{
	ui.text_proclamation->setGeometry(QRect(0, 40 + 80, 299, m_proclamationHeight - 80));
	ui.button_sendMseeage_cancel->show();
	ui.button_sendMseeage_3->show();	
	ui.textEdit_2->show();
	ui.button_sendMseeage_2->hide();
	ui.text_proclamation->show();
}
// 点击【发布】按钮
void UIChatRoom::putTalk()
{
	ui.text_proclamation->setGeometry(QRect(0, 40, 299, m_proclamationHeight));
	ui.button_sendMseeage_cancel->hide();
	ui.button_sendMseeage_3->hide();
	ui.textEdit_2->hide();
	ui.button_sendMseeage_2->show();
	ui.text_proclamation->show();

	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
	stringToHtml(current_date, timeColor);

	QString announcement = ui.textEdit_2->toPlainText();//增加新公告
	OnSendAnnouncements(announcement);

	stringToHtml(announcement, contentColor);

	QTextCursor textCursor = ui.text_proclamation->textCursor();
	textCursor.movePosition(QTextCursor::Start);
	textCursor.insertImage("./images/announcement.png");
	textCursor.insertText("  ");
	textCursor.insertHtml(current_date);
	textCursor.insertText("\n");
	textCursor.insertHtml(announcement);
	textCursor.insertText("\n\r");

	ui.textEdit_2->clear();
		
}

// 点击【取消】按钮
void UIChatRoom::putTalkCancel()
{
	ui.text_proclamation->setGeometry(QRect(0, 40, 299, m_proclamationHeight));
	ui.button_sendMseeage_cancel->hide();
	ui.button_sendMseeage_3->hide();
	ui.textEdit_2->hide();
	ui.button_sendMseeage_2->show();
	ui.text_proclamation->show();

	ui.textEdit_2->clear();
}
// 发布编辑框输入文字设置发布按钮是否可以被点击。
void UIChatRoom::proclamationTextChage()
{
	if (ui.textEdit_2->toPlainText().isEmpty())
	{
		ui.button_sendMseeage_3->setEnabled(false);
	}
	else{
		ui.button_sendMseeage_3->setEnabled(true);
	}
}

// 关闭日历槽函数
void UIChatRoom::colseCalendar()
{
	ui.timeWidget->hide();
}
// 日历当前页面被改变设置年月的顺序
void UIChatRoom::calendaCurrentPageChanged(int year, int month)
{
	pPreMonthButton1 = ui.timeWidget->findChild<QToolButton*>(QLatin1String("qt_calendar_yearbutton"));	
	pPreMonthButton1->move(50, 0);
	pPreMonthButton = ui.timeWidget->findChild<QToolButton*>(QLatin1String("qt_calendar_monthbutton"));
	pPreMonthButton->move(150, 0);

}

void UIChatRoom::PackageMsg(nim::IMMessage &msg)
{
	msg.session_type_ = nim::kNIMSessionTypeTeam;
	msg.receiver_accid_ = m_CurChatID;//会话ID
	msg.sender_accid_ = m_accid.toStdString();//用户名
	msg.client_msg_id_ = nim::Client::GetUuid();
	msg.msg_setting_.resend_flag_ = nim::BS_FALSE;

	//base获取的时间单位是s，服务器的时间单位是ms
	msg.timetag_ = QDateTime::currentMSecsSinceEpoch();

	msg.status_ = nim::kNIMMsgLogStatusSending;
}


// 接收消息
bool UIChatRoom::ReceiverMsg(nim::IMMessage* pMsg)
{
	bool bValid = false;
	if (pMsg->type_ == nim::kNIMMessageTypeNotification) // 过滤系统消息
	{
		Json::Value json;
		Json::Reader reader;
		if (reader.parse(pMsg->attach_, json))
		{
			nim::NIMNotificationId id = (nim::NIMNotificationId)json[nim::kNIMNotificationKeyId].asInt();

			// 加入消息
			if (id == nim::kNIMNotificationIdTeamInvite)
			{
				QString strName;
				Json::Value array = json[nim::kNIMNotificationKeyData][nim::kNIMNotificationKeyDataIds];
				if (!array.empty() && array.isArray())
				{
					int len = array.size();
					std::vector<std::string> ids;
					for (int i = 0; i < len; i++)
					{
						ids.push_back(array[i].asString());
					}
					if (!ids.empty())
					{
						int n = ids.size();
						int i = 0;
						for (; i < n && i < 3; i++)
						{
							m_AddMemberID = ids[i];
							QueryMember();
							bValid = true;
							return bValid;
						}
					}
				}
			}
		}
		return bValid;
	}

	// 判断当前过来的消息，是不是此会话窗口
	if (strcmp(pMsg->local_talk_id_.c_str(), m_CurChatID.c_str()) == 0 && pMsg->type_ == nim::kNIMMessageTypeText)
	{
		stepMsgDays(QDateTime::fromMSecsSinceEpoch(pMsg->timetag_));

		std::string strName = pMsg->readonly_sender_nickname_;
		std::string strContent = pMsg->content_;
		std::string strID = pMsg->sender_accid_;

		QString qTime = QDateTime::fromMSecsSinceEpoch(pMsg->timetag_).toString("hh:mm:ss");// 原型yyyy-MM-dd hh:mm:ss
		QString qName = QString::fromStdString(strName);
		QString qContent = QString::fromStdString(strContent);		

		// 聊天头像
		QString img;
		personListBuddy* Buddy = NULL;
		Buddy = ui.student_list->findID(QString::fromStdString(strID));
		if (Buddy)
			img= Buddy->head->accessibleDescription();

		m_ChatHtml->receiverMsg(img,qName, qTime, qContent);
		bValid = true;
	}
	else if (strcmp(pMsg->local_talk_id_.c_str(), m_CurChatID.c_str()) == 0 && pMsg->type_ == nim::kNIMMessageTypeImage)
	{
		stepMsgDays(QDateTime::fromMSecsSinceEpoch(pMsg->timetag_));

		std::string strName = pMsg->readonly_sender_nickname_;
		std::string strContent = pMsg->content_;
		std::string strID = pMsg->sender_accid_;

		QString qTime = QDateTime::fromMSecsSinceEpoch(pMsg->timetag_).toString("hh:mm:ss");// 原型yyyy-MM-dd hh:mm:ss
		QString qName = QString::fromStdString(strName);
		QString qContent = QString::fromStdString(pMsg->local_res_path_);
		QString qContentNew = qContent;
		qContentNew += ".png";
		qContentNew.replace("\\", "/");
		// 聊天头像
		QString img;
		personListBuddy* Buddy = NULL;
		Buddy = ui.student_list->findID(QString::fromStdString(strID));
		if (Buddy)
			img = Buddy->head->accessibleDescription();

		// 如果下载失败
		if (!QFile::copy(qContent, qContentNew))
		{
			MyImageInfo ImgInfo;
			ImgInfo.PhotoImg = img;
			ImgInfo.name = qName;
			ImgInfo.time = qTime;
			ImgInfo.ReceiverImg = qContent;
			ImgInfo.chatID = pMsg->local_talk_id_;
			m_VerReceiveImg.push_back(ImgInfo);
			m_LoadImgTimer->start(500);
			return false;
		}

		m_ChatHtml->receiverImageMsg(img, qName, qTime, qContentNew);
		bValid = true;
	}

	return bValid;
}

bool UIChatRoom::IsHasFace(QString qContect)
{
	int iStartPos = qContect.indexOf("[");
	int iEndPos = qContect.indexOf("]");
	if (iStartPos != -1 && iEndPos != -1) // 找到[]
	{
		QString qFace = "[em_number]";
		for (int iCount = 1; iCount < 76; iCount++)		//详细对比
		{
			qFace = qFace.replace("number", QString::number(iCount));
			if (qContect.indexOf(qFace) != -1)
				return true;
			else
				qFace = "[em_number]";
		}
	}

	return false;
}

void UIChatRoom::ParseFace(QString qContect)
{
	ui.text_talk->append("");
	QString qFace="";			// 表情
	bool    bHas = false;		// 判断当前是否有表情
	int iCount =qContect.size();
	for (int i = 0; i < iCount;i++)
	{
		QString str = qContect.at(i);
		if (str == "[")
		{
			QString sBlock = qContect.mid(i, 4);
			// 谨慎处理，完全比对
			if (sBlock == "[em_")
				bHas = true;
		}	

		if (bHas)
			qFace += str;

		if (str == "]" && bHas)
		{
			QString strFace = BuildFaceToUrl(qFace);
			ui.text_talk->insertHtml("<img src='" + strFace + "'/>");  //   此处的test 即 url
			ui.text_talk->addAnimation(QUrl(strFace), strFace);  //添加一个动画.	

			qFace = "";
			bHas = false;
			continue;
		}

		if (!bHas)
		{
			ui.text_talk->insertHtml(str);
		}
	}
}

QString UIChatRoom::BuildFaceToUrl(QString qFace)
{
	qFace.replace("[", "");
	qFace.replace("]", "");
	
	QString qFacePath = "./images/em.gif";
	qFacePath.replace("em",qFace);
	return qFacePath;
}

void UIChatRoom::ReceiverRecordMsg(nim::QueryMsglogResult* pMsg)
{
	std::vector<nim::IMMessage> vec;
	for (auto& msg : pMsg->msglogs_)
	{
		vec.push_back(msg);
	}
	ShowMsgs(vec);
}

void UIChatRoom::ShowMsgs(const std::vector<nim::IMMessage> &msg)
{
	//msg倒序排列
	size_t len = msg.size();
	for (size_t i = 0; i < len; i++)
	{
		ShowMsg(msg[i]);
	}
	
	//修正最远时间
	if (len > 0)
	{
		m_farst_msg_time = msg[len - 1].timetag_;
	}
	else
	{
		QString strTip = "没有更多消息记录了！";
		QString strText = ui.talkRecord->toPlainText();
		if (strText.indexOf(strTip)>=0)
			return;
		
		int fontwidth = fontMetrics().width(strTip);
		int iMargin = 300 - fontwidth;
		QTextCursor textCursor = ui.talkRecord->textCursor();
		textCursor.movePosition(QTextCursor::Start);

		QTextFrameFormat frameFormat2;
		frameFormat2.setLeftMargin(iMargin / 2);	//设置左边距
		frameFormat2.setRightMargin(iMargin / 2);	//设置右边距
		frameFormat2.setBottomMargin(20);	//设置下边距
		textCursor.insertFrame(frameFormat2);		//在光标处插入框架

		QTextCharFormat fmt;
		fmt.setForeground(contentColor);
		textCursor.setCharFormat(fmt);
		textCursor.insertText(strTip);
		return;
	}

	// 滚动条跳转到新加载消息的位置(用时间做标识)
	for (size_t i = 0; i < len; i++)
	{
		if (msg[i].type_ != nim::kNIMMessageTypeNotification)
		{
			QString time = QDateTime::fromMSecsSinceEpoch(msg[i].timetag_).toString("hh:mm:ss");
			QTextCursor tc = ui.talkRecord->textCursor();
			int position = ui.talkRecord->document()->find(time).position();
			tc.setPosition(position, QTextCursor::MoveAnchor);
			ui.talkRecord->setTextCursor(tc);

			QScrollBar* pScrollBar = ui.talkRecord->verticalScrollBar();
			if (pScrollBar)
			{
				int Pos = pScrollBar->sliderPosition();
				pScrollBar->setSliderPosition(Pos + 20);
			}
			break;
		}
	}

	//如果请求的20条消息填补不了窗口，则再次请求
	if (!ui.talkRecord->verticalScrollBar()->isVisible())
		nim::MsgLog::QueryMsgOnlineAsync(m_CurChatID, nim::kNIMSessionTypeTeam, kMsgLogNumberShow, 0, m_farst_msg_time, 0, false, true);
}

void UIChatRoom::ShowMsg(nim::IMMessage pMsg)
{
	if (pMsg.type_ == nim::kNIMMessageTypeNotification) // 过滤系统消息
	{
		Json::Value json;
		Json::Reader reader;
		if (reader.parse(pMsg.attach_, json))
		{
			nim::NIMNotificationId id = (nim::NIMNotificationId)json[nim::kNIMNotificationKeyId].asInt();

			// 加入消息
			if (id == nim::kNIMNotificationIdTeamInvite)
			{
				QString strName;
				Json::Value array = json[nim::kNIMNotificationKeyData][nim::kNIMNotificationKeyDataIds];
				if (!array.empty() && array.isArray())
				{
					int len = array.size();
					std::vector<std::string> ids;
					for (int i = 0; i < len; i++)
					{
						ids.push_back(array[i].asString());
					}
					if (!ids.empty())
					{
						int n = ids.size();
						int i = 0;
						for (; i < n && i < 3; i++)
						{
							if (!strName.isEmpty())
								strName += "，";

							if (m_StudentInfo.size() == 0)
								return;

							strName += *m_StudentInfo.find(QString::fromStdString(ids[i]));
						}
					}

					strName += " 加入了群聊";
					int fontwidth = fontMetrics().width(strName);
					int iMargin = 300 - fontwidth;
					QTextCursor textCursor = ui.talkRecord->textCursor();
					textCursor.movePosition(QTextCursor::Start);

					QTextFrameFormat frameFormat2;
					frameFormat2.setLeftMargin(iMargin/2);	//设置左边距
					frameFormat2.setRightMargin(iMargin / 2);	//设置右边距
					frameFormat2.setBottomMargin(20);	//设置下边距
					textCursor.insertFrame(frameFormat2);		//在光标处插入框架

					QTextCharFormat fmt;
					fmt.setForeground(contentColor);
					textCursor.setCharFormat(fmt);
					textCursor.insertText(strName);
				}
			}
		}
		return;
	}
	
	// 跨天处理
	stepDays(QDateTime::fromMSecsSinceEpoch(pMsg.timetag_));

	std::string strName = pMsg.readonly_sender_nickname_;
	std::string strContent = pMsg.content_;
	QString qTime = QDateTime::fromMSecsSinceEpoch(pMsg.timetag_).toString("hh:mm:ss");// 原型yyyy-MM-dd hh:mm:ss
	QString qContent = QString::fromStdString(strContent);
	QString qName;
	if (pMsg.sender_accid_ == m_accid.toStdString())
	{
		strName = m_TeachterName.toStdString();
		strName += "(我) ";
		qName = QString::fromStdString(strName);
		stringToHtml(qName, selfColor);
	}
	else
	{
		qName = QString::fromStdString(strName);
		stringToHtml(qName, nameColor);
	}
	
	stringToHtml(qTime, timeColor);
	stringToHtml(qContent, contentColor);

	// 显示消息
	QTextCursor textCursor = ui.talkRecord->textCursor();
	textCursor.movePosition(QTextCursor::Start);
	textCursor.insertHtml(qName + " " + qTime);
	textCursor.insertText("\n");
	textCursor.insertHtml(qContent);
	textCursor.insertText("\n\r");
	
	m_switchTime = false;	
}

//////////////////////////////添加云信功能////////////////////////////////
void UIChatRoom::initSDK()
{
	nim::SDKConfig config;

	//sdk能力参数（必填）
	config.database_encrypt_key_ = "Netease"; //string（db key必填，目前只支持最多32个字符的加密密钥！建议使用32个字符）
	bool ret = false;
	ret = nim::SDKFunction::LoadSdkDll();
	ret = LoadConfig("Netease", "", config);

	if (!ret)
		assert(0);// 初始化失败

	nim_http::Init(); // 初始化云信http

	// 接受消息回调
	nim::Talk::RegReceiveCb(&nim_comp::TalkCallback::OnReceiveMsgCallback);
	// 发送消息状态回调
	nim::Talk::RegSendMsgCb(&nim_comp::TalkCallback::OnSendMsgCallback);
}

void UIChatRoom::OnLoginCallback(const nim::LoginRes& login_res, const void* user_data)
{
	bool bsuc = true;

	if (login_res.res_code_ == nim::kNIMResSuccess)
		bsuc = true;
	else
		bsuc = false;
}

bool UIChatRoom::LoadConfig(const std::string& app_data_dir, const std::string& app_install_dir, nim::SDKConfig &config)
{
	QJsonObject config_root;
	QJsonObject config_values;
	config_values[kNIMDataBaseEncryptKey] = config.database_encrypt_key_.c_str();//string（db key必填，目前只支持最多32个字符的加密密钥！建议使用32个字符）
	config_values[kNIMPreloadAttach] = config.preload_attach_;        //bool （选填，是否需要预下载附件缩略图， sdk默认预下载）
	config_values[kNIMSDKLogLevel] = config.sdk_log_level_;
	config_root[kNIMGlobalConfig] = config_values;

	return NIM_SDK_GET_FUNC(nim_client_init)(app_data_dir.c_str(), app_install_dir.c_str(), GetJsonStringWithNoStyled(config_root).c_str());
}

std::string UIChatRoom::GetJsonStringWithNoStyled(const QJsonObject& values)
{
	QJsonDocument document;
	document.setObject(values);

	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	QString json_str(byte_array);
	return json_str.toStdString();
}

void UIChatRoom::setCurChatID(QString chatID, QString courseid)
{
	m_CurChatID = chatID.toStdString();
	m_CurCourseID = courseid;
}

void UIChatRoom::setChatInfo(QJsonObject &chatInfo, QString token)
{
	m_accid = chatInfo["accid"].toString();
	m_token = chatInfo["token"].toString();

	mRemeberToken = token;
}

void UIChatRoom::setKeyAndLogin(QString key)
{
	m_appKey = key;

	// 登陆
	auto cb = std::bind(OnLoginCallback, std::placeholders::_1, nullptr);
	bool bFail = nim::Client::Login(m_appKey.toStdString(), m_accid.toStdString(), m_token.toStdString(), cb);
	if (!bFail)
	{
		CMessageBox::showMessage(QString("答疑时间"), QString("失败！"), QString("确定"), QString("取消"));
		return;
	}

	m_bLogin = true;
}

void UIChatRoom::ReceiverLoginMsg(nim::LoginRes* pRes)
{
	if (pRes->res_code_ == nim::kNIMResSuccess && pRes->login_step_ == kNIMLoginStepLogin) // 登录成功
	{
		// 从云信再次获取群成员信息
		QueryGroup();
	}
}

// 初始化获取群成员的禁言状态
void UIChatRoom::ReceiverMemberMsg(std::list<nim::TeamMemberProperty>* pMemberMsg)
{
	ui.student_list->setStyleSheet("border-image: url(:/LoginWindow/images/alpha.png);");

	for (auto it : *pMemberMsg)
	{
		if (it.IsMute())
		{
			QString accid = QString::fromStdString(it.GetAccountID());
			personListBuddy* pBuddy = NULL;
			pBuddy = ui.student_list->findID(accid);
			if (pBuddy)
			{
				pBuddy->setCheckBox(true);
			}
		}
	}
}

bool UIChatRoom::IsLogin()
{
	return m_bLogin;
}

bool UIChatRoom::IsCurChatRoom(QString chatID)
{
	bool bChatRoom = false;
	std::string strChatID = chatID.toStdString();

	// 判断当前过来的消息，是不是此会话窗口
	if (strcmp(strChatID.c_str(), m_CurChatID.c_str()) == 0)
	{
		bChatRoom = true;
	}

	return bChatRoom;
}

void UIChatRoom::OnTeamEventCallback(const nim::TeamEvent& result)
{
}

//禁言
void UIChatRoom::chickChage(int b, QString qAccid, QString name)
{
	std::string accid = qAccid.toStdString();
	auto cb = std::bind(OnTeamEventCallback, std::placeholders::_1);
	nim::Team::MuteMemberAsync(m_CurChatID, accid, b, cb);	
}

// 添加成员
bool UIChatRoom::AddStudent(QString iconUrl, QString name, QString accid)
{
	// 老师本人不添加进群成员
	if (accid != m_accid)
	{
		ui.student_list->addStrdent(iconUrl, name, accid);
		m_StudentInfo.insert(accid, name);
		return true;
	}
	else
		return false;
}
void UIChatRoom::AddStudentNumbers(int num)
{
	ui.student_list->setStrdentNumbers(num);
	if (num==0)
	{
		ui.student_list->clear();
	}
}

// 添加公告
void UIChatRoom::AddAnnouncement(QString announcement, QString time)
{
	stringToHtml(time, timeColor);
	stringToHtml(announcement, contentColor);
	QTextCursor textCursor = ui.text_proclamation->textCursor();
	textCursor.insertImage("./images/announcement.png");
	textCursor.insertText(" ");
	textCursor.insertHtml(time);
	textCursor.insertText("\n");
	textCursor.insertHtml(announcement);
	textCursor.insertText("\n\r");
	ui.text_proclamation->setTextCursor(textCursor);
}

// 滚动条事件
void UIChatRoom::RecordMoved(int iPos)
{
	if (iPos == 0 && !m_switchTime)
	{
		nim::MsgLog::QueryMsgOnlineAsync(m_CurChatID, nim::kNIMSessionTypeTeam, kMsgLogNumberShow, 0, m_farst_msg_time, 0, false, true);
	}
}

// 查询群成员信息
void UIChatRoom::QueryGroup()
{
	nim::Team::QueryTeamMembersAsync(m_CurChatID);
}

// 历史消息记录跨天处理
void UIChatRoom::stepDays(QDateTime dateTime)
{
	QString newDay = dateTime.toString("MM-dd");
	QString oldDay = m_RecordTime.toString("MM-dd");
	if (newDay != oldDay && !ui.talkRecord->toPlainText().isEmpty())
	{
		QTextCursor textCursor = ui.talkRecord->textCursor();
		textCursor.movePosition(QTextCursor::Start);

		int fontwidth = fontMetrics().width(oldDay);
		int iMargin = 300 - fontwidth;

		QTextFrameFormat frameFormat2;
		frameFormat2.setLeftMargin(iMargin/2);	//设置左边距
		frameFormat2.setRightMargin(iMargin/2);	//设置右边距
		frameFormat2.setBottomMargin(20);	//设置右边距
		textCursor.insertFrame(frameFormat2);		//在光标处插入框架

		QTextCharFormat fmt;
		fmt.setForeground(contentColor);
		textCursor.setCharFormat(fmt);
		textCursor.insertText(oldDay);
	}

	m_RecordTime = dateTime;
}

void UIChatRoom::stepMsgDays(QDateTime dateTime)
{
	if (m_ReceiveTime.isNull())
	{
		m_ReceiveTime = dateTime;
		return;
	}

	QString newDay = dateTime.toString("MM-dd");
	QString oldDay = m_ReceiveTime.toString("MM-dd");
	if (newDay != oldDay && !ui.text_talk->toPlainText().isEmpty())
	{
		m_ChatHtml->CenterMsg(newDay);
	}

	m_ReceiveTime = dateTime;
}

void UIChatRoom::clearAll()
{
	ui.text_talk->clear();
	ui.text_proclamation->clear();
	ui.student_list->cleanStudents();
	m_StudentInfo.clear();
}

// 发送群公告
void UIChatRoom::OnSendAnnouncements(QString Announcements)
{
	if (m_CurCourseID.isEmpty())
		return;
	
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/courses/{id}/announcements";
	strUrl.replace("{id}", m_CurCourseID);
#else
	strUrl = "https://qatime.cn/api/v1/live_studio/courses/{id}/announcements";
	strUrl.replace("{id}", m_CurCourseID);
#endif

	QUrl url = QUrl(strUrl);
	QByteArray append("content=");
	append += Announcements;
	QNetworkRequest request(url);
	request.setRawHeader("Remember-Token", mRemeberToken.toUtf8());	
	reply = manager.post(request, append);
}
void UIChatRoom ::colseBrow()
{
	m_smallEmotionWidget->setHidden(true);
}

void UIChatRoom::QueryMember()
{
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/courses/{id}/realtime";
	strUrl.replace("{id}", m_CurCourseID);
#else
	strUrl = "https://qatime.cn/api/v1/live_studio/courses/{id}/realtime";
	strUrl.replace("{id}", m_CurCourseID);
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	request.setRawHeader("Remember-Token", mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIChatRoom::returnMember);
}

void UIChatRoom::returnMember()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1 && data.contains("members"))
	{
		// 群成员信息
		QJsonArray members = data["members"].toArray();
		int i = 0;
		foreach(const QJsonValue & value, members)
		{
			QJsonObject obj = value.toObject();
			YXMember *pMember = new YXMember();
			pMember->readJsonToMember(value.toObject());

			if (!m_StudentInfo.contains(pMember->accid()) && pMember->accid() != m_accid)
			{
				ui.student_list->addStrdent(pMember->url(), pMember->name(), pMember->accid());
				m_StudentInfo.insert(pMember->accid(), pMember->name());

				QString sName;
				sName += pMember->name();
				sName += " 加入了群聊";
				
				m_ChatHtml->CenterMsg(sName);

				SetStudentName(m_studentNum + 1);
			}

			//用完之后删除
			delete pMember;
		}
	}
}

void UIChatRoom::style(QTextBrowser *style)
{
	if (style)
	{
		// 设置滚动条样式
		style->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
			"{"
			"width:8px;"
			"background:rgba(0,0,0,0%);"
			"margin:0px,0px,0px,0px;"
			"padding-top:9px;"
			"padding-bottom:9px;"
			"}"
			"QScrollBar::handle:vertical"
			"{"
			"width:8px;"
			"background:rgba(0,0,0,25%);"
			" border-radius:4px;"
			"min-height:20;"
			"}"
			"QScrollBar::handle:vertical:hover"
			"{"
			"width:8px;"
			"background:rgba(0,0,0,50%);"
			" border-radius:4px;"
			"min-height:20;"
			"}"
			"QScrollBar::add-line:vertical"
			"{"
			"height:9px;width:8px;"
			"border-image:url(:/images/a/3.png);"
			"subcontrol-position:bottom;"
			"}"
			"QScrollBar::sub-line:vertical"
			"{"
			"height:9px;width:8px;"
			"border-image:url(:/images/a/1.png);"
			"subcontrol-position:top;"
			"}"
			"QScrollBar::add-line:vertical:hover"
			"{"
			"height:9px;width:8px;"
			"border-image:url(:/images/a/4.png);"
			"subcontrol-position:bottom;"
			"}"
			"QScrollBar::sub-line:vertical:hover"
			"{"
			"height:9px;width:8px;"
			"border-image:url(:/images/a/2.png);"
			"subcontrol-position:top;"
			"}"
			"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
			"{"
			"background:rgba(0,0,0,10%);"
			"border-radius:4px;"
			"}"
			);
	}
}

void UIChatRoom::SetStudentName(int iNum)
{
	m_studentNum = iNum;
	QString liveNum = "学员";
	liveNum += "(";
	liveNum.append(QString::number(iNum));
	liveNum += ")";
	
	ui.button_studentList->setText(liveNum);
}

void UIChatRoom::setMainWindow(UIMainWindow* parent)
{
	m_parent = parent;
}

// 初始化自适应的高度
void UIChatRoom::setAdaptHeight(int iHeight)
{
	ui.talkRecord->setFixedHeight(iHeight-90);
	m_proclamationHeight = iHeight - 105;
}

// 拉伸以后变化的高度
void UIChatRoom::setResize(int iWidth, int iHeight)
{
	// 消息记录
	ui.talkRecord->setFixedSize(ui.talkRecord->width()+iWidth, ui.talkRecord->height() + iHeight);
	ui.toolButton_3->move(ui.toolButton_3->geometry().left()+iWidth, ui.toolButton_3->geometry().top());
	// 聊天
	if (m_ChatHtml->m_TalkView)
	{
		int iWidthi = m_ChatHtml->m_TalkView->width();
		m_ChatHtml->setFixedSize(m_ChatHtml->m_TalkView->width() + iWidth, m_ChatHtml->m_TalkView->height() + iHeight);
		m_ChatHtml->m_TalkView->setFixedSize(m_ChatHtml->m_TalkView->width() + iWidth,m_ChatHtml->m_TalkView->height() + iHeight);
	}
	// 学生信息
	ui.student_list->setAllWidth(iWidth);

	// 公告
	ui.text_proclamation->setFixedWidth(ui.text_proclamation->width()+iWidth);
	ui.textEdit_2->setFixedWidth(ui.textEdit_2->width() + iWidth);
	ui.button_sendMseeage_3->move(ui.button_sendMseeage_3->geometry().left() + iWidth, ui.button_sendMseeage_3->geometry().top());
	ui.button_sendMseeage_cancel->move(ui.button_sendMseeage_cancel->geometry().left() + iWidth, ui.button_sendMseeage_cancel->geometry().top());
	ui.button_sendMseeage_2->move((ui.text_proclamation->width() - 80) / 2, ui.button_sendMseeage_2->geometry().top());
}

bool UIChatRoom::IsFous()
{
	return ui.textEdit->hasFocus();
}

void UIChatRoom::SendImage(const std::wstring src, QString &filename, QString msgid/*=""*/)
{
	nim::IMMessage msg;
	PackageMsg(msg);
	msg.type_ = nim::kNIMMessageTypeImage;

	if (!msgid.isEmpty())
		msg.client_msg_id_ = msgid.toStdString();

	QString appdata = UserAppdataPath();
	filename = QString::fromStdString(msg.client_msg_id_);

	QDir dir(appdata);
	if (!dir.exists())
		dir.mkdir(appdata);
	
	appdata += filename;

	GenerateUploadImage(src, appdata.toStdWString());
	msg.local_res_path_ = appdata.toStdString();

	nim::IMImage img;
	img.md5_ = GetFileMD5(appdata);
	img.size_ = GetFileSize(appdata);

	Gdiplus::Image image(appdata.toStdWString().c_str());
	if (image.GetLastStatus() != Gdiplus::Ok)
	{
		assert(0);
	}
	else
	{
		img.width_ = image.GetWidth();
		img.height_ = image.GetHeight();
	}

	msg.attach_ = img.ToJsonString();

	std::string json_msg = nim::Talk::CreateImageMessage(msg.receiver_accid_, msg.session_type_, msg.client_msg_id_, img, msg.local_res_path_, nim::MessageSetting(), msg.timetag_);
//	nim::Talk::FileUpPrgCallback* pcb = new nim::Talk::FileUpPrgCallback();
	nim::Talk::SendMsg(json_msg, msg.client_msg_id_);
}

QString UIChatRoom::UserAppdataPath()
{
	QString appPath = QProcessEnvironment::systemEnvironment().value("APPDATA");
	appPath += "\\Netease\\NIM\\";
	appPath.replace("Roaming", "Local");
	return appPath;
}

void UIChatRoom::GenerateUploadImage(const std::wstring src, const std::wstring dest)
{
	std::wstring mime_type = ZoomImage::GetMimeType(src);
	if (mime_type.empty())
		return;

	if (mime_type == kImageGIF)
	{
//		nbase::CopyFile(src, dest);
	}
	else
	{
		std::wstring str = src;

		ZoomImage zoom;
		zoom.SetImagePath(dest);
		zoom.SetAutoZoom(true, 1280, 1280);
		if (zoom.Zoom(str, mime_type == kImagePNG ? kImagePNG : kImageJPEG))
			return;
		else if (zoom.ConvertImageFormat(str, mime_type == kImagePNG ? kImagePNG : kImageJPEG))
			return;
	}
}

std::string UIChatRoom::GetFileMD5(QString path)
{
	QFile theFile(path);
	theFile.open(QIODevice::ReadOnly);
	QByteArray ba = QCryptographicHash::hash(theFile.readAll(), QCryptographicHash::Md5);
	theFile.close();
	QString MD5;
	MD5 = QString(QLatin1String(ba.toHex().constData()));
	return MD5.toStdString();
}

long UIChatRoom::GetFileSize(QString path)
{
	QFile theFile(path);
	theFile.open(QIODevice::ReadOnly);
	long size = theFile.size();
	theFile.close();
	return size;
}

void UIChatRoom::clickPic()
{
	if (strcmp(m_CurChatID.c_str(), "") == 0)
	{
		QToolTip::showText(QCursor::pos(), "请选择直播间！");
		return;
	}

	QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png)"));
	if (path.length() == 0)
		return;
	else 
	{
		QString sMsgID;
		SendImage(path.toStdWString(), sMsgID);

		// 本人头像
		QString image = m_parent->TeacherPhotoPixmap();
		QString name = m_TeachterName;
		QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
		// 跨天处理
		stepMsgDays(time);
		QString timeStr = time.toString("hh:mm:ss");
		name += "(我) ";
		m_ChatHtml->sendImageMsg(image, name, timeStr, path,sMsgID);
	}
}

void UIChatRoom::LoadImgTimeout()
{
	if (m_VerReceiveImg.size() == 0)
	{
		m_LoadImgTimer->stop();
	}

	std::vector<MyImageInfo>::iterator it;
	for (it = m_VerReceiveImg.begin(); it != m_VerReceiveImg.end(); it++)
	{
		MyImageInfo VecImg = *it;
		QString qContentNew = VecImg.ReceiverImg;
		qContentNew += ".png";
		qContentNew.replace("\\", "/");

		// 如果下载失败
		QFile file(qContentNew);
		if (file.exists())
		{
			if (strcmp(VecImg.chatID.c_str(), m_CurChatID.c_str()) == 0)
				m_ChatHtml->receiverImageMsg(VecImg.PhotoImg, VecImg.name, VecImg.time, qContentNew);

			m_VerReceiveImg.erase(it);

			if (m_VerReceiveImg.size() == 0)
			{
				m_LoadImgTimer->stop();
				return;
			}
			return;
		}

		if (QFile::copy(VecImg.ReceiverImg, qContentNew))
		{
			if (strcmp(VecImg.chatID.c_str(), m_CurChatID.c_str()) == 0)
				m_ChatHtml->receiverImageMsg(VecImg.PhotoImg, VecImg.name, VecImg.time, qContentNew);
			
			m_VerReceiveImg.erase(it);

			if (m_VerReceiveImg.size() == 0)
			{
				m_LoadImgTimer->stop();
				return;
			}
			return;
		}
	}
}

void UIChatRoom::UpLoadPicProcess(double diff)
{
	if (m_ChatHtml)
		m_ChatHtml->PicProcess(diff);
}

void UIChatRoom::SendStatus(nim::SendMessageArc* arcNew)
{
	if (m_ChatHtml)
		m_ChatHtml->SendStatus(QString::fromStdString(arcNew->msg_id_));
}