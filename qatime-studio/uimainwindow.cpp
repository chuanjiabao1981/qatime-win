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
	setFixedSize(QSize(985, 770));

	connect(ui.mainmin_pushBtn, SIGNAL(clicked()), this, SLOT(MinDialog()));
	connect(ui.mainclose_pushBtn, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.expansion_pushBtn, SIGNAL(clicked()), this, SLOT(Expansion()));
	connect(ui.Live_pushBtn, SIGNAL(clicked()), this, SLOT(slot_startOrStopLiveStream()));
	connect(ui.Audio_checkBox, SIGNAL(stateChanged(int)), this, SLOT(AudioStatus(int)));
	connect(ui.video_checkBox, SIGNAL(stateChanged(int)), this, SLOT(VideoStatus(int)));
	connect(ui.fullscreen_checkBox, SIGNAL(stateChanged(int)), this, SLOT(FullScreenStatus(int)));
	connect(ui.app_pushBtn, SIGNAL(clicked()), this, SLOT(OtherApp()));
	connect(ui.AudioCorner_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(clickChangeAudio(int)));
	connect(ui.videoCorner_pushBtn, SIGNAL(stateChanged(int)), this, SLOT(clickChangeVideo(int)));
	connect(ui.ratio_pushBtn, SIGNAL(clicked()), this, SLOT(clickChangeRatio()));
	connect(ui.lesson_pushButton, SIGNAL(clicked()), this, SLOT(clickLessonList()));
	
	m_VideoInfo = new UIVideo(this);
	m_VideoInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_VideoInfo->SetMainWnd(this);
	m_VideoInfo->move(15, 75);
	m_VideoInfo->resize(960, 605);
	m_VideoInfo->show();

	m_OtherAppInfo = new UIOtherApp(this);
	m_OtherAppInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_OtherAppInfo->hide();

	m_AuxiliaryPanel = new UIAuxiliaryPanel(this);
	m_AuxiliaryPanel->setWindowFlags(Qt::FramelessWindowHint);
	m_AuxiliaryPanel->setParent(this);
	m_AuxiliaryPanel->move(5, 40);
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

	// ֱ����ť
	ui.Live_pushBtn->setText("��ʼֱ��");
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: red;border-radius: 10px; }"
								   "QPushButton{border: 2px groove gray; border - style: outset;}"
								   "QPushButton:pressed{border-style: inset; }");

	// ��ʱ�� �ı�ֱ��ʱ��
	m_CountTimer = new QTimer(this);
	connect(m_CountTimer, SIGNAL(timeout()), this, SLOT(slot_onCountTimeout()));

	// ֱ����ʼ��ÿ5���ӷ���һ������
	m_HeartTimer = new QTimer(this);
	connect(m_HeartTimer, SIGNAL(timeout()), this, SLOT(slot_onHeartTimeout()));

	m_ShowVideoTimer = new QTimer(this);
	connect(m_ShowVideoTimer, SIGNAL(timeout()), this, SLOT(setVideoPos()));

	// ���չ����ť��ʼλ��
	ui.expansion_pushBtn->move(QPoint(5, 290));

	// ��������ͷ��ƵԴ��ʽ
	ui.video_checkBox->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 32px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/camera_close.png);}"
		"QCheckBox::indicator:checked{image: url(./images/camera_open.png);}");

	// ������˷���ʽ
	ui.Audio_checkBox->setStyleSheet("QCheckBox{spacing: 2px;color: white;}"
		"QCheckBox::indicator{width: 32px;height: 32px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/mic_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/mic_close.png);}");

	// ����ץȡȫ����ʽ
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
	ui.mainmin_pushBtn->setPixmap(pixmap,4);
	ui.mainclose_pushBtn->setPixmap(pixmap1, 4);
	m_charRoom = new UIChatRoom(this);
	m_charRoom->setWindowFlags(Qt::FramelessWindowHint);
	m_charRoom->move(985, 50);
	m_charRoom->hide();

	ui.video_checkBox->setCheckState(Qt::CheckState::Checked);

	ui.line_2->setVisible(false);
	ui.time_label->setVisible(false);
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

	//ɾ����ʱ��
	if (m_CountTimer)
	{
		if (m_CountTimer->isActive())
			m_CountTimer->stop();
		delete m_CountTimer;
		m_CountTimer = NULL;
	}

	//ɾ����ʱ��
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
	showMaximized();
}

void UIMainWindow::CloseDialog()
{
	int iStatus = CMessageBox::showMessage(
		QString("����ʱ��"),
		QString("�Ƿ�رյ�ǰӦ�ó���"),
		QString("ȷ��"),
		QString("ȡ��"));
	if (iStatus == 1)
	{
		// ���ͽ���ֱ����Ϣ�ٹر�
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

	// ������ʦ����
	QString teacherName = data["name"].toString();
	m_AuxiliaryPanel->setTeacherName(teacherName);

	QString strWelcome = "��ӭ ";
	strWelcome += teacherName;
	strWelcome += " ��ʦ��¼����ʱ�䣬ף��ֱ����죡";
	ui.welcome_label->setText(strWelcome);
	ui.welcome_label_2->setText(strWelcome);

	// ������ʦͷ��
	QString teacherPhoto_url = data["avatar_url"].toString();
	m_AuxiliaryPanel->setNetworkPic(teacherPhoto_url);

	// ��ʦ������Ϣ
	m_charRoom->setChatInfo(data["chat_account"].toObject(), mRemeberToken);

	m_LessonTable->InitToken(mRemeberToken, m_teacherID);
}

void UIMainWindow::setAutoTeacherInfo(QString teacherID, QString teacherName, QString teacherUrl, QString accid, QString token)
{
	m_teacherID = teacherID;
	m_AuxiliaryPanel->setTeacherID(m_teacherID);

	// ������ʦ����
	m_AuxiliaryPanel->setTeacherName(teacherName);

	QString strWelcome = "��ӭ ";
	strWelcome += teacherName;
	strWelcome += " ��ʦ��¼����ʱ�䣬ף��ֱ����죡";
	ui.welcome_label->setText(strWelcome);
	ui.welcome_label_2->setText(strWelcome);

	// ������ʦͷ��
	m_AuxiliaryPanel->setNetworkPic(teacherUrl);

	// ��ʦ������Ϣ
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
				QString("����ʱ��"),
				QString("�Ƿ�رյ�ǰֱ����"),
				QString("ȷ��"),
				QString("ȡ��"));
			if (iStatus == 1)
			{
				ui.Live_pushBtn->setText("��ʼֱ��");
				m_VideoInfo->StopLiveVideo();
				SendStopLiveHttpMsg();

				if (m_CountTimer->isActive())
				{
					m_CountTimer->stop();					// ֹͣ��ʱ
					m_iTimerCount = 0;						// ��������
					ui.time_label->setText("00:00:00");		// ����ʱ��
					ui.time_label->setVisible(false);		// ����
				}

				if (m_HeartTimer->isActive())
				{
					m_HeartTimer->stop();					// ֹͣ��������
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
					QString("����ʱ��"),
					QString("��ѡ����Ҫ��ʼ�Ŀγ��ٽ���ֱ����"),
					QString("ȷ��"));
				return;
			}

			if (m_AuxiliaryPanel->getURL().isEmpty())
			{
				CMessageBox::showMessage(
					QString("����ʱ��"),
					QString("������ַΪ�գ�"),
					QString("ȷ��"));
				return;
			}

// 			if (m_VideoInfo->m_videoSourceType == EN_NLSS_VIDEOIN_NONE)
// 			{
// 				CMessageBox::showMessage(
// 					QString("����ʱ��"),
// 					QString("��ѡ��Ҫ���ŵ���ƵԴ��"),
// 					QString("ȷ��"));
// 				return;
// 			}
			
// 			QString url;
// 			url = m_AuxiliaryPanel->getURL();
			
// 			m_VideoInfo->setPlugFlowUrl(url);
 			m_VideoInfo->StartLiveVideo();
			SendVideoMsg((UINT)MSG_VIDEO_START_LIVE);
			ui.Live_pushBtn->setText("����ֱ��");
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
//		m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_CAMERA;
//		m_VideoInfo->ChangeLiveVideo();
		m_VideoInfo->show();

		ui.fullscreen_checkBox->setCheckState(Qt::CheckState::Unchecked);
		m_bOtherApp = false;
		SendVideoMsg((UINT)MSG_VIDEO_CAMERA);
	}
	else
	{
		if (m_bOtherApp)
			return;

		// ��ȫ����ѡ��״̬�������ش���
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
//		m_VideoInfo->m_videoSourceType = EN_NLSS_VIDEOIN_FULLSCREEN;
		m_VideoInfo->show();
		ui.video_checkBox->setCheckState(Qt::CheckState::Unchecked);
		m_bOtherApp = false;
		SendVideoMsg((UINT)MSG_VIDEO_FULLSCREEN);
	}
	else
	{
		if (m_bOtherApp)
			return;

		// ����Ƶͷ��ѡ��״̬�������ش���
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
		m_AudioChangeInfo->move(QPoint(x - 10, y - 24 - m_VideoInfo->m_iAudioDeviceNum * 30));
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
		m_VideoChangeInfo->move(QPoint(x - 10, y - 24 - m_VideoInfo->m_iVideoDeviceNum * 30));
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
		m_RatioChangeInfo->move(QPoint(x - 10, y - 24 - 60));
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

void UIMainWindow::OtherApp()
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
// 			// ��ѡ������Ӧ��ʱ������ͷ��ȫ����������
// 			ui.video_checkBox->setCheckState(Qt::CheckState::Unchecked);
// 			ui.fullscreen_checkBox->setCheckState(Qt::CheckState::Unchecked);
// 		}
		SendVideoMsg(MSG_VIDEO_OTHREAPP);
	}
}

void UIMainWindow::slot_onCountTimeout()
{
	m_iTimerCount++;
	QString str = QString().sprintf("%02lld:%02lld:%02lld", m_iTimerCount / 3600, m_iTimerCount % 3600 / 60, m_iTimerCount % 60);
	ui.time_label->setText(str);
	ui.time_label->show();
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
		case MSG_CLIENT_RECEIVE:  // ����������Ϣ
		{
			MSG* Msg = pMsg;
			nim::IMMessage* pIMsg = (nim::IMMessage*)Msg->wParam;
			
			if (m_charRoom)
				m_charRoom->ReceiverMsg(pIMsg);

			delete pIMsg;
		}
		break;
		case MSG_CLIENT_RECORD:  // ������ʷ��Ϣ
		{
			MSG* Msg = pMsg;
			nim::QueryMsglogResult* pIMsg = (nim::QueryMsglogResult*)Msg->wParam;

			if (m_charRoom)
				m_charRoom->ReceiverRecordMsg(pIMsg);

			delete pIMsg;
		}
		break;
		case MSG_LOGIN:  // ���յ�¼���ؽ��
		{
			MSG* Msg = pMsg;
			nim::LoginRes* pLogMsg = (nim::LoginRes*)Msg->wParam;

			if (m_charRoom)
				m_charRoom->ReceiverLoginMsg(pLogMsg);

			delete pLogMsg;
		}
		break; 
		case MSG_MEMBERS_INFO:  // ����Ⱥ��Ա��Ϣ
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

			// ��ѡ������Ӧ��ʱ������ͷ��ȫ����������
			ui.video_checkBox->setCheckState(Qt::CheckState::Unchecked);
			ui.fullscreen_checkBox->setCheckState(Qt::CheckState::Unchecked);
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

// ������Ӱ
void UIMainWindow::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(2, 2, this->width()-4, this->height()-4);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::white));

	QColor color(102, 204, 204);
	for (int i = 0; i < 4; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);

		for(int j = 0; j < 40;j++)
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
			if ( m_RatioChangeInfo != self)
				m_RatioChangeInfo->hide();
			if (m_AudioChangeInfo != self)
				m_AudioChangeInfo->hide();
			if ( m_VideoChangeInfo != self)
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
				ui.ratio_pushBtn->setText("����");
				m_VideoInfo->m_videoQ = EN_NLSS_VIDEOQUALITY_MIDDLE;
				break;
			}
		case 1:
			{
				ui.ratio_pushBtn->setText("����");
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
		QString("����ʱ��"),
		QString("�Ƿ����µ�½��"),
		QString("ȷ��"),
		QString("ȡ��"));
	if (iStatus == 1)
	{
		// ���ͽ���ֱ����Ϣ�ٹر�
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
		// ����ǵ�ǰ�Ự���ڣ�����Ҫ�ٴ�����Ⱥ��Ա
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
		// Ⱥ��Ա��Ϣ
		QJsonArray members = data["members"].toArray();
		int i = 0;
		foreach(const QJsonValue & value, members)
		{
			QJsonObject obj = value.toObject();
			YXMember *pMember = new YXMember();
			pMember->readJsonToMember(value.toObject());

			if (m_charRoom)
			{
				// ��ӳ�Ա�ɹ���������ʾ����
				bool bSuc = m_charRoom->AddStudent(pMember->url(), pMember->name(), pMember->accid());
				if (bSuc)
					i++;
			}
			
			//����֮��ɾ��
			delete pMember;
		}
		m_charRoom->AddStudentNumbers(i);
		m_charRoom->QueryGroup();

		// ���ùۿ�����
		ui.person_pushButton->setIcon(QIcon("./images/eyes.png"));
		ui.person_pushButton->setIconSize(QSize(16, 16));
		ui.person_pushButton->setText(QString::number(i));

		// Ⱥ������Ϣ
		QJsonArray announcements = data["announcements"].toArray();
		foreach(const QJsonValue & value, announcements)
		{
			QJsonObject obj = value.toObject();
			YXMember *announcements = new YXMember();
			announcements->readJsonToAnnouncement(value.toObject());

			if (m_charRoom)
				m_charRoom->AddAnnouncement(announcements->announcement(), announcements->announcementTime());

			//����֮��ɾ��
			delete announcements;
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
		return;
	}

	// û��¼��������key����¼
	if (!m_charRoom->IsLogin())
	{
		RequestKey();
	}
}

void UIMainWindow::setVideoLesson(QString strLessonName)
{
	ui.lesson_label->setText(strLessonName);
}

// ��ʾ���촰��
void UIMainWindow::showChatRoomWnd()
{
	if (m_charRoom && !m_charRoom->isVisible())
	{
		ui.line_2->setVisible(true);
		m_charRoom->show();

		QPoint closeQt = ui.mainclose_pushBtn->pos();
		ui.mainclose_pushBtn->move(QPoint(closeQt.x() + 295, closeQt.y()));

		QPoint minQt = ui.mainmin_pushBtn->pos();
		ui.mainmin_pushBtn->move(QPoint(minQt.x() + 295, minQt.y()));

		resize(1150+135+15, 770);
		setFixedSize(QSize(1150+135+15, 770));
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

		int x = ui.lesson_pushButton->geometry().x();
		int y = ui.lesson_pushButton->geometry().y();
		m_LessonTable->move(QPoint(x - 200, y - 294));
		m_LessonTable->RequestLesson();
		m_LessonTable->show();
	}
}

// �γ̱���ѡ��γ̡���������������
void UIMainWindow::LessonTable_Auxiliary(QString sLessonID, QString sCourseID)
{
	m_AuxiliaryPanel->ergodicItem(sLessonID, sCourseID);
}

void UIMainWindow::setLiveBtnEnable(bool bEnable)
{
	ui.video_checkBox->setEnabled(bEnable);
	ui.fullscreen_checkBox->setEnabled(bEnable);
	ui.app_pushBtn->setEnabled(bEnable);
}

void UIMainWindow::setVideoPos()
{
	if (m_VideoWnd)
	{
		if (m_ShowVideoTimer->isActive())
			m_ShowVideoTimer->stop();
		MoveWindow(m_VideoWnd, 0, 0, 960, 605, true);
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
		strError = QString("��Ȩ����,�����µ�¼��");
	else if (error["code"].toInt() == 1003)
		strError = QString("û��Ȩ�޷��ʣ�");
	else if (error["code"].toInt() == 1004)
		strError = QString("��Ȩʧ��,�����µ�¼��");
	else if (error["code"].toInt() == 3001)
		strError = QString("��������,�����µ�¼��");
	else if (error["code"].toInt() == 3002)
		strError = QString("���ݲ��Ϸ�,�����µ�¼��");
	else if (error["code"].toInt() == 4001)
		strError = QString("�Ҳ�����Դ,�����µ�¼��");
	else if (error["code"].toInt() == 9999)
		strError = QString("����������,�����µ�¼��");
	else
		return;

	int iStatus = CMessageBox::showMessage(
		QString("����ʱ��"),
		QString(strError),
		QString("ȷ��"),
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
}