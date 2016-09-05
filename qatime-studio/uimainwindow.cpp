#include "uimainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "course.h"
#include <QJsonArray>
#include <QRect>
#include "windows.h"
#include <QBitmap>
#include <QPainter>
#include <windowsx.h>
#include <QFile>
#include <math.h>

#define MAINWINDOW_X_MARGIN 6
#define MAINWINDOW_Y_MARGIN 6
#define MAINWINDOW_TITLE_HEIGHT 49

UIMainWindow::UIMainWindow(QWidget *parent)
	: QWidget(parent)
	, m_AuxiliaryInfo(NULL)
	, m_LessonInfo(NULL)
	, m_VideoInfo(NULL)
	, m_iTimerCount(0)
{
	ui.setupUi(this);
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(255, 255, 255));
	//palette.setBrush(QPalette::Background, QBrush(QPixmap(":/background.png")));
	setPalette(palette);

	connect(ui.mainmin_pushBtn, SIGNAL(clicked()), this, SLOT(MinDialog()));
	connect(ui.mainclose_pushBtn, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.expansion_pushBtn, SIGNAL(clicked()), this, SLOT(Expansion()));
	connect(ui.Live_pushBtn, SIGNAL(clicked()), this, SLOT(slot_startOrStopLiveStream()));
	connect(ui.videosource_comboBox, SIGNAL(activated(int)), this, SLOT(VideoSourceChange(int)));
	connect(ui.app_comboBox, SIGNAL(activated(int)), this, SLOT(VideoAppChange(int)));
	connect(ui.Audio_checkBox, SIGNAL(stateChanged(int)), this, SLOT(AudioStatus(int)));
	connect(ui.video_checkBox, SIGNAL(stateChanged(int)), this, SLOT(VideoStatus(int)));

	m_VideoInfo = new UIVideo(this);
	m_VideoInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_VideoInfo->move(30, 60);
	m_VideoInfo->resize(970, 560);
	m_VideoInfo->hide();

	m_AuxiliaryInfo = new UIAuxiliary(this);
	m_AuxiliaryInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_AuxiliaryInfo->hide();

	m_LessonInfo = new UILesson(this);
	m_LessonInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_LessonInfo->hide();

	m_AuxiliaryPanel = new UIAuxiliaryPanel(this);
	m_AuxiliaryPanel->setWindowFlags(Qt::FramelessWindowHint);
	m_AuxiliaryPanel->move(11, 48);
	m_AuxiliaryPanel->hide();

	// 设置视频源
	QFile file("styles/ComboBox.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = file.readAll();
	ui.videosource_comboBox->setStyleSheet(styleSheet);
	ui.videosource_comboBox->addItem(QString("摄像头"));
	ui.videosource_comboBox->addItem(QString("全屏桌面"));
	ui.videosource_comboBox->addItem(QString("其他APP"));
	ui.videosource_comboBox->setCurrentIndex(0);
	file.close();

	// 设置其他应用combox属性
	ui.app_comboBox->setStyleSheet(styleSheet);

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

	ui.expansion_pushBtn->move(QPoint(15, 290));
}

UIMainWindow::~UIMainWindow()
{
	if (m_AuxiliaryInfo)
	{
		delete m_AuxiliaryInfo;
		m_AuxiliaryInfo = NULL;
	}

	if (m_LessonInfo)
	{
		delete m_LessonInfo;
		m_LessonInfo = NULL;
	}

	if (m_AuxiliaryPanel)
	{
		delete m_AuxiliaryPanel;
		m_AuxiliaryPanel = NULL;
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
	exit(0);
}

void UIMainWindow::setTeacherInfo(QJsonObject &data)
{
	int iID = data["id"].toInt();
	m_teacherID = QString::number(iID);
	m_AuxiliaryPanel->setTeacherID(m_teacherID);

	// 设置老师名字
	QString teacherName = data["name"].toString();
	m_AuxiliaryPanel->setTeacherName(teacherName);

	// 设置老师头像
	QString teacherPhoto_url = data["avatar_url"].toString();
	m_AuxiliaryPanel->setNetworkPic(teacherPhoto_url);
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
	if (m_VideoInfo)
	{
		if (m_VideoInfo->IsCurrentPreview())
		{
			ui.Live_pushBtn->setText("开始直播");
			m_VideoInfo->StopLiveVideo();
			SendStopLiveHttpMsg();
			ui.app_comboBox->setEnabled(true);
			ui.videosource_comboBox->setEnabled(true);

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
			ui.Live_pushBtn->setText("结束直播");
			SendStartLiveHttpMsg();
			m_VideoInfo->StartLiveVideo();
			ui.app_comboBox->setEnabled(false);
			ui.videosource_comboBox->setEnabled(false);

			m_CountTimer->start(1000);
			m_HeartTimer->start(1000*60*5);
		}

		ui.video_widget->hide();
		m_VideoInfo->show();
	}
}

void UIMainWindow::VideoSourceChange(int index)
{
	switch (index)
	{
	case 0:
		m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_CAMERA;
		m_VideoInfo->ChangeLiveVideo();
		break;
	case 1:
		m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_FULLSCREEN;
		m_VideoInfo->ChangeLiveVideo();
		break;
	case 2:
		{
			m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_APP;
			SetSourceAppPath();
			break;
		}
	default:
		break;
	}
}

void UIMainWindow::VideoStatus(int iStatus)
{	
	if (iStatus)
	{
		m_VideoInfo->SetPauseVideo();
	}
	else
	{
		m_VideoInfo->SetResumeVideo();
	}
}

void UIMainWindow::AudioStatus(int iStatus)
{
	if (iStatus)
	{
		m_VideoInfo->SetPauseAudio();
	}
	else
	{
		m_VideoInfo->SetResumeAudio();
	}
}

void UIMainWindow::VideoAppChange(int index)
{
	if (m_VideoInfo)
	{
		m_VideoInfo->ChangeAppPath(index);
		m_VideoInfo->ChangeLiveVideo();
	}
}

void UIMainWindow::SetSourceAppPath()
{
	for (int i = 0; i < m_VideoInfo->m_iAppWindNum; i++)
	{
		QString qStrPath = m_VideoInfo->m_pAppWinds[i].paFriendlyName;
		ui.app_comboBox->addItem(qStrPath);
	}
	
	if (m_VideoInfo->m_iAppWindNum != 0)
	{
		ui.app_comboBox->setCurrentIndex(0);
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
					QPoint pt = ui.mainclose_pushBtn->mapFromGlobal(QPoint(GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam)));
					QRect Rect = ui.mainclose_pushBtn->geometry();
					if (x > Rect.left() && x < Rect.right() && y > Rect.top() && y < Rect.bottom())
					{
						return false;
					}

					pt = ui.mainmin_pushBtn->mapFromGlobal(QPoint(GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam)));
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