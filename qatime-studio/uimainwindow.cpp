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

#define MAINWINDOW_X_MARGIN 6
#define MAINWINDOW_Y_MARGIN 6
#define MAINWINDOW_TITLE_HEIGHT 49

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
{
	ui.setupUi(this);
	setFocusPolicy(Qt::ClickFocus);
	//	setFixedSize(QSize(985, 770));
	setMinimumSize(QSize(985, 720));
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rc, 0);
	a = 0;
	ui.person_pushButton->setFlat(true);
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

	m_VideoInfo = new UIVideo(this);
	m_VideoInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_VideoInfo->SetMainWnd(this);

	m_VideoInfo->move(15, 80);
	video_Width = 960;
	video_Heigth = 540;
	m_VideoInfo->resize(video_Width, video_Heigth);
	m_VideoInfo->hide();


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
	chat_Y = 50;
	chat_Width = m_charRoom->size().width();
	chat_Heigth = m_charRoom->size().height();
	m_charRoom->move(chat_X, chat_Y);
	m_charRoom->hide();

	

	m_MenuTool = new UIMenuTool(this);
	m_MenuTool->setWindowFlags(Qt::FramelessWindowHint);
	m_MenuTool->resize(video_Width - 50, 80);
	m_MenuTool->move(50, this->size().height() - 100);
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

//	ui.line_2->setVisible(false);
//	ui.video_checkBox->setCheckState(Qt::CheckState::Checked);
	m_MenuTool->setVideoCheckState(Qt::CheckState::Checked);
//	ui.time_label->setVisible(false);
	m_MenuTool->setTimeLabelVisible(false);
	ui.expansion_pushBtn->raise();
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
	ui.welcome_label->setText(strWelcome);
	ui.welcome_label_2->setText(strWelcome);

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
	ui.welcome_label->setText(strWelcome);
	ui.welcome_label_2->setText(strWelcome);

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

		if (!m_charRoom->isHidden())
		{
			m_charRoom->move(chat_X, 50);
			m_charRoom->resize(chat_Width, this->size().height() - 90 + 30);
		}
		m_MenuTool->resize(video_Width-50, 80);
		m_MenuTool->move(50, this->size().height() - 100);
		m_VideoInfo->resize(video_Width, this->size().height() - 180);
		m_AuxiliaryPanel->resize(m_AuxiliaryPanel->size().width(),this->size().height()-140);
		PostMessage(m_VideoWnd, MSG_VIDEO_CHANGE_SIZE, (WPARAM)video_Width, (LPARAM)(this->size().height() - 180));
	}
	a++;
	m_mutex.unlock();
	//TODO zp 添加改变子窗口的事件
	// 	QBitmap bmp(this->size());
	// 	bmp.fill();
	// 	QPainter p(&bmp);
	// 	p.setPen(Qt::NoPen);
	// 	p.setBrush(Qt::black);
	// 	p.drawRoundedRect(bmp.rect(), 10, 10);
	// 	setMask(bmp);
	// 	bmp.clear();
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
//				ui.Live_pushBtn->setText("开始直播");
				m_MenuTool->setLivePushBtnText("开始直播");
				m_VideoInfo->StopLiveVideo();
				SendVideoMsg((UINT)MSG_VIDEO_STOP_LIVE);
				SendStopLiveHttpMsg();

				if (m_CountTimer->isActive())
				{
					m_CountTimer->stop();					// 停止计时
					m_iTimerCount = 0;						// 重置秒数
// 					ui.time_label->setText("00:00:00");		// 重置时间
// 					ui.time_label->setVisible(false);		// 隐藏
					m_MenuTool->setTimeLabelText("00:00:00");
					m_MenuTool->setTimeLabelVisible(false);
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


// 			if (m_VideoInfo->m_videoSourceType == EN_NLSS_VIDEOIN_NONE)
// 			{
// 				CMessageBox::showMessage(
// 					QString("答疑时间"),
// 					QString("请选择要播放的视频源！"),
// 					QString("确定"));
// 				return;
// 			}
			
// 			QString url;
// 			url = m_AuxiliaryPanel->getURL();
			
// 			m_VideoInfo->setPlugFlowUrl(url);
 			m_VideoInfo->StartLiveVideo();
			SendVideoMsg((UINT)MSG_VIDEO_START_LIVE);
			ui.Live_pushBtn->setText("结束直播");
// =======
// 			if (m_VideoInfo->m_videoSourceType == EN_NLSS_VIDEOIN_NONE)
// 			{
// 				CMessageBox::showMessage(
// 					QString("答疑时间"),
// 					QString("请选择要播放的视频源！"),
// 					QString("确定"));
// 				return;
// 			}
// 
// 			QString url;
// 			url = m_AuxiliaryPanel->getURL();
// 
// 			m_VideoInfo->setPlugFlowUrl(url);
// 			m_VideoInfo->StartLiveVideo();
// //			ui.Live_pushBtn->setText("结束直播");
// 			m_MenuTool->setLivePushBtnText("结束直播");
// >>>>>>> Stashed changes
			SendStartLiveHttpMsg();

			m_CountTimer->start(1000);
			m_HeartTimer->start(1000 * 60 * 5);
		}

		m_AuxiliaryPanel->setPreview(!bLiving);
//		ui.video_widget->hide();
		m_VideoInfo->show();
	}
}

void UIMainWindow::VideoStatus(int iStatus)
{
	HideOtherUI();
	if (iStatus)
	{
//		m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_CAMERA;
//		m_VideoInfo->ChangeLiveVideo();
		m_VideoInfo->show();

//		ui.fullscreen_checkBox->setCheckState(Qt::CheckState::Unchecked);
		m_MenuTool->setFullScreenCheck(Qt::CheckState::Unchecked);
		m_bOtherApp = false;
		SendVideoMsg((UINT)MSG_VIDEO_CAMERA);
	}
	else
	{
		if (m_bOtherApp)
			return;

		// 如全屏在选中状态，则不隐藏窗口
//		if (!ui.fullscreen_checkBox->isChecked())
		if (!m_MenuTool->getFullScreenIsChecked())
		{
			m_VideoInfo->StopCaptureVideo();
			m_VideoInfo->hide();
		}
	}
}

void UIMainWindow::FullScreenStatus(int iStatus)
{
	HideOtherUI();
	if (iStatus)
	{
//		m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_FULLSCREEN;
		m_VideoInfo->show();

//		ui.video_checkBox->setCheckState(Qt::CheckState::Unchecked);
		m_MenuTool->setMenuCheckState(Qt::CheckState::Unchecked);

		m_bOtherApp = false;
		SendVideoMsg((UINT)MSG_VIDEO_FULLSCREEN);
	}
	else
	{
		if (m_bOtherApp)
			return;

		// 如视频头在选中状态，则不隐藏窗口
//		if (!ui.video_checkBox->isChecked())
		if (!m_MenuTool->getisChecked())
		{
			m_VideoInfo->StopCaptureVideo();
			m_VideoInfo->hide();
		}
	}
}

void UIMainWindow::InitAudioList()
{
	if (m_VideoInfo)
	{
		m_AudioChangeInfo->SetAudioInfo(m_VideoInfo->m_iAudioDeviceNum, m_VideoInfo->m_pAudioDevices);
	}

}

void UIMainWindow::InitVideoList()
{
	if (m_VideoInfo)
	{
		m_VideoChangeInfo->SetVideoInfo(m_VideoInfo->m_iVideoDeviceNum, m_VideoInfo->m_pVideoDevices);
	}
}

void UIMainWindow::clickChangeAudio(int)
{
	HideOtherUI(m_AudioChangeInfo);
	if (m_AudioChangeInfo && m_VideoInfo)
	{
// 		std::string str = "m_szData";
// 		COPYDATASTRUCT cpd;
// 
// 		cpd.cbData = str.length() + 1;
// 
// 		//GetLength()只是取得实际字符的长度，没有包括'\0'.
// 
// 		cpd.lpData = (void*)str.data();
// 
// 		HWND hWnd = FindWindow(L"NLSLiveForm", L"QATIME_HELPER");
// 		::SendMessage(hWnd, WM_COPYDATA, (WPARAM)this->winId(), (LPARAM)&cpd);
// 
// 		return;

		if (m_AudioChangeInfo->isVisible())
		{
			m_AudioChangeInfo->hide();
			return;
		}
// 		int x = ui.Audio_checkBox->geometry().x();
// 		int y = ui.Audio_checkBox->geometry().y();
//		m_AudioChangeInfo->move(QPoint(x - 10, y - 24 - m_VideoInfo->m_iAudioDeviceNum * 30));
		int x = video_Width /4;
		int y = video_Heigth;
		m_AudioChangeInfo->move(QPoint(x - 10, y));
		m_AudioChangeInfo->resize(m_AudioChangeInfo->geometry().width(), m_VideoInfo->m_iAudioDeviceNum * 30);
		m_AudioChangeInfo->show();
	}
}

void UIMainWindow::clickChangeVideo(int)
{
	HideOtherUI(m_VideoChangeInfo);
	if (m_VideoChangeInfo && m_VideoInfo)
	{
		if (m_VideoChangeInfo->isVisible())
		{
			m_VideoChangeInfo->hide();
			return;
		}
// 		int x = ui.video_checkBox->geometry().x();
// 		int y = ui.video_checkBox->geometry().y();
//		m_VideoChangeInfo->move(QPoint(x - 10, y - 24 - m_VideoInfo->m_iVideoDeviceNum * 30));
		int x = video_Width *3/8;
		int y = video_Heigth;
		m_VideoChangeInfo->move(QPoint(x - 10, y+20));
		m_VideoChangeInfo->resize(m_VideoChangeInfo->geometry().width(), m_VideoInfo->m_iVideoDeviceNum * 30);
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
		m_RatioChangeInfo->move(QPoint(15, size().height()-160));
		m_RatioChangeInfo->show();
	}
}

void UIMainWindow::AudioStatus(int iStatus)
{
	HideOtherUI();
	if (iStatus)
	{
		m_VideoInfo->SetPauseAudio();
	}
	else
	{
		m_VideoInfo->SetResumeAudio();
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
//		QRect MainRect = this->geometry();
//		QRect InfoRect = m_OtherAppInfo->geometry();
//		m_VideoInfo->EnumAvailableMediaDevices();
//		m_OtherAppInfo->setAppInfo(m_VideoInfo->m_pAppWinds, m_VideoInfo->m_iAppWindNum);
// 		m_OtherAppInfo->move((MainRect.width() - InfoRect.width()) / 2, (MainRect.height() - InfoRect.height()) / 2);
// 		if (m_OtherAppInfo->exec() != 0)
// 		{
// 			m_bOtherApp = true;
// 			m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_APP;
// 
// 			int index = m_OtherAppInfo->getCurrentIndex();
// 			m_VideoInfo->ChangeAppPath(index+1);
// 			m_VideoInfo->ChangeLiveVideo();
// 			m_VideoInfo->show();
// 
// 			// 当选中其他应用时，摄像头和全屏都不可用
// 			ui.video_checkBox->setCheckState(Qt::CheckState::Unchecked);
// 			ui.fullscreen_checkBox->setCheckState(Qt::CheckState::Unchecked);
// 		}
		SendVideoMsg(MSG_VIDEO_OTHREAPP);
// =======
// 		QRect MainRect = this->geometry();
// 		QRect InfoRect = m_OtherAppInfo->geometry();
// 		m_VideoInfo->EnumAvailableMediaDevices();
// 		m_OtherAppInfo->setAppInfo(m_VideoInfo->m_pAppWinds, m_VideoInfo->m_iAppWindNum);
// 		m_OtherAppInfo->move((MainRect.width() - InfoRect.width()) / 2, (MainRect.height() - InfoRect.height()) / 2);
// 		if (m_OtherAppInfo->exec() != 0)
// 		{
// 			m_bOtherApp = true;
// 			m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_APP;
// 
// 			int index = m_OtherAppInfo->getCurrentIndex();
// 			m_VideoInfo->ChangeAppPath(index + 1);
// 			m_VideoInfo->ChangeLiveVideo();
// 			m_VideoInfo->show();
// 
// 			// 当选中其他应用时，摄像头和全屏都不可用
// //			ui.video_checkBox->setCheckState(Qt::CheckState::Unchecked);
// //			ui.fullscreen_checkBox->setCheckState(Qt::CheckState::Unchecked);
// 			m_MenuTool->setVideoCheckState(Qt::CheckState::Unchecked);
// 			m_MenuTool->setFullScreenCheck(Qt::CheckState::Unchecked);
// 
// 		}
// >>>>>>> Stashed changes
	}
}

void UIMainWindow::slot_onCountTimeout()
{
	m_iTimerCount++;
	QString str = QString().sprintf("%02lld:%02lld:%02lld", m_iTimerCount / 3600, m_iTimerCount % 3600 / 60, m_iTimerCount % 60);
//	ui.time_label->setText(str);
//	ui.time_label->show();
	m_MenuTool->setTimeLabelText(str);
	m_MenuTool->setTimeLabelVisible(true);

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
			m_VideoInfo->show();

			// 当选中其他应用时，摄像头和全屏都不可用
			m_MenuTool->setVideoCheckState(Qt::CheckState::Unchecked);
			m_MenuTool->setFullScreenCheck(false);
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
// 		QString status = data["status"].toString();
// 		m_AuxiliaryPanel->ChangeLessonStatus(status);
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
// 		QString status = data["status"].toString();
// 		m_AuxiliaryPanel->ChangeLessonStatus(status);
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

void UIMainWindow::setAudioChangeIndex(int index)
{
	if (m_VideoInfo)
	{
		m_VideoInfo->m_CurrentMicIndex = index;
	}
}

void UIMainWindow::setVideoChangeIndex(int index)
{
	if (m_VideoInfo)
	{
		m_VideoInfo->m_CurrentVideoIndex = index;
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
//			ui.ratio_pushBtn->setText("标清");
			m_MenuTool->setRatioText("标清");
			m_VideoInfo->m_videoQ = EN_NLSS_VIDEOQUALITY_MIDDLE;
			break;
		}
		case 1:
		{
//			ui.ratio_pushBtn->setText("高清");
			m_MenuTool->setRatioText("高清");
			m_VideoInfo->m_videoQ = EN_NLSS_VIDEOQUALITY_HIGH;
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
		QString("是否重新登陆！"),
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
}

// 显示聊天窗口
void UIMainWindow::showChatRoomWnd()
{
	if (m_charRoom && !m_charRoom->isVisible())
	{
		//		ui.line_2->setVisible(true);
		//  	video_Width = this->size().width() - 240;
		//  	chat_X = this->size().width() - 255;
		// 		m_charRoom->move(this->size().width() - 305, 50);
		// 		m_VideoInfo->resize(this->size().width() - 500, this->size().height() - 180);
		resize(this->size().width() + chat_Width, this->height());
		m_VideoInfo->resize(video_Width-=chat_Width, video_Heigth);
//		m_charRoom->resize(chat_Width, chat_Heigth);
		m_charRoom->move(chat_X, 50);
		m_charRoom->show();
		m_MenuTool->resize(video_Width - 50, 80);
		m_MenuTool->move(50, this->size().height() - 100);

		QPoint closeQt = ui.mainclose_pushBtn->pos();
		ui.mainclose_pushBtn->move(QPoint(closeQt.x() + 295, closeQt.y()));

		QPoint minQt = ui.mainmin_pushBtn->pos();
		ui.mainmin_pushBtn->move(QPoint(minQt.x() + 295, minQt.y()));
		
		//		resize( chat_X + 135 + 150, 770);
		//		resize(1150+135+15, 770);
		//		setFixedSize(QSize(1150+135+15, 770));
	}
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

// 		int x = ui.lesson_pushButton->geometry().x();
// 		int y = ui.lesson_pushButton->geometry().y();
		int x = video_Width / 6 * 5;
		int y = video_Heigth;

		m_LessonTable->move(QPoint(x - 200, y - 200));
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
	//	ui.video_checkBox->setEnabled(bEnable);
	m_MenuTool->setVideoEnabled(bEnable);
	//	ui.fullscreen_checkBox->setEnabled(bEnable);
	m_MenuTool->setFullScreenEnabled(bEnable);
	//	ui.app_pushBtn->setEnabled(bEnable);
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

void UIMainWindow::SendVideoMsg(UINT iMsg)
{
	if (m_VideoWnd)
	{
		if (iMsg == MSG_VIDEO_START_LIVE)
		{
 			TCHAR szTempPath[MAX_PATH] = { 0 };
 			GetCurrentDirectory(MAX_PATH, szTempPath);
 			lstrcat(szTempPath, L"\\config.ini");
 			QString url = m_AuxiliaryPanel->getURL();
 			WritePrivateProfileString(L"CONFIG_PATH", L"LIVEURL", (LPCTSTR)url.utf16(), szTempPath);
		}
		
		PostMessage(m_VideoWnd, iMsg,0,0);
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
	if (iStatus == 1)
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
// void UIMainWindow::setLiveBtnEnable(bool bEnable)
// {
//	ui.video_checkBox->setEnabled(bEnable);
//	m_MenuTool->setVideoEnabled(bEnable);
//	ui.fullscreen_checkBox->setEnabled(bEnable);
//	m_MenuTool->setFullScreenEnabled(bEnable);
//	ui.app_pushBtn->setEnabled(bEnable);
//	m_MenuTool->setLessonEnabled(bEnable);

}