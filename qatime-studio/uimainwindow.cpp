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

#define MAINWINDOW_X_MARGIN 6
#define MAINWINDOW_Y_MARGIN 6
#define MAINWINDOW_TITLE_HEIGHT 49

#ifdef TEST
	#define _DEBUG
#else
#endif
UIMainWindow::UIMainWindow(QWidget *parent)
	: QWidget(parent)
	, m_OtherAppInfo(NULL)
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
	, m_SideScreenTool(NULL)
	, m_SwichScreenTimerId(0)
	, m_VideoOrCamera(NULL)
	, m_CameraOrVideo(NULL)
	, m_ShowChatRoomTimerId(0)
{
	ui.setupUi(this);
	setFocusPolicy(Qt::ClickFocus);
	//	setFixedSize(QSize(985, 770));
	setMinimumSize(QSize(985, 720));
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rc, 0);
	a = 0;
	ui.person_pushButton->setFlat(true);
//	ui.label->hide();
//	ui.lesson_label->setText()
	connect(ui.mainmin_pushBtn, SIGNAL(clicked()), this, SLOT(MinDialog()));
	connect(ui.MinMax_pushBtn, SIGNAL(clicked()), this, SLOT(MaxDialog()));
	connect(ui.mainclose_pushBtn, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.expansion_pushBtn, SIGNAL(clicked()), this, SLOT(Expansion()));
// 	connect(ui.Live_pushBtn, SIGNAL(clicked()), this, SLOT(slot_startOrStopLiveStream()));
// 	connect(ui.Audio_checkBox, SIGNAL(stateChanged(int)), this, SLOT(AudioStatus(int)));
// 	connect(ui.video_checkBox, SIGNAL(stateChanged(int)), this, SLOT(VideoStatus(int)));
// 	connect(ui.fullscreen_checkBox, SIGNAL(stateChanged(int)), this, SLOT(FullScreenStatus(int)));
// 	connect(ui.app_pushBtn, SIGNAL(clicked()), this, SLOT(OtherApp()));
// 	connect(ui.AudioCorner_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(clickChangeAudio(int)));
// 	connect(ui.videoCorner_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(clickChangeVideo(int)));
// 	connect(ui.ratio_pushBtn, SIGNAL(clicked()), this, SLOT(clickChangeRatio()));
// 	connect(ui.lesson_pushButton, SIGNAL(clicked()), this, SLOT(clickLessonList()));
	ui.lesson_pushButton->hide();
	ui.ratio_pushBtn->hide();
	ui.videoCorner_pushBtn->hide();
	ui.AudioCorner_pushBtn->hide();
	ui.app_pushBtn->hide();
	ui.fullscreen_checkBox->hide();
	ui.video_checkBox->hide();
	ui.Audio_checkBox->hide();
	ui.Live_pushBtn->hide();
	ui.time_label->hide();

	m_CameraInfo = new UICamera(this);
	m_CameraInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_CameraInfo->SetMainWindow(this);
	m_CameraInfo->show();
	m_CameraOrVideo = m_CameraInfo;

	m_SideScreenTool = new UISideScreen(this);
	m_SideScreenTool->setWindowFlags(Qt::FramelessWindowHint);
	m_SideScreenTool->SetMainWindow(this);
	m_SideScreenTool->show();
	connect(m_SideScreenTool, SIGNAL(emit_HideSideScreen()), this, SLOT(HideSideScreen()));
	connect(m_SideScreenTool, SIGNAL(emit_SwichScreen()), this, SLOT(SwichScreen()));

	m_VideoInfo = new UIVideo(this);
	m_VideoInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_VideoInfo->SetMainWnd(this);

	m_VideoInfo->move(15, 70);
	video_Width = 960;
	video_Heigth = 540;
	m_VideoInfo->resize(video_Width, video_Heigth);
	m_VideoInfo->show();
	m_VideoOrCamera = m_VideoInfo;

	m_OtherAppInfo = new UIOtherApp(this);
	m_OtherAppInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_OtherAppInfo->hide();

	m_AuxiliaryPanel = new UIAuxiliaryPanel(this);
	m_AuxiliaryPanel->setWindowFlags(Qt::FramelessWindowHint);
	m_AuxiliaryPanel->setParent(this);
	m_AuxiliaryPanel->move(5, 40);
	m_AuxiliaryPanel->resize(590, 580);
	m_AuxiliaryPanel->hide();

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

	// 直播按钮
	ui.Live_pushBtn->setText("开始直播");
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: red;border-radius: 10px; }"
		"QPushButton{border: 2px groove gray; border - style: outset;}"
		"QPushButton:pressed{border-style: inset; }");

	// 计时器 改变直播时间
	m_CountTimer = new QTimer(this);
	connect(m_CountTimer, SIGNAL(timeout()), this, SLOT(slot_onCountTimeout()));

	// 直播开始后每5分钟发送一次心跳
	m_HeartTimer = new QTimer(this);
	connect(m_HeartTimer, SIGNAL(timeout()), this, SLOT(slot_onHeartTimeout()));

	m_ShowVideoTimer = new QTimer(this);
	connect(m_ShowVideoTimer, SIGNAL(timeout()), this, SLOT(setVideoPos()));

	m_ShowCameraTimer = new QTimer(this);
	connect(m_ShowCameraTimer, SIGNAL(timeout()), this, SLOT(setCameraPos()));

	m_tempTimers = new QTimer(this);
	connect(m_tempTimers, SIGNAL(timeout()), this, SLOT(slot_onTempTimeout()));

	// 面板展开按钮初始位置
	ui.expansion_pushBtn->move(QPoint(5, 290));

	// 设置摄像头视频源样式
	ui.video_checkBox->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 32px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/camera_close.png);}"
		"QCheckBox::indicator:checked{image: url(./images/camera_open.png);}");

	// 设置麦克风样式
	ui.Audio_checkBox->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 32px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/mic_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/mic_close.png);}");

	// 设置抓取全屏样式
	ui.fullscreen_checkBox->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 32px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/screen_close.png);}"
		"QCheckBox::indicator:checked{image: url(./images/screen_open.png);}");

	ui.AudioCorner_pushBtn->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 16px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/corner.png);}"
		"QCheckBox::indicator:checked{image: url(./images/corner.png);}"
		"QCheckBox::indicator:unchecked:pressed{image: url(./images/corner_push.png);}"
		"QCheckBox::indicator:checked:pressed{image: url(./images/corner_push.png);}");

	ui.videoCorner_pushBtn->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 16px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/corner.png);}"
		"QCheckBox::indicator:checked{image: url(./images/corner.png);}"
		"QCheckBox::indicator:unchecked:pressed{image: url(./images/corner_push.png);}"
		"QCheckBox::indicator:checked:pressed{image: url(./images/corner_push.png);}");

	InitAudioList();
	InitVideoList();

	QPixmap pixmap(QCoreApplication::applicationDirPath() + "/images/btn_07.png");
	QPixmap pixmap1(QCoreApplication::applicationDirPath() + "/images/btn_off.png");
	QPixmap pixmap2(QCoreApplication::applicationDirPath() + "/images/btn_08.png");
	ui.mainmin_pushBtn->setPixmap(pixmap, 4);
	ui.mainclose_pushBtn->setPixmap(pixmap1, 4);
	ui.MinMax_pushBtn->setPixmap(pixmap2,4);
	m_charRoom = new UIChatRoom(this);
	m_charRoom->setWindowFlags(Qt::FramelessWindowHint);
	chat_X = this->size().width()-m_charRoom->size().width()-10;
	chat_Y = 45+190;//45
	chat_Width = m_charRoom->size().width();
	chat_Heigth = m_charRoom->size().height();
	m_charRoom->move(chat_X, chat_Y);
	m_charRoom->resize(chat_Width, this->size().height() - 90 + 30 - 190);
	m_charRoom->show();

	m_CameraInfo->move(chat_X + 20, 50 + 20);
	m_CameraInfo->resize(302, 169);

	m_SideScreenTool->move(chat_X + 20, 50);
	m_SideScreenTool->resize(302, 20);

	m_MenuTool = new UIMenuTool(this);
	m_MenuTool->setWindowFlags(Qt::FramelessWindowHint);
	m_MenuTool->resize(video_Width - 50, 80);	
	m_MenuTool->move(20, this->size().height() - 100);
	m_MenuTool->show();
	connect(m_MenuTool, SIGNAL(emit_startOrStopLiveStream()), this, SLOT(slot_startOrStopLiveStream()));
	connect(m_MenuTool, SIGNAL(emit_AudioStatus(int)), this, SLOT(AudioStatus(int)));
	connect(m_MenuTool, SIGNAL(emit_VideoStatus(int)), this, SLOT(VideoStatus(int)));
	connect(m_MenuTool, SIGNAL(emit_FullScreenStatus(int)), this, SLOT(FullScreenStatus(int)));
	connect(m_MenuTool, SIGNAL(emit_OtherApp(int)), this, SLOT(OtherApp(int)));
	connect(m_MenuTool, SIGNAL(emit_clickChangeAudio(int)), this, SLOT(clickChangeAudio(int)));
	connect(m_MenuTool, SIGNAL(emit_clickChangeVideo(int)), this, SLOT(clickChangeVideo(int)));
	connect(m_MenuTool, SIGNAL(emit_clickChangeRatio()), this, SLOT(clickChangeRatio()));
	connect(m_MenuTool, SIGNAL(emit_clickLessonList()), this, SLOT(clickLessonList()));
	connect(m_MenuTool, SIGNAL(emit_FulSereen(int)), this, SLOT(fullSereen(int)));
	m_MenuTool->InitMoveLiveBtn();
	m_MenuTool->setVideoCheckState(Qt::CheckState::Checked);
	m_MenuTool->setFullScreenCheck(Qt::CheckState::Checked);
	ui.domain_label->setVisible(true);

//	m_ShowChatRoomTimerId = startTimer(300);
}

UIMainWindow::~UIMainWindow()
{
	if (m_OtherAppInfo)
	{
		delete m_OtherAppInfo;
		m_OtherAppInfo = NULL;
	}

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

	//删除定时器
	if (m_HeartTimer)
	{
		if (m_HeartTimer->isActive())
			m_HeartTimer->stop();
		delete m_HeartTimer;
		m_HeartTimer = NULL;
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

	if (m_SideScreenTool)
	{
		delete m_SideScreenTool;
		m_SideScreenTool = NULL;
	}
}
void UIMainWindow::fullSereen(int b)
{		
	if (b)
	{
		ui.label->hide();
		ui.domain_label->setVisible(false);
		m_charRoom->setHidden(b);
		video_Width = m_charRoom->size().width() + m_VideoOrCamera->size().width();
		video_Heigth = m_VideoOrCamera->size().height();
		m_VideoOrCamera->resize(video_Width, video_Heigth);
		if (m_VideoOrCamera == m_VideoInfo)
			PostMessage(m_VideoWnd, MSG_VIDEO_CHANGE_SIZE, m_VideoOrCamera->size().width(), m_VideoOrCamera->size().height());
		else
			PostMessage(m_CameraWnd, MSG_VIDEO_CHANGE_SIZE, m_VideoOrCamera->size().width(), m_VideoOrCamera->size().height());
		m_MenuTool->resize(m_charRoom->size().width() + m_MenuTool->size().width(), m_MenuTool->size().height());
		m_MenuTool->moveLiveBtn();
		m_SideScreenTool->setHidden(true);
		m_CameraOrVideo->setHidden(true);
	}
	else
	{
		ui.label->show();
		ui.domain_label->setVisible(true);
		m_charRoom->setHidden(b);
		video_Width = m_VideoOrCamera->size().width() - m_charRoom->size().width();
		video_Heigth = m_VideoOrCamera->size().height();
		m_VideoOrCamera->resize(m_VideoOrCamera->size().width() - m_charRoom->size().width(), m_VideoOrCamera->size().height());
		if (m_VideoOrCamera == m_VideoInfo)
			PostMessage(m_VideoWnd, MSG_VIDEO_CHANGE_SIZE, m_VideoOrCamera->size().width(), m_VideoOrCamera->size().height());
		else
			PostMessage(m_CameraWnd, MSG_VIDEO_CHANGE_SIZE, m_VideoOrCamera->size().width(), m_VideoOrCamera->size().height());
		m_MenuTool->resize(m_MenuTool->size().width() - m_charRoom->size().width() , m_MenuTool->size().height());
		m_MenuTool->moveLiveBtn();
		m_SideScreenTool->setHidden(false);
		m_CameraOrVideo->setHidden(false);
	}

	resizeEvent(NULL);
//	m_tempTimers->start(100);
}
void UIMainWindow::MinDialog()
{
	showMinimized();
}

void UIMainWindow::MaxDialog()
{
	if (this->isMaximized())
	{
		showNormal();
		resizeEvent(NULL);
	}
	else
	{
		showMaximized();
		resizeEvent(NULL);
	}
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
		SendStopLiveHttpMsg(false);
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
	m_AuxiliaryPanel->setTeacherName(teacherName);

	QString strWelcome = "欢迎 ";
	strWelcome += teacherName;
	strWelcome += " 老师登录答疑时间，祝您直播愉快！";
	ui.lesson_label->setText(strWelcome);
//	ui.welcome_label_2->setText(strWelcome);

	// 设置老师头像
	QString teacherPhoto_url = data["avatar_url"].toString();
	m_AuxiliaryPanel->setNetworkPic(teacherPhoto_url);

	// 老师云信信息
	m_charRoom->setChatInfo(data["chat_account"].toObject(), mRemeberToken);

	m_LessonTable->InitToken(mRemeberToken, m_teacherID);
}

void UIMainWindow::setAutoTeacherInfo(QString teacherID, QString teacherName, QString teacherUrl, QString accid, QString token)
{
	m_teacherID = teacherID;
	m_AuxiliaryPanel->setTeacherID(m_teacherID);

	// 设置老师名字
	m_AuxiliaryPanel->setTeacherName(teacherName);

	QString strWelcome = "欢迎 ";
	strWelcome += teacherName;
	strWelcome += " 老师登录答疑时间，祝您直播愉快！";
	ui.lesson_label->setText(strWelcome);
//	ui.welcome_label_2->setText(strWelcome);

	// 设置老师头像
	m_AuxiliaryPanel->setNetworkPic(teacherUrl);

	// 老师云信信息
	QJsonObject data;
	data["accid"] = accid;
	data["token"] = token;
	m_charRoom->setChatInfo(data, mRemeberToken);

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
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/teachers/{teacher_id}/courses/full";
	strUrl.replace("{teacher_id}", m_teacherID);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/teachers/{teacher_id}/courses/full";
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

void UIMainWindow::Expansion()
{
	HideOtherUI();
	m_AuxiliaryPanel->setVisible(true);
}

void UIMainWindow::resizeEvent(QResizeEvent *e)
{
	m_mutex.lock();
	int w, h;
	if (a > 0 && e != NULL)
	{
		w = e->size().width() - e->oldSize().width();
		h = e->size().height() - e->oldSize().height();
		chat_X += w;
		video_Width += w;
		video_Heigth += h;
		chat_Heigth += h;
	}
	if (a > 1 || e == NULL)
	{

		if (!m_SideScreenTool->isHidden())
		{
			m_SideScreenTool->move(chat_X + 20, 50);
			m_SideScreenTool->resize(302, 20);
		}

		m_CameraOrVideo->move(chat_X + 20, 50 + 20);
		m_CameraOrVideo->resize(302, 169);

		if (!m_charRoom->isHidden())
		{
			if (m_CameraOrVideo->isHidden())
			{
				m_charRoom->move(chat_X, 50 + 20);
				m_charRoom->resize(chat_Width, this->size().height() - 90 + 30 - 20);
			}
			else
			{
				m_charRoom->move(chat_X, 50 + 190);
				m_charRoom->resize(chat_Width, this->size().height() - 90 + 30 - 190);
			}
		}
		
		m_MenuTool->resize(video_Width-50, 80);
		m_MenuTool->move(20, this->size().height() - 100);
		m_MenuTool->moveLiveBtn();
		m_VideoOrCamera->resize(video_Width, this->size().height() - 180);
		m_AuxiliaryPanel->resize(m_AuxiliaryPanel->size().width(),this->size().height()-140);
		if (m_VideoOrCamera == m_VideoInfo)
			PostMessage(m_VideoWnd, MSG_VIDEO_CHANGE_SIZE, (WPARAM)video_Width, (LPARAM)(this->size().height() - 180));
		else
			PostMessage(m_CameraWnd, MSG_VIDEO_CHANGE_SIZE, (WPARAM)video_Width, (LPARAM)(this->size().height() - 180));
	}
	a++;
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
				m_MenuTool->setLivePushBtnText(true);
				m_VideoInfo->StopLiveVideo();
				SendVideoMsg((UINT)MSG_VIDEO_STOP_LIVE);
				SendCameraMsg((UINT)MSG_VIDEO_STOP_LIVE);
				SendStopLiveHttpMsg();

				if (m_CountTimer->isActive())
				{
					m_CountTimer->stop();					// 停止计时
					m_iTimerCount = 0;						// 重置秒数
					m_MenuTool->setTimeLabelVisible(false); // 隐藏
					m_MenuTool->setTimeLabelText("00:00:00");// 重置时间
				}

				if (m_HeartTimer->isActive())
				{
					m_HeartTimer->stop();					// 停止发送心跳
				}
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
					QString("请选择您要开始的课程再进行直播！"),
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

 			m_VideoInfo->StartLiveVideo();
			SendVideoMsg((UINT)MSG_VIDEO_START_LIVE);
			SendCameraMsg((UINT)MSG_VIDEO_START_LIVE);
			m_MenuTool->setLivePushBtnText(false);
			m_MenuTool->setTimeLabelVisible(true);
			SendStartLiveHttpMsg();

			m_CountTimer->start(1000);
			m_HeartTimer->start(1000 * 60 * 5);
		}

		m_AuxiliaryPanel->setPreview(!bLiving);
		m_VideoInfo->show();
	}
}

void UIMainWindow::VideoStatus(int iStatus)
{
	HideOtherUI();
	if (iStatus)
	{
		ShowWindow(m_CameraWnd, SW_SHOW);

		if (m_CameraOrVideo == m_CameraInfo)
			m_SideScreenTool->ChangeTip("摄像头开启中");
	}
	else
	{
		ShowWindow(m_CameraWnd, SW_HIDE);

		if (m_CameraOrVideo == m_CameraInfo)
			m_SideScreenTool->ChangeTip("摄像头关闭中");
	}
}

void UIMainWindow::FullScreenStatus(int iStatus)
{
	HideOtherUI();
	if (iStatus)
	{
		ShowWindow(m_VideoWnd, SW_SHOW);

		m_bOtherApp = false;
		SendVideoMsg((UINT)MSG_VIDEO_FULLSCREEN);

		//改变副屏幕上的状态
		if (m_CameraOrVideo == m_VideoInfo)
			m_SideScreenTool->ChangeTip("主直播开启中");
	}
	else
	{
		if (m_bOtherApp)
			return;

		ShowWindow(m_VideoWnd, SW_HIDE);

		if (m_CameraOrVideo == m_VideoInfo)
			m_SideScreenTool->ChangeTip("主直播关闭中");
	}
}

void UIMainWindow::InitAudioList()
{
	if (m_VideoInfo)
	{
//		m_AudioChangeInfo->SetAudioInfo(m_VideoInfo->m_iAudioDeviceNum, m_VideoInfo->m_pAudioDevices);
	}

}

void UIMainWindow::InitVideoList()
{
	if (m_VideoInfo)
	{
//		m_VideoChangeInfo->SetVideoInfo(m_VideoInfo->m_iVideoDeviceNum, m_VideoInfo->m_pVideoDevices);
	}
}

void UIMainWindow::clickChangeAudio(int)
{
	if (m_AudioChangeInfo->iCount == 0)
		PostMessage(m_VideoWnd, MSG_DEVICE_AUDIO, 0, 0);

	HideOtherUI(m_AudioChangeInfo);
	if (m_AudioChangeInfo && m_VideoInfo)
	{
		if (m_AudioChangeInfo->isVisible())
		{
			m_AudioChangeInfo->hide();
			return;
		}

		m_AudioChangeInfo->show();
	}
}

void UIMainWindow::clickChangeVideo(int)
{
	if (m_VideoChangeInfo->iCount == 0)
		PostMessage(m_VideoWnd, MSG_DEVICE_VIDEO, 0, 0);

	HideOtherUI(m_VideoChangeInfo);
	if (m_VideoChangeInfo)
	{
		if (m_VideoChangeInfo->isVisible())
		{
			m_VideoChangeInfo->hide();
			return;
		}

		m_VideoChangeInfo->show();
	}
}

void UIMainWindow::clickChangeRatio()
{
	HideOtherUI(m_RatioChangeInfo);
	if (m_RatioChangeInfo)
	{
		if (m_RatioChangeInfo->isVisible())
		{
			m_RatioChangeInfo->hide();
			return;
		}

		int x = ui.ratio_pushBtn->geometry().x();
 		int y = ui.ratio_pushBtn->geometry().y();
		m_RatioChangeInfo->move(QPoint(15, size().height()-170));
		m_RatioChangeInfo->show();
	}
}

void UIMainWindow::AudioStatus(int iStatus)
{
	HideOtherUI();
	if (iStatus)
	{
		m_VideoInfo->SetPauseAudio(m_VideoWnd);
	}
	else
	{
		m_VideoInfo->SetResumeAudio(m_VideoWnd);
	}
}

void UIMainWindow::OtherApp(int i)
{
	HideOtherUI();
	if (m_OtherAppInfo->isVisible())
	{
		m_OtherAppInfo->hide();
		return;
	}

	if (m_OtherAppInfo && m_VideoInfo)
	{
		SendVideoMsg(MSG_VIDEO_OTHREAPP);
	}
}

void UIMainWindow::slot_onCountTimeout()
{
	m_iTimerCount++;
	QString str = QString().sprintf("%02lld:%02lld:%02lld", m_iTimerCount / 3600, m_iTimerCount % 3600 / 60, m_iTimerCount % 60);
	m_MenuTool->setTimeLabelText(str);
	m_MenuTool->setTimeLabelVisible(true);
}

void UIMainWindow::slot_onTempTimeout()
{
//	m_tempTimers->stop();

// 	if (m_charRoom->isHidden())
// 	{
// 		showMaximized();
// 		resizeEvent(NULL);
// 	}
// 	else
// 	{
// 		showNormal();
// 		resizeEvent(NULL);
// 	}
}

void UIMainWindow::slot_onHeartTimeout()
{
	SendHeartBeatHttpMsg();
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
				m_charRoom->ReceiverMsg(pIMsg);

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

			// 当选中其他应用时，全屏不可用
			m_MenuTool->setFullScreenCheck(Qt::CheckState::Unchecked);
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
			
			if (pMsg->wParam == 0)
			{
				m_VideoChangeInfo->SetVideoInfo(iCount, QString(QLatin1String(m_result.friendly_name_)), QString(QLatin1String(m_result.device_path_)));
				m_VideoChangeInfo->resize(m_VideoChangeInfo->geometry().width(), iCount * 30);
				m_VideoChangeInfo->move(QPoint(15, size().height() - 110 - iCount * 30));
				m_VideoChangeInfo->show();
			}
			else if (pMsg->wParam == 1)
			{
				m_AudioChangeInfo->SetAudioInfo(iCount, QString::fromStdString(m_result.friendly_name_), QString::fromStdString(m_result.device_path_));
				m_AudioChangeInfo->resize(m_VideoChangeInfo->geometry().width(), iCount * 30);
				m_AudioChangeInfo->move(QPoint(15, size().height() - 110 - iCount * 30));
				m_AudioChangeInfo->show();
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

					*result = HTCAPTION;
				}
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

void UIMainWindow::SendStartLiveHttpMsg()
{
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_start";
	strUrl.replace("{lessons_id}", m_AuxiliaryPanel->getLessonID());
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_start";
	strUrl.replace("{lessons_id}", m_AuxiliaryPanel->getLessonID());
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIMainWindow::FinishStartLive);
}

void UIMainWindow::FinishStartLive()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1 && data.contains("live_token"))
	{
		m_liveToken = data["live_token"].toString();
		SendRequestStatus();
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
}

void UIMainWindow::SendHeartBeatHttpMsg()
{
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/heartbeat?token={token}";
	strUrl.replace("{lessons_id}", m_AuxiliaryPanel->getLessonID());
	strUrl.replace("{token}", m_liveToken);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/heartbeat?token={token}";
	strUrl.replace("{lessons_id}", m_AuxiliaryPanel->getLessonID());
	strUrl.replace("{token}", m_liveToken);
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
}

void UIMainWindow::SendStopLiveHttpMsg(bool bConnect)
{
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_end";
	strUrl.replace("{lessons_id}", m_AuxiliaryPanel->getLessonID());
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_end";
	strUrl.replace("{lessons_id}", m_AuxiliaryPanel->getLessonID());
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	if (bConnect)
		connect(reply, &QNetworkReply::finished, this, &UIMainWindow::FinishStopLive);
}

void UIMainWindow::FinishStopLive()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1 )
	{
		SendRequestStatus();
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
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
	painter.fillPath(path, QBrush(Qt::white));

	QColor color(102, 204, 204);
	for (int i = 0; i < 4; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);

		for (int j = 0; j < 40; j++)
		{
			path.addRect(4 - i, j - i, this->width() - (4 - i) * 2, this->height() - (2 - i) * 2);
		}

		painter.setPen(color);
		painter.drawPath(path);
	}
}

void UIMainWindow::focusInEvent(QFocusEvent *e)
{
	HideOtherUI();
}

void UIMainWindow::HideOtherUI(QWidget* self)
{
	if (m_AuxiliaryPanel &&
		m_RatioChangeInfo &&
		m_AudioChangeInfo &&
		m_VideoChangeInfo &&
		m_LessonTable)
	{
		if (self != NULL)
		{
			if (m_RatioChangeInfo != self)
				m_RatioChangeInfo->hide();
			if (m_AudioChangeInfo != self)
				m_AudioChangeInfo->hide();
			if (m_VideoChangeInfo != self)
				m_VideoChangeInfo->hide();
			if (m_LessonTable != self)
				m_LessonTable->hide();

			m_AuxiliaryPanel->hide();
		}
		else
		{
			m_RatioChangeInfo->hide();
			m_AudioChangeInfo->hide();
			m_VideoChangeInfo->hide();
			m_AuxiliaryPanel->hide();
			m_LessonTable->hide();
		}
	}
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
		m_VideoInfo->SetChangeVideoAudio(m_VideoWnd, path);
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
		SendStopLiveHttpMsg(false);

		if (m_LoginWindow)
			m_LoginWindow->ReturnLogin();
	}
}

void UIMainWindow::setCurChatRoom(QString chatID, QString courseid)
{
	m_MenuTool->setFullEnabled();
	if (m_charRoom)
	{
		ui.welcome_label_2->setVisible(false);
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
		ui.person_pushButton->setIcon(QIcon("./images/eyes.png"));
		ui.person_pushButton->setIconSize(QSize(16, 16));
		ui.person_pushButton->setText(QString::number(i));

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
	ui.welcome_label_2->setText("");
	ui.welcome_label_2->show();

	CMessageBox::showMessage(QString("答疑时间"), QString("您已成功进入直播教室！"), QString("确定"), QString(""), NULL, true);
}

// 显示聊天窗口
void UIMainWindow::showChatRoomWnd()
{
	if (m_charRoom && !m_charRoom->isVisible())
	{
//		ui.label->show();
		resize(this->size().width() + chat_Width, this->height());
		m_VideoOrCamera->resize(video_Width-=chat_Width, video_Heigth);
		m_charRoom->move(chat_X, 50+190);
		m_charRoom->show();		
		m_MenuTool->resize(video_Width - 50, 80);
		m_MenuTool->move(20, this->size().height() - 100);
		m_MenuTool->moveLiveBtn();

		QPoint closeQt = ui.mainclose_pushBtn->pos();
		ui.mainclose_pushBtn->move(QPoint(closeQt.x() + 295, closeQt.y()));

		QPoint minQt = ui.mainmin_pushBtn->pos();
		ui.mainmin_pushBtn->move(QPoint(minQt.x() + 295, minQt.y()));
		ui.domain_label->setVisible(true);
	}

	if (m_SideScreenTool)
	{
		m_SideScreenTool->move(chat_X + 20, 50);
		m_SideScreenTool->resize(302, 20);
		m_SideScreenTool->show();
	}

	if (m_CameraOrVideo)
	{
		m_CameraOrVideo->move(chat_X + 20, 50 + 20);
		m_CameraOrVideo->resize(302, 169);
		m_CameraOrVideo->show();
	}
	
// 	static bool bInit = false;
// 	if (!bInit)
// 	{
// 		int x = QApplication::desktop()->width() - this->width() - m_charRoom->width();
// 		chat_X += x;
// 		video_Width += x;
// 		showMaximized();
// 		bInit = true;
// 	}
// 
// 	CMessageBox::showMessage(QString("答疑时间"),QString("您已成功进入直播教室！"),QString("确定"),QString(""),NULL,true);
}

void UIMainWindow::clickLessonList()
{
	HideOtherUI(m_LessonTable);
	if (m_LessonTable)
	{
		if (m_LessonTable->isVisible())
		{
			m_LessonTable->hide();
			return;
		}

		int x = video_Width / 6 * 5;
		int y = video_Heigth;

		m_LessonTable->move(QPoint(x - 250, size().height() - 370));
		m_LessonTable->RequestLesson();
		m_LessonTable->show();
	}
}

// 课程表中选择课程——关联到辅导班
void UIMainWindow::LessonTable_Auxiliary(QString sLessonID, QString sCourseID)
{
	m_AuxiliaryPanel->ergodicItem(sLessonID, sCourseID);
}

void UIMainWindow::setLiveBtnEnable(bool bEnable)
{
	m_MenuTool->setVideoEnabled(bEnable);
	m_MenuTool->setFullScreenEnabled(bEnable);
	m_MenuTool->setLessonEnabled(bEnable);
}

void UIMainWindow::setVideoPos()
{
	if (m_VideoWnd)
	{
		if (m_ShowVideoTimer->isActive())
			m_ShowVideoTimer->stop();
		MoveWindow(m_VideoWnd, 0, 0, 960, 540, true);
	}
}

void UIMainWindow::setCameraPos()
{
	if (m_CameraWnd)
	{
		if (m_ShowCameraTimer->isActive())
			m_ShowCameraTimer->stop();
		MoveWindow(m_CameraWnd, 0, 0, 302, 169, true);
	}
}

void UIMainWindow::SendVideoMsg(UINT iMsg)
{
	if (m_VideoWnd)
	{
		if (iMsg == MSG_VIDEO_START_LIVE)
		{
			QString url = m_AuxiliaryPanel->getURL();

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
	if (m_CameraOrVideo->isHidden())
	{
		m_CameraOrVideo->setHidden(false);

		m_charRoom->move(chat_X, 50 + 190);
		m_charRoom->resize(m_charRoom->width(), this->size().height() - 90 + 30 - 190);
	}
	else
	{
		m_CameraOrVideo->setHidden(true);

		m_charRoom->move(chat_X, 50 + 20);
		m_charRoom->resize(m_charRoom->width(), m_charRoom->height() + 170);
	}
}

// 切换屏幕
void UIMainWindow::SwichScreen()
{
	if (m_VideoOrCamera == m_VideoInfo)
	{
		m_VideoOrCamera = m_CameraInfo;
		m_CameraOrVideo = m_VideoInfo;
	}
	else
	{
		m_CameraOrVideo = m_CameraInfo;
		m_VideoOrCamera = m_VideoInfo;
	}

	m_videoRect = m_VideoInfo->geometry();
	m_cameraRect = m_CameraInfo->geometry();

	m_VideoInfo->setGeometry(m_cameraRect);
	m_CameraInfo->setGeometry(m_videoRect);

	m_SwichScreenTimerId = startTimer(100);

	//改变副屏幕上的状态
	if (m_CameraOrVideo == m_VideoInfo)
	{
		if (IsWindowVisible(m_VideoWnd))
			m_SideScreenTool->ChangeTip("主直播开启中");
		else
			m_SideScreenTool->ChangeTip("主直播关闭中");
	}
	else
	{
		if (IsWindowVisible(m_CameraWnd))
			m_SideScreenTool->ChangeTip("摄像头开启中");
		else
			m_SideScreenTool->ChangeTip("摄像头关闭中");
	}
	
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

