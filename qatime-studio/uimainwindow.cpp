#include "uimainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "course.h"
#include <QJsonArray>
#include <QRect>
#include <QBitmap>
#include <QPainter>
#include <windowsx.h>
#include <QFile>
#include <math.h>
#include <QCheckBox>
#include <QComboBox>
#include <QDir>
#include <QToolTip>
#include "UIMessageBox.h"
#include <QMovie>
#include "define.h"
#include "member.h"
#include <QHBoxLayout>
#include <iosfwd>
#include <sstream>
#include <QDesktopWidget>
#include <QTextCodec>
#include <QDesktopWidget>
#include <QMouseEvent>
#include "qmath.h"

#define MAINWINDOW_X_MARGIN 6
#define MAINWINDOW_Y_MARGIN 6
#define MAINWINDOW_TITLE_HEIGHT 130
#define MAINWINDOW_MAXHEIGHT	832		//客户端最大高度

#ifdef TEST
	#define _DEBUG
#else
#endif
UIMainWindow::UIMainWindow(QWidget *parent)
	: QWidget(parent)
	, m_VideoInfo(NULL)
	, m_iTimerCount(0)
	, m_bOtherApp(false)
	, m_AudioChangeInfo(NULL)
	, m_AuxiliaryPanel(NULL)
	, m_VideoChangeInfo(NULL)
	, m_RatioChangeInfo(NULL)
	, m_charRoom(NULL)
	, m_LoginWindow(NULL)
	, m_LessonTable(NULL)
	, m_VideoWnd(NULL)
	, m_CameraInfo(NULL)
	, m_SwichScreenTimerId(0)
	, m_VideoOrCamera(NULL)
	, m_CameraOrVideo(NULL)
	, m_ShowChatRoomTimerId(0)
	, m_RangeCapture(NULL)
	, m_MenuTool(NULL)
	, m_SetParam(NULL)
	, m_HoverWnd(NULL)
	, m_SeeLiveWnd(NULL)
	, m_ScreenTip(NULL)
	, m_ScreenTipTimer(NULL)
	, m_PersonNum(NULL)
	, m_LiveStatusManager(NULL)
	, bHasCamera(false)
	, m_EnumStatus(CameraStatusTeaching)
	, m_HelpWord(NULL)
	, m_icount(0)
{
	ui.setupUi(this);
	setFocusPolicy(Qt::ClickFocus);
	QDesktopWidget *dsk = QApplication::desktop();
	//除去任务栏的高度
	int iHeight = dsk->height() - 40;
	if (iHeight < MAINWINDOW_MAXHEIGHT)
	{
		setAdaptHeight(iHeight - 10);
	}

	connect(ui.mainmin_pushBtn, SIGNAL(clicked()), this, SLOT(MinDialog()));
	connect(ui.mainclose_pushBtn, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.return_pushButton, SIGNAL(clicked()), this, SLOT(returnClick()));
 	connect(ui.Live_pushBtn, SIGNAL(clicked()), this, SLOT(slot_startOrStopLiveStream()));
 	connect(ui.Audio_checkBox, SIGNAL(stateChanged(int)), this, SLOT(AudioStatus(int)));
 	connect(ui.video_checkBox, SIGNAL(stateChanged(int)), this, SLOT(VideoStatus(int)));
	connect(ui.switchScreen_checkBox, SIGNAL(stateChanged(int)), this, SLOT(SwitchScreenStatus(int)));
	connect(ui.set_checkBox, SIGNAL(clicked()), this, SLOT(SetParamWindow()));
 	connect(ui.lesson_pushButton, SIGNAL(clicked()), this, SLOT(clickLessonList()));
	ui.time_label->hide();
	
	m_SeeLiveWnd = new UISeeLiveWnd();
	m_SeeLiveWnd->setWindowFlags(Qt::FramelessWindowHint);
	m_SeeLiveWnd->SetMainWnd(this);
	m_SeeLiveWnd->hide();

	m_VideoInfo = new UIVideo(ui.video_widget);
	m_VideoInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_VideoInfo->SetMainWnd(this);
	m_VideoInfo->resize(316, 179);
	m_VideoInfo->show();

	m_AuxiliaryPanel = new UIAuxiliaryPanel(ui.Auxiliary_widget);
	m_AuxiliaryPanel->setWindowFlags(Qt::FramelessWindowHint);
	m_AuxiliaryPanel->setParent(this);
	m_AuxiliaryPanel->resize(ui.Auxiliary_widget->size().width()+2, ui.Auxiliary_widget->size().height()+5);
	
	m_AudioChangeInfo = new UIAudioChange(this);
	m_AudioChangeInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_AudioChangeInfo->setAudioChange(this);
	m_AudioChangeInfo->hide();

	m_VideoChangeInfo = new UIVideoChange(this);
	m_VideoChangeInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_VideoChangeInfo->setVideoChange(this);
	m_VideoChangeInfo->hide();

	m_RatioChangeInfo = new UIRatio(this);
	m_RatioChangeInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_RatioChangeInfo->setVideoChange(this);
	m_RatioChangeInfo->hide();

	m_LessonTable = new UILessonTable(this);
	m_LessonTable->setWindowFlags(Qt::FramelessWindowHint);
	m_LessonTable->SetMainWindow(this);
	m_LessonTable->hide();

	m_RangeCapture = new RangeCapture();
	m_RangeCapture->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	m_RangeCapture->setMainWindow(this);
	m_RangeCapture->move(0, 0);
	m_RangeCapture->hide();

	m_HoverWnd = new UIHoverWindow();
	m_HoverWnd->setWindowFlags(Qt::FramelessWindowHint);
	m_HoverWnd->setParentWindow(this);
	m_HoverWnd->hide();

	m_LiveStatusManager = new LiveStatusManager(this);
	m_LiveStatusManager->setMainWindow(this);

	// 直播按钮
	ui.Live_pushBtn->setText("开始直播");
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");

	// 计时器 改变直播计时时间
	m_CountTimer = new QTimer(this);
	connect(m_CountTimer, SIGNAL(timeout()), this, SLOT(slot_onCountTimeout()));

	m_ShowVideoTimer = new QTimer(this);
	connect(m_ShowVideoTimer, SIGNAL(timeout()), this, SLOT(setVideoPos()));

	m_ShowCameraTimer = new QTimer(this);
	connect(m_ShowCameraTimer, SIGNAL(timeout()), this, SLOT(setCameraPos()));

	m_tempTimers = new QTimer(this);
	connect(m_tempTimers, SIGNAL(timeout()), this, SLOT(slot_onTempTimeout()));
	m_tempTimers->start(500);

	m_ScreenTipTimer = new QTimer(this);
	connect(m_ScreenTipTimer, SIGNAL(timeout()), this, SLOT(slot_ScreenTipTimeout()));
	m_ScreenTipTimer->start(2000);

	// 设置摄像头视频源样式
	ui.video_checkBox->setStyleSheet("QCheckBox::indicator{width: 18px;height: 16px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/camera_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/camera_close.png);}");

	// 设置麦克风样式
	ui.Audio_checkBox->setStyleSheet("QCheckBox::indicator{width: 18px;height: 16px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/mic_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/mic_close.png);}");

	// 设置窗口切换样式
	ui.switchScreen_checkBox->setStyleSheet("QCheckBox::indicator{width: 18px;height: 16px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/switch.png);}"
		"QCheckBox::indicator:checked{image: url(./images/switch.png);}");

	// 设置参数窗口
	ui.set_checkBox->setStyleSheet("QCheckBox::indicator{width: 18px;height: 16px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/set.png);}"
		"QCheckBox::indicator:checked{image: url(./images/set.png);}");

	// 关闭窗口采集样式
	ui.close_radioButton->setStyleSheet("QRadioButton::indicator{width: 62px;height: 22px;}"
		"QRadioButton::indicator:unchecked{image: url(./images/captureclose_uncheck.png);}"
		"QRadioButton::indicator:checked{image: url(./images/captureclose_check.png);}"
		"QRadioButton::indicator:checked:disabled{image:url(./images/captureclose_disabled.png);}"
		"QRadioButton::indicator:unchecked:disabled{image:url(./images/captureclose_disabled.png);}");

	// 窗口采集样式
	ui.rect_radioButton->setStyleSheet("QRadioButton::indicator{width: 62px;height: 22px;}"
		"QRadioButton::indicator:unchecked{image: url(./images/capturerect_uncheck.png);}"
		"QRadioButton::indicator:checked{image: url(./images/capturerect_check.png);}");

	// 全屏采集样式
	ui.full_radioButton->setStyleSheet("QRadioButton::indicator{width: 62px;height: 22px;}"
		"QRadioButton::indicator:unchecked{image: url(./images/capturefull_uncheck.png);}"
		"QRadioButton::indicator:checked{image: url(./images/capturefull_check.png);}");

	QPixmap pixmap(QCoreApplication::applicationDirPath() + "/images/login_min.png");
	QPixmap pixmap1(QCoreApplication::applicationDirPath() + "/images/login_close.png");
	ui.mainmin_pushBtn->setPixmap(pixmap, 1);
	ui.mainclose_pushBtn->setPixmap(pixmap1, 1);
	
	m_charRoom = new UIChatRoom(ui.chat_widget);
	m_charRoom->setWindowFlags(Qt::FramelessWindowHint);
	m_charRoom->setMainWindow(this);
 	m_charRoom->move(10, 0);
	m_charRoom->resize(m_charRoom->size().width(), ui.chat_widget->size().height());
	m_charRoom->setAdaptHeight(ui.chat_widget->size().height());
	m_charRoom->show();

	ui.titel_pushButton->installEventFilter(this);
	ui.titel1_pushButton->installEventFilter(this);

	// TabWidget背景变换
	ui.tabWidget->setStyleSheet("QTabWidget:pane{border: 1px solid white;background-color:white;}");

	// TabBar背景图片
	ui.tabWidget->setStyleSheet("QTabBar::tab { height: 25px; width: 170px; }"
		"QTabBar::tab:!selected {color:rgb(204,204,204);background: transparent;}"
		"QTabBar::tab:selected {color:rgb(5, 158, 213);}");

	QFont font;
	font.setPointSize(12);
	font.setFamily(("微软雅黑"));
	ui.tabWidget->setFont(font);

	ui.close_radioButton->installEventFilter(this);
	ui.rect_radioButton->installEventFilter(this);
	ui.full_radioButton->installEventFilter(this);

	ui.help_pushButton->setIcon(QIcon("./images/help.png"));
	ui.help_pushButton->setText("帮助");
	connect(ui.help_pushButton, SIGNAL(clicked()), this, SLOT(HelpDialog()));
}

UIMainWindow::~UIMainWindow()
{
	if (m_AuxiliaryPanel)
	{
		delete m_AuxiliaryPanel;
		m_AuxiliaryPanel = NULL;
	}

	if (m_VideoInfo)
	{
		delete m_VideoInfo;
		m_VideoInfo = NULL;
	}

	if (m_AudioChangeInfo)
	{
		delete m_AudioChangeInfo;
		m_AudioChangeInfo = NULL;
	}

	if (m_VideoChangeInfo)
	{
		delete m_VideoChangeInfo;
		m_VideoChangeInfo = NULL;
	}

	//删除定时器
	if (m_CountTimer)
	{
		if (m_CountTimer->isActive())
			m_CountTimer->stop();
		delete m_CountTimer;
		m_CountTimer = NULL;
	}

	if (m_LoginWindow)
		m_LoginWindow = NULL;

	if (m_LessonTable)
	{
		delete m_LessonTable;
		m_LessonTable = NULL;
	}

	if (m_CameraInfo)
	{
		delete m_CameraInfo;
		m_CameraInfo = NULL;
	}

	if (m_RangeCapture)
	{
		delete m_RangeCapture;
		m_RangeCapture = NULL;
	}

	if (m_SetParam)
	{
		delete m_SetParam;
		m_SetParam = NULL;
	}

	if (m_HoverWnd)
	{
		delete m_HoverWnd;
		m_HoverWnd = NULL;
	}

	if (m_ScreenTip)
	{
		delete m_ScreenTip;
		m_ScreenTip = NULL;
	}

	if (m_LiveStatusManager)
	{
		delete m_LiveStatusManager;
		m_LiveStatusManager = NULL;
	}

	if (m_HelpWord)
	{
		delete m_HelpWord;
		m_HelpWord = NULL;
	}
}

void UIMainWindow::setAdaptHeight(int iHeight)
{
	setMaximumSize(QSize(342, iHeight));
	ui.tabWidget->setMaximumHeight(iHeight - 71);
	ui.chat_widget->setMaximumHeight(iHeight - 382);
}
void UIMainWindow::WhiteBoard()
{
	m_RangeCapture->setShowOrHide(true);
	SetWindowPos((HWND)m_RangeCapture->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void UIMainWindow::MinDialog()
{
	//需求更改为，点击最小化，隐藏此窗口，弹出悬浮窗
	this->hide();
	QDesktopWidget *dsk = QApplication::desktop();
	SetWindowPos((HWND)m_HoverWnd->winId(), HWND_TOPMOST, dsk->width() - 45, dsk->height()*0.6, 0, 0, SWP_NOSIZE);
	m_HoverWnd->resize(44, m_HoverWnd->size().height());
	m_HoverWnd->SetNumber();
	m_HoverWnd->show();
}

void UIMainWindow::CloseDialog()
{
	int iStatus = CMessageBox::showMessage(
		QString("答疑时间"),
		QString("是否关闭当前应用程序！"),
		QString("确定"),
		QString("取消"));
	if (iStatus == 1)
	{
		// 发送结束直播消息再关闭
		m_LiveStatusManager->SendStopLiveHttpMsg(false);

		//隐藏设置窗口
		if (m_SetParam)
			m_SetParam->setVisible(false);

		if (m_RangeCapture)
			m_RangeCapture->setShowOrHide(false);

		if (m_ScreenTip)
			m_ScreenTip->setVisible(false);

		if (m_HelpWord)
			m_HelpWord->setVisible(false);
		
		close();
	}
}

void UIMainWindow::closeEvent(QCloseEvent *e)
{
	__super::closeEvent(e);
}

void UIMainWindow::setTeacherInfo(QJsonObject &data)
{
	int iID = data["id"].toInt();
	m_teacherID = QString::number(iID);
	m_AuxiliaryPanel->setTeacherID(m_teacherID);

	// 设置老师名字
	QString teacherName = data["name"].toString();
	ui.teacherName_label->setText(teacherName);

	// 设置老师头像
	QString teacherPhoto_url = data["avatar_url"].toString();
	setNetworkPic(teacherPhoto_url);

	QString strWelcome = "欢迎 ";
	strWelcome += teacherName;
	strWelcome += " 老师登录答疑时间！";
	ui.lesson_label->setText(strWelcome);

	// 老师云信信息
	m_charRoom->setChatInfo(data["chat_account"].toObject(), mRemeberToken);
	m_charRoom->m_TeachterName = teacherName;

	m_LessonTable->InitToken(mRemeberToken, m_teacherID);
}

void UIMainWindow::setAutoTeacherInfo(QString teacherID, QString teacherName, QString teacherUrl, QString accid, QString token)
{
	m_teacherID = teacherID;
	m_AuxiliaryPanel->setTeacherID(m_teacherID);

	// 设置老师名字
	ui.teacherName_label->setText(teacherName);

	// 设置老师头像
	setNetworkPic(teacherUrl);

	QString strWelcome = "欢迎 ";
	strWelcome += teacherName;
	strWelcome += " 老师登录答疑时间！";
	ui.lesson_label->setText(strWelcome);

	// 老师云信信息
	QJsonObject data;
	data["accid"] = accid;
	data["token"] = token;
	m_charRoom->setChatInfo(data, mRemeberToken);
	m_charRoom->m_TeachterName = teacherName;

	m_LessonTable->InitToken(mRemeberToken, m_teacherID);
}

void UIMainWindow::setRemeberToken(const QString &token)
{
	mRemeberToken = token;
	m_AuxiliaryPanel->setRemeberToken(token);
}

void UIMainWindow::ShowAuxiliary()
{
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/teachers/{teacher_id}/courses/full?status=teaching";
	strUrl.replace("{teacher_id}", m_teacherID);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/teachers/{teacher_id}/courses/full?status=teaching";
	strUrl.replace("{teacher_id}", m_teacherID);
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIMainWindow::AuxiliaryRequestFinished);
}

void UIMainWindow::AuxiliaryRequestFinished()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	m_AuxiliaryPanel->setAuxiliaryInfo(document.object());
}

void UIMainWindow::resizeEvent(QResizeEvent *e)
{
	m_mutex.lock();
	int w, h;
	if (m_icount > 0 && e != NULL)
	{
		w = e->size().width() - e->oldSize().width();
		h = e->size().height() - e->oldSize().height();
		chat_X += w;
		video_Width += w;
		video_Heigth += h;
		chat_Heigth += h;
	}
	if (m_icount > 0 || e == NULL)
	{
		ui.title_widget->resize(ui.title_widget->width() + w, ui.title_widget->height());
		ui.help_widget->resize(ui.help_widget->width() + w, ui.help_widget->height());
		ui.welcome_widget->resize(ui.welcome_widget->width() + w, ui.welcome_widget->height());
		ui.btn_widget->resize(ui.btn_widget->width() + w, ui.btn_widget->height());
		ui.videoCamera_widget->resize(ui.videoCamera_widget->width() + w, ui.videoCamera_widget->height());
		ui.right_label->move(ui.right_label->geometry().left() + w, ui.right_label->geometry().top());

		if (m_charRoom)
		{
			ui.tabWidget->setFixedSize(ui.tabWidget->width()+w, ui.tabWidget->height() + h);
			ui.chat_widget->setFixedSize(ui.chat_widget->width()+w, ui.chat_widget->height() + h);
			m_charRoom->setFixedSize(m_charRoom->width()+w,m_charRoom->height() + h);
			m_charRoom->setResize(w,h);
		}
		
// 		m_VideoOrCamera->resize(video_Width, this->size().height() - 180);
// 		m_AuxiliaryPanel->resize(m_AuxiliaryPanel->size().width(),this->size().height()-140);
// 		if (m_VideoOrCamera == m_VideoInfo)
// 			PostMessage(m_VideoWnd, MSG_VIDEO_CHANGE_SIZE, (WPARAM)video_Width, (LPARAM)(this->size().height() - 180));
// 		else
// 			PostMessage(m_CameraWnd, MSG_VIDEO_CHANGE_SIZE, (WPARAM)video_Width, (LPARAM)(this->size().height() - 180));
	}
	m_icount++;
	m_mutex.unlock();
}

void UIMainWindow::slot_startOrStopLiveStream()
{
	HideOtherUI();
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
				SendVideoMsg((UINT)MSG_VIDEO_STOP_LIVE);
				if (IsHasCamera() && m_EnumStatus != CameraStatusClose)
					SendCameraMsg((UINT)MSG_VIDEO_STOP_LIVE);
				m_LiveStatusManager->SendStopLiveHttpMsg();

				if (m_CountTimer->isActive())
				{
					m_CountTimer->stop();					// 停止计时
					m_iTimerCount = 0;						// 重置秒数
					ui.time_label->setVisible(false);		// 隐藏
					ui.time_label->setText("00:00:00");		// 重置时间
					
					if (m_HoverWnd)
						m_HoverWnd->SetLiveTimer("00:00:00");
				}

				ui.close_radioButton->setEnabled(true);
				m_AuxiliaryPanel->setPreview(false);
			}
			else
				return;
		}
		else
		{
			if (m_AuxiliaryPanel->getLessonID().isEmpty())
			{
				CMessageBox::showMessage(
					QString("答疑时间"),
					QString("请选择您要开始的课程进行直播！"),
					QString("确定"));
				return;
			}

			if (m_AuxiliaryPanel->getURL().isEmpty())
			{
				CMessageBox::showMessage(
					QString("答疑时间"),
					QString("推流地址为空！"),
					QString("确定"));
				return;
			}

			if (ui.close_radioButton->isChecked())
			{
				CMessageBox::showMessage(
					QString("答疑时间"),
					QString("请选择采集模式后再开始直播！"),
					QString("确定"));
				return;
			}

			
			if (!IsHasCamera())
				m_EnumStatus = CameraStatusClose;
			m_LiveStatusManager->SendStartLiveHttpMsg(1, m_EnumStatus, m_AuxiliaryPanel->getLessonID(),mRemeberToken);
		}
	}
}

void UIMainWindow::startLiveStream()
{
	m_VideoInfo->StartLiveVideo();
	SendVideoMsg((UINT)MSG_VIDEO_START_LIVE);

	if (IsHasCamera() && m_EnumStatus != CameraStatusClose)
		SendCameraMsg((UINT)MSG_VIDEO_START_LIVE);

	ui.time_label->setVisible(true);
	ui.Live_pushBtn->setText("结束直播");
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: red;border-radius: 5px; border: 2px solid red;}");

	m_CountTimer->start(1000);
	m_LiveStatusManager->StartHeartBeat();

	ui.close_radioButton->setEnabled(false);
	m_AuxiliaryPanel->setPreview(true);
}

void UIMainWindow::VideoStatus(int iStatus)
{
	if (m_CameraWnd == NULL)
		return;
	
	if (iStatus)
	{
		ShowWindow(m_CameraWnd, SW_HIDE);
		if (m_VideoInfo)
		{
			m_EnumStatus = CameraStatusClose;

			if (!IsHasCamera())
				return;

			if (!m_VideoInfo->IsCurrentLiving())
				return;

			SendCameraMsg((UINT)MSG_VIDEO_STOP_LIVE);
			m_LiveStatusManager->SendCameraSwitchMsg(1, m_EnumStatus);
		}
	}
	else
	{
		ShowWindow(m_CameraWnd, SW_SHOW);
		if (m_VideoInfo)
		{
			m_EnumStatus = CameraStatusTeaching;

			if (!IsHasCamera())
				return;

			if (!m_VideoInfo->IsCurrentLiving())
				return;

			SendCameraMsg((UINT)MSG_VIDEO_START_LIVE);
			m_LiveStatusManager->SendCameraSwitchMsg(1, m_EnumStatus);
		}
	}
}

void UIMainWindow::SwitchScreenStatus(int iStatus)
{
	bool bVisible = ui.camera_widget->isVisible();
	ui.camera_widget->setVisible(!bVisible);
	ui.video_widget->setVisible(bVisible);
}

void UIMainWindow::AudioStatus(int iStatus)
{
	if (iStatus)
	{
		m_VideoInfo->SetPauseAudio(m_VideoWnd);
	}
	else
	{
		m_VideoInfo->SetResumeAudio(m_VideoWnd);
	}
}

// 设置参数窗口
void UIMainWindow::SetParamWindow()
{
// 	m_SeeLiveWnd->show();
// 	m_CameraInfo->setParent(m_SeeLiveWnd, Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
// 	m_CameraInfo->show();
// 	m_CameraInfo->move(4, 29);
//  	return;
	m_SetParam->setVisible(!m_SetParam->isVisible());
}

void UIMainWindow::slot_onCountTimeout()
{
	m_iTimerCount++;
	QString str = QString().sprintf("%02lld:%02lld:%02lld", m_iTimerCount / 3600, m_iTimerCount % 3600 / 60, m_iTimerCount % 60);
	ui.time_label->setText(str);

	if (m_HoverWnd)
		m_HoverWnd->SetLiveTimer(str);
}

void UIMainWindow::slot_onTempTimeout()
{
	m_tempTimers->stop();

	m_CameraInfo = new UICamera(ui.camera_widget);
	m_CameraInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_CameraInfo->SetMainWindow(this);
	m_CameraInfo->show();
	m_CameraInfo->resize(316, 179);
}

void UIMainWindow::slot_ScreenTipTimeout()
{
	m_ScreenTipTimer->stop();

	m_ScreenTip = new UIScreenTip();
	m_ScreenTip->setWindowFlags(Qt::FramelessWindowHint);
	if (m_ScreenTip->IsTip())
		m_ScreenTip->hide();
	else
	{
		SetWindowPos((HWND)m_ScreenTip->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		m_ScreenTip->show();
	}
}

bool UIMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	if ("windows_generic_MSG" == eventType)
	{
		PMSG pMsg = static_cast<PMSG>(message);
		switch (pMsg->message)
		{
		case MSG_CLIENT_RECEIVE:  // 接收聊天消息
		{
			MSG* Msg = pMsg;
			nim::IMMessage* pIMsg = (nim::IMMessage*)Msg->wParam;

			if (m_charRoom)
			{
				if (m_charRoom->ReceiverMsg(pIMsg))
					m_HoverWnd->UpdateChatNumber();
			}
				
			delete pIMsg;
		}
		break;
		case MSG_CLIENT_RECORD:  // 接收历史消息
		{
			MSG* Msg = pMsg;
			nim::QueryMsglogResult* pIMsg = (nim::QueryMsglogResult*)Msg->wParam;

			if (m_charRoom)
				m_charRoom->ReceiverRecordMsg(pIMsg);

			delete pIMsg;
		}
		break;
		case MSG_LOGIN:  // 接收登录返回结果
		{
			MSG* Msg = pMsg;
			nim::LoginRes* pLogMsg = (nim::LoginRes*)Msg->wParam;

			if (m_charRoom)
				m_charRoom->ReceiverLoginMsg(pLogMsg);

			delete pLogMsg;
		}
		break;
		case MSG_MEMBERS_INFO:  // 接收群成员信息
		{
			MSG* Msg = pMsg;
			std::list<nim::TeamMemberProperty>* pMemberMsg = (std::list<nim::TeamMemberProperty>*)Msg->wParam;

			if (m_charRoom)
				m_charRoom->ReceiverMemberMsg(pMemberMsg);

			delete pMemberMsg;
		}
		break;
		case WM_NCLBUTTONDBLCLK:
		{
			int y = GET_Y_LPARAM(pMsg->lParam) - this->frameGeometry().y();
			if (y > MAINWINDOW_Y_MARGIN && y <= MAINWINDOW_TITLE_HEIGHT)
			{
				// titleDoubleClick();
				return true;
			}
		}
		break;
		case MSG_VIDEO_HWND:
		{
			HWND hwnd = (HWND)pMsg->wParam;
			m_VideoWnd = hwnd;
			m_VideoInfo->SetVideoWnd(hwnd);
			m_ShowVideoTimer->start(50);
			return true;
		}
		break;
		case MSG_VIDEO_SELECTAPP:
		{
			m_bOtherApp = true;
			ShowWindow(m_VideoWnd, SW_SHOW);
			return true;
		}
		break;
		case WM_COPYDATA:
		{
			COPYDATASTRUCT* pCopyDataStruct = (COPYDATASTRUCT*)pMsg->lParam;
			struct MEDIA_DEVICE_DRIVE_INFO1
			{
				char device_path_[260];
				char friendly_name_[260];
			};

			MEDIA_DEVICE_DRIVE_INFO1 m_result;
			memcpy(&m_result, pCopyDataStruct->lpData, sizeof(m_result));
			int iCount = (int)pCopyDataStruct->dwData;
			
			// 0：摄像头 1：麦克风
			if (pMsg->wParam == 0)
			{
				bHasCamera = true;
				m_SetParam->setVideoParam(iCount, QString(QLatin1String(m_result.friendly_name_)), QString(QLatin1String(m_result.device_path_)));
			}
			else if (pMsg->wParam == 1)
			{
				m_SetParam->setAudioParam(iCount, QString::fromStdString(m_result.friendly_name_), QString::fromStdString(m_result.device_path_));
			}
			return true;
		}
		break;
		case MSG_CAMERA_HWND:
		{
			HWND hwnd = (HWND)pMsg->wParam;
			m_CameraWnd = hwnd;
			m_ShowCameraTimer->start(50);
			return true;
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
				if (ui.mainclose_pushBtn && ui.mainmin_pushBtn)
				{
					QRect Rect = ui.mainclose_pushBtn->geometry();
					if (x > Rect.left() && x < Rect.right() && y > Rect.top() && y < Rect.bottom())
					{
						return false;
					}

					Rect = ui.mainmin_pushBtn->geometry();
					if (x > Rect.left() && x < Rect.right() && y > Rect.top() && y < Rect.bottom())
					{
						return false;
					}
				}
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

void UIMainWindow::SendRequestStatus()
{
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/teachers/{teacher_id}/courses/{id}";
	strUrl.replace("{teacher_id}",	m_teacherID);
	strUrl.replace("{id}", m_AuxiliaryPanel->getCouresID());
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/teachers/{teacher_id}/courses/{id}";
	strUrl.replace("{teacher_id}", m_teacherID);
	strUrl.replace("{id}", m_AuxiliaryPanel->getCouresID());
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIMainWindow::RequestStatus);
}

void UIMainWindow::RequestStatus()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1 && data.contains("lessons"))
	{
		QJsonArray lessons = data["lessons"].toArray();
		foreach(const QJsonValue & value, lessons)
		{
			QJsonObject objLesson = value.toObject();
			QString status = objLesson["status"].toString();
			QString strId = QString::number(objLesson["id"].toInt());
			m_AuxiliaryPanel->ChangeLessonStatus(strId, status);
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
}

// 增加阴影
void UIMainWindow::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(2, 2, this->width() - 4, this->height() - 4);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QColor color(5, 158, 213);
	for (int i = 0; i < 4; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);

		for (int j = 0; j < 95; j++)
		{
			path.addRect(4 - i, j - i, this->width() - (4 - i) * 2, this->height() - (2 - i) * 2);
		}

		painter.setPen(color);
		painter.drawPath(path);
	}

// 	QPainterPath path;
// 	path.setFillRule(Qt::WindingFill);
// 	path.addRect(10, 10, this->width() - 20, this->height() - 20);
// 
// 	QPainter painter(this);
// 	painter.setRenderHint(QPainter::Antialiasing, true);
// 	painter.fillPath(path, QBrush(Qt::white));
// 
// 	QColor color(0, 0, 0, 50);
// 	for (int i = 0; i < 10; i++)
// 	{
// 		QPainterPath path;
// 		path.setFillRule(Qt::WindingFill);
// 		path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
// 		color.setAlpha(150 - qSqrt(i) * 50);
// 		painter.setPen(color);
// 		painter.drawPath(path);
// 	}
}

void UIMainWindow::focusInEvent(QFocusEvent *e)
{
	HideOtherUI();
}

void UIMainWindow::HideOtherUI(QWidget* self)
{
	if (m_LessonTable )
		m_LessonTable->hide();

	if (m_SetParam)
		m_SetParam->setVisible(false);
}

void UIMainWindow::setAudioChangeIndex(QString path)
{
	if (m_VideoInfo)
	{
		m_VideoInfo->SetChangeVideoAudio(m_VideoWnd, path,false);
	}
}

void UIMainWindow::setVideoChangeIndex(QString path)
{
	if (m_VideoInfo)
	{
		m_VideoInfo->SetChangeVideoAudio(m_CameraWnd, path);
	}
}

void UIMainWindow::setRatioChangeIndex(int index)
{
	if (m_VideoInfo)
	{
		switch (index)
		{
		case 0:
		{
			m_MenuTool->setRatioText("标清");
			break;
		}
		case 1:
		{
			m_MenuTool->setRatioText("高清");
			break;
		}
		default:
			break;
		}
	}
}

void UIMainWindow::setLoginWindow(LoginWindow* parent)
{
	m_LoginWindow = parent;
}

void UIMainWindow::returnClick()
{
	int iStatus = CMessageBox::showMessage(
		QString("答疑时间"),
		QString("是否退出当前账号？"),
		QString("确定"),
		QString("取消"));
	if (iStatus == 1)
	{
		// 发送结束直播消息再关闭
		m_LiveStatusManager->SendStopLiveHttpMsg(false);

		if (m_LoginWindow)
			m_LoginWindow->ReturnLogin();
	}
}

void UIMainWindow::setCurChatRoom(QString chatID, QString courseid)
{
	if (m_charRoom)
	{
		// 如果是当前会话窗口，则不需要再次请求群成员
		if (!m_charRoom->IsCurChatRoom(m_AuxiliaryPanel->getChatID()))
		{
			m_charRoom->setCurChatID(chatID, courseid);
			m_charRoom->clearAll();
			RequestMember();
		}		
		
	}
}

void UIMainWindow::RequestKey()
{
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/app_constant/im_app_key";
#else
	strUrl = "http://qatime.cn/api/v1/app_constant/im_app_key";
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIMainWindow::returnKey);
}

void UIMainWindow::returnKey()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1 && data.contains("im_app_key"))
	{
		QString key = data["im_app_key"].toString();
		m_charRoom->setKeyAndLogin(key);
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
}

void UIMainWindow::RequestMember()
{
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/courses/{id}/realtime";
	strUrl.replace("{id}", m_AuxiliaryPanel->getCouresID());
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/courses/{id}/realtime";
	strUrl.replace("{id}", m_AuxiliaryPanel->getCouresID());
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIMainWindow::returnMember);
}

void UIMainWindow::returnMember()
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

			if (m_charRoom)
			{
				// 添加成员成功，才算显示人数
				bool bSuc = m_charRoom->AddStudent(pMember->url(), pMember->name(), pMember->accid());
				if (bSuc)
					i++;
			}

			//用完之后删除
			delete pMember;
		}
		m_charRoom->AddStudentNumbers(i);
		m_charRoom->QueryGroup();

		// 设置观看人数
		m_charRoom->SetStudentName(i);

		// 群公告信息
		QJsonArray announcements = data["announcements"].toArray();
		foreach(const QJsonValue & value, announcements)
		{
			QJsonObject obj = value.toObject();
			YXMember *announcements = new YXMember();
			announcements->readJsonToAnnouncement(value.toObject());

			if (m_charRoom)
				m_charRoom->AddAnnouncement(announcements->announcement(), announcements->announcementTime());

			//用完之后删除
			delete announcements;
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
		return;
	}

	// 没登录，则请求key并登录
	if (!m_charRoom->IsLogin())
	{
		RequestKey();
	}
}

void UIMainWindow::setVideoLesson(QString strLessonName)
{
	ui.lesson_label->setText(strLessonName);

	CMessageBox::showMessage(QString("答疑时间"), QString("您已成功进入直播教室！"), QString("确定"), QString(""), NULL, true);
}

// 显示聊天窗口
void UIMainWindow::showChatRoomWnd()
{
	if (m_charRoom && !m_charRoom->isVisible())
	{
		resize(this->size().width() + chat_Width, this->height());
		m_charRoom->move(chat_X, 50+195);
		m_charRoom->show();		

		QPoint closeQt = ui.mainclose_pushBtn->pos();
		ui.mainclose_pushBtn->move(QPoint(closeQt.x() + 295, closeQt.y()));

		QPoint minQt = ui.mainmin_pushBtn->pos();
		ui.mainmin_pushBtn->move(QPoint(minQt.x() + 295, minQt.y()));
	}
}

void UIMainWindow::clickLessonList()
{
	if (m_LessonTable)
	{
		if (m_LessonTable->isVisible())
		{
			m_LessonTable->hide();
			return;
		}

		m_LessonTable->move(QPoint(2, 95));
		m_LessonTable->RequestLesson();
		m_LessonTable->show();
	}
}

// 课程表中选择课程——关联到辅导班
void UIMainWindow::LessonTable_Auxiliary(QString sLessonID, QString sCourseID)
{
	m_AuxiliaryPanel->ergodicItem(sLessonID, sCourseID);
}

void UIMainWindow::setVideoPos()
{
	if (m_VideoWnd)
	{
		if (m_ShowVideoTimer->isActive())
			m_ShowVideoTimer->stop();
		MoveWindow(m_VideoWnd, 0, 0, 316, 179, true);
		ui.video_widget->setVisible(false);
		
		if (m_RangeCapture)
			m_RangeCapture->setVideoWnd(m_VideoWnd);

		if (m_SetParam == NULL)
		{
			m_SetParam = new UISetParam();
			m_SetParam->setWindowFlags(Qt::FramelessWindowHint);
			m_SetParam->setParent(this);
			m_SetParam->hide();
			SetWindowPos((HWND)m_SetParam->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

			PostMessage(m_VideoWnd, MSG_DEVICE_AUDIO, 0, 0);
			PostMessage(m_VideoWnd, MSG_DEVICE_VIDEO, 0, 0);
		}
	}
}

void UIMainWindow::setCameraPos()
{
	if (m_CameraWnd)
	{
		if (m_ShowCameraTimer->isActive())
			m_ShowCameraTimer->stop();
		MoveWindow(m_CameraWnd, 0, 0, 317, 178, true);

		if (m_RangeCapture)
		{
			m_RangeCapture->setShowOrHide(true);
			m_RangeCapture->ToRectDialog();
			SetWindowPos((HWND)m_RangeCapture->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}
	}
}

void UIMainWindow::SendVideoMsg(UINT iMsg)
{
	if (m_VideoWnd)
	{
		if (iMsg == MSG_VIDEO_START_LIVE)
		{
			QString url = m_AuxiliaryPanel->getURL();
//			url = "rtmp://pa0a19f55.live.126.net/live/1243a663c3e54b099d1cc35ee83a7921?wsSecret=e9f02c94465ba21c6e1316e5233fd5d3&wsTime=1480509627";
			COPYDATASTRUCT sendData;
			char result[MAX_PATH];
			QByteArray chPath = url.toLatin1();

			strcpy(result, chPath.data());

			ZeroMemory(&sendData, sizeof(sendData));
			sendData.lpData = result;
			sendData.cbData = MAX_PATH;

			if (IsWindow(m_VideoWnd))
				::SendMessage(m_VideoWnd, WM_COPYDATA, 2, (LPARAM)&sendData);

			return;
		}
		
		PostMessage(m_VideoWnd, iMsg,0,0);
	}
}

void UIMainWindow::SendCameraMsg(UINT iMsg)
{
	if (m_CameraWnd)
	{
		if (iMsg == MSG_VIDEO_START_LIVE)
		{
			QString url = m_AuxiliaryPanel->getCameraURL();
//			url = "rtmp://pa0a19f55.live.126.net/live/02dce8e380034cf9b2ef1f9c26c4234c?wsSecret=9322d438e9410afd21bdd9785b15a4bb&wsTime=1480509609";
			COPYDATASTRUCT sendData;
			char result[MAX_PATH];
			QByteArray chPath = url.toLatin1();

			strcpy(result, chPath.data());

			ZeroMemory(&sendData, sizeof(sendData));
			sendData.lpData = result;
			sendData.cbData = MAX_PATH;

			if (IsWindow(m_CameraWnd))
				::SendMessage(m_CameraWnd, WM_COPYDATA, 2, (LPARAM)&sendData);

			return;
		}

		PostMessage(m_CameraWnd, iMsg, 0, 0);
	}
}

void UIMainWindow::RequestError(QJsonObject& error, bool bTrue)
{
	QString strError;
	if (error["code"].toInt() == 1002)
		strError = QString("授权过期,请重新登录！");
	else if (error["code"].toInt() == 1003)
		strError = QString("没有权限访问！");
	else if (error["code"].toInt() == 1004)
		strError = QString("授权失败,请重新登录！");
	else if (error["code"].toInt() == 3001)
		strError = QString("参数错误,请重新登录！");
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
		if (m_LoginWindow && bTrue)
			m_LoginWindow->ReturnLogin();
	}
}

void UIMainWindow::SendChangeStatusMsg(QString id)
{
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{id}/finish";
	strUrl.replace("{id}", id);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/lessons/{id}/finish";
	strUrl.replace("{id}", id);
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	QByteArray append("");
	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.put(request, append);
	connect(reply, &QNetworkReply::finished, this, &UIMainWindow::returnChangeStatus);
}

void UIMainWindow::returnChangeStatus()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1)
	{
		return;
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error, false);
	}
}

void UIMainWindow::HideSideScreen()
{
// 	if (m_CameraOrVideo->isHidden())
// 	{
// 		m_CameraOrVideo->setHidden(false);
// 
// 		m_charRoom->move(chat_X, 50 + 195);
// 		m_charRoom->resize(m_charRoom->width(), this->size().height() - 90 + 30 - 195);
// 	}
// 	else
// 	{
// 		m_CameraOrVideo->setHidden(true);
// 
// 		m_charRoom->move(chat_X, 50 + 20);
// 		m_charRoom->resize(m_charRoom->width(), m_charRoom->height() + 170);
// 	}
}

void UIMainWindow::timerEvent(QTimerEvent *event)
{
	if (m_SwichScreenTimerId != 0 && event->timerId() == m_SwichScreenTimerId)
	{
		killTimer(m_SwichScreenTimerId);
		m_SwichScreenTimerId = 0;
		PostMessage(m_VideoWnd, MSG_VIDEO_CHANGE_SIZE, m_cameraRect.width(), m_cameraRect.height());
		PostMessage(m_CameraWnd, MSG_VIDEO_CHANGE_SIZE, m_videoRect.width(), m_videoRect.height());
	}
}

void UIMainWindow::setPausedBtn()
{
	ui.Live_pushBtn->setText("继续直播");
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");
}

// 拖动标题做的处理
bool UIMainWindow::eventFilter(QObject *target, QEvent *event)
{
	if (target == ui.titel_pushButton || target == ui.titel1_pushButton)
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
		else if (event->type() == QEvent::MouseButtonRelease)
		{
			bool bTop = false;

			QRect rc;
			QRect rect;
			rect = this->geometry();
			rc.setRect(rect.x(), rect.y(), rect.width(), rect.height());
			if (rect.top() <= 0)
			{
				rect.setX(rc.x());
				rect.setY(0);
				move(rc.x(), 0);
				bTop = true;
			}

			if (rect.right() >= QApplication::desktop()->width())
			{
				if (bTop)
					move(QApplication::desktop()->width() - this->size().width(), 0);
				else
					move(QApplication::desktop()->width() - this->size().width(), rc.y());
			}
		}
		return false;
	}
	else if (target == ui.full_radioButton)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			if (ui.full_radioButton->isChecked())
				return false;
			
			bool bCheck = ui.close_radioButton->isChecked();
			m_RangeCapture->setShowOrHide(true);
			m_RangeCapture->ToFullDialog(bCheck);

			ShowWindow(m_VideoWnd, SW_SHOW);
			ui.close_radioButton->setEnabled(true);
		}
	}
	else if (target == ui.rect_radioButton)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			if (ui.rect_radioButton->isChecked())
				return false;
			
			if ( m_RangeCapture)
			{
				m_RangeCapture->setShowOrHide(true);
				m_RangeCapture->ToRectDialog();
				SetWindowPos((HWND)m_RangeCapture->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

				ShowWindow(m_VideoWnd, SW_SHOW);
			}
		}
	}
	else if (target == ui.close_radioButton)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			if (!ui.close_radioButton->isEnabled())
				return false;
			
			if (ui.rect_radioButton->isChecked())
				m_RangeCapture->SetCurRect();
			
			m_RangeCapture->setShowOrHide(false);

			ShowWindow(m_VideoWnd, SW_HIDE);
		}
	}
	return false;
}

void UIMainWindow::enterEvent(QEvent *)
{
	QRect rc;
	QRect rect;
	rect = this->geometry();
	rc.setRect(rect.x(), rect.y(), rect.width(), rect.height());
	if (rect.top() <= 0)
	{
		rect.setX(rc.x());
		rect.setY(0);
		move(rc.x(), -2);
	}
}

void UIMainWindow::leaveEvent(QEvent *)
{
// 	QRect rc;
// 	QRect rect;
// 	rect = this->geometry();
// 	if (rect.contains(QCursor::pos()))
// 		return;
// 
// 	rc.setRect(rect.x(), rect.y(), rect.width(), rect.height());
// 	if (rect.top() <= 0)
// 	{
// 		if (m_charRoom->IsFous())
// 		{
// 			return;
// 		}
// 		this->hide();
// 
// 		QDesktopWidget *dsk = QApplication::desktop();
// 		m_HoverWnd->move(dsk->width()-44,150);
// 		m_HoverWnd->show();
// 	}
}

void UIMainWindow::setNetworkPic(const QString &szUrl)
{
	QUrl url(szUrl);
	QNetworkAccessManager manager;
	QEventLoop loop;

	QNetworkReply *reply = manager.get(QNetworkRequest(url));
	//请求结束并下载完成后，退出子事件循环 
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//开启子事件循环 
	loop.exec();

	QByteArray jpegData = reply->readAll();
	QPixmap pixmap;
	pixmap.loadFromData(jpegData);
	QPixmap scaledPixmap = pixmap.scaled(QSize(70, 70), Qt::KeepAspectRatio);
	ui.teacherPhoto_Label->setPixmap(scaledPixmap);
	m_teacherImg = szUrl;
}

// 切换到直播页
void UIMainWindow::LivePage()
{
	ui.tabWidget->setCurrentIndex(0);
}

void UIMainWindow::setComeBack()
{
	m_SeeLiveWnd->hide();
	m_CameraInfo->setParent(ui.camera_widget, Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
	m_CameraInfo->show();
	m_CameraInfo->move(0, 0);
}

QString UIMainWindow::TeacherPhotoPixmap()
{
	return m_teacherImg;
}

void UIMainWindow::clickRectRadio()
{
	ui.rect_radioButton->setChecked(true);
}

void UIMainWindow::clickFullRadio()
{
	ui.full_radioButton->setChecked(true);
}

void UIMainWindow::setBoradCamera(QString sBoard, QString sCamera)
{
	m_sBoardRtmp = sBoard;
	m_sCemeraRtmp = sCamera;
}
void UIMainWindow::showErrorTip(QString sError)
{
	CMessageBox::showMessage(
		QString("答疑时间"),
		QString(sError),
		QString("确定"));
}

bool UIMainWindow::IsHasCamera()
{
	return bHasCamera;
}

void UIMainWindow::HelpDialog()
{
	if (m_HelpWord ==NULL)
	{
		m_HelpWord = new UIHelpWord();
		m_HelpWord->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
		m_HelpWord->show();
		SetWindowPos((HWND)m_HelpWord->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		return;
	}

	m_HelpWord->setVisible(!m_HelpWord->isVisible());
}