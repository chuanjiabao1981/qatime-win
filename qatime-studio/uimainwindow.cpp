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
#include "UIMessageBox.h"
#include <QMovie>
#include "define.h"
#include "member.h"
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
{
	ui.setupUi(this);
	setFocusPolicy(Qt::ClickFocus);
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(255, 255, 255));
	//palette.setBrush(QPalette::Background, QBrush(QPixmap(":/background.png")));
	setPalette(palette);

	connect(ui.mainmin_pushBtn, SIGNAL(clicked()), this, SLOT(MinDialog()));
	connect(ui.mainclose_pushBtn, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.expansion_pushBtn, SIGNAL(clicked()), this, SLOT(Expansion()));
	connect(ui.Live_pushBtn, SIGNAL(clicked()), this, SLOT(slot_startOrStopLiveStream()));
	connect(ui.Audio_checkBox, SIGNAL(stateChanged(int)), this, SLOT(AudioStatus(int)));
	connect(ui.video_checkBox, SIGNAL(stateChanged(int)), this, SLOT(VideoStatus(int)));
	connect(ui.fullscreen_checkBox, SIGNAL(stateChanged(int)), this, SLOT(FullScreenStatus(int)));
	connect(ui.app_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(OtherApp(int)));
	connect(ui.AudioCorner_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(clickChangeAudio(int)));
	connect(ui.videoCorner_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(clickChangeVideo(int)));
	connect(ui.ratio_pushBtn, SIGNAL(clicked()), this, SLOT(clickChangeRatio()));

	
	m_VideoInfo = new UIVideo(this);
	m_VideoInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_VideoInfo->move(30, 60);
	m_VideoInfo->resize(970, 560);
	m_VideoInfo->hide();

	m_OtherAppInfo = new UIOtherApp(this);
	m_OtherAppInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_OtherAppInfo->hide();

	m_AuxiliaryPanel = new UIAuxiliaryPanel(this);
	m_AuxiliaryPanel->setWindowFlags(Qt::FramelessWindowHint);
	m_AuxiliaryPanel->setParent(this);
	m_AuxiliaryPanel->move(11, 48);
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

	// 面板展开按钮初始位置
	ui.expansion_pushBtn->move(QPoint(15, 290));

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

	// 设置抓取其他应用样式
	ui.app_pushBtn->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 32px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/app.png);}"
		"QCheckBox::indicator:checked{image: url(./images/app.png);}");

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
	ui.mainmin_pushBtn->setPixmap(pixmap,4);
	ui.mainclose_pushBtn->setPixmap(pixmap1, 4);
	m_charRoom = new UIChatRoom(this);
	m_charRoom->setWindowFlags(Qt::FramelessWindowHint);
	m_charRoom->move(725+215, 50);
	m_charRoom->show();
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
}

void UIMainWindow::MinDialog()
{
	showMinimized();
}

void UIMainWindow::MaxDialog()
{
	showMaximized();
}

void UIMainWindow::CloseDialog()
{
	int iStatus = CMessageBox::showMessage(
		QString("答疑时间"),
		QString("是否关闭当前直播！"),
		QString("确定"),
		QString("取消"));
	if (iStatus == 1)
	{
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

	// 设置老师头像
	QString teacherPhoto_url = data["avatar_url"].toString();
	m_AuxiliaryPanel->setNetworkPic(teacherPhoto_url);

	// 老师云信信息
	m_charRoom->setChatInfo(data["chat_account"].toObject());
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
	QBitmap bmp(this->size());
	bmp.fill();
	QPainter p(&bmp);
	p.setPen(Qt::NoPen);
	p.setBrush(Qt::black);
	p.drawRoundedRect(bmp.rect(), 10, 10);
	setMask(bmp);
	bmp.clear();
}

void UIMainWindow::slot_startOrStopLiveStream()
{
	HideOtherUI();
	if (m_VideoInfo)
	{
		bool bLiving = m_VideoInfo->IsCurrentLiving();
		if (bLiving)
		{
			ui.Live_pushBtn->setText("开始直播");
			m_VideoInfo->StopLiveVideo();
			m_VideoInfo->setLessonName("");
			SendStopLiveHttpMsg();

			if (m_CountTimer->isActive())
			{
				m_CountTimer->stop();					// 停止计时
				m_iTimerCount = 0;						// 重置秒数
				ui.time_label->setText("00:00:00");		// 重置时间
			}

			if (m_HeartTimer->isActive())
			{
				m_HeartTimer->stop();					// 停止发送心跳
			}
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

			if (m_VideoInfo->m_videoSourceType == EN_NLSS_VIDEOIN_NONE)
			{
				CMessageBox::showMessage(
					QString("答疑时间"),
					QString("请选择要播放的视频源！"),
					QString("确定"));
				return;
			}
			
			QString url;
			url = m_AuxiliaryPanel->getURL();
			
			m_VideoInfo->setPlugFlowUrl(url);
			m_VideoInfo->StartLiveVideo();
			m_VideoInfo->setLessonName(m_AuxiliaryPanel->getLessonName());
			ui.Live_pushBtn->setText("结束直播");
			SendStartLiveHttpMsg();

			m_CountTimer->start(1000);
			m_HeartTimer->start(1000*60*5);
		}

		m_AuxiliaryPanel->setPreview(!bLiving);
		ui.video_widget->hide();
		m_VideoInfo->show();
	}
}

void UIMainWindow::VideoStatus(int iStatus)
{	
	HideOtherUI();
	if (iStatus)
	{
		m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_CAMERA;
		m_VideoInfo->ChangeLiveVideo();
		m_VideoInfo->show();

		ui.fullscreen_checkBox->setCheckState(Qt::CheckState::Unchecked);
		m_bOtherApp = false;
	}
	else
	{
		if (m_bOtherApp)
			return;

		// 如全屏在选中状态，则不隐藏窗口
		if (!ui.fullscreen_checkBox->isChecked())
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
		m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_FULLSCREEN;
		m_VideoInfo->ChangeLiveVideo();
		m_VideoInfo->show();

		ui.video_checkBox->setCheckState(Qt::CheckState::Unchecked);
		m_bOtherApp = false;
	}
	else
	{
		if (m_bOtherApp)
			return;

		// 如视频头在选中状态，则不隐藏窗口
		if (!ui.video_checkBox->isChecked())
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
		if (m_AudioChangeInfo->isVisible())
		{
			m_AudioChangeInfo->hide();
			return;
		}
		int x = ui.Audio_checkBox->geometry().x();
		int y = ui.Audio_checkBox->geometry().y();
		m_AudioChangeInfo->move(QPoint(x - 15, y - 28 - m_VideoInfo->m_iAudioDeviceNum * 30));
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
		int x = ui.video_checkBox->geometry().x();
		int y = ui.video_checkBox->geometry().y();
		m_VideoChangeInfo->move(QPoint(x - 15, y - 28 - m_VideoInfo->m_iVideoDeviceNum * 30));
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
		m_RatioChangeInfo->move(QPoint(x - 15, y - 28 - 60));
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
	if (m_OtherAppInfo && m_VideoInfo)
	{
		QRect MainRect = this->geometry();
		QRect InfoRect = m_OtherAppInfo->geometry();
		m_VideoInfo->EnumAvailableMediaDevices();
		m_OtherAppInfo->setAppInfo(m_VideoInfo->m_pAppWinds, m_VideoInfo->m_iAppWindNum);
		m_OtherAppInfo->move((MainRect.width() - InfoRect.width()) / 2, (MainRect.height() - InfoRect.height()) / 2);
		if (m_OtherAppInfo->exec() != 0)
		{
			m_bOtherApp = true;
			m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_APP;

			int index = m_OtherAppInfo->getCurrentIndex();
			m_VideoInfo->ChangeAppPath(index+1);
			m_VideoInfo->ChangeLiveVideo();
			m_VideoInfo->show();

			// 当选中其他应用时，摄像头和全屏都不可用
			ui.video_checkBox->setCheckState(Qt::CheckState::Unchecked);
			ui.fullscreen_checkBox->setCheckState(Qt::CheckState::Unchecked);
		}
	}
}

void UIMainWindow::slot_onCountTimeout()
{
	m_iTimerCount++;
	QString str = QString().sprintf("%02lld:%02lld:%02lld", m_iTimerCount / 3600, m_iTimerCount % 3600 / 60, m_iTimerCount % 60);
	ui.time_label->setText(str);
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
		case MSG_CLIENT_RECEIVE:
		{
			MSG* Msg = pMsg;
			nim::IMMessage* pIMsg = (nim::IMMessage*)Msg->wParam;
			
			if (m_charRoom)
				m_charRoom->ReceiverMsg(pIMsg);

			delete pIMsg;
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
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
}

void UIMainWindow::SendHeartBeatHttpMsg()
{
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/heartbeat";
	strUrl.replace("{lessons_id}", m_AuxiliaryPanel->getLessonID());
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/heartbeat";
	strUrl.replace("{lessons_id}", m_AuxiliaryPanel->getLessonID());
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
}

void UIMainWindow::SendStopLiveHttpMsg()
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
}

// 增加阴影
void UIMainWindow::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(5, 5, this->width() - 10, this->height() - 10);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::white));

	QColor color(0, 0, 0, 50);
	for (int i = 0; i < 10; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);
		path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
		color.setAlpha(150 - sqrt(i) * 50);
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
		m_VideoChangeInfo)
	{
		if (self != NULL)
		{
			if ( m_RatioChangeInfo != self)
				m_RatioChangeInfo->hide();
			if (m_AudioChangeInfo != self)
				m_AudioChangeInfo->hide();
			if ( m_VideoChangeInfo != self)
				m_VideoChangeInfo->hide();

			m_AuxiliaryPanel->hide();
		}
		else
		{
			m_RatioChangeInfo->hide();
			m_AudioChangeInfo->hide();
			m_VideoChangeInfo->hide();
			m_AuxiliaryPanel->hide();
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
				ui.ratio_pushBtn->setText("标清");
				m_VideoInfo->m_videoQ = EN_NLSS_VIDEOQUALITY_MIDDLE;
				break;
			}
		case 1:
			{
				ui.ratio_pushBtn->setText("高清");
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
		if (m_LoginWindow)
			m_LoginWindow->ReturnLogin();
	}
}

void UIMainWindow::setCurChatRoom(QString chatID)
{
	if (m_charRoom)
	{
		// 如果是当前会话窗口，则不需要再次请求群成员
		if (!m_charRoom->IsCurChatRoom(m_AuxiliaryPanel->getChatID()))
			RequestMember();

		m_charRoom->setCurChatID(chatID);
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
	if (obj["status"].toInt() == 1 && data.contains("im_app_key"))
	{
		QString key = data["im_app_key"].toString();
		m_charRoom->setKeyAndLogin(key);
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
	if (obj["status"].toInt() == 1 && data.contains("members"))
	{
		// 群成员信息
		QJsonArray members = data["members"].toArray();
		foreach(const QJsonValue & value, members)
		{
			QJsonObject obj = value.toObject();
			YXMember *pMember = new YXMember();
			pMember->readJsonToMember(value.toObject());

			//用完之后删除
			delete pMember;
		}
		
		// 群公告信息
		QJsonArray announcements = data["announcements"].toArray();
		foreach(const QJsonValue & value, announcements)
		{
			QJsonObject obj = value.toObject();
			YXMember *announcements = new YXMember();
			announcements->readJsonToMember(value.toObject());

			//用完之后删除
			delete announcements;
		}
	}

	// 没登录，则请求key并登录
	if (!m_charRoom->IsLogin())
		RequestKey();
}