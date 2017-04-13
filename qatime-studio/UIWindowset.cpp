#include "UIWindowset.h"
#include <QPainter>
#include "UIMessageBox.h"
#include "member.h"
#include <QToolTip>

#define MAINWINDOW_X_MARGIN 10
#define MAINWINDOW_Y_MARGIN 10
#define MAINWINDOW_TITLE_HEIGHT 0

UIWindowSet::UIWindowSet(QWidget *parent)
	: QWidget(parent)
	, m_parent(NULL)
	, m_EnvironmentalTyle(true)
	, m_VideoInfo(NULL)
	, m_CameraInfo(NULL)
	, m_bHasCamera(false)
	, m_NoticeWnd(NULL)
	, m_VideoChangeInfo(NULL)
	, m_AudioChangeInfo(NULL)
	, m_curTags(NULL)
	, m_curChatRoom(NULL)
	, m_EnumStatus(CameraStatusTeaching)
	, m_CountTimer(NULL)
	, m_iTimerCount(0)
	, m_LiveLesson(NULL)
{
	ui.setupUi(this);
	setAutoFillBackground(true);
	QPalette p = palette();
	p.setColor(QPalette::Window, QColor(255, 255, 255));
	setPalette(p);

	ui.logo_pushButton->installEventFilter(this);
	ui.title_pushButton->installEventFilter(this);
	ui.whiteboard_widget->installEventFilter(this);
	initFont();

	connect(ui.min_pushButton, SIGNAL(clicked()), this, SLOT(MinDialog()));
	connect(ui.max_pushButton, SIGNAL(clicked()), this, SLOT(MaxDialog()));
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.change_pushButton, SIGNAL(clicked(bool)), this, SLOT(clickChange(bool)));
	connect(ui.Live_pushBtn, SIGNAL(clicked()), this, SLOT(clickLive()));
	connect(ui.time_label, SIGNAL(clicked()), this, SLOT(clickLive()));
	connect(ui.notice_pushButton, SIGNAL(clicked()), this, SLOT(clickNotice()));
	connect(ui.course_pushButton, SIGNAL(clicked()), this, SLOT(clickCourse()));
	connect(ui.person_pushButton, SIGNAL(clicked()), this, SLOT(clickPerson()));
	connect(ui.lesson_pushButton, SIGNAL(clicked()), this, SLOT(clickLesson()));
	connect(ui.Audio_checkBox, SIGNAL(stateChanged(int)), this, SLOT(AudioStatus(int)));
	connect(ui.Audio_pushButton, SIGNAL(clicked()), this, SLOT(clickAudioParam()));
	connect(ui.video_checkBox, SIGNAL(stateChanged(int)), this, SLOT(VideoStatus(int)));
	connect(ui.video_pushButton, SIGNAL(clicked()), this, SLOT(clickVideoParam()));
	connect(ui.ratio_pushButton, SIGNAL(clicked()), this, SLOT(clickRatioParam()));
	connect(ui.Bullet_checkBox, SIGNAL(stateChanged(int)), this, SLOT(BulletStatus(int)));
	connect(ui.Bullet_pushButton, SIGNAL(clicked()), this, SLOT(clickBulletParam()));
	connect(this, SIGNAL(sig_Modle(bool)), this, SLOT(slots_Modle(bool)));

	m_RatioChangeInfo = new UIRatio(this);
	m_RatioChangeInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_RatioChangeInfo->setVideoChange(this);
	m_RatioChangeInfo->hide();

	m_AudioChangeInfo = new UIAudioChange(this);
	m_AudioChangeInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_AudioChangeInfo->setAudioChange(this);
	m_AudioChangeInfo->hide();

	m_VideoChangeInfo = new UIVideoChange(this);
	m_VideoChangeInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_VideoChangeInfo->setVideoChange(this);
	m_VideoChangeInfo->hide();

	m_VideoInfo = new UIVideo(ui.full_widget);
	m_VideoInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_VideoInfo->SetMainWnd(this);
	m_VideoInfo->InitDeviceParam();
	ui.horizontalLayout_10->addWidget(m_VideoInfo);
	m_VideoInfo->show();

	m_CameraInfo = new UICamera(ui.camera1_widget);//camera1_widget
	m_CameraInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_CameraInfo->SetMainWnd(this);
	ui.horizontalLayout_17->addWidget(m_CameraInfo);//17
	m_CameraInfo->show();

	m_NoticeWnd = new UINoticeWnd(this);
	m_NoticeWnd->setParentBtn(this);
	m_NoticeWnd->hide();

	m_CourseWnd = new UICourseWnd(this);
	m_CourseWnd->setParentBtn(this);
	m_CourseWnd->hide();

	m_PersonWnd = new UIPersonWnd(this);
	m_PersonWnd->setParentBtn(this);
	m_PersonWnd->hide();
	connect(m_PersonWnd, SIGNAL(sig_stateChange(bool, QString, QString)), this, SLOT(slot_stateChange(bool, QString, QString)));

	m_LessonWnd = new UILessonList(this);
	m_LessonWnd->setParentBtn(this);
	m_LessonWnd->hide();

	m_LiveLesson = new UILiveLessonWnd(this);
	m_LiveLesson->setWindowFlags(Qt::FramelessWindowHint);
	connect(m_LiveLesson, SIGNAL(sig_PullStreaming(QString, QString, QString, QString, QString)), this, SLOT(slot_PullStreaming(QString, QString, QString, QString, QString)));
	connect(m_LiveLesson, SIGNAL(sig_changeLessonStatus(QString, QString)), this, SLOT(slot_changeLessonStatus(QString, QString)));
	m_LiveLesson->hide();

	m_BulletParamInfo = new UIBulletParam(this);
	m_BulletParamInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_BulletParamInfo->setParent(this);
	m_BulletParamInfo->hide();

	m_BulletScreen = new UIBulletScreen();
	m_BulletScreen->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	m_BulletScreen->setMainWindow(this);
	m_BulletScreen->hide();
	SetWindowPos((HWND)m_BulletScreen->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	ui.live1v1_widget->setVisible(false);
	ChangeBtnStyle(false);

	// 设置麦克风样式
	ui.Audio_checkBox->setStyleSheet("QCheckBox::indicator{width: 24px;height: 19px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/mic_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/mic_close.png);}");
	ui.Audio_pushButton->setStyleSheet("border-image:url(./images/Arrow.png);");

	// 设置摄像头视频源样式
	ui.video_checkBox->setStyleSheet("QCheckBox::indicator{width: 29px;height: 19px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/camera_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/camera_close.png);}");
	ui.video_pushButton->setStyleSheet("border-image:url(./images/Arrow.png);");

	// 设置弹幕样式
	ui.Bullet_checkBox->setStyleSheet("QCheckBox::indicator{width: 26px;height: 19px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/bullet_close.png);}"
		"QCheckBox::indicator:checked{image: url(./images/bullet_open.png);}");
	ui.Bullet_pushButton->setStyleSheet("border-image:url(./images/Arrow.png);");

	// 计时器 改变直播计时时间
	m_CountTimer = new QTimer(this);
	connect(m_CountTimer, SIGNAL(timeout()), this, SLOT(slot_onCountTimeout()));

	// 摄像头延迟1秒推流，避免和全屏桌面一起推流启冲突
	m_tempTimers = new QTimer(this);
	connect(m_tempTimers, SIGNAL(timeout()), this, SLOT(slot_onTempTimeout()));
	
	// 直播按钮
	ui.Live_pushBtn->setText("开始直播");
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");

	m_LiveStatusManager = new LiveStatusManager(this);
	m_LiveStatusManager->setMainWindow(this);

	m_ScreenTip = new UIScreenTip();
	m_ScreenTip->setWindowFlags(Qt::FramelessWindowHint);
	m_ScreenTip->hide();
	SetWindowPos((HWND)m_ScreenTip->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

UIWindowSet::~UIWindowSet()
{
	if (m_VideoInfo)
	{
		delete m_VideoInfo;
		m_VideoInfo = NULL;
	}

	if (m_CameraInfo)
	{
		delete m_CameraInfo;
		m_CameraInfo = NULL;
	}

	if (m_BulletScreen)
	{
		delete m_BulletScreen;
		m_BulletScreen = NULL;
	}
}


void UIWindowSet::MinDialog()
{
	showMinimized();
}

void UIWindowSet::CloseDialog()
{
	if (m_VideoInfo->IsCurrentLiving())
	{
		m_ScreenTip->setErrorTip("请先结束直播，再关闭窗口！");
		return;
	}

	if (m_vecTags.size() > 0)
	{
		std::vector<UITags*>::iterator it;
		for (it = m_vecTags.begin(); it != m_vecTags.end(); it++)
		{
			UITags* tags = *it;
			tags->GetRoom()->setVisible(false);
			tags->close();
			delete tags;
		}
	}
	m_vecTags.clear();
	m_mapTags.clear();
	m_curTags = NULL;
	m_curChatRoom = NULL;
	// 停止采集和互动预览
	m_CameraInfo->StopCaptureVideo();
	// 课程名置空
	ui.lesson_label->setText("");
	// 退出弹幕
	emit ui.Bullet_checkBox->stateChanged(0);
	CloseBullet();
	hide();
}

void UIWindowSet::MaxDialog()
{
	if (this->isMaximized())
	{
		showNormal();
		return;
	}
	showMaximized();
}

bool UIWindowSet::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	if ("windows_generic_MSG" == eventType)
	{
		PMSG pMsg = static_cast<PMSG>(message);
		switch (pMsg->message)
		{
		case WM_NCLBUTTONDBLCLK:
		{
			int y = GET_Y_LPARAM(pMsg->lParam) - this->frameGeometry().y();
			if (y > MAINWINDOW_Y_MARGIN && y <= MAINWINDOW_TITLE_HEIGHT)
			{
				MaxDialog();
				return true;
			}
		}
		break;
		case WM_NCHITTEST:
		{
			int x = GET_X_LPARAM(pMsg->lParam) - this->frameGeometry().x();
			int y = GET_Y_LPARAM(pMsg->lParam) - this->frameGeometry().y();

			int xflag = (x <= MAINWINDOW_X_MARGIN) ? -1 : ((x < this->width() - MAINWINDOW_X_MARGIN) ? 0 : 1);
			int yflag = (y <= MAINWINDOW_Y_MARGIN) ? -1 : ((y < this->height() - MAINWINDOW_Y_MARGIN) ? 0 : 1);

			if (-1 == xflag && -1 == yflag)
			{
				*result = HTTOPLEFT;
			}
			else if (-1 == xflag && 0 == yflag)
			{
				*result = HTLEFT;
			}
			else if (-1 == xflag && 1 == yflag)
			{
				*result = HTBOTTOMLEFT;
			}
			else if (0 == xflag && -1 == yflag)
			{
				*result = HTTOP;
			}
			else if (0 == xflag && 0 == yflag)
			{
				*result = HTCLIENT;
			}
			else if (0 == xflag && 1 == yflag)
			{
				*result = HTBOTTOM;
			}
			else if (1 == xflag && -1 == yflag)
			{
				*result = HTTOPRIGHT;
			}
			else if (1 == xflag && 0 == yflag)
			{
				*result = HTRIGHT;
			}
			else if (1 == xflag && 1 == yflag)
			{
				*result = HTBOTTOMRIGHT;
			}
			if (0 == xflag && y > MAINWINDOW_Y_MARGIN && y <= MAINWINDOW_TITLE_HEIGHT)
			{
				*result = HTCAPTION;
			}
			return true;
		}
		break;
		default:
			return false;
			break;
		}
	}
	return false;
}

void UIWindowSet::initFont()
{
	QFont font = ui.notice_pushButton->font();
	font.setPixelSize(15);
	ui.notice_pushButton->setFont(font);

	font = ui.course_pushButton->font();
	font.setPixelSize(15);
	ui.course_pushButton->setFont(font);

	font = ui.lesson_pushButton->font();
	font.setPixelSize(15);
	ui.lesson_pushButton->setFont(font);

	font = ui.person_pushButton->font();
	font.setPixelSize(15);
	ui.person_pushButton->setFont(font);

	font = ui.change_pushButton->font();
	font.setPixelSize(15);
	ui.change_pushButton->setFont(font);

	font = ui.return_pushButton->font();
	font.setPixelSize(15);
	ui.return_pushButton->setFont(font);

	font = ui.ratio_pushButton->font();
	font.setPixelSize(14);
	ui.ratio_pushButton->setFont(font);
	ui.ratio_pushButton->setText("高清");

	font = ui.lesson_label->font();
	font.setPixelSize(15);
	ui.lesson_label->setFont(font);

	ui.lessonpic_label->setStyleSheet("border-image: url(./images/lessonPic.png);");
	ui.lesson_widget->setVisible(false);
	ui.camera_widget->setVisible(false);
	ui.whiteboard_widget->setVisible(false);
	ui.time_label->setVisible(false);

	ui.white_label->setVisible(false);
	ui.return_pushButton->setVisible(false);
}

void UIWindowSet::setMainWindow(UIMainNewWindow* parent)
{
	m_parent = parent;
}

void UIWindowSet::SetEnvironmental(bool bType)
{
	m_EnvironmentalTyle = bType;
	m_LiveStatusManager->SetEnvironmental(bType);
}

void UIWindowSet::SetToken(QString token)
{
	m_Token = token;
}

void UIWindowSet::setTeacherID(QString id, QString name)
{
	m_teacherID = id;
	m_teacherName = name;
}

void UIWindowSet::setAccid(QString accid)
{
	m_accid = accid;
}

void UIWindowSet::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	QPainter painter(this);

	QColor color(5, 157, 213);
	for (int i = 0; i < 45; i++)
		path.addRect(0, i, this->width() - 1, this->height() - i - 1);

	painter.setPen(color);
	painter.drawPath(path);
}

// 拖动标题做的处理
bool UIWindowSet::eventFilter(QObject *target, QEvent *event)
{
	if (target == ui.logo_pushButton)
	{
		QMouseEvent* pMe = static_cast<QMouseEvent*>(event);
		if (event->type() == QEvent::MouseButtonPress)
		{
			m_startPos = pMe->globalPos();
			m_WndCurPos = this->pos();
		}
		else if (event->type() == QEvent::MouseMove)
		{
			m_clickPos = pMe->globalPos();
			this->move(m_WndCurPos + (m_clickPos - m_startPos));
		}
		return false;
	}
	else if (target == ui.title_pushButton)
	{
		QMouseEvent* pMe = static_cast<QMouseEvent*>(event);
		if (event->type() == QEvent::MouseButtonPress)
		{
			m_startPos = pMe->globalPos();
			m_WndCurPos = this->pos();
		}
		else if (event->type() == QEvent::MouseMove)
		{
			m_clickPos = pMe->globalPos();
			this->move(m_WndCurPos + (m_clickPos - m_startPos));
		}
		return false;
	}
	return false;
}

void UIWindowSet::AddChatRoom(QString chatID, QString courseid, QString teacherid, QString token, QString studentName, std::string strCurAudioPath, QString name, int UnreadCount, QString status, bool b1v1Lesson)
{
	show();
	if (isMinimized())
		showNormal();
	SetWindowPos((HWND)winId(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	QMap<QString, UITags*>::iterator it;
	it = m_mapTags.find(chatID);
	if (it == m_mapTags.end())
	{
		// 先做下限制
		if (m_vecTags.size() == 10)
		{
			CMessageBox::showMessage(QString("答疑时间"), QString("最多能添加10个辅导班！！"), QString("确定"), QString());
			return;
		}
	}

	if (IsHasTag(chatID, status))
		return;

	UIChatRoom* chatRoom = IsHasRoom(chatID);
	if (chatRoom == NULL)
	{
		chatRoom = new UIChatRoom(ui.chat_widget);
		chatRoom->setWindowFlags(Qt::FramelessWindowHint);
		chatRoom->setMainWindow(this);
		chatRoom->SetEnvironmental(m_EnvironmentalTyle);
		chatRoom->setCurChatID(chatID, courseid, teacherid, token, studentName, m_accid, UnreadCount);
		chatRoom->SetCurAudioPath(strCurAudioPath);
		chatRoom->InitAudioCallBack();
		ui.horizontalLayout_6->addWidget(chatRoom);
		m_vecChatRoom.push_back(chatRoom);
		m_mapChatRoom.insert(chatID, chatRoom);
		chatRoom->show();
//		chatRoom->setEditFocus();
	}

	AddTag(chatID, name, courseid, true, chatRoom, status, b1v1Lesson);
}

bool UIWindowSet::IsHasTag(QString chatID, QString status)
{
	bool bHas = false;
	if (m_vecTags.size() > 0)
	{
		std::vector<UITags*>::iterator it;
		for (it = m_vecTags.begin(); it != m_vecTags.end(); it++)
		{
			UITags* tags = *it;
			if (tags->ChatID() == chatID)
			{
				tags->setStyle(true);
				tags->GetRoom()->setVisible(true);
				m_curTags = tags;
				m_curChatRoom = tags->GetRoom();
				bHas = true;

				if (m_curTags->IsModle())
					emit sig_Modle(true);
				else
					emit sig_Modle(false);
			}
			else
			{
				// 隐藏没选中的
				tags->setStyle(false);
				tags->GetRoom()->setVisible(false);
			}
		}
	}

	return bHas;
}

UIChatRoom* UIWindowSet::IsHasRoom(QString chatID)
{
	if (m_vecChatRoom.size() > 0)
	{
		std::vector<UIChatRoom*>::iterator it;
		for (it = m_vecChatRoom.begin(); it != m_vecChatRoom.end(); it++)
		{
			UIChatRoom* room = *it;
			if (QString::fromStdString(room->GetCurChatID()) == chatID)
			{
				return room;
			}
		}
	}

	return NULL;
}

void UIWindowSet::AddTag(QString chatID, QString name, QString ID, bool sel, UIChatRoom* room, QString status, bool b1v1Lesson)
{
 	UITags* tag = new UITags(ui.tags_widget);
 	tag->setMaximumWidth(200);
 	tag->SetCourseNameAndID(name, ID, chatID);
	tag->SetRoom(room);
 	tag->setStyle(sel);
	tag->set1v1Lesson(b1v1Lesson);
 	tag->show();
	connect(tag, SIGNAL(clickTag(UITags*)), this, SLOT(clickTag(UITags*)));
	connect(tag, SIGNAL(DeleteTag(UITags*)), this, SLOT(DeleteTag(UITags*)));
	ui.horizontalLayout_3->addWidget(tag);
 
	m_vecTags.push_back(tag);
	m_mapTags.insert(tag->ChatID(), tag);
	titleButtonWidth();
	room->show();

 	m_curTags = tag;
	m_curChatRoom = room;
	
	m_curTags->setModle(false);
	emit sig_Modle(false);
}

// 可拖动标题的宽度
void UIWindowSet::titleButtonWidth()
{
	int iCount = m_vecTags.size();
	int iWidth = iCount * 200 + iCount * 6;
	int iTagWidth = width() - ui.logo_pushButton->width() - ui.widget->width() - 48;
	if (iWidth >= iTagWidth)
	{
		ui.tags_widget->setMaximumWidth(iTagWidth);
	}
	else
	{
		ui.tags_widget->setMaximumWidth(iWidth);
	}
}

QPixmap UIWindowSet::TeacherPhotoPixmap()
{
	if (m_parent)
		return m_parent->TeacherPhotoPixmap();

	return NULL;
}

void UIWindowSet::DeleteTag(UITags* tag)
{
	if (m_curTags->IsModle() && m_VideoInfo->IsCurrentLiving())
	{
		m_ScreenTip->setErrorTip("请先结束直播，再关闭窗口！");
		return;
	}

	QMap<QString, UITags*>::iterator iter;
	UITags* item = NULL;
	iter = m_mapTags.find(tag->ChatID());
	if (iter != m_mapTags.end())
	{
		m_mapTags.erase(iter);
	}

	if (m_vecTags.size() > 0)
	{
		std::vector<UITags*>::iterator it;
		for (it = m_vecTags.begin(); it != m_vecTags.end(); it++)
		{
			UITags* tags = *it;
			bool bSel = false;
			// 判断是当前会话窗口
			if (tag == tags)
			{
				bSel = tags->IsSelect();
				tags->GetRoom()->setVisible(false);

				m_vecTags.erase(it);
				tags->close();
				delete tags;
				titleButtonWidth();

				// 关闭的窗口，是否是当前显示的
				if (bSel)
					AgainSelectTag();

				return;
			}
		}
	}
}

void UIWindowSet::AgainSelectTag()
{
	if (m_vecTags.size() == 0)
	{
		m_curTags = NULL;
		m_curChatRoom = NULL;
		//关闭摄像头采集，包括互动直播、直播器
		if (m_CameraInfo)
			m_CameraInfo->StopCaptureVideo();
		// 课程名置空
		ui.lesson_label->setText("");
		// 退出弹幕
		emit ui.Bullet_checkBox->stateChanged(0);
		CloseBullet();
		hide();
	}
	else
	{
		UITags* tag = m_vecTags.front();
		clickTag(tag);
	}
}

bool UIWindowSet::ReceiverMsg(nim::IMMessage* pIMsg)
{
	QString strChatID = QString::fromStdString(pIMsg->local_talk_id_.c_str());
	QMap<QString, UIChatRoom*>::Iterator it;
	UIChatRoom* room = NULL;
	it = m_mapChatRoom.find(strChatID);
	if (it != m_mapChatRoom.end())
	{
		room = *it;
		room->ReceiverMsg(pIMsg);
	}

	QMap<QString, UITags*>::Iterator itTag;
	itTag = m_mapTags.find(strChatID);
	if (itTag != m_mapTags.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UIWindowSet::ReceiverRecordMsg(nim::QueryMsglogResult* pIMsg)
{
	if (m_vecChatRoom.size() > 0)
	{
		std::vector<UIChatRoom*>::iterator it;
		for (it = m_vecChatRoom.begin(); it != m_vecChatRoom.end(); it++)
		{
			UIChatRoom* room = *it;

			// 判断历史消息>0， 且是当前会话窗口
			if (pIMsg->msglogs_.size() > 0 && strcmp(pIMsg->msglogs_.front().local_talk_id_.c_str(), room->GetCurChatID().c_str()) == 0)
			{
				room->ReceiverRecordMsg(pIMsg);
				return;
			}
		}
	}
}

void UIWindowSet::ReceiverChatMsg(nim::IMMessage* pIMsg)
{
	QString strChatID = QString::fromStdString(pIMsg->local_talk_id_.c_str());
	QMap<QString, UIChatRoom*>::Iterator it;
	UIChatRoom* room = NULL;
	it = m_mapChatRoom.find(strChatID);
	if (it != m_mapChatRoom.end())
	{
		room = *it;
		room->ShowChatMsg(*pIMsg);
	}
}

void UIWindowSet::ReceiverLoginMsg(nim::LoginRes* pLogMsg)
{
	if (m_vecChatRoom.size() > 0)
	{
		std::vector<UIChatRoom*>::iterator it;
		for (it = m_vecChatRoom.begin(); it != m_vecChatRoom.end(); it++)
		{
			UIChatRoom* room = *it;
			room->ReceiverLoginMsg(pLogMsg);
		}
	}
}

void UIWindowSet::ReceiverMemberMsg(std::string sid, std::list<nim::TeamMemberProperty>* pMemberMsg)
{
	if (m_vecChatRoom.size() > 0)
	{
		std::vector<UIChatRoom*>::iterator it;
		for (it = m_vecChatRoom.begin(); it != m_vecChatRoom.end(); it++)
		{
			UIChatRoom* room = *it;
			// 判断是当前会话窗口
			if (strcmp(sid.c_str(), room->GetCurChatID().c_str()) == 0)
			{
				room->ReceiverMemberMsg(pMemberMsg);
				return;
			}
		}
	}
}

void UIWindowSet::SendStatus(nim::SendMessageArc* arcNew)
{
	if (m_vecChatRoom.size() > 0)
	{
		std::vector<UIChatRoom*>::iterator it;
		for (it = m_vecChatRoom.begin(); it != m_vecChatRoom.end(); it++)
		{
			UIChatRoom* room = *it;
			// 判断是当前会话窗口
			if (strcmp(arcNew->talk_id_.c_str(), room->GetCurChatID().c_str()) == 0 && arcNew->rescode_ != 200)
			{
				room->SendStatus(arcNew);
				return;
			}
		}
	}
}

void UIWindowSet::OnStopPlayAudio(std::string sid, char* msgid)
{
	if (m_vecChatRoom.size() > 0)
	{
		std::vector<UIChatRoom*>::iterator it;
		for (it = m_vecChatRoom.begin(); it != m_vecChatRoom.end(); it++)
		{
			UIChatRoom* room = *it;
			// 判断是当前会话窗口
			if (strcmp(sid.c_str(), room->GetCurChatID().c_str()) == 0)
			{
				room->OnStopPlayAudio(msgid);
				return;
			}
		}
	}
}

void UIWindowSet::clickTag(UITags* tag)
{
	if (m_vecTags.size() > 0)
	{
		std::vector<UITags*>::iterator it;
		for (it = m_vecTags.begin(); it != m_vecTags.end(); it++)
		{
			UITags* tags = *it;
			if (tags == tag)
			{
				tags->setStyle(true);
				tags->GetRoom()->setVisible(true);
				m_curTags = tags;
				m_curChatRoom = tags->GetRoom();
			}
			else
			{
				// 隐藏没选中的
				tags->setStyle(false);
				tags->GetRoom()->setVisible(false);
			}
		}
	}

	if (m_NoticeWnd){
		ui.notice_pushButton->setStyleSheet("color:rgb(0,0,0);border-image: url(./images/AuxiliaryPanelBack.png);");//background-color: rgb(255, 255, 255);
		m_NoticeWnd->hide();
	}

	if (m_CourseWnd){
		ui.course_pushButton->setStyleSheet("color:rgb(0,0,0);border-image: url(./images/AuxiliaryPanelBack.png);");
		m_CourseWnd->hide();
	}

	if (m_PersonWnd){
		ui.person_pushButton->setStyleSheet("color:rgb(0,0,0);border-image: url(./images/AuxiliaryPanelBack.png);");
		m_PersonWnd->hide();
	}

	if (m_LessonWnd){
		ui.lesson_pushButton->setStyleSheet("color:rgb(0,0,0);border-image: url(./images/AuxiliaryPanelBack.png);");
		m_LessonWnd->hide();
	}

	if (m_curTags->IsModle())
		emit sig_Modle(true);
	else
		emit sig_Modle(false);
}

void UIWindowSet::ChangeBtnStyle(bool bLive)
{
	if (bLive)
	{
		ui.change_pushButton->setStyleSheet("QPushButton{border-image:url(./images/liveChange_nor.png);}"
			"QPushButton:pressed{border-image:url(./images/liveChange_hover.png);}");
	}
	else
	{
		ui.change_pushButton->setStyleSheet("QPushButton{border-image:url(./images/noliveChange_nor.png);}"
			"QPushButton:pressed{border-image:url(./images/noliveChange_hover.png);}");
	}
}

void UIWindowSet::clickLive()
{
	if (m_VideoInfo)
	{
		bool bLiving = m_VideoInfo->IsCurrentLiving();
		if (bLiving)
		{
			int iStatus = CMessageBox::showMessage(
				QString("答疑时间"),
				QString("是否关闭当前直播！"),
				QString("确定"),
				QString("取消"));
			if (iStatus == 1)
			{
				ui.Live_pushBtn->setText("开始直播");
				ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");

				m_VideoInfo->StopLiveVideo();
				if (IsHasCamera() && m_EnumStatus != CameraStatusClose)
					m_CameraInfo->StopLiveVideo();

				m_LiveStatusManager->SendStopLiveHttpMsg();

				if (m_CountTimer->isActive())
				{
					m_CountTimer->stop();					// 停止计时
					m_iTimerCount = 0;						// 重置秒数
					ui.time_label->setVisible(false);		// 隐藏
					ui.time_label->setText("00:00:00");		// 重置时间
				}
			}
			else
				return;
		}
		else
		{
			if (m_curTags == NULL)
				return;
			
			m_LiveLesson->DeleteItem();
			m_LiveLesson->setCourseID(m_curTags->CourseID());
			m_LiveLesson->move(width()/2 - m_LiveLesson->width()/2,height()/2-m_LiveLesson->height()/2);
			m_LiveLesson->show();
		}
	}
}

// 选中课程开始直播
void UIWindowSet::slot_PullStreaming(QString id, QString courseid, QString boardurl, QString cameraurl, QString name)
{
	// 推白板
	m_lessonName = name;
	ui.lesson_label->setText(m_lessonName);
	m_lessonid = id;
	m_boardUrl = boardurl;
	m_cameraUrl = cameraurl;
//	m_boardUrl = "rtmp://pa0a19f55.live.126.net/live/2794c854398f4d05934157e05e2fe419?wsSecret=16c5154fb843f7b7d2819554d8d3aa94&wsTime=1480648811";
//	m_cameraUrl = "rtmp://pa0a19f55.live.126.net/live/0ca7943afaa340c9a7c1a8baa5afac97?wsSecret=f49d13a6ab68601884b5b71487ff51e1&wsTime=1480648749";
	m_VideoInfo->setPlugFlowUrl(m_boardUrl);
	m_VideoInfo->StartLiveVideo();
}

void UIWindowSet::slot_changeLessonStatus(QString id, QString status)
{
	m_parent->changeLessonStatus(id,status);
}

// 更新开始直播按钮、计时器等状态
void UIWindowSet::startLiveStream()
{
	m_CountTimer->start(1000);
	ui.time_label->setVisible(true);
	ui.Live_pushBtn->setText("结束直播");
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: red;border-radius: 5px; border: 2px solid red;}");
}

void UIWindowSet::slot_onCountTimeout()
{
	m_iTimerCount++;
	QString str = QString().sprintf("%02d:%02d:%02d", m_iTimerCount / 3600, m_iTimerCount % 3600 / 60, m_iTimerCount % 60);
	ui.time_label->setText(str);
}

void UIWindowSet::clickChange(bool checked)
{
	if (m_vecTags.size() > 0)
	{
		std::vector<UITags*>::iterator it;
		for (it = m_vecTags.begin(); it != m_vecTags.end(); it++)
		{
			UITags* tags = *it;
			if (tags->IsModle() && tags != m_curTags)
			{ 
				m_ScreenTip->setErrorTip(" 进入失败！请确保其它课程已离开\n直播室后再进入！");
				return;
			}
			else if (tags == m_curTags && tags->IsModle())
			{
				if (IsLiving())
					return;
			}
		}
	}

	if (ui.camera_widget->isVisible())
	{
		ui.white_label->setVisible(false);
		ui.camera_widget->setVisible(false);
		ui.whiteboard_widget->setVisible(false);
		ui.lesson_widget->setVisible(false);
		ui.chatcamera_widget->setMaximumWidth(3000);

		m_CameraInfo->StopCaptureVideo();

		if (m_curTags)
			m_curTags->setModle(false);

		ChangeBtnStyle(false);
		// 课程名置空
		ui.lesson_label->setText("");
		// 退出弹幕
		emit ui.Bullet_checkBox->stateChanged(0);
		CloseBullet();
	}
	else
	{
		ui.white_label->setVisible(true);
		ui.camera_widget->setVisible(true);
		ui.whiteboard_widget->setVisible(true);
		ui.chatcamera_widget->setMaximumWidth(300);
		ui.lesson_widget->setVisible(true);
// 		InitBoardView();

		if (m_curTags)
			m_curTags->setModle(true);

		ChangeBtnStyle(true);
		m_CameraInfo->StartLiveVideo();
// 		QueryLiveInfo();
	}
}

void UIWindowSet::slots_Modle(bool bModle)
{
	if (!bModle)
	{
		ui.white_label->setVisible(false);
		ui.camera_widget->setVisible(false);
		ui.whiteboard_widget->setVisible(false);
		ui.chatcamera_widget->setMaximumWidth(3000);
		ui.lesson_widget->setVisible(false);
//		关闭摄像头（直播/互动）
//		PlayLive("", "");

		ChangeBtnStyle(false);
	}
	else
	{
		ui.white_label->setVisible(true);
		ui.camera_widget->setVisible(true);
		ui.whiteboard_widget->setVisible(true);
		ui.chatcamera_widget->setMaximumWidth(300);
		ui.lesson_widget->setVisible(true);
//		打开摄像头（直播/互动）
// 		InitBoardView();
// 		QueryLiveInfo();
		ChangeBtnStyle(true);
	}
}

bool UIWindowSet::IsHasFoucs(BTN_TYPE type)
{
	QPoint pt = mapFromGlobal(QCursor::pos());
	QPushButton* btn = NULL;
	switch (type)
	{
	case UIWindowSet::NoticeWnd:
	{
		btn = ui.notice_pushButton;
		break;
	}
	case UIWindowSet::CourseWnd:
	{
		btn = ui.course_pushButton;
		break;
	}
	case UIWindowSet::PersonWnd:
	{
		btn = ui.person_pushButton;
		break;
	}
	case UIWindowSet::LessonWnd:
	{
		btn = ui.lesson_pushButton;
		break;
	}
	default:
		break;
	}

	QRect rc = btn->geometry();
	rc.setY(rc.y() + ui.title_widget->geometry().height() + 5);
	rc.setHeight(rc.height() + ui.title_widget->geometry().height() + 5);
	if (rc.contains(pt))
		return true;
	else
		return false;
}

QPushButton*	 UIWindowSet::GetNoticeBtn()
{
	return ui.notice_pushButton;
}
QPushButton*	 UIWindowSet::GetLessonBtn()
{
	return ui.lesson_pushButton;
}
QPushButton*	 UIWindowSet::GetPersonBtn()
{
	return ui.person_pushButton;
}
QPushButton*	 UIWindowSet::GetCourseBtn()
{
	return ui.course_pushButton;
}

void UIWindowSet::clickNotice()
{
	if (m_NoticeWnd)
	{
		if (m_NoticeWnd->isVisible())
		{
			ui.notice_pushButton->setStyleSheet("color:rgb(0,0,0);border-image: url(./images/AuxiliaryPanelBack.png);");
			m_NoticeWnd->setVisible(false);
			return;
		}

		m_NoticeWnd->initDate();
		ui.notice_pushButton->setStyleSheet("color:rgb(255, 255, 255);border-image: url(./images/Bullet.png);");//background-color: rgb(5,157,213);
		m_NoticeWnd->move(4, 74);
		m_NoticeWnd->DeleteNotice();
		m_NoticeWnd->show();
		m_NoticeWnd->setFocus();
		QueryNotice();
	}
}

void UIWindowSet::QueryNotice()
{
	QString strCourseID = "";
	if (m_curTags)
		strCourseID = m_curTags->CourseID();
	else
		return;

	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl = "https://qatime.cn/api/v1/live_studio/courses/{id}/realtime";
		strUrl.replace("{id}", strCourseID);
	}
	else
	{
		strUrl = "http://testing.qatime.cn/api/v1/live_studio/courses/{id}/realtime";
		strUrl.replace("{id}", strCourseID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	request.setRawHeader("Remember-Token", m_Token.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIWindowSet::returnNotice);
}

void UIWindowSet::returnNotice()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1 && data.contains("members"))
	{
		// 群公告信息
		QJsonArray announcements = data["announcements"].toArray();
		foreach(const QJsonValue & value, announcements)
		{
			QJsonObject obj = value.toObject();
			YXMember *announcements = new YXMember();
			announcements->readJsonToAnnouncement(value.toObject());

			if (m_NoticeWnd)
				m_NoticeWnd->AddNotic(announcements->announcement(), announcements->announcementTime());

			//用完之后删除
			delete announcements;
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		return;
	}
}

void UIWindowSet::OnSendAnnouncements(QString text)
{
	if (m_curChatRoom)
	{
		m_curChatRoom->OnSendAnnouncements(text);
	}
}

void UIWindowSet::clickCourse()
{
	if (m_CourseWnd)
	{
		if (m_CourseWnd->isVisible())
		{
			ui.course_pushButton->setStyleSheet("color:rgb(0,0,0);border-image: url(./images/AuxiliaryPanelBack.png);");
			m_CourseWnd->setVisible(false);
			return;
		}

		ui.course_pushButton->setStyleSheet("color:rgb(255, 255, 255);border-image: url(./images/Bullet.png);");
		m_CourseWnd->move(4, 74);
		m_CourseWnd->show();
		m_CourseWnd->setFocus();

		QueryCourse();
	}
}

void UIWindowSet::QueryCourse()
{
	QString strCourseID = "";
	if (m_curTags)
		strCourseID = m_curTags->CourseID();
	else
		return;

	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		if (m_curTags->Is1v1Lesson())
		{
			strUrl = "http://qatime.cn/api/v1/live_studio/interactive_courses/{id}";
			strUrl.replace("{id}", strCourseID);
		}
		else
		{
			strUrl = "https://qatime.cn/api/v1/live_studio/teachers/{teacherid}/courses/{id}";
			strUrl.replace("{id}", strCourseID);
			strUrl.replace("{teacherid}", m_teacherID);
		}
	}
	else
	{
		if (m_curTags->Is1v1Lesson())
		{
			strUrl = "http://testing.qatime.cn/api/v1/live_studio/interactive_courses/{id}";
			strUrl.replace("{id}", strCourseID);
		}
		else
		{
			strUrl = "http://testing.qatime.cn/api/v1/live_studio/teachers/{teacherid}/courses/{id}";
			strUrl.replace("{id}", strCourseID);
			strUrl.replace("{teacherid}", m_teacherID);
		}
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	request.setRawHeader("Remember-Token", m_Token.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIWindowSet::returnCourse);
}

void UIWindowSet::returnCourse()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1)
	{
		// 辅导班信息
		QString coursePic;
		if (m_curTags->Is1v1Lesson())
			coursePic = data["publicize_url"].toString();
		else
			coursePic = data["publicize"].toString();
		
		QString courseName = data["name"].toString();
		QString courseGrade = data["grade"].toString();
		QString courseGrade1 = data["subject"].toString();
		QString teacherName = data["teacher_name"].toString();
		QString coursePross = QString::number(data["completed_lesson_count"].toInt());
		QString courseProsses = QString::number(data["lessons_count"].toInt());

		QString courseStart = data["live_start_time"].toString();
		QString courseEnd = data["live_end_time"].toString();
		QString courseDesc = data["description"].toString();

		// 年级信息
		courseGrade = courseGrade + courseGrade1 + " | 主讲: " + m_teacherName;
		coursePross = "课程进度: " + coursePross + "/" + courseProsses;
		courseStart = "时      间: " + courseStart + "至" + courseEnd;
		if (m_CourseWnd)
			m_CourseWnd->AddInfo(coursePic, courseName, courseGrade, coursePross, courseStart, courseDesc);
	}
	else if (obj["status"].toInt() == 0)
	{
		return;
	}
}

void UIWindowSet::clickLesson()
{
	if (m_LessonWnd)
	{
		if (m_LessonWnd->isVisible())
		{
			ui.lesson_pushButton->setStyleSheet("color:rgb(0,0,0);border-image: url(./images/AuxiliaryPanelBack.png);");
			m_LessonWnd->setVisible(false);
			return;
		}

		ui.lesson_pushButton->setStyleSheet("color:rgb(255, 255, 255);border-image: url(./images/Bullet.png);");
		m_LessonWnd->move(4, 74);
		m_LessonWnd->DeleteLesson();
		m_LessonWnd->show();
		m_LessonWnd->setFocus();
		QueryLesson();
	}
}

void UIWindowSet::QueryLesson()
{
	QString strCourseID = "";
	if (m_curTags)
		strCourseID = m_curTags->CourseID();
	else
		return;

	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		if (m_curTags->Is1v1Lesson())
		{
			strUrl = "http://qatime.cn/api/v1/live_studio/interactive_courses/{id}";
			strUrl.replace("{id}", strCourseID);
		}
		else
		{
			strUrl = "https://qatime.cn/api/v1/live_studio/teachers/{teacherid}/courses/{id}";
			strUrl.replace("{id}", strCourseID);
			strUrl.replace("{teacherid}", m_teacherID);
		}
	}
	else
	{
		if (m_curTags->Is1v1Lesson())
		{
			strUrl = "http://testing.qatime.cn/api/v1/live_studio/interactive_courses/{id}";
			strUrl.replace("{id}", strCourseID);
		}
		else
		{
			strUrl = "http://testing.qatime.cn/api/v1/live_studio/teachers/{teacherid}/courses/{id}";
			strUrl.replace("{id}", strCourseID);
			strUrl.replace("{teacherid}", m_teacherID);
		}
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	request.setRawHeader("Remember-Token", m_Token.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIWindowSet::returnLesson);
}

void UIWindowSet::returnLesson()
{
	int iCount = 0;
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1)
	{
		// 课程信息
		QJsonArray lesson;
		if (m_curTags->Is1v1Lesson())
		{
			lesson = data["interactive_lessons"].toArray();
			foreach(const QJsonValue & value, lesson)
			{
				iCount++;
				QJsonObject obj = value.toObject();
				QString strClassDate = obj["class_date"].toString();
				QString strStartTime = obj["start_time"].toString();
				QString strEndTime = obj["end_time"].toString();
				QString strStatus = obj["status"].toString();
				QString strName = obj["name"].toString();

				strClassDate = strClassDate + "  " + strStartTime + "-" + strEndTime;
				if (m_LessonWnd)
					m_LessonWnd->AddLesson(QString().sprintf("%02d", iCount), strClassDate, strName, strStatus);
			}
		}
		else
		{
			lesson = data["lessons"].toArray();
			foreach(const QJsonValue & value, lesson)
			{
				iCount++;
				QJsonObject obj = value.toObject();
				QString strClassDate = obj["class_date"].toString();
				QString strLiveTime = obj["live_time"].toString();
				QString strStatus = obj["status"].toString();
				QString strName = obj["name"].toString();

				strClassDate = strClassDate + "  " + strLiveTime;
				if (m_LessonWnd)
					m_LessonWnd->AddLesson(QString().sprintf("%02d", iCount), strClassDate, strName, strStatus);
			}
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		return;
	}
}

void UIWindowSet::clickPerson()
{
	if (m_PersonWnd)
	{
		if (m_PersonWnd->isVisible())
		{
			ui.person_pushButton->setStyleSheet("color:rgb(0,0,0);border-image: url(./images/AuxiliaryPanelBack.png);");
			m_PersonWnd->setVisible(false);
			return;
		}

		ui.person_pushButton->setStyleSheet("color:rgb(255, 255, 255);border-image: url(./images/Bullet.png);");
		m_PersonWnd->move(280, 74);
		m_PersonWnd->DeletePerson();
		m_PersonWnd->show();
		m_PersonWnd->setFocus();
		QueryPerson();
	}
}

void UIWindowSet::QueryPerson()
{
	if (m_curChatRoom)
	{
		std::vector<personListBuddy*> vecBuddy;
		if (!m_curChatRoom->IsPerson())
		{
			QToolTip::showText(QCursor::pos(), "成员初始化中,请稍后查询！");
			m_PersonWnd->hide();
			ui.person_pushButton->setStyleSheet("color:rgb(0,0,0);border-image: url(./images/AuxiliaryPanelBack.png);");
			return;
		}

		vecBuddy = m_curChatRoom->GetBuddy();
		if (m_PersonWnd)
			m_PersonWnd->AddPerson(vecBuddy, QString::fromStdString(m_curChatRoom->GetCurChatID()));
	}
}

// 设置开始直播按钮状态
void UIWindowSet::setLiveBtnEnable(bool bEnable)
{
	ui.Live_pushBtn->setEnabled(bEnable);
}


void UIWindowSet::slot_stateChange(bool state, QString id, QString chatID)
{
	if (m_curChatRoom)
	{
		if (QString::fromStdString(m_curChatRoom->GetCurChatID()) == chatID)
		{
			m_curChatRoom->MuteStudent(state,id);
		}
	}
}

/****************************直播推流**************************************/
void UIWindowSet::UpatateLiveStatus(QWidget* widget, bool bSuc)
{
	if (bSuc)
	{
		if (m_VideoInfo == widget)
		{
			// 如果摄像头关闭，或者没有摄像头
			if (!IsHasCamera() || m_EnumStatus == CameraStatusClose)
			{
				m_ScreenTip->setErrorTip("开启直播成功！");
				m_EnumStatus = CameraStatusClose;
				m_LiveStatusManager->SendStartLiveHttpMsg(1, m_EnumStatus, m_lessonid, m_Token);
			}
			else//如果摄像头开着，接着推摄像头流
			{
				//推摄像头
				m_EnumStatus = CameraStatusTeaching;
				m_CameraInfo->setPlugFlowUrl(m_cameraUrl);
				m_CameraInfo->StartLiveVideo();
//				m_tempTimers->start(10000);
			}
		}
		else if (m_CameraInfo == widget)
		{
			m_ScreenTip->setErrorTip("开启直播成功！");
			m_LiveStatusManager->SendStartLiveHttpMsg(1, m_EnumStatus, m_lessonid, m_Token);
		}
	}
	else
	{
		// 防止推流一方失败，另一方也要结束
		if (m_VideoInfo == widget)
		{
			// 提示错误信息
			m_ScreenTip->setErrorTip("直播过程中网络出现错误，请重新\n开启直播！");
		}
		else
		{
			m_CameraInfo->setVisible(true);
			m_VideoInfo->StopLiveVideo();
			// 提示错误信息
			m_ScreenTip->setErrorTip("直播过程中网络出现错误，请重新\n开启直播！");
		}
	}
}

// 摄像头延迟1秒推流
void UIWindowSet::slot_onTempTimeout()
{
	m_tempTimers->stop();
	m_EnumStatus = CameraStatusTeaching;
	m_CameraInfo->setPlugFlowUrl(m_cameraUrl);
	m_CameraInfo->StartLiveVideo();
}

void UIWindowSet::setCameraEnable()
{
	ui.video_checkBox->setEnabled(true);
}

// 直播过程中出错，停止直播
void UIWindowSet::ErrorStopLive(QWidget* pWidget)
{
	if (m_CameraInfo && m_VideoInfo)
	{
		if (!m_VideoInfo->IsCurrentLiving() && !m_CameraInfo->IsCurrentLiving())
		{
			return;
		}

		m_VideoInfo->StopLiveVideo();
		if (IsHasCamera() && m_EnumStatus != CameraStatusClose)
			m_CameraInfo->StopLiveVideo();

		ui.Live_pushBtn->setText("开始直播");
		ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");
		if (m_CountTimer->isActive())
		{
			m_CountTimer->stop();					// 停止计时
			m_iTimerCount = 0;						// 重置秒数
			ui.time_label->setVisible(false);		// 隐藏
			ui.time_label->setText("00:00:00");		// 重置时间
		}

		m_ScreenTip->setErrorTip("直播过程中网络出现错误，请重新\n开启直播！");
		m_LiveStatusManager->SendStopLiveHttpMsg();
	}
}

void UIWindowSet::ErrorStop()
{
	if (m_CameraInfo)
		m_CameraInfo->StopLiveVideo();

	if (m_VideoInfo)
		m_VideoInfo->StopLiveVideo();
}

bool UIWindowSet::IsHasCamera()
{
	return m_bHasCamera;
}

// 添加视频源
void UIWindowSet::addVideoDevices(QString path)
{
	m_bHasCamera = true;
	m_VideoChangeInfo->setVideoParam(path, path);
}

// 添加麦克风
void UIWindowSet::addAudioDevices(QString path)
{
	m_AudioChangeInfo->setAudioParam(path, path);
}

void UIWindowSet::AddTodayToLesson(QString  id, QString courseid, QString boardUrl, QString CameraUrl, QString time, QString status, QString name)
{
	m_LiveLesson->addLesson(id,courseid,boardUrl,CameraUrl,time,status, name);
}
/****************************推流参数设置**************************************/
void UIWindowSet::AudioStatus(int iStatus)
{
	if (iStatus)
	{
		m_VideoInfo->SetPauseAudio(NULL);
	}
	else
	{
		m_VideoInfo->SetResumeAudio(NULL);
	}
}

void UIWindowSet::BulletStatus(int iStatus)
{
	if (iStatus)
		m_BulletScreen->showDialog(QString::fromStdString(m_curChatRoom->GetCurChatID()));
	else
		m_BulletScreen->hide();
}

void UIWindowSet::SendStudentBullet(QString name, QString content, QString chatid)
{
	if (m_BulletScreen)
		m_BulletScreen->ReciverStudent(name, content,chatid);
}

void UIWindowSet::SendTeacherBullet(QString name, QString content, QString chatid)
{
	if (m_BulletScreen)
		m_BulletScreen->ReciverTeacher(name, content,chatid);
}


void UIWindowSet::BulletDelay(int iDelay)
{
	if (m_BulletScreen)
		m_BulletScreen->setTriggerDelay(iDelay);
}

void UIWindowSet::setTriggerType(bool bType)
{
	if (m_BulletScreen)
		m_BulletScreen->setBulletTriggerType(bType);
}

void UIWindowSet::CloseBullet()
{
	ui.Bullet_checkBox->setCheckState(Qt::CheckState::Unchecked);
}

void UIWindowSet::clickAudioParam()
{
	m_AudioChangeInfo->show();
	m_AudioChangeInfo->move(width() / 2 - m_AudioChangeInfo->width() / 2, height() / 2 - m_AudioChangeInfo->height() / 2);
	m_AudioChangeInfo->setFocus();
}

void UIWindowSet::setAudioChangeIndex(int index)
{
	if (m_VideoInfo)
	{
		m_VideoInfo->SetChangeAudio(index);
	}
}

void UIWindowSet::VideoStatus(int iStatus)
{
	if (iStatus)
	{
		//隐藏
		m_CameraInfo->setVisible(false);
		if (m_VideoInfo)
		{
			m_EnumStatus = CameraStatusClose;

			if (!IsHasCamera())
				return;

			if (!m_VideoInfo->IsCurrentLiving())
				return;

			ui.video_checkBox->setEnabled(false);
			m_CameraInfo->StopLiveVideo();
			m_LiveStatusManager->SendCameraSwitchMsg(1, m_EnumStatus);
		}
	}
	else
	{
		//显示
		m_CameraInfo->setVisible(true);
		if (m_VideoInfo)
		{
			m_EnumStatus = CameraStatusTeaching;

			if (!IsHasCamera())
				return;

			if (!m_VideoInfo->IsCurrentLiving())
				return;

			ui.video_checkBox->setEnabled(false);
			m_CameraInfo->setPlugFlowUrl(m_cameraUrl);
			m_CameraInfo->StartLiveVideo();
			m_LiveStatusManager->SendCameraSwitchMsg(1, m_EnumStatus);
		}
	}
}

void UIWindowSet::clickVideoParam()
{
	m_VideoChangeInfo->show();
	m_VideoChangeInfo->move(width() / 2 - m_VideoChangeInfo->width() / 2, height() / 2 - m_VideoChangeInfo->height() / 2);
	m_VideoChangeInfo->setFocus();
}

void UIWindowSet::setVideoChangeIndex(int index)
{
	if (m_VideoInfo)
	{
		m_VideoInfo->SetChangeAudio(index);
	}
}

void UIWindowSet::clickRatioParam()
{
	m_RatioChangeInfo->show();
	m_RatioChangeInfo->move(width() / 2 - m_RatioChangeInfo->width() / 2, height() / 2 - m_RatioChangeInfo->height() / 2);
	m_RatioChangeInfo->setFocus();
}

void UIWindowSet::clickBulletParam()
{
	m_BulletParamInfo->show();
	m_BulletParamInfo->move(width() / 2 - m_BulletParamInfo->width() / 2, height() / 2 - m_BulletParamInfo->height() / 2);
	m_BulletParamInfo->setFocus();
}

void UIWindowSet::setRatioChangeIndex(QString ratio)
{
	ui.ratio_pushButton->setText(ratio);
}

void UIWindowSet::SendStopLive()
{
	// 发送结束直播消息再关闭
	if (m_VideoInfo->IsCurrentLiving())
		m_LiveStatusManager->SendStopLiveHttpMsg(false);
}

void UIWindowSet::SendRequestStatus(QString status)
{
	m_parent->changeLessonStatus(m_lessonid, status);
}

void UIWindowSet::showErrorTip(QString sError)
{
	CMessageBox::showMessage(
		QString("答疑时间"),
		QString(sError),
		QString("确定"));
}

bool UIWindowSet::IsLiving()
{
	if (m_VideoInfo->IsCurrentLiving())
		m_ScreenTip->setErrorTip("请先结束直播，再关闭窗口！");

	return m_VideoInfo->IsCurrentLiving();
}