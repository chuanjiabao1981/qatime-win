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

#include "define.h"
#include <QMouseEvent>
#include <QToolTip>
#include <QNetworkRequest>
#include <QFileDialog>

#include "calendar/DefineCalendar.h"

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
	, m_studentNum(0)
	, m_proclamationHeight(0)
	, m_LoadImgTimer(NULL)
	, m_drawingWidth(0)
	, m_bZoom(false)
	, m_bClickPic(false)
	, m_uitalkRecord(NULL)
	, m_bPerson(false)
	, m_EnvironmentalTyle(true)
	, m_UnreadCount(0)
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
	connect(ui.text_talk, SIGNAL(colseBrow()), this, SLOT(colseBrow()));
	connect(ui.textEdit, SIGNAL(colseBrow()), this, SLOT(colseBrow()));
	connect(ui.timeWidget, SIGNAL(currentPageChanged(int, int)), this, SLOT(calendaCurrentPageChanged(int, int)));
	connect(ui.pic_pushButton, SIGNAL(clicked()), this, SLOT(clickPic()));
	connect(this, SIGNAL(sig_editClear()), this, SLOT(slot_eidtClear()));

	QScrollBar* TalkRecordScrollBar;
	TalkRecordScrollBar = (QScrollBar*)ui.talkRecord->verticalScrollBar();
	if (TalkRecordScrollBar)
		connect((QObject*)TalkRecordScrollBar, SIGNAL(valueChanged(int)), this, SLOT(RecordMoved(int)));

	initEmotion();
	this->clickTalk();
	m_isBorw = false;
	
	QDate cdate = QDate::currentDate();   //获取今天的日期
	QTextCharFormat format;
	format.setBackground(Qt::green);  //设置格式，颜色自选
	ui.timeWidget->setDateTextFormat(cdate, format);//设置当前日期始终高亮！
	ui.timeWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
	ui.timeWidget->setGridVisible(true);	

	// 消息编辑框粘贴过滤代码
	ui.textEdit->setContextMenuPolicy(Qt::NoContextMenu);
	ui.textEdit->installEventFilter(this);

	ui.text_talk->installEventFilter(this);
	ui.msgRecord->installEventFilter(this);

	style(ui.text_proclamation);
	style(ui.talkRecord);
	style(ui.text_talk);

	ui.student_list->setStyleSheet("border-image: url(:/LoginWindow/images/nochat.png);");

	m_uitalk = new UITalk(ui.text_talk);
	m_uitalk->setMainWindow(this);
	
  	m_uitalkRecord = new UITalkRecord(ui.talkRecord);
  	m_uitalkRecord->setMainWindow(this);
	connect(m_uitalkRecord, SIGNAL(colseCalendar()), this, SLOT(colseCalendar()));

	m_LoadImgTimer = new QTimer(this);
	connect(m_LoadImgTimer, SIGNAL(timeout()), this, SLOT(LoadImgTimeout()));

	//隐藏讨论、公告、成员
	ui.header_widget->setVisible(false);

	m_defDateTimeEdit = new DefDateTimeEdit(this);
	ui.calendar_horizontalLayout->addWidget(m_defDateTimeEdit);
	connect(m_defDateTimeEdit, SIGNAL(sig_CalendarClick(QDate)), this, SLOT(slot_CalendarClick(QDate)));

	ui.textEdit->setWordWrapMode(QTextOption::WrapAnywhere);
}

UIChatRoom::~UIChatRoom()
{
	m_StudentInfo.clear();

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
	if (target == ui.textEdit) 
	{
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
// 			if (m_parent)
// 				m_parent->PosInWindow();
		}
	}
	else if (target == ui.text_talk)
	{
		if (event->type() == QEvent::Resize)
		{
			QRect rc = ui.text_talk->geometry();
			m_uitalk->setFixedSize(ui.text_talk->width(), ui.text_talk->height());
			m_uitalk->ScrollDown();
		}
	}
	else if (target == ui.msgRecord)
	{
		if (event->type() == QEvent::Resize)
		{
			QRect rc = ui.msgRecord->geometry();
			ui.talkRecord->setFixedSize(ui.msgRecord->width(), ui.msgRecord->height() - 30);
			m_uitalkRecord->setFixedSize(ui.msgRecord->width(), ui.msgRecord->height() - 30);
			ui.toolButton_3->move(ui.msgRecord->width() - 50, ui.toolButton_3->y());
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

	ui.btn_widget->setVisible(true);
	ui.send_widget->setVisible(true);

	ui.proclamationWidget->setVisible(false);
	ui.text_talk->setVisible(true);
	ui.student_list->setVisible(false);
	ui.msgRecord->setVisible(false);
	ui.text_talk->moveCursor(QTextCursor::End);

	update();
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

	ui.btn_widget->setVisible(false);
	ui.send_widget->setVisible(false);

	ui.proclamationWidget->setVisible(false);
	ui.text_talk->setVisible(false);
	ui.student_list->setVisible(true);
	ui.msgRecord->setVisible(false);
	ui.student_list->initMenu();
	update();
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

	ui.button_sendMseeage_cancel->setVisible(false);
	ui.button_sendMseeage_3->setVisible(false);
	ui.textEdit_2->setVisible(false);
	ui.text_talk->setVisible(false);
	ui.student_list->setVisible(false);
	ui.msgRecord->setVisible(false);

	ui.btn_widget->setVisible(false);
	ui.send_widget->setVisible(false);
 	
	ui.proclamationWidget->setVisible(true);
	ui.text_proclamation->show();
	ui.text_proclamation->setGeometry(QRect(0, 40, 280, m_proclamationHeight));

	if (!m_CurCourseID.isEmpty())
		ui.button_sendMseeage_2->setVisible(true);
	else
		ui.button_sendMseeage_2->setVisible(false);

	update();
}
// 清屏
void UIChatRoom::clickCleanText()
{
	delete m_uitalk;
	m_uitalk = NULL;
	ui.text_talk->removeEventFilter(m_uitalk);
	m_uitalk = new UITalk(ui.text_talk);
	m_uitalk->setMainWindow(this);
	m_uitalk->show();
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
		return;
	}
	// 消息记录日期
	m_RecordTime = QDateTime::currentDateTime();
	QDateTime date = QDateTime::currentDateTime();
	m_defDateTimeEdit->setDate(date.date());

	ui.btn_widget->setVisible(false);
	ui.send_widget->setVisible(false);
	ui.proclamationWidget->setVisible(false);
	ui.text_talk->setVisible(false);
	ui.student_list->setVisible(false);
	ui.msgRecord->setVisible(true);
	ui.timeWidget->hide();
	// 清除消息记录
	delete m_uitalkRecord;
	m_uitalkRecord = NULL;
	ui.talkRecord->removeEventFilter(m_uitalkRecord);
	m_uitalkRecord = new UITalkRecord(ui.talkRecord);
	m_uitalkRecord->setMainWindow(this);
	m_uitalkRecord->setFixedSize(ui.talkRecord->width(), ui.talkRecord->height());
	m_uitalkRecord->show();		
	connect(m_uitalkRecord, SIGNAL(colseCalendar()), this, SLOT(colseCalendar()));

	m_farst_msg_time = 0;
	nim::MsgLog::QueryMsgOnlineAsync(m_CurChatID, nim::kNIMSessionTypeTeam, kMsgLogNumberShow, 0, m_farst_msg_time, 0, false, true, &UIWindowSet::QueryMsgOnlineCb);
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

//加载所有表情
void UIChatRoom::initEmotion()
{
	// 初始化小表情框;
	m_smallEmotionWidget = new MyEmotionWidget(this);
	connect(m_smallEmotionWidget, SIGNAL(emitFileName(QString)), this, SLOT(setBrow(QString)));
	m_smallEmotionWidget->setRowAndColumn(10, 8);
	m_smallEmotionWidget->setEmotionSize(QSize(34, 33));
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
	ui.textEdit->insertHtml("<img src='" + path + "' width='24' height='24' />");  //   此处的test 即 url
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
			QPixmap image = m_parent->TeacherPhotoPixmap();
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
			m_parent->SendTeacherBullet("(我)", contect, QString::fromStdString(m_CurChatID));
			stringToHtmlFilter(sendText);

			m_uitalk->InsertEmoji(&image, name, timeStr, sendMsg);
		}
		else
		{
			QString qName = m_TeachterName;
			qName += "(我) ";

			// 本人头像
 			QPixmap image = m_parent->TeacherPhotoPixmap();
			m_parent->SendTeacherBullet("(我)", sendText, QString::fromStdString(m_CurChatID));

			m_uitalk->InsertChat(&image, qName, timeStr, sendText);
		}
//		ui.textEdit->clear();
		emit sig_editClear();
	}
	else
	{
		QToolTip::showText(QCursor::pos(), "不能发送空数据！");
		return;
	}

	//发往服务器
	nim::IMMessage msg;
	PackageMsg(msg);
	msg.type_ = nim::kNIMMessageTypeText;
	msg.content_ = sendMsg.toStdString();
	
	nim::MessageSetting setting;
	std::string json_msg = nim::Talk::CreateTextMessage(msg.receiver_accid_, msg.session_type_, msg.client_msg_id_, msg.content_, setting, msg.timetag_);
	nim::Talk::FileUpPrgCallback* pcb = new nim::Talk::FileUpPrgCallback();
	nim::Talk::SendMsg(json_msg,"",pcb);
	m_borw.clear();

	ui.textEdit->setFocus();
}

void UIChatRoom::chickChoseTime()
{
// 	m_defDateTimeEdit->click();
// 	return;
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

void UIChatRoom::slot_CalendarClick(QDate date)
{
	QDate dateNow = QDate::currentDate();
	if (dateNow < date)
	{
		//TODO提示，消息记录只能查看之前的消息
		QToolTip::showText(QCursor::pos(), "没有今天之后的消息记录！");
	}
	else
	{
		m_defDateTimeEdit->setDate(date);
		QString dtstr = date.toString("yyyy-MM-dd");

		QueryRecord(dtstr);
	}
}

void UIChatRoom::forwardTime()
{
// 	QDate date = ui.timeWidget->selectedDate().addDays(1);	
// 	ui.timeWidget->setSelectedDate(date);
// 	QString dtstr = date.toString("yyyy-MM-dd");
// 	ui.timeShow->setText(dtstr);

	QDate date = m_defDateTimeEdit->date().addDays(1);
	m_defDateTimeEdit->setDate(date);
	QString dtstr = date.toString("yyyy-MM-dd");

	if (!m_parent->m_bQueryMsg)
		QueryRecord(dtstr);
}

void UIChatRoom::afterTime()
{
// 	QDate date = ui.timeWidget->selectedDate().addDays(-1);
// 	ui.timeWidget->setSelectedDate(date);
// 	QString dtstr = date.toString("yyyy-MM-dd");
// 	ui.timeShow->setText(dtstr);

	QDate date = m_defDateTimeEdit->date().addDays(-1);
	m_defDateTimeEdit->setDate(date);
	QString dtstr = date.toString("yyyy-MM-dd");
	
	if (!m_parent->m_bQueryMsg)
		QueryRecord(dtstr);
}

void UIChatRoom::QueryRecord(QString dtstr)
{
	delete m_uitalkRecord;
	m_uitalkRecord = NULL;
	ui.talkRecord->removeEventFilter(m_uitalkRecord);
	m_uitalkRecord = new UITalkRecord(ui.talkRecord);
	m_uitalkRecord->setMainWindow(this);
	m_uitalkRecord->setFixedSize(ui.talkRecord->width(), ui.talkRecord->height());
	m_uitalkRecord->show();
	connect(m_uitalkRecord, SIGNAL(colseCalendar()), this, SLOT(colseCalendar()));

	// 获取时间
	m_switchTime = true;
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

	m_parent->m_bQueryMsg = true;
	nim::MsgLog::QueryMsgOnlineAsync(m_CurChatID, nim::kNIMSessionTypeTeam, kMsgLogNumberShow, 0, m_farst_msg_time, 0, false, true, &UIWindowSet::QueryMsgOnlineCb);
}

// 点击【发布公告】按钮
void UIChatRoom::announce()
{
	ui.text_proclamation->setGeometry(QRect(0, 40 + 80, 299, m_proclamationHeight - 80));
	ui.button_sendMseeage_cancel->setVisible(true);
	ui.button_sendMseeage_3->setVisible(true);
	ui.textEdit_2->setVisible(true);
	ui.button_sendMseeage_2->setVisible(false);
	ui.text_proclamation->setVisible(true);
}
// 点击【发布】按钮
void UIChatRoom::putTalk()
{
	QString announcement = ui.textEdit_2->toPlainText();//增加新公告
	OnSendAnnouncements(announcement);

	ui.text_proclamation->setGeometry(QRect(0, 40, 299, m_proclamationHeight));
	ui.button_sendMseeage_cancel->setVisible(false);
	ui.button_sendMseeage_3->setVisible(false);
	ui.textEdit_2->setVisible(false);
	ui.button_sendMseeage_2->setVisible(true);
	ui.text_proclamation->setVisible(true);
}

// 点击【取消】按钮
void UIChatRoom::putTalkCancel()
{
	ui.text_proclamation->setGeometry(QRect(0, 40, 299, m_proclamationHeight));
	ui.button_sendMseeage_cancel->setVisible(false);
	ui.button_sendMseeage_3->setVisible(false);
	ui.textEdit_2->setVisible(false);
	ui.button_sendMseeage_2->setVisible(true);
	ui.text_proclamation->setVisible(true);

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
	msg.client_msg_id_ = nim::Tool::GetUuid();
	msg.msg_setting_.resend_flag_ = nim::BS_FALSE;

	//base获取的时间单位是s，服务器的时间单位是ms
	msg.timetag_ = QDateTime::currentMSecsSinceEpoch();

	msg.status_ = nim::kNIMMsgLogStatusSending;
}


// 接收消息
bool UIChatRoom::ReceiverMsg(const nim::IMMessage* pMsg)
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
		qDebug() << "进入此聊天";
		stepMsgDays(QDateTime::fromMSecsSinceEpoch(pMsg->timetag_));

		std::string strName = pMsg->readonly_sender_nickname_;
		std::string strContent = pMsg->content_;
		std::string strID = pMsg->sender_accid_;

		QString qTime = QDateTime::fromMSecsSinceEpoch(pMsg->timetag_).toString("hh:mm:ss");// 原型yyyy-MM-dd hh:mm:ss
		QString qName = QString::fromStdString(strName);
		QString qContent = QString::fromStdString(strContent);		

		// 聊天头像
		QPixmap* img = NULL;
		personListBuddy* Buddy = NULL;
		Buddy = ui.student_list->findID(QString::fromStdString(strID));
		if (Buddy)
			img= (QPixmap*)Buddy->head->pixmap();
		
		m_parent->SendStudentBullet(qName, qContent, QString::fromStdString(m_CurChatID));
		if (IsHasFace(qContent))
			m_uitalk->InsertEmoji(img, qName, qTime, qContent, false);
		else
			m_uitalk->InsertChat(img, qName, qTime, qContent, false);
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
		QPixmap* img=NULL;
		personListBuddy* Buddy = NULL;
		Buddy = ui.student_list->findID(QString::fromStdString(strID));
		if (Buddy)
			img = (QPixmap*)Buddy->head->pixmap();

		// 如果下载失败
		if (!QFile::copy(qContent, qContentNew))
		{
			if (img == NULL)
				img = &QPixmap("./images/teacherPhoto.png");
			
			MyImageInfo ImgInfo;
			ImgInfo.PhotoImg = *img;
			ImgInfo.name = qName;
			ImgInfo.time = qTime;
			ImgInfo.ReceiverImg = qContent;
			ImgInfo.chatID = pMsg->local_talk_id_;
			m_VerReceiveImg.push_back(ImgInfo);
			m_LoadImgTimer->start(500);
			return false;
		}

		m_uitalk->InsertPic(img, qName, qTime, qContentNew,"",false);
		
		m_parent->SendStudentBullet(qName, "[图片消息]", QString::fromStdString(m_CurChatID));
		bValid = true;
	}
	else if (strcmp(pMsg->local_talk_id_.c_str(), m_CurChatID.c_str()) == 0 && pMsg->type_ == nim::kNIMMessageTypeAudio)
	{
		std::string path;	// 路径
		QString qduration;	// 语音时长
		std::string sid;	// 会话窗口ID
		std::string msgid;	// 当前消息ID
		Json::Value values;
		Json::Reader reader;
		if (reader.parse(pMsg->attach_, values) && values.isObject())
		{
			path = values["md5"].asString();
			path = m_AudioPath + path;
			sid = pMsg->local_talk_id_;
			msgid = pMsg->client_msg_id_;
			int duration = values["dur"].asUInt();
			qDebug() << QString::number(duration);
			qduration = QString::number((duration+500)/1000);
			if (qduration.toInt() > 60)
				qduration = "60";
		}

		stepMsgDays(QDateTime::fromMSecsSinceEpoch(pMsg->timetag_));

		std::string strName = pMsg->readonly_sender_nickname_;
		std::string strContent = pMsg->content_;
		std::string strID = pMsg->sender_accid_;

		QString qTime = QDateTime::fromMSecsSinceEpoch(pMsg->timetag_).toString("hh:mm:ss");// 原型yyyy-MM-dd hh:mm:ss
		QString qName = QString::fromStdString(strName);
		// 聊天头像
		QPixmap* img=NULL;
		personListBuddy* Buddy = NULL;
		Buddy = ui.student_list->findID(QString::fromStdString(strID));
		if (Buddy)
			img = (QPixmap*)Buddy->head->pixmap();


		m_uitalk->InsertAudioChat(img, qName, qTime, qduration, path, sid, msgid, false);
		
		m_parent->SendStudentBullet(qName, "[语音消息]", QString::fromStdString(m_CurChatID));
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

void UIChatRoom::ParseFace(QString qContect, QString qName, QString qTime)
{
	QTextCursor textCursor = ui.talkRecord->textCursor();
	textCursor.movePosition(QTextCursor::Start);
	textCursor.insertHtml(qName + " " + qTime);
	textCursor.insertText("\n");

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
			ui.talkRecord->insertHtml("<img src='" + strFace + "'/>");  //   此处的test 即 url
			ui.talkRecord->addAnimation(QUrl(strFace), strFace);  //添加一个动画.	

			qFace = "";
			bHas = false;
			continue;
		}

		if (!bHas)
		{
			ui.talkRecord->insertHtml(str);
		}
	}
	textCursor.insertText("\n\r");
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
	
	if (len < 20)
	{
		m_uitalkRecord->InsertNotice("没有更多消息记录了！");
		return;
	}

	m_uitalkRecord->InsertBtn();

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

							strName = m_StudentInfo.value(QString::fromStdString(ids[i]), "无名");
						}
					}

					strName += " 加入了群聊";
					m_uitalkRecord->InsertNotice(strName);
					stepDays(QDateTime::fromMSecsSinceEpoch(pMsg.timetag_));
				}
			}
			else if (id == nim::kNIMNotificationIdTeamMuteMember)
			{
				QString name;
				std::string obj_account = json[nim::kNIMNotificationKeyData][nim::kNIMNotificationKeyDataId].asString();
				personListBuddy* Buddy = NULL;
				Buddy = ui.student_list->findID(QString::fromStdString(obj_account));
				if (Buddy)
					name = Buddy->name->text();

				bool set_mute = json[nim::kNIMNotificationKeyData]["mute"].asInt() == 1;
				if (set_mute)
					name += "已被禁言";
				else
					name += "已被解除禁言";

				m_uitalkRecord->InsertNotice(name);

				stepDays(QDateTime::fromMSecsSinceEpoch(pMsg.timetag_));
			}
		}
		return;
	}
	
	// 跨天处理
	stepDays(QDateTime::fromMSecsSinceEpoch(pMsg.timetag_));

	// 判断当前过来的消息，是不是此会话窗口
	if (pMsg.type_ == nim::kNIMMessageTypeText)
	{
		qDebug() << "进入此聊天";
		std::string strName = pMsg.readonly_sender_nickname_;
		std::string strContent = pMsg.content_;
		std::string strID = pMsg.sender_accid_;

		QString qTime = QDateTime::fromMSecsSinceEpoch(pMsg.timetag_).toString("hh:mm:ss");// 原型yyyy-MM-dd hh:mm:ss
		QString qName = QString::fromStdString(strName);
		QString qContent = QString::fromStdString(strContent);

		// 聊天头像
		QPixmap* img=NULL;
		personListBuddy* Buddy = NULL;
		Buddy = ui.student_list->findID(QString::fromStdString(strID));
		if (Buddy)
			img = (QPixmap*)Buddy->head->pixmap();

		if (pMsg.sender_accid_ == m_accid.toStdString())
		{
			strName = m_TeachterName.toStdString();
			strName += "(我) ";
			qName = QString::fromStdString(strName);
			// 本人头像
			QPixmap image = m_parent->TeacherPhotoPixmap();
			if (IsHasFace(qContent))
				m_uitalkRecord->InsertEmoji(&image, qName, qTime, qContent);
			else
				m_uitalkRecord->InsertChat(&image, qName, qTime, qContent);
		}
		else
		{
			qName = QString::fromStdString(strName);

			if (IsHasFace(qContent))
				m_uitalkRecord->InsertEmoji(img, qName, qTime, qContent, false);
			else
				m_uitalkRecord->InsertChat(img, qName, qTime, qContent, false);
		}
	}
	else if (pMsg.type_ == nim::kNIMMessageTypeImage)
	{
		std::string strName = pMsg.readonly_sender_nickname_;
		std::string strContent = pMsg.content_;
		std::string strID = pMsg.sender_accid_;

		QString qTime = QDateTime::fromMSecsSinceEpoch(pMsg.timetag_).toString("hh:mm:ss");// 原型yyyy-MM-dd hh:mm:ss
		QString qName = QString::fromStdString(strName);
		QString qContent = QString::fromStdString(pMsg.local_res_path_);
		QString qContentNew = qContent;
		qContentNew += ".png";
		qContentNew.replace("\\", "/");
		// 聊天头像
		QPixmap* img=NULL;
		personListBuddy* Buddy = NULL;
		Buddy = ui.student_list->findID(QString::fromStdString(strID));
		if (Buddy)
			img = (QPixmap*)Buddy->head->pixmap();

		Json::Value values;
		Json::Reader reader;
		std::string url;
		if (reader.parse(pMsg.attach_, values) && values.isObject())
		{
			url = values["url"].asString();
		}
	
		if (pMsg.sender_accid_ == m_accid.toStdString())
		{
			strName = m_TeachterName.toStdString();
			strName += "(我) ";
			qName = QString::fromStdString(strName);
			// 本人头像
			QPixmap image = m_parent->TeacherPhotoPixmap();

			m_uitalkRecord->InsertPic(&image, qName, qTime, QString::fromStdString(url));
		}
		else
		{
			m_uitalkRecord->InsertPic(img, qName, qTime, QString::fromStdString(url),"", false);
		}
	}
	else if (pMsg.type_ == nim::kNIMMessageTypeAudio)
	{
		std::string path;	// 路径
		QString qduration;	// 语音时长
		std::string sid;	// 会话窗口ID
		std::string msgid;	// 当前消息ID
		Json::Value values;
		Json::Reader reader;
		if (reader.parse(pMsg.attach_, values) && values.isObject())
		{
			path = values["md5"].asString();
			path = m_AudioPath + path;
			sid = pMsg.local_talk_id_;
			msgid = pMsg.client_msg_id_;
			int duration = values["dur"].asUInt();
			qduration = QString::number((duration+500) / 1000);
			if (qduration.toInt() > 60)
				qduration = "60";
		}

		stepMsgDays(QDateTime::fromMSecsSinceEpoch(pMsg.timetag_));

		std::string strName = pMsg.readonly_sender_nickname_;
		std::string strContent = pMsg.content_;
		std::string strID = pMsg.sender_accid_;

		QString qTime = QDateTime::fromMSecsSinceEpoch(pMsg.timetag_).toString("hh:mm:ss");// 原型yyyy-MM-dd hh:mm:ss
		QString qName = QString::fromStdString(strName);
		// 聊天头像
		QPixmap* img=NULL;
		personListBuddy* Buddy = NULL;
		Buddy = ui.student_list->findID(QString::fromStdString(strID));
		if (Buddy)
			img = (QPixmap*)Buddy->head->pixmap();
	
		if (pMsg.sender_accid_ == m_accid.toStdString())
		{
			strName = m_TeachterName.toStdString();
			strName += "(我) ";
			qName = QString::fromStdString(strName);
			// 本人头像
			QPixmap image = m_parent->TeacherPhotoPixmap();
			m_uitalkRecord->InsertAudioChat(&image, qName, qTime, qduration, path, sid, msgid,true);
		}
		else
		{
			m_uitalkRecord->InsertAudioChat(img, qName, qTime, qduration, path, sid, msgid, false);
		}

		QString audioPath = QString::fromStdString(path);
		QFile file(audioPath);
		if (!file.exists())
		{
			nim::NOS::FetchMedia(pMsg, nim::NOS::DownloadMediaCallback(), nim::NOS::ProgressCallback());
		}
	}
	
	m_switchTime = false;	
}

//////////////////////////////添加云信功能////////////////////////////////
void UIChatRoom::OnLoginCallback(const nim::LoginRes& login_res, const void* user_data)
{
	bool bsuc = true;

	if (login_res.res_code_ == nim::kNIMResSuccess)
		bsuc = true;
	else
		bsuc = false;
}

// bool UIChatRoom::LoadConfig(const std::string& app_data_dir, const std::string& app_install_dir, nim::SDKConfig &config)
// {
// 	QJsonObject config_root;
// 	QJsonObject config_values;
// 	config_values[kNIMDataBaseEncryptKey] = config.database_encrypt_key_.c_str();//string（db key必填，目前只支持最多32个字符的加密密钥！建议使用32个字符）
// 	config_values[kNIMPreloadAttach] = config.preload_attach_;        //bool （选填，是否需要预下载附件缩略图， sdk默认预下载）
// 	config_values[kNIMSDKLogLevel] = config.sdk_log_level_;
// 	config_root[kNIMGlobalConfig] = config_values;
// 
// 	return NIM_SDK_GET_FUNC(nim_client_init)(app_data_dir.c_str(), app_install_dir.c_str(), GetJsonStringWithNoStyled(config_root).c_str());
// }

// std::string UIChatRoom::GetJsonStringWithNoStyled(const QJsonObject& values)
// {
// 	QJsonDocument document;
// 	document.setObject(values);
// 
// 	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
// 	QString json_str(byte_array);
// 	return json_str.toStdString();
// }

void UIChatRoom::setCurChatID(QString chatID, QString courseid, QString teacherid, QString token, QString teacherName, QString accid, int UnreadCount, bool b1v1)
{
	m_CurChatID = chatID.toStdString();
	m_CurCourseID = courseid;
	m_CurTeacherID = teacherid;
	m_accid = accid;
	mRemeberToken = token;
	m_teacherName = teacherName;
	m_UnreadCount = UnreadCount;
	m_b1v1Lesson = b1v1;
	if (b1v1)
		Request1v1Member();
	else
		RequestMember();
}

std::string UIChatRoom::GetCurChatID()
{
	return m_CurChatID;
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

void UIChatRoom::ReceiverLoginMsg(const nim::LoginRes& pRes)
{
	if (pRes.res_code_ == nim::kNIMResSuccess) // 登录成功
	{
		if (pRes.login_step_ == nim::kNIMLoginStepLogin)
		{
			// 从云信再次获取群成员信息
			QueryGroup();
		}
	}
	else
	{
		int ErrorCode = pRes.res_code_;
		QString sError = "云信登录失败，错误码";
		sError += QString::number(ErrorCode);
		CMessageBox::showMessage(QString("答疑时间"), QString(sError), QString("确定"), QString("取消"));
	}
}

void UIChatRoom::SetCurAudioPath(std::string path)
{
	m_AudioPath = path;
}

void UIChatRoom::OnPlayAudioCallback(int code, const char* file_path, const char* sid, const char* cid)
{
/*	Post2UI(nbase::Bind(&UIPlayAudioCallback, code, std::string(sid), std::string(cid)));*/
}

void UIChatRoom::OnStopAudioCallback(int code, const char* file_path, const char* sid, const char* cid)
{
	QString strSid = QString(QLatin1String(sid));
	char* pData = new char[strlen(cid)];
	memcpy(pData, cid, strlen(cid));
	HWND hWnd = FindWindow(L"Qt5QWindowToolSaveBits", L"UIMainWindow");
	PostMessage(hWnd, MSG_SEND_AUDIO_MSG, (WPARAM)(int)strSid.toInt(), (LPARAM)pData);
}

// 初始化获取群成员的禁言状态
void UIChatRoom::ReceiverMemberMsg(std::list<nim::TeamMemberProperty>* pMemberMsg)
{
	ui.student_list->setStyleSheet("border-image: url(:/LoginWindow/images/AuxiliaryPanelBack.png);");

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

	ui.student_list->setBuddyWidth(width());
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

	if (b)
		name += "已被禁言";
	else
		name += "已被解除禁言";

	m_uitalk->InsertNotice(name);
}

// 添加成员
bool UIChatRoom::AddStudent(QString iconUrl, QString name, QString accid)
{
	// 老师本人不添加进群成员
	if (accid != m_accid)
	{
		ui.student_list->addStrdent(iconUrl, name, accid, m_drawingWidth);
		m_StudentInfo.insert(accid, name);

		int iHeight = ui.student_list->parentWidget()->height() - 40;
		int iCount = ui.student_list->count();
		if (iHeight < iCount * 30)
		{
			ui.student_list->setZoomWidth(-12);
		}
		
		return true;
	}
	else
		return false;
}

void UIChatRoom::MuteStudent(bool state, QString id)
{
	personListBuddy* Buddy = NULL;
	Buddy = ui.student_list->findID(id);
	if (Buddy)
	{
		Buddy->setMute(state);
	}
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
void UIChatRoom::RecordMoved()
{
	nim::MsgLog::QueryMsgOnlineAsync(m_CurChatID, nim::kNIMSessionTypeTeam, kMsgLogNumberShow, 0, m_farst_msg_time, 0, false, true,&UIWindowSet::QueryMsgOnlineCb);
}

// 查询群成员信息
void UIChatRoom::QueryGroup()
{
	nim::Team::QueryTeamMembersAsync(m_CurChatID, &UIWindowSet::OnGetTeamMemberCallback);
}

// 历史消息记录跨天处理
void UIChatRoom::stepDays(QDateTime dateTime)
{
	QString newDay = dateTime.toString("MM-dd");
	QString oldDay = m_RecordTime.toString("MM-dd");
	if (newDay != oldDay && m_uitalkRecord->m_Ver->count() != 0)
	{
		if (m_uitalkRecord)
			m_uitalkRecord->InsertNotice(oldDay);
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
		m_uitalk->InsertNotice(newDay);
	}

	m_ReceiveTime = dateTime;
}

void UIChatRoom::clearAll()
{
	//删除聊天内容
	delete m_uitalk;
	m_uitalk = NULL;
	ui.text_talk->removeEventFilter(m_uitalk);
	m_uitalk = new UITalk(ui.text_talk);
	m_uitalk->setMainWindow(this);
	m_uitalk->show();

	ui.text_proclamation->clear();
	ui.student_list->cleanStudents(m_drawingWidth);
	m_StudentInfo.clear();
	m_bZoom = false;
}

// 发送群公告
void UIChatRoom::OnSendAnnouncements(QString Announcements)
{
	if (m_CurCourseID.isEmpty())
		return;
	
	QString strUrl;
	if (m_b1v1Lesson)
	{
		if (m_EnvironmentalTyle)
		{
			strUrl = "https://qatime.cn/api/v1/live_studio/interactive_courses/{id}/announcements";
			strUrl.replace("{id}", m_CurCourseID);
		}
		else
		{
			strUrl = "http://testing.qatime.cn/api/v1/live_studio/interactive_courses/{id}/announcements";
			strUrl.replace("{id}", m_CurCourseID);
		}
	}
	else
	{
		if (m_EnvironmentalTyle)
		{
			strUrl = "https://qatime.cn/api/v1/live_studio/courses/{id}/announcements";
			strUrl.replace("{id}", m_CurCourseID);
		}
		else
		{
			strUrl = "http://testing.qatime.cn/api/v1/live_studio/courses/{id}/announcements";
			strUrl.replace("{id}", m_CurCourseID);
		}
	}

	Announcements = parse(Announcements);

	QUrl url = QUrl(strUrl);
	QByteArray append("content=");
	append += Announcements;
	QNetworkRequest request(url);
	request.setRawHeader("Remember-Token", mRemeberToken.toUtf8());	
	reply = manager.post(request, append);
	connect(reply, &QNetworkReply::finished, this, &UIChatRoom::ReturnAnnouncements);
}
void UIChatRoom::ReturnAnnouncements()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1)
	{
		QString announcement = ui.textEdit_2->toPlainText();//增加新公告
		QDateTime current_date_time = QDateTime::currentDateTime();
		QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
		stringToHtml(current_date, timeColor);
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
		return;
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
	else
	{
		// 重试5次后放弃
		
	}
}

void UIChatRoom::RequestError(QJsonObject& error)
{
	QString strError;
	if (error["code"].toInt() == 1002)
	{
		strError = QString("授权过期,请重新登录！");
		int iStatus = CMessageBox::showMessage(
			QString("答疑时间"),
			QString(strError),
			QString("确定"),
			QString());
		if (iStatus == 1 || iStatus == 0)
		{
			if (m_parent)
				m_parent->ReturnLogin();
		}
		return;
	}
	else if (error["code"].toInt() == 1003)
		strError = QString("没有权限访问！");
	else if (error["code"].toInt() == 1004)
		strError = QString("授权失败,请重新登录！");
	else if (error["code"].toInt() == 3001)
	{
		strError = QString("输入内容格式有误，请重新输入！");
		CMessageBox::showMessage(
			QString("答疑时间"),
			QString(strError),
			QString("确定"),
			QString());
		return;
	}
	else if (error["code"].toInt() == 3002)
		strError = QString("数据不合法,请重新登录！");
	else if (error["code"].toInt() == 4001)
		strError = QString("找不到资源,请重新登录！");
	else if (error["code"].toInt() == 9999)
		strError = QString("服务器错误,请重新登录！");
	else
		return;

	int iStatus = CMessageBox::showMessage(
		QString("答疑时间"),
		QString(strError),
		QString("确定"),
		QString());
	if (iStatus == 1 || iStatus == 0)
	{
// 		if (m_parent)
// 			m_parent->GetLoginWnd()->ReturnLogin();
	}
}

void UIChatRoom ::colseBrow()
{
	m_smallEmotionWidget->setHidden(true);
}

void UIChatRoom::QueryMember()
{
	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl = "https://qatime.cn/api/v1/live_studio/courses/{id}/realtime";
		strUrl.replace("{id}", m_CurCourseID);
	}
	else
	{
		strUrl = "http://testing.qatime.cn/api/v1/live_studio/courses/{id}/realtime";
		strUrl.replace("{id}", m_CurCourseID);
	}

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
				ui.student_list->addStrdent(pMember->url(), pMember->name(), pMember->accid(), m_drawingWidth);
				m_StudentInfo.insert(pMember->accid(), pMember->name());

				int iHeight = ui.student_list->parentWidget()->height()-40;

				QString sName;
				sName += pMember->name();
				sName += " 加入了群聊";
				
				m_uitalk->InsertNotice(sName);

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

void UIChatRoom::setMainWindow(UIWindowSet* parent)
{
	m_parent = parent;
}

// 拉伸以后变化的高度
void UIChatRoom::setResize(int iWidth, int iHeight)
{
	m_drawingWidth += iWidth;
	// 消息记录
	ui.talkRecord->setFixedSize(ui.talkRecord->width()+iWidth, ui.talkRecord->height() + iHeight);
	ui.toolButton_3->move(ui.toolButton_3->geometry().left()+iWidth, ui.toolButton_3->geometry().top());
	// 聊天
	ui.text_talk->setFixedSize(ui.text_talk->width() + iWidth, ui.text_talk->height() + iHeight);
	m_uitalk->setFixedSize(m_uitalk->width() + iWidth, m_uitalk->height() + iHeight);

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
	nim::Talk::FileUpPrgCallback* pcb = new nim::Talk::FileUpPrgCallback();
	nim::Talk::SendMsg(json_msg, msg.client_msg_id_, pcb);
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
	m_bClickPic = true;
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
		QPixmap image = m_parent->TeacherPhotoPixmap();
		QString name = m_TeachterName;
		QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
		// 跨天处理
		stepMsgDays(time);
		QString timeStr = time.toString("hh:mm:ss");
		name += "(我) ";
		m_uitalk->InsertPic(&image,name,timeStr,path,sMsgID);
		
		m_parent->SendTeacherBullet("(我)", "[图片消息]", QString::fromStdString(m_CurChatID));
	}
	m_bClickPic = false;
	ui.textEdit->setFocus();
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
				m_uitalk->InsertPic(&VecImg.PhotoImg, VecImg.name, VecImg.time, qContentNew, "", false);

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
				m_uitalk->InsertPic(&VecImg.PhotoImg, VecImg.name, VecImg.time, qContentNew,"",false);
			
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
	if (m_uitalk)
		m_uitalk->PicProcess(diff);
}

void UIChatRoom::SendStatus(nim::SendMessageArc* arcNew)
{
	if (m_uitalk)
		m_uitalk->SendStatus(QString::fromStdString(arcNew->msg_id_));
}

bool UIChatRoom::IsClickPic()
{
	return m_bClickPic;
}

void UIChatRoom::TalkDown()
{
	if (m_uitalk)
		m_uitalk->DelaySrcoll();
}

void UIChatRoom::OnPlayAudio(std::string path, std::string sid, std::string msgid, bool isPlay)
{
	if (isPlay)
	{
		nim_audio::nim_audio_type audio_format = nim_audio::AAC;
		{
			std::wstring wpath = QString::fromStdString(path).toStdWString();
			FILE* audio_file;
			if (_wfopen_s(&audio_file, wpath.c_str(), L"rb"))
				return;

			char header[6];
			int n = fread(header, 1, 6, audio_file);
			if (n == 6 && !memcmp(header, "#!AMR\n", 6)) //AMR文件
				audio_format = nim_audio::AMR;
			fclose(audio_file);
		}
		nim_audio::Audio::PlayAudio(path.c_str(), sid.c_str(), msgid.c_str(), audio_format);
	}
	else
	{
		nim_audio::Audio::StopPlayAudio();
	}
}

void UIChatRoom::OnStopPlayAudio(char* msgid)
{
	if (m_uitalk)
		m_uitalk->stopAudio(msgid);
	
	if (m_uitalkRecord)
		m_uitalkRecord->stopAudio(msgid);
}

void UIChatRoom::Request1v1Member()
{
	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl = "https://qatime.cn/api/v1/live_studio/interactive_courses/{id}/realtime";
		strUrl.replace("{id}", m_CurCourseID);
	}
	else
	{
		strUrl = "http://testing.qatime.cn/api/v1/live_studio/interactive_courses/{id}/realtime";
		strUrl.replace("{id}", m_CurCourseID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIChatRoom::returnAllMember);
}

void UIChatRoom::RequestMember()
{
	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl = "https://qatime.cn/api/v1/live_studio/courses/{id}/realtime";
		strUrl.replace("{id}", m_CurCourseID);
	}
	else
	{
		strUrl = "http://testing.qatime.cn/api/v1/live_studio/courses/{id}/realtime";
		strUrl.replace("{id}", m_CurCourseID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIChatRoom::returnAllMember);
}

void UIChatRoom::returnAllMember()
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

			// 添加成员成功，才算显示人数
			bool bSuc = AddStudent(pMember->url(), pMember->name(), pMember->accid());
			if (bSuc)
				i++;
	
			//用完之后删除
			delete pMember;
		}


		emit sig_StartLoading();

		m_bPerson = true;

		ResultMsg();

		QueryGroup();
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
		return;
	}
}

// 获取成员
std::vector<personListBuddy*>  UIChatRoom::GetBuddy()
{
	return ui.student_list->GetBuddy();
}

// 是否请求完成员
bool UIChatRoom::IsPerson()
{
	return m_bPerson;
}

// 第一次点击请求之前的消息
void UIChatRoom::ResultMsg()
{
	QString namber = QString::number(m_UnreadCount);
	QString chatid = QString::fromStdString(m_CurChatID);
	if (m_UnreadCount > 50)
		m_UnreadCount = 50;

	long long time = QDateTime::currentMSecsSinceEpoch();
	nim::MsgLog::QueryMsgOnlineAsync(m_CurChatID, nim::kNIMSessionTypeTeam, m_UnreadCount, 0, time, 0, false, true, &UIWindowSet::QueryFirstMsgOnlineCb);
}

void UIChatRoom::SetEnvironmental(bool bType)
{
	m_EnvironmentalTyle = bType;
}

void UIChatRoom::ShowChatMsg(nim::IMMessage pMsg)
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

							personListBuddy* Buddy = NULL;
							Buddy = ui.student_list->findID(QString::fromStdString(ids[i]));
							if (Buddy)
								strName = Buddy->name->text();
							else
								strName = "无名";
						}
					}

					strName += " 加入了群聊";
					m_uitalk->InsertNotice(strName);
				}
			}
			else if (id == nim::kNIMNotificationIdTeamMuteMember)
			{
				QString name;
				std::string obj_account = json[nim::kNIMNotificationKeyData][nim::kNIMNotificationKeyDataId].asString();
				personListBuddy* Buddy = NULL;
				Buddy = ui.student_list->findID(QString::fromStdString(obj_account));
				if (Buddy)
					name = Buddy->name->text();

				bool set_mute = json[nim::kNIMNotificationKeyData]["mute"].asInt() == 1;
				if (set_mute)
					name += "已被禁言";
				else
					name += "已被解除禁言";

				m_uitalk->InsertNotice(name);
			}
		}
		return;
	}

	// 跨天处理
	stepMsgDays(QDateTime::fromMSecsSinceEpoch(pMsg.timetag_));

	// 判断当前过来的消息，是不是此会话窗口
	if (pMsg.type_ == nim::kNIMMessageTypeText)
	{
		qDebug() << "进入此聊天";
		std::string strName = pMsg.readonly_sender_nickname_;
		std::string strContent = pMsg.content_;
		std::string strID = pMsg.sender_accid_;

		QString qTime = QDateTime::fromMSecsSinceEpoch(pMsg.timetag_).toString("hh:mm:ss");// 原型yyyy-MM-dd hh:mm:ss
		QString qName = QString::fromStdString(strName);
		QString qContent = QString::fromStdString(strContent);

		// 聊天头像
		QPixmap* img = NULL;
		personListBuddy* Buddy = NULL;
		Buddy = ui.student_list->findID(QString::fromStdString(strID));
		if (Buddy)
			img = (QPixmap*)Buddy->head->pixmap();

		m_parent->SendStudentBullet(qName, qContent, QString::fromStdString(m_CurChatID));
		if (IsHasFace(qContent))
			m_uitalk->InsertEmoji(img, qName, qTime, qContent,false);
		else
			m_uitalk->InsertChat(img, qName, qTime, qContent,false);
	
	}
	else if (pMsg.type_ == nim::kNIMMessageTypeImage)
	{
		std::string strName = pMsg.readonly_sender_nickname_;
		std::string strContent = pMsg.content_;
		std::string strID = pMsg.sender_accid_;

		QString qTime = QDateTime::fromMSecsSinceEpoch(pMsg.timetag_).toString("hh:mm:ss");// 原型yyyy-MM-dd hh:mm:ss
		QString qName = QString::fromStdString(strName);

		// 聊天头像
		QPixmap* img = NULL;
		personListBuddy* Buddy = NULL;
		Buddy = ui.student_list->findID(QString::fromStdString(strID));
		if (Buddy)
			img = (QPixmap*)Buddy->head->pixmap();

		Json::Value values;
		Json::Reader reader;
		std::string url;
		if (reader.parse(pMsg.attach_, values) && values.isObject())
		{
			url = values["url"].asString();
		}

		m_uitalk->InsertPicUrl(img, qName, qTime, QString::fromStdString(url),"",false);
		m_parent->SendStudentBullet(qName, "[图片消息]", QString::fromStdString(m_CurChatID));
	}
	else if (pMsg.type_ == nim::kNIMMessageTypeAudio)
	{
		std::string path;	// 路径
		QString qduration;	// 语音时长
		std::string sid;	// 会话窗口ID
		std::string msgid;	// 当前消息ID
		Json::Value values;
		Json::Reader reader;
		if (reader.parse(pMsg.attach_, values) && values.isObject())
		{
			path = values["md5"].asString();
			path = m_AudioPath + path;
			sid = pMsg.local_talk_id_;
			msgid = pMsg.client_msg_id_;
			int duration = values["dur"].asUInt();
			qduration = QString::number((duration + 500) / 1000);
			if (qduration.toInt() > 60)
				qduration = "60";
		}

		stepMsgDays(QDateTime::fromMSecsSinceEpoch(pMsg.timetag_));

		std::string strName = pMsg.readonly_sender_nickname_;
		std::string strContent = pMsg.content_;
		std::string strID = pMsg.sender_accid_;

		QString qTime = QDateTime::fromMSecsSinceEpoch(pMsg.timetag_).toString("hh:mm:ss");// 原型yyyy-MM-dd hh:mm:ss
		QString qName = QString::fromStdString(strName);
		// 聊天头像
		QPixmap* img = NULL;
		personListBuddy* Buddy = NULL;
		Buddy = ui.student_list->findID(QString::fromStdString(strID));
		if (Buddy)
			img = (QPixmap*)Buddy->head->pixmap();

		m_uitalk->InsertAudioChat(img, qName, qTime, qduration, path, sid, msgid, false);

		QString audioPath = QString::fromStdString(path);
		QFile file(audioPath);
		if (!file.exists())
		{
			nim::NOS::FetchMedia(pMsg, nim::NOS::DownloadMediaCallback(), nim::NOS::ProgressCallback());
		}

		m_parent->SendStudentBullet(qName, "[语音消息]", QString::fromStdString(m_CurChatID));
	}

	m_switchTime = false;
}

QString UIChatRoom::parse(QString str)
{
	QString encode;
	int iCount = str.count();
	for (int i = 0; i < iCount; i++)
	{
		QString s = str.at(i);
		if (s == "~")
			s = "%7E";
		else if (s == "`")
			s = "%60";
		else if (s == "!")
			s = "%21";
		else if (s == "@")
			s = "%40";
		else if (s == "#")
			s = "%23";
		else if (s == "$")
			s = "%24";
		else if (s == "%")
			s = "%25";
		else if (s == "^")
			s = "%5E";
		else if (s == "&")
			s = "%26";
		else if (s == "*")
			s = "%2A";
		else if (s == "(")
			s = "%28";
		else if (s == ")")
			s = "%29";
		else if (s == "_")
			s = "%5F";
		else if (s == "+")
			s = "%2B";
		else if (s == "=")
			s = "%3D";
		else if (s == "\\")
			s = "%5C";
		else if (s == "|")
			s = "%7C";
		else if (s == "[")
			s = "%5B";
		else if (s == "]")
			s = "%5D";
		else if (s == ":")
			s = "%3A";
		else if (s == ";")
			s = "%3B";
		else if (s == "'")
			s = "%27";
		else if (s == "\"")
			s = "%22";
		else if (s == ",")
			s = "%2C";
		else if (s == "<")
			s = "%3C";
		else if (s == ".")
			s = "%2E";
		else if (s == ">")
			s = "%3E";
		else if (s == "/")
			s = "%2F";
		else if (s == "?")
			s = "%3F";

		encode += s;
	}
	return encode;
}

void UIChatRoom::slot_eidtClear()
{
	ui.textEdit->clear();
//	m_uitalk->ScrollDown();
}