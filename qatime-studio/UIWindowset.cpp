#include "UIWindowset.h"
#include <QPainter>
#include "UIMessageBox.h"
#include "member.h"
#include <QToolTip>
#include <QDesktopWidget>
#include <QScreen>
#include "UIVideo.h"
#include "UICamera.h"
#include "lesson.h"
#include "IMInterface.h"
#include "HttpRequest.h"

#include "UIVideoRecord.h"
#include "ZPublicDefine.h"
#include "ZWeb.h"




extern int		m_AutoAudioState;
extern bool		m_IsAudioPlaying;
int m_PublicCameraStatus;	// 1v1ֱ��ʱ������ͷ״̬��1�򿪣�0�رգ��������жϽ�ʦ�Ƿ�����˰װ��������л�

#define MAINWINDOW_X_MARGIN 10
#define MAINWINDOW_Y_MARGIN 10
#define MAINWINDOW_TITLE_HEIGHT 0
#define LIVE_BUTTON_NAME	"ѡ��ֱ��"
#define LESSON_LABEL		"����ֱ��"

#define AudioDevice 0		//��Ƶ�豸
#define AudioOut 2			//������
#define Video 3				//��Ƶ�豸
#define VIDEO_FPS 50  

#define CameraIsClose	1	// ����ͷ�ر�
#define CameraIsOpen	2	// ����ͷ��
#define CameraIsPulling	3	// ����ͷ����



UIWindowSet* m_This = NULL;
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
	, m_ScreenType(SCREEN_4_3)
	, mWhiteBoard(NULL)
	, m_Camera1v1Info(NULL)
	, m_AudioChangeInfo1v1(NULL)
	, m_AudioOutChangeInfo1v1(NULL)
	, m_bLiving1v1(false)
	, m_CameraS1v1Info(NULL)
	, m_VideoInfo1v1(NULL)
	, m_AppWnd1v1(NULL)
	, m_QueryOnlieTimers(NULL)
	, m_VideoRecordInfo(NULL)
	, m_bQueryMsg(false)
	, m_bEveryTime(false)
	, m_PullCamera(0)
{
	ui.setupUi(this);
	m_This = this;
	setAutoFillBackground(true);
	QPalette p = palette();
	p.setColor(QPalette::Window, QColor(255, 255, 255));
	setPalette(p);
	setMinimumSize(966, 665);

	ui.logo_pushButton->installEventFilter(this);
	ui.title_pushButton->installEventFilter(this);
	ui.whiteboard_widget->installEventFilter(this);
	ui.full_widget->installEventFilter(this);
	ui.full1v1_widget->installEventFilter(this);
	initFont();
	MathScreenSize();
	InitSetParamWnds();

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

	m_VideoInfo = new UIVideo(ui.fullS_widget);
	m_VideoInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_VideoInfo->SetMainWnd(this);
	m_VideoInfo->InitDeviceParam();
	ui.verticalLayout_6->addWidget(m_VideoInfo);
	m_VideoInfo->show();

	// ��·ֱ��
	m_VideoRecordInfo = new UIVideoRecord();

	m_CameraInfo = new UICamera(ui.widget_5);//camera1_widget
	m_CameraInfo->setWindowFlags(Qt::FramelessWindowHint);
	m_CameraInfo->SetMainWnd(this);
	ui.horizontalLayout_11->addWidget(m_CameraInfo);//17
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

	// ȥ��ѡ��ֱ���ĸ�����
	 m_LiveLesson = new UILiveLessonWnd(this);
	// m_LiveLesson = new UILiveLessonWnd();
	m_LiveLesson->setWindowFlags(Qt::FramelessWindowHint);
	connect(m_LiveLesson, SIGNAL(sig_PullStreaming(QString, QString, QString, QString, QString, int, bool)), this, SLOT(slot_PullStreaming(QString, QString, QString, QString, QString, int, bool)));
	connect(m_LiveLesson, SIGNAL(sig_changeLessonStatus(QString, QString)), this, SLOT(slot_changeLessonStatus(QString, QString)));
	m_LiveLesson->hide();
	SetWindowPos((HWND)m_LiveLesson->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

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

	// ������˷���ʽ
	ui.Audio_checkBox->setStyleSheet("QCheckBox::indicator{width: 24px;height: 19px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/mic_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/mic_close.png);}");
	ui.Audio_pushButton->setStyleSheet("border-image:url(./images/Arrow.png);");

	// ��������ͷ��ƵԴ��ʽ
	ui.video_checkBox->setStyleSheet("QCheckBox::indicator{width: 29px;height: 19px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/camera_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/camera_close.png);}");
	ui.video_pushButton->setStyleSheet("border-image:url(./images/Arrow.png);");

	// ���õ�Ļ��ʽ
	ui.Bullet_checkBox->setStyleSheet("QCheckBox::indicator{width: 26px;height: 19px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/bullet_close.png);}"
		"QCheckBox::indicator:checked{image: url(./images/bullet_open.png);}");
	ui.Bullet_pushButton->setStyleSheet("border-image:url(./images/Arrow.png);");

	// ��ʱ�� �ı�ֱ����ʱʱ��
	m_CountTimer = new QTimer(this);
	connect(m_CountTimer, SIGNAL(timeout()), this, SLOT(slot_onCountTimeout()));

	m_QueryOnlieTimers = new QTimer(this);
	connect(m_QueryOnlieTimers, SIGNAL(timeout()), this, SLOT(slot_onOnlineTimeout()));

	// ����ͷ�ӳ�1�������������ȫ������һ����������ͻ
	m_tempTimers = new QTimer(this);
	connect(m_tempTimers, SIGNAL(timeout()), this, SLOT(slot_onTempTimeout()));
	
	// ֱ����ť
	ui.Live_pushBtn->setText(LIVE_BUTTON_NAME);
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");

	// 1v1ֱ����ť
	ui.Live1v1_pushBtn->setText(LIVE_BUTTON_NAME);
	ui.Live1v1_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");

	// Ĭ������״̬
	ui.label_NetState->setText("����");
	ui.label_NetState->setStyleSheet("color: rgb(0, 255, 0);");
	ui.label_NetState->setToolTip("�������ã���˳��ֱ��");

	m_LiveStatusManager = new LiveStatusManager(this);
	m_LiveStatusManager->setMainWindow(this);

	m_ScreenTip = new UIScreenTip();
	m_ScreenTip->setWindowFlags(Qt::FramelessWindowHint);
	m_ScreenTip->hide();
	SetWindowPos((HWND)m_ScreenTip->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	ui.notice_pushButton->setStyleSheet("border-image: url(./images/notice_nor.png);");
	ui.lesson_pushButton->setStyleSheet("border-image: url(./images/lessonBtn_nor.png);"); 
	ui.course_pushButton->setStyleSheet("border-image: url(./images/courseBtn_nor.png);");
	ui.person_pushButton->setStyleSheet("border-image: url(./images/personBtn_nor.png);");
	ui.pic_label->setStyleSheet("border-image: url(./images/online.png);");

	// ������ѯ����Ⱥ��֪ͨ�źŲ�
	connect(this, SIGNAL(sig_AddAnnouncement(QString, QString)), this, SLOT(slot_AddAnnouncement(QString, QString)));

	m_PublicCameraStatus = 1;	//Ĭ������ͷ����
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

	if (m_ScreenTip)
	{
		delete m_ScreenTip;
		m_ScreenTip = NULL;
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
		m_ScreenTip->setErrorTip("���Ƚ���ֱ�����ٹرմ��ڣ�");
		return;
	}

	if (m_bLiving1v1)
	{
		m_ScreenTip->setErrorTip("���Ƚ���ֱ�����ٹرմ��ڣ�");
		return;
	}

	//add by zbc 20170712
	StopAudioAutoPlay();
	if (m_vecTags.size() > 0)
	{
		std::vector<UITags*>::iterator it;
		for (it = m_vecTags.begin(); it != m_vecTags.end(); it++)
		{
			UITags* tags = *it;
			tags->GetRoom()->setVisible(false);
			//�������д��ڵ��Զ�����״̬
			tags->GetRoom()->GetNowAutoAudioID()->setStyleSheet("QPushButton{border-image:url(./images/AutoAudio2.png);}");
			tags->close();
			delete tags;
		}
	}
	m_vecTags.clear();
	m_mapTags.clear();
	m_curTags = NULL;
	m_curChatRoom = NULL;
	// ֹͣ�ɼ��ͻ���Ԥ��
	m_CameraInfo->StopCaptureVideo();
	m_VideoInfo->StopCaptureVideo();
	// �γ����ÿ�
	ui.lesson_label->setText(LESSON_LABEL);
	// �˳���Ļ
	emit ui.Bullet_checkBox->stateChanged(0);
	CloseBullet();

	//�������ŵ�����ͷ�ɼ�
	IMInterface::getInstance()->endDevice(Video);
	IMInterface::getInstance()->endDevice(AudioDevice);
	IMInterface::getInstance()->endDevice(AudioOut);
	hide();

	// ��հװ�
	if (mWhiteBoard)
		mWhiteBoard->cleanUp();
}

void UIWindowSet::MaxDialog()
{
	if (this->isMaximized())
	{
		ui.max_pushButton->setStyleSheet("border-image: url(./images/login_max.png);");
		showNormal();
		return;
	}
	showMaximized();
	ui.max_pushButton->setStyleSheet("border-image: url(./images/login_max1.png);");
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
	ui.ratio_pushButton->setText("����");

	font = ui.lesson_label->font();
	font.setPixelSize(15);
	ui.lesson_label->setFont(font);

	font = ui.time_label->font();
	font.setPixelSize(10);
	ui.time_label->setFont(font);

	font = ui.time1v1_label->font();
	font.setPixelSize(10);
	ui.time1v1_label->setFont(font);

	ui.lesson_widget->setVisible(false);
	ui.camera_widget->setVisible(false);
	ui.whiteboard_widget->setVisible(false);
	ui.time_label->setVisible(false);
	ui.time1v1_label->setVisible(false);

	ui.white_label->setVisible(false);
	ui.return_pushButton->setVisible(false);
}

void UIWindowSet::setMainWindow(UIMainNewWindow* parent)
{
	m_parent = parent;
}

void UIWindowSet::SetEnvironmental(bool bType, QString homePage)
{
	m_homePage = homePage;
	m_EnvironmentalTyle = bType;
	m_LiveStatusManager->SetEnvironmental(bType, homePage);
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

// �϶��������Ĵ���
bool UIWindowSet::eventFilter(QObject *target, QEvent *event)
{
	if (target == ui.logo_pushButton)
	{
		if (this->isMaximized())
			return false;

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
		if (this->isMaximized())
			return false;

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
	else if (target == ui.full_widget)
	{
		if (event->type() == QEvent::Resize)
		{
			int iWidth = ui.full_widget->width();
			int iHeight = ui.full_widget->height();

			int iVideoWidth = iWidth;
			int iVideoHeight = mathVideoWidth(iWidth, m_ScreenType);
			// ���VIDEO�ĸߴ��������ĸߣ����ø�������VIDEO�Ĵ�С
			if (iVideoHeight > iHeight)
			{
				iVideoWidth = mathVideoHeight(iHeight, m_ScreenType);
				m_VideoInfo->setFixedSize(iVideoWidth, iHeight);
			}
			else
				m_VideoInfo->setFixedSize(iVideoWidth, iVideoHeight);
		}
	}
	else if (target == ui.full1v1_widget) // �װ�4��3��������Ӧ
	{
		if (event->type() == QEvent::Resize)
		{
			int iWidth = ui.full1v1_widget->width();
			int iHeight = ui.full1v1_widget->height();

			int iVideoWidth = iWidth;
			int iVideoHeight = mathVideoWidth(iWidth,SCREEN_4_3);
			// ���VIDEO�ĸߴ��������ĸߣ����ø�������VIDEO�Ĵ�С
			if (iVideoHeight > iHeight)
			{
				iVideoWidth = mathVideoHeight(iHeight, SCREEN_4_3);
				mWhiteBoard->setFixedSize(iVideoWidth, iHeight);
			}
			else
				mWhiteBoard->setFixedSize(iVideoWidth, iVideoHeight);

			QRect rc = ui.full1v1_widget->geometry();
			m_WhiteBoardTool->move(rc.x() + (iWidth - m_WhiteBoardTool->width())/2, rc.y() + iHeight - 30);


			// ����ץȡ
			int iScreenWidth = iWidth;
			int iScreenHeight = ((double)iWidth / (double)m_VideoInfo1v1->ScreenWidth())*m_VideoInfo1v1->ScreenHeight();
			if (iScreenHeight > iHeight)
			{
				iScreenWidth = ((double)iHeight / (double)m_VideoInfo1v1->ScreenHeight())*m_VideoInfo1v1->ScreenWidth();
				m_VideoInfo1v1->setFixedSize(iScreenWidth, iHeight);
			}
			else
				m_VideoInfo1v1->setFixedSize(iScreenWidth, iScreenHeight);

			m_AppWndTool1v1->move(rc.x() + (iWidth - m_AppWndTool1v1->width()) / 2, rc.y() + iHeight - 30);
		}
	}
	return false;
}

int UIWindowSet::mathVideoWidth(int iwidth, SCREEN_TYPE type)
{
	int iVideoWidth = 0;
	switch (type)
	{
	case SCREEN_16_10:
		iVideoWidth = iwidth / 16 * 10;
		break;
	case SCREEN_16_9:
		iVideoWidth = iwidth / 16 * 9;
		break;
	case SCREEN_4_3:
		iVideoWidth = iwidth / 4 * 3;
		break;
	default:
		break;
	}

	return iVideoWidth;
}

int UIWindowSet::mathVideoHeight(int iheight, SCREEN_TYPE type)
{
	int iVideoHeight = 0;
	switch (type)
	{
	case SCREEN_16_10:
		iVideoHeight = iheight / 10 * 16;
		break;
	case SCREEN_16_9:
		iVideoHeight = iheight / 9 * 16;
		break;
	case SCREEN_4_3:
		iVideoHeight = iheight / 3 * 4;
		break;
	default:
		break;
	}

	return iVideoHeight;
}

void UIWindowSet::AddChatRoom(QString chatID, QString courseid, QString teacherid, QString token, QString studentName, std::string strCurAudioPath,
							QString courseName, int UnreadCount, QString status, QString boardurl, QString cameraUrl, int mLessonType)
{
	show();
	if (isMinimized())
		showNormal();
	SetWindowPos((HWND)winId(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	QMap<QString, UITags*>::iterator it;
	it = m_mapTags.find(chatID);
	if (it == m_mapTags.end())
	{
		// ����������
		if (m_vecTags.size() == 10)
		{
			CMessageBox::showMessage(QString("����ʱ��"), QString("��������10�������࣡��"), QString("ȷ��"), QString());
			return;
		}
	}

	qDebug() << __FILE__ << __LINE__<<"event1" << courseid;
	if (IsHasTag(chatID, status))
		return;
	qDebug() << __FILE__ << __LINE__ << "event2" << courseid;

	if (mLessonType == d_1V1Lesson)
	{ 
		OpenCourse1v1(chatID, courseid, teacherid, token, studentName, strCurAudioPath, courseName, UnreadCount, status, boardurl, cameraUrl, mLessonType);
	}
	else
	{
		OpenCourse(chatID, courseid, teacherid, token, studentName, strCurAudioPath, courseName, UnreadCount, status, boardurl, cameraUrl, mLessonType);
	}
	qDebug() << __FILE__ << __LINE__ << "event3" << courseid;

}

// �򿪻���ֱ��
void UIWindowSet::OpenCourse1v1(QString chatID, QString courseid, QString teacherid, QString token, QString studentName, std::string strCurAudioPath,
	QString courseName, int UnreadCount, QString status, QString boardurl, QString cameraUrl, int mLessonType)
{
	UIChatRoom* chatRoom = IsHasRoom(chatID);
	if (chatRoom == NULL)
	{
		chatRoom = new UIChatRoom(ui.chat1v1_widget);
		chatRoom->setWindowFlags(Qt::FramelessWindowHint);
		chatRoom->setMainWindow(this);
		chatRoom->SetEnvironmental(m_EnvironmentalTyle,m_homePage);
		chatRoom->setCurChatID(chatID, courseid, teacherid, token, studentName, m_accid, UnreadCount,mLessonType);
		chatRoom->SetCurAudioPath(strCurAudioPath);
		ui.horizontalLayout_18->addWidget(chatRoom);
		m_vecChatRoom.push_back(chatRoom);
		m_mapChatRoom.insert(chatID, chatRoom);
		chatRoom->show();
	}

	AddTag(chatID, courseName, courseid, true, chatRoom, status, mLessonType, boardurl, cameraUrl);
}

// ��ֱ����
void UIWindowSet::OpenCourse(QString chatID, QString courseid, QString teacherid, QString token, QString studentName, std::string strCurAudioPath,
	QString courseName, int UnreadCount, QString status, QString boardurl, QString cameraUrl, int mLessonType)
{
	UIChatRoom* chatRoom = IsHasRoom(chatID);
	if (chatRoom == NULL)
	{
		chatRoom = new UIChatRoom(ui.chat_widget);
		chatRoom->setWindowFlags(Qt::FramelessWindowHint);
		chatRoom->setMainWindow(this);
		chatRoom->SetEnvironmental(m_EnvironmentalTyle, m_homePage);
		chatRoom->setCurChatID(chatID, courseid, teacherid, token, studentName, m_accid, UnreadCount, mLessonType);	
		chatRoom->SetCurAudioPath(strCurAudioPath);
		ui.horizontalLayout_6->addWidget(chatRoom);
		m_vecChatRoom.push_back(chatRoom);
		m_mapChatRoom.insert(chatID, chatRoom);
		chatRoom->show();
	}

	AddTag(chatID, courseName, courseid, true, chatRoom, status, mLessonType, boardurl, cameraUrl);
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
				// ��������ʾ����ֱ��
				if (tags->GetLessonType() == d_1V1Lesson)
				{
					ui.live_widget->setVisible(false);
					ui.live1v1_widget->setVisible(true);
				}
				else
				{
					ui.live_widget->setVisible(true);
					ui.live1v1_widget->setVisible(false);
				}

				
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
				// ����ûѡ�е�
				tags->setStyle(false);
				tags->GetRoom()->setVisible(false);
				//add by zbc 20170712
				StopAudioAutoPlay();
				tags->GetRoom()->GetNowAutoAudioID()->setStyleSheet("QPushButton{border-image:url(./images/AutoAudio2.png);}");

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

void UIWindowSet::AddTag(QString chatID, QString name, QString ID, bool sel, UIChatRoom* room, QString status, int mLessonType, QString board, QString camera)
{
 	UITags* tag = new UITags(ui.tags_widget);
 	tag->setMaximumWidth(200);
 	tag->SetCourseNameAndID(name, ID, chatID,board,camera);
	tag->SetRoom(room);
 	tag->setStyle(sel);
	tag->setLessonType(mLessonType);
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
	
	// ��������ʾ����ֱ��
	if (tag->GetLessonType() == d_1V1Lesson)
	{
		ui.live_widget->setVisible(false);
		ui.live1v1_widget->setVisible(true);
	}
	else
	{
		ui.live_widget->setVisible(true);
		ui.live1v1_widget->setVisible(false);
	}

	m_curTags->setModle(false);
	emit sig_Modle(false);
}

// ���϶�����Ŀ��
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
		m_ScreenTip->setErrorTip("���Ƚ���ֱ�����ٹرմ��ڣ�");
		return;
	}

	if (m_curTags->IsModle() && m_bLiving1v1)
	{
		m_ScreenTip->setErrorTip("���Ƚ���ֱ�����ٹرմ��ڣ�");
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
			// �ж��ǵ�ǰ�Ự����
			if (tag == tags)
			{
				//add by zbc 20170712
				StopAudioAutoPlay();
				tags->GetRoom()->GetNowAutoAudioID()->setStyleSheet("QPushButton{border-image:url(./images/AutoAudio2.png);}");
				// �жϵ�ǰ�����Ƿ����ֱ����
				if (tags->IsModle())
				{
					// �Ƿ���һ��һ
					if (tags->GetLessonType() == d_1V1Lesson)
					{		
						//�������ŵ�����ͷ�ɼ�
						IMInterface::getInstance()->endDevice(Video);
						IMInterface::getInstance()->endDevice(AudioDevice);
						IMInterface::getInstance()->endDevice(AudioOut);

						// ��հװ�
						if (mWhiteBoard)
							mWhiteBoard->cleanUp();
					}
					else 
						if ((tags->GetLessonType() == d_AuxiliryLesson) || (tags->GetLessonType() == d_ExclusiveLesson))  // ��ֱ���λ���ר����
					{
						m_CameraInfo->StopCaptureVideo();
						m_VideoInfo->StopCaptureVideo();
					}
					// �˳���Ļ
					emit ui.Bullet_checkBox->stateChanged(0);
					CloseBullet();
				}

				bSel = tags->IsSelect();
				tags->GetRoom()->setVisible(false);

				m_vecTags.erase(it);
				tags->close();
				delete tags;
				titleButtonWidth();

				// �رյĴ��ڣ��Ƿ��ǵ�ǰ��ʾ��
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
		//�ر�����ͷ�ɼ�����������ֱ����ֱ����
		if (m_CameraInfo)
			m_CameraInfo->StopCaptureVideo();

		if (m_VideoInfo)
			m_VideoInfo->StopCaptureVideo();

		// �γ����ÿ�
		ui.lesson_label->setText(LESSON_LABEL);
		// �˳���Ļ
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

void UIWindowSet::ReceiverMsg(const nim::IMMessage* pIMsg)
{
	QString strChatID = QString::fromStdString(pIMsg->local_talk_id_.c_str());
	UIChatRoom* room = NULL;
	room = m_mapChatRoom.value(strChatID,NULL);
	if (room)
	{
		room->ReceiverMsg(pIMsg);
	}

	QMap<QString, UITags*>::Iterator itTag;
	itTag = m_mapTags.find(strChatID);
	if (itTag == m_mapTags.end())
	{
		m_parent->changeMsgNumber(QString::fromStdString(pIMsg->local_talk_id_));
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

			// �ж���ʷ��Ϣ>0�� ���ǵ�ǰ�Ự����
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
	UIChatRoom* room = NULL;
	room = m_mapChatRoom.value(strChatID,NULL);
	if (room)
	{
		room->ShowChatMsg(*pIMsg);
	}
}

void UIWindowSet::ReceiverLoginMsg(const nim::LoginRes& pLogMsg)
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
			// �ж��ǵ�ǰ�Ự����
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
			// �ж��ǵ�ǰ�Ự����
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
			// �ж��ǵ�ǰ�Ự����
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
				// ��������ʾ����ֱ��
				if (tags->GetLessonType() == d_1V1Lesson)
				{
					ui.live_widget->setVisible(false);
					ui.live1v1_widget->setVisible(true);
				}
				else
				{
					ui.live_widget->setVisible(true);
					ui.live1v1_widget->setVisible(false);
				}


				tags->setStyle(true);
				tags->GetRoom()->setVisible(true);
				m_curTags = tags;
				m_curChatRoom = tags->GetRoom();

				
			}
			else
			{
				// ����ûѡ�е�
				tags->setStyle(false);
				tags->GetRoom()->setVisible(false);

				//�л���ǩʱ��ֱ�Ӱѵ�ǰ��ǩ�µ��Զ��������Ź��ܹر�
				StopAudioAutoPlay();
				tags->GetRoom()->GetNowAutoAudioID()->setStyleSheet("QPushButton{border-image:url(./images/AutoAudio2.png);}");
			}
		}
	}

	if (m_NoticeWnd){
		ui.notice_pushButton->setStyleSheet("border-image: url(./images/notice_nor.png);");//background-color: rgb(255, 255, 255);
		m_NoticeWnd->hide();
	}

	if (m_CourseWnd){
		ui.course_pushButton->setStyleSheet("border-image: url(./images/courseBtn_nor.png);");
		m_CourseWnd->hide();
	}

	if (m_PersonWnd){
		ui.person_pushButton->setStyleSheet("border-image: url(./images/personBtn_nor.png);");
		m_PersonWnd->hide();
	}

	if (m_LessonWnd){
		ui.lesson_pushButton->setStyleSheet("border-image: url(./images/lessonBtn_nor.png);");
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
				QString("����ʱ��"),
				QString("�Ƿ�رյ�ǰֱ����"),
				QString("ȷ��"),
				QString("ȡ��"));
			
			if (iStatus == 1)
			{
				ui.Live_pushBtn->setText(LIVE_BUTTON_NAME);
				ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");

				setLiveBtnEnable(false);
				m_VideoInfo->StopLiveVideo();
				qDebug() << "λ��1";
				m_LiveStatusManager->SendStopLiveHttpMsg();

				if (m_CountTimer->isActive())
				{
					m_CountTimer->stop();					// ֹͣ��ʱ
					m_iTimerCount = 0;						// ��������
					ui.time_label->setVisible(false);		// ����
					ui.time_label->setText("00:00:00");		// ����ʱ��
				}
				QString mOnlineState = "";
				QString mEndText = "ֱ������";
				if (m_bEveryTime)
				{
					mOnlineState = "InstantLesson";
					mEndText = "�������ɽ���";
				}
				else
				{
					mOnlineState = "ScheduledLesson";
				}
				m_curChatRoom->SendSelfDefineMessageScreen(false, mOnlineState);
				m_curChatRoom->m_uitalk->InsertOneNotice(mEndText);
			}
			else
				return;
		}
		else
		{
			if (m_curTags == NULL)
				return;
			m_LiveLesson->DeleteItem();
			m_LiveLesson->move(width()/2 - m_LiveLesson->width()/2,height()/2-m_LiveLesson->height()/2);
			m_LiveLesson->show();
			m_LiveLesson->setFocus();
			ShowLesson();
		}
	}
}

// ѡ�пγ̿�ʼֱ��
void UIWindowSet::slot_PullStreaming(QString id, QString courseid, QString boardurl, QString cameraurl, QString name, int mLessonType, bool bEveryTime)
{
	m_lessonName = name;
	ui.lesson_label->setText(m_lessonName);
	m_lessonid = id;

	if (ui.Audio_checkBox->checkState() == 2)
	{
		emit ui.Audio_checkBox->stateChanged(2);
	}


	// �ж������1V1�Ļ�
	if (mLessonType == d_1V1Lesson)
	{
		if (ui.Audio1v1_checkBox->checkState() == 2)
		{
			emit ui.Audio1v1_checkBox->stateChanged(2);
		}
		QString chatID = m_curTags->ChatID();
		createRtsRoom(chatID);
	}
	else
	{
		m_CameraInfo->setVisible(false);
		m_boardUrl = m_curTags->BoardStream();
//		m_boardUrl = "rtmp://pdl1f3ddaa0.live.126.net/live/98cbe5a825824c8baf03d8f4313c960d?wsSecret=ec590d57704f2cfac12866761399fbfe&wsTime=1497959633";
		qDebug() << "�װ�������ַ��" << m_boardUrl;
		m_cameraUrl = m_curTags->CameraStream();
//		m_cameraUrl = "rtmp://pdl1f3ddaa0.live.126.net/live/a41d93c303a84f8b9d1e197e7bc421b5?wsSecret=4689c5753a21b5e5c5b404ddf5d07c03&wsTime=1497959678  ";
		qDebug() << "����ͷ������ַ��" << m_cameraUrl;

		m_VideoInfo->setPlugFlowUrl(m_boardUrl);
		m_VideoInfo->StartLiveVideo();
		//������Ϣ��֪ͨ
		QString mOnline = "";
		QString mStartText = "ֱ����ʼ";
		if (bEveryTime)
		{
			mOnline = "InstantLesson";
			mStartText = "�������ɿ�ʼ";
		}
		else
		{
			mOnline = "ScheduledLesson";
		}
		m_bEveryTime = bEveryTime;
		m_curChatRoom->SendSelfDefineMessageScreen(true, mOnline);
		m_curChatRoom->m_uitalk->InsertOneNotice(mStartText);
		setLiveBtnEnable(false);
	}
}

void UIWindowSet::createRtsRoom(const QString &roomName, const QString &roomInfo /* = "" */)
{
	// �ȴ����װ�
	IMInterface::getInstance()->createRtsRoom(roomName.toStdString(), roomInfo.toStdString());
}

void UIWindowSet::slot_changeLessonStatus(QString id, QString status)
{
	m_parent->changeLessonStatus(id,status);
}

// ���¿�ʼֱ����ť����ʱ����״̬
void UIWindowSet::startLiveStream()
{
	m_CountTimer->start(1000);
	ui.time_label->setVisible(true);
	ui.Live_pushBtn->setText("����ֱ��");
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: red;border-radius: 5px; border: 2px solid red;}");
}

void UIWindowSet::slot_onCountTimeout()
{
	m_iTimerCount++;
	QString str = QString().sprintf("%02d:%02d:%02d", m_iTimerCount / 3600, m_iTimerCount % 3600 / 60, m_iTimerCount % 60);
	if (m_bLiving1v1)
		ui.time1v1_label->setText(str);
	else
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
				m_ScreenTip->setErrorTip(" ����ʧ�ܣ���ȷ�������γ����˳�\nֱ���Һ��ٽ��룡");
				return;
			}
			else if (tags == m_curTags && tags->IsModle())
			{
				if (IsLiving())
					return;
			}
		}
	}

	if (m_curTags->GetLessonType() == d_1V1Lesson)
	{
		if (ui.camera1v1_widget->isVisible())
		{
			ui.label_2->setVisible(false);
			ui.line_label->setVisible(false);
			ui.camera1v1_widget->setVisible(false);
			ui.whiteboard1v1_widget->setVisible(false);
			ui.lesson_widget->setVisible(false);
			m_WhiteBoardTool->setVisible(false);
			m_AppWndTool1v1->setVisible(false);
			ui.chatcamera1v1_widget->setMaximumWidth(3000);

			//�������ŵ�����ͷ�ɼ�
			IMInterface::getInstance()->endDevice(Video);
			IMInterface::getInstance()->endDevice(AudioDevice);
			IMInterface::getInstance()->endDevice(AudioOut);

			if (m_curTags)
				m_curTags->setModle(false);

			ChangeBtnStyle(false);
			// �γ����ÿ�
			ui.lesson_label->setText(LESSON_LABEL);
			// �˳���Ļ
			emit ui.Bullet_checkBox->stateChanged(0);
			CloseBullet();

			// ��հװ�
			if (mWhiteBoard)
				mWhiteBoard->cleanUp();
		}
		else
		{
			ui.label_2->setVisible(true);
			ui.line_label->setVisible(true);
			ui.camera1v1_widget->setVisible(true);
			ui.whiteboard1v1_widget->setVisible(true);
			ui.lesson_widget->setVisible(true);

			if (mWhiteBoard->isVisible())
				m_WhiteBoardTool->setVisible(true);
			else
				m_AppWndTool1v1->setVisible(true);

			ui.chatcamera1v1_widget->setMaximumWidth(300);

			//�������ŵ�����ͷ�ɼ�
			if (!ui.video1v1_checkBox->isChecked())
			{
				QString dPath = m_VideoChangeInfo1v1->GetCurPath();
				if (!dPath.isNull())
					IMInterface::getInstance()->startDevice(Video, dPath.toStdString(), VIDEO_FPS, 0, 0);
			}

			//������˷�Ĳɼ�
			if (!ui.Audio1v1_checkBox->isChecked())
			{
				QString dPath = m_AudioChangeInfo1v1->GetCurPath();
				if (!dPath.isNull())
					IMInterface::getInstance()->startDevice(AudioDevice, dPath.toStdString(), 0, 0, 0);
			}

			//�����������Ĳɼ�
			if (!ui.Audio1v1_checkBox->isChecked())
			{
				QString dPath = m_AudioOutChangeInfo1v1->GetCurPath();
				if (!dPath.isNull())
					IMInterface::getInstance()->startDevice(AudioOut, dPath.toStdString(), 0, 0, 0);
			}
			

			if (m_curTags)
				m_curTags->setModle(true);

			ChangeBtnStyle(true);
		}
		ui.online_widget->setVisible(false);
	}
	else
	{
		if (ui.camera_widget->isVisible())
		{
			ui.white_label->setVisible(false);
			ui.camera_widget->setVisible(false);
			ui.whiteboard_widget->setVisible(false);
			ui.lesson_widget->setVisible(false);
			ui.chatcamera_widget->setMaximumWidth(3000);

			m_CameraInfo->StopCaptureVideo();
			m_VideoInfo->StopCaptureVideo();

			if (m_curTags)
				m_curTags->setModle(false);

			ChangeBtnStyle(false);
			// �γ����ÿ�
			ui.lesson_label->setText(LESSON_LABEL);
			// �˳���Ļ
			emit ui.Bullet_checkBox->stateChanged(0);
			CloseBullet();

			// ֹͣ��ѯ��������
			stopQueryOnlineNum();
		}
		else
		{
			ui.white_label->setVisible(true);
			ui.camera_widget->setVisible(true);
			ui.whiteboard_widget->setVisible(true);
			ui.chatcamera_widget->setMaximumWidth(300);
			ui.lesson_widget->setVisible(true);

			if (m_curTags)
				m_curTags->setModle(true);

			ChangeBtnStyle(true);
			m_CameraInfo->StartLiveVideo();
			m_VideoInfo->StartLiveVideo();

			// ��ʼ��ѯ��������
			ui.online_widget->setVisible(true);
			startQueryOnlineNum();
		}
	}
}

void UIWindowSet::slots_Modle(bool bModle)
{
	if (!bModle)
	{
		if (m_curTags && (m_curTags->GetLessonType() == d_1V1Lesson))
		{
			ui.whiteboard1v1_widget->setVisible(false);
			ui.line_label->setVisible(false);
			ui.label_2->setVisible(false);
			ui.camera1v1_widget->setVisible(false);
			m_WhiteBoardTool->setVisible(false);
			m_AppWndTool1v1->setVisible(false);
			ui.chatcamera1v1_widget->setMaximumWidth(3000);
		}
		else
		{
			ui.white_label->setVisible(false);
			ui.camera_widget->setVisible(false);
			ui.whiteboard_widget->setVisible(false);
			ui.chatcamera_widget->setMaximumWidth(3000);
		}

		stopQueryOnlineNum();

		ui.lesson_widget->setVisible(false);
		ChangeBtnStyle(false);
	}
	else
	{
		if (m_curTags && (m_curTags->GetLessonType() == d_1V1Lesson))
		{
			ui.whiteboard1v1_widget->setVisible(true);
			ui.line_label->setVisible(true);
			ui.label_2->setVisible(true);
			ui.camera1v1_widget->setVisible(true);
			if (mWhiteBoard->isVisible())
				m_WhiteBoardTool->setVisible(true);
			else
				m_AppWndTool1v1->setVisible(true);
			ui.chatcamera1v1_widget->setMaximumWidth(300);
		}
		else
		{
			ui.white_label->setVisible(true);
			ui.camera_widget->setVisible(true);
			ui.whiteboard_widget->setVisible(true);
			m_WhiteBoardTool->setVisible(false);
			m_AppWndTool1v1->setVisible(false);
			ui.chatcamera_widget->setMaximumWidth(300);

			startQueryOnlineNum();
		}
		
//		������ͷ��ֱ��/������
		ui.lesson_widget->setVisible(true);
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
			ui.notice_pushButton->setStyleSheet("color:rgb(0,0,0);border-image: url(./images/notice_nor.png);");
			m_NoticeWnd->setVisible(false);
			return;
		}

		m_NoticeWnd->initDate();
		ui.notice_pushButton->setStyleSheet("color:rgb(255, 255, 255);border-image: url(./images/notice_hor.png);");//background-color: rgb(5,157,213);
		m_NoticeWnd->move(0, 70);
		m_NoticeWnd->DeleteNotice();
		m_NoticeWnd->show();
		m_NoticeWnd->setFocus();
		// ȫ�������Ų�ѯ���һ������
		//QueryNotice();
		QueryNoticeFromYX();
	}
}

void UIWindowSet::QueryNoticeFromYX()
{
	nim::Team::QueryTeamInfoAsync(m_curChatRoom->GetCurChatID(), &UIWindowSet::QueryTeamInfoCallback);
}

void UIWindowSet::QueryTeamInfoCallback(const std::string& tid, const nim::TeamInfo& result)
{
	std::string announcement = result.GetAnnouncement();
	QDateTime timeDate = QDateTime::currentDateTime();
	QString strTime = timeDate.toString("yyyy-MM-dd hh:mm:ss");
	if (m_This)
	{
		emit m_This->sig_AddAnnouncement(QString::fromStdString(announcement), strTime);
	}
	
}

void UIWindowSet::slot_AddAnnouncement(QString mAnnouncement, QString mTime)
{
	if (m_NoticeWnd)
	{
		m_NoticeWnd->AddNotic(mAnnouncement, mTime);
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
	if (m_curTags->GetLessonType() == d_1V1Lesson)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_courses/{id}/realtime";
		strUrl.replace("{id}", strCourseID);
	}
	else
	if (m_curTags->GetLessonType() == d_1V1Lesson)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/courses/{id}/realtime";
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
		// Ⱥ������Ϣ
		QJsonArray announcements = data["announcements"].toArray();
		foreach(const QJsonValue & value, announcements)
		{
			QJsonObject obj = value.toObject();
			YXMember *announcements = new YXMember();
			announcements->readJsonToAnnouncement(value.toObject());

			if (m_NoticeWnd)
				m_NoticeWnd->AddNotic(announcements->announcement(), announcements->announcementTime());

			//����֮��ɾ��
			delete announcements;
			return;
		}
	}
	else if (error["code"].toInt() == 1002)
	{
		QString strError = QString("��Ȩ�ѹ��ڣ������µ�¼");
		int iStatus = CMessageBox::showMessage(
			QString("����ʱ��"),
			QString(strError),
			QString("ȷ��"),
			QString());
		if (iStatus == 1 || iStatus == 0)
		{
			ReturnLogin();
		}
		return;
	}
	else
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

// ����������
void UIWindowSet::clickCourse()
{
	if (m_CourseWnd)
	{
		if (m_CourseWnd->isVisible())
		{
			ui.course_pushButton->setStyleSheet("border-image: url(./images/courseBtn_nor.png);");
			m_CourseWnd->setVisible(false);
			return;
		}

		ui.course_pushButton->setStyleSheet("border-image: url(./images/courseBtn_hor.png);");
		m_CourseWnd->move(0, 70);
		m_CourseWnd->show();
		m_CourseWnd->setFixedHeight(510);
		m_CourseWnd->setFixedWidth(410);
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
	// ��ͬ��������ʲ�ͬ�ӿ�
	if (m_curTags->GetLessonType() == d_1V1Lesson)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_courses/{id}";
		strUrl.replace("{id}", strCourseID);
	}
	else
	if (m_curTags->GetLessonType() == d_AuxiliryLesson)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/teachers/{teacherid}/courses/{id}";
		strUrl.replace("{id}", strCourseID);
		strUrl.replace("{teacherid}", m_teacherID);
	}
	else
	if (m_curTags->GetLessonType() == d_ExclusiveLesson)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/customized_groups/{id}/detail";
		strUrl.replace("{id}", strCourseID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	request.setRawHeader("Remember-Token", m_Token.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIWindowSet::returnCourse);
}

// ת��ʱ���Ϊʱ���ַ���
QString mFun_ToTimeString(int mTime)
{
	if (mTime == 0)
	{
		return "";
	}
	QDateTime time = QDateTime::currentDateTime();   //��ȡ��ǰʱ��  
	int timeT = time.toTime_t();   //����ǰʱ��תΪʱ���  
	QDateTime m1TestTime = QDateTime::fromTime_t(timeT);
	QDateTime mTestTime = QDateTime::fromTime_t(mTime);
	return mTestTime.toString("yyyy-MM-dd");
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
		// ��������Ϣ
		QString coursePic;
		QString teacherName = "";
		if (m_curTags->GetLessonType() == d_1V1Lesson)
		{
			coursePic = data["publicize_list_url"].toString();
			teacherName = data["teacher_name"].toString();
		}
		else
		if (m_curTags->GetLessonType() == d_AuxiliryLesson)
		{
			coursePic = data["publicize"].toString();
			teacherName = data["teacher_name"].toString();
		}
		QString courseName = data["name"].toString();
		QString courseGrade = data["grade"].toString();
		QString courseGrade1 = data["subject"].toString();
		
		QString coursePross = QString::number(data["closed_lessons_count"].toInt());
		QString courseProsses = QString::number(data["lessons_count"].toInt());
		QString courseStart = data["live_start_time"].toString();
		QString courseEnd = data["live_end_time"].toString();
		QString courseDesc = data["description"].toString();
		QString mCourseTarget = data["objective"].toString();
		QString mFitPeople = data["suit_crowd"].toString();
		QString mStatus = data["status"].toString();
		// ר���ε��ֶ�
		if (m_curTags->GetLessonType() == d_ExclusiveLesson)
		{
			coursePic = data["customized_group"].toObject()["publicizes_url"].toObject()["list"].toString();
			courseName = data["customized_group"].toObject()["subject"].toString();
			courseGrade = data["customized_group"].toObject()["grade"].toString();
			courseGrade1 = data["customized_group"].toObject()["subject"].toString();
			teacherName = data["customized_group"].toObject()["teacher_name"].toString();
			coursePross = QString::number(data["customized_group"].toObject()["closed_events_count"].toInt());
			courseProsses = QString::number(data["customized_group"].toObject()["events_count"].toInt());
			courseStart = mFun_ToTimeString(data["customized_group"].toObject()["start_at"].toInt());
			courseEnd = mFun_ToTimeString(data["customized_group"].toObject()["end_at"].toInt());
			courseDesc = data["customized_group"].toObject()["description"].toString();
		}

		// �꼶��Ϣ
		//courseGrade = courseGrade + courseGrade1 + " | ����: " + m_teacherName;
		QString mStatusAndCourseProcess = "";
		if (mStatus =="published")
		{
			mStatus = "������";
		}
		else
		if (mStatus == "teaching")
		{
			mStatus = "������";
		}
		else
		if (mStatus == "completed")
		{
			mStatus = "�ѽ���";
		}

		mStatusAndCourseProcess = mStatus + "[����" + coursePross + "/" + courseProsses + "]";
		courseStart = courseStart.left(10) + " �� " + courseEnd.left(10);

		QJsonArray mTagArray = data["tag_list"].toArray();
		if (m_CourseWnd)
		{
			m_CourseWnd->CleanCourseInfo();
			if (!mTagArray.empty())
			{
				m_CourseWnd->AddCourseTag(mTagArray);
			}
			m_CourseWnd->AddInfo(coursePic, courseGrade1, courseGrade, mStatusAndCourseProcess, courseStart, courseDesc, mCourseTarget, mFitPeople, courseProsses, m_teacherName, courseName);
		}
			
	}
	else if (error["code"].toInt() == 1002)
	{
		QString strError = QString("��Ȩ�ѹ��ڣ������µ�¼");
		int iStatus = CMessageBox::showMessage(
			QString("����ʱ��"),
			QString(strError),
			QString("ȷ��"),
			QString());
		if (iStatus == 1 || iStatus == 0)
		{
			ReturnLogin();
		}
		return;
	}
	else
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
			ui.lesson_pushButton->setStyleSheet("border-image: url(./images/lessonBtn_nor.png);");
			m_LessonWnd->setVisible(false);
			return;
		}

		ui.lesson_pushButton->setStyleSheet("border-image: url(./images/lessonBtn_hor.png);");
		m_LessonWnd->move(0, 70);
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

	if (m_curTags->GetLessonType() == d_1V1Lesson)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_courses/{id}";
		strUrl.replace("{id}", strCourseID);
	}
	else
	if (m_curTags->GetLessonType() == d_AuxiliryLesson)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/teachers/{teacherid}/courses/{id}";
		strUrl.replace("{id}", strCourseID);
		strUrl.replace("{teacherid}", m_teacherID);
	}
	else
	if (m_curTags->GetLessonType() == d_ExclusiveLesson)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/customized_groups/{id}/detail";
		strUrl.replace("{id}", strCourseID);
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
	int mWndHeight = 0;	// ������ʾ�߶�
	if (obj["status"].toInt() == 1)
	{
		// �γ���Ϣ
		QJsonArray lesson;
		if (m_curTags->GetLessonType() == d_1V1Lesson)
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
			if (m_curTags->GetLessonType() == d_ExclusiveLesson)
			{
				lesson = data["customized_group"].toObject()["offline_lessons"].toArray();
				QString mLessonTitle;
				if (lesson.count() > 0)
				{
					mLessonTitle = "����ֱ��";
					m_LessonWnd->AddLessonTitle(mLessonTitle);
					mWndHeight = mWndHeight + 40;
				}					
				foreach(const QJsonValue & value, lesson)
				{
					iCount++;
					QJsonObject obj = value.toObject();
					QString strClassDate = obj["class_date"].toString();
					QString strLiveTime = obj["start_time"].toString();
					QString strStatus = obj["status"].toString();
					QString strName = obj["name"].toString();

					strClassDate = strClassDate + "  " + strLiveTime;
					if (m_LessonWnd)
						m_LessonWnd->AddLesson(QString().sprintf("%02d", iCount), strClassDate, strName, strStatus);
				}
				lesson = data["customized_group"].toObject()["scheduled_lessons"].toArray();
				if (lesson.count() > 0)
				{
					mLessonTitle = "���½���";
					m_LessonWnd->AddLessonTitle(mLessonTitle);
					mWndHeight = mWndHeight + 40;
				}				
				foreach(const QJsonValue & value, lesson)
				{
					iCount++;
					QJsonObject obj = value.toObject();
					QString strClassDate = obj["class_date"].toString();
					QString strLiveTime = obj["start_time"].toString();
					QString strStatus = obj["status"].toString();
					QString strName = obj["name"].toString();

					strClassDate = strClassDate + "  " + strLiveTime;
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
		// ��Կγ���ʾ�Ĵ��ڽ��и߶�����Ӧ
		if (iCount != 0 && (iCount * 40 < 500))
		{
			if ((mWndHeight == 0) && iCount == 1)
			{
				mWndHeight = 10;
			}
			m_LessonWnd->setFixedHeight(mWndHeight + iCount*42 + 5);
		}
	}
	else if (error["code"].toInt() == 1002)
	{
		QString strError = QString("��Ȩ�ѹ��ڣ������µ�¼");
		int iStatus = CMessageBox::showMessage(
			QString("����ʱ��"),
			QString(strError),
			QString("ȷ��"),
			QString());
		if (iStatus == 1 || iStatus == 0)
		{
			ReturnLogin();
		}
		return;
	}
	else
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
			ui.person_pushButton->setStyleSheet("color:rgb(0,0,0);border-image: url(./images/personBtn_nor.png);");
			m_PersonWnd->setVisible(false);
			return;
		}

		ui.person_pushButton->setStyleSheet("color:rgb(255, 255, 255);border-image: url(./images/personBtn_hor.png);");
		m_PersonWnd->move(305, 70);
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
			QToolTip::showText(QCursor::pos(), "��Ա��ʼ����,���Ժ��ѯ��");
			m_PersonWnd->hide();
			ui.person_pushButton->setStyleSheet("color:rgb(0,0,0);border-image: url(./images/personBtn_nor.png);");
			return;
		}

		vecBuddy = m_curChatRoom->GetBuddy();
		if (m_PersonWnd)
			m_PersonWnd->AddPerson(vecBuddy, QString::fromStdString(m_curChatRoom->GetCurChatID()));
	}
}

// ���ÿ�ʼֱ����ť״̬
void UIWindowSet::setLiveBtnEnable(bool bEnable)
{
	ui.Live_pushBtn->setEnabled(bEnable);
	ui.time_label->setEnabled(bEnable);
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

/****************************ֱ������**************************************/
void UIWindowSet::UpatateLiveStatus(QWidget* widget, bool bSuc)
{
	if (bSuc)
	{
		if (m_VideoInfo == widget)
		{
			// �������ͷ�رգ�����û������ͷ
			if (!IsHasCamera() || m_EnumStatus == CameraStatusClose)
			{
				m_ScreenTip->setErrorTip("����ֱ���ɹ���");
				m_EnumStatus = CameraStatusClose;
				// 1������ǰװ壬һֱΪ����״̬
				m_LiveStatusManager->SendStartLiveHttpMsg(1, m_EnumStatus, m_lessonid, m_Token, m_curTags->GetLessonType());

				setLiveBtnEnable(true);
			}
			else//�������ͷ���ţ�����������ͷ��
			{
				//������ͷ
// 				m_EnumStatus = CameraStatusTeaching;
// 				m_CameraInfo->setVisible(true);
// 				m_CameraInfo->setPlugFlowUrl(m_cameraUrl);
// 				m_CameraInfo->StartLiveVideo();
				m_tempTimers->start(50);
			}
		}
		else if (m_CameraInfo == widget)
		{
			if (!ui.time_label->isVisible())
			{
				m_ScreenTip->setErrorTip("����ֱ���ɹ���");
				m_LiveStatusManager->SendStartLiveHttpMsg(1, m_EnumStatus, m_lessonid, m_Token, m_curTags->GetLessonType());
				setLiveBtnEnable(true);
			}
			/*
			else
			{
				//if (m_PullCamera == CameraIsOpen)
				{
					m_tempTimers->start(50);
					m_PullCamera = CameraIsPulling;
				}
										
			}
			*/
		}
	}
	else
	{
		// ��ֹ����һ��ʧ�ܣ���һ��ҲҪ����
		if (m_VideoInfo == widget)
		{
			// ��ʾ������Ϣ
			if (m_EnumStatus == CameraStatusTeaching)
				m_CameraInfo->setVisible(true);
			m_ScreenTip->setErrorTip("ֱ��������������ִ���������\n����ֱ����");
		}
		else
		{
			m_CameraInfo->setVisible(true);
			m_VideoInfo->StopLiveVideo();
			// ��ʾ������Ϣ
			m_ScreenTip->setErrorTip("ֱ��������������ִ���������\n����ֱ����");
		}
		setLiveBtnEnable(true);
	}
}

// ����ͷ�ӳ�1������
void UIWindowSet::slot_onTempTimeout()
{
	m_tempTimers->stop();
	m_EnumStatus = CameraStatusTeaching;
	m_CameraInfo->setVisible(true);
	m_CameraInfo->setPlugFlowUrl(m_cameraUrl);
	m_CameraInfo->StartLiveVideo();
}

void UIWindowSet::setCameraEnable()
{
	ui.video_checkBox->setEnabled(true);
}

// ֱ�������г���ֱֹͣ��
void UIWindowSet::ErrorStopLive(QWidget* pWidget)
{
	if (m_CameraInfo && m_VideoInfo)
	{
		if (!m_VideoInfo->IsCurrentLiving() && !m_CameraInfo->IsCurrentLiving())
		{
			return;
		}
		QString strError = "ֱ��������������ִ���������\n����ֱ����";
		int iStatus = CMessageBox::showMessage(
			QString("����ʱ��"),
			QString(strError),
			QString("ȷ��"),
			QString());

		m_VideoInfo->StopLiveVideo();

		ui.Live_pushBtn->setText(LIVE_BUTTON_NAME);
		ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");
		if (m_CountTimer->isActive())
		{
			m_CountTimer->stop();					// ֹͣ��ʱ
			m_iTimerCount = 0;						// ��������
			ui.time_label->setVisible(false);		// ����
			ui.time_label->setText("00:00:00");		// ����ʱ��
		}
		
		qDebug() << "λ��2";
		//m_LiveStatusManager->SendStopLiveHttpMsg();	add by zbc 20170921 ���粻������£��Ȳ�����ֱ���ر�

		setLiveBtnEnable(true);
	}
}

void UIWindowSet::ErrorStop()
{
	if (m_CameraInfo)
	{
		m_CameraInfo->StopLiveVideo();
		m_CameraInfo->StopCaptureVideo();
		m_CameraInfo->DeleteThread();	// add by zbc 20170920
	}
		
	if (m_VideoInfo)
	{
		m_VideoInfo->StopLiveVideo();
		m_VideoInfo->StopCaptureVideo();
		m_VideoInfo->DeleteThread();	// add by zbc 20170920
	}
		
}

bool UIWindowSet::IsHasCamera()
{
	return m_bHasCamera;
}

// �����ƵԴ
void UIWindowSet::addVideoDevices(QString name, QString path)
{
	m_bHasCamera = true;
	m_VideoChangeInfo->setVideoParam(name, path);
}

// �����˷�
void UIWindowSet::addAudioDevices(QString name, QString path)
{
	m_AudioChangeInfo->setAudioParam(name, path);
}

void UIWindowSet::AddTodayToLesson(QString  id, QString courseid, QString boardUrl, QString CameraUrl, QString time, QString status, QString name)
{
	m_LiveLesson->addLesson(id,courseid,boardUrl,CameraUrl,time,status, name);
}
/****************************������������**************************************/
void UIWindowSet::AudioStatus(int iStatus)
{
	qDebug() << __FILE__ << __LINE__ << "��Ƶ����" << "״ֵ̬Ϊ��" << iStatus;
	if (iStatus)
	{
		try
		{
			m_VideoInfo->SetPauseAudio(NULL);
		}
		catch (...)
		{
			qDebug() << __FILE__ << __LINE__ << "��Ƶ����ʧ��" << "״ֵ̬Ϊ��"<< iStatus;
			m_VideoInfo->SetPauseAudio(NULL);
		}
		
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
	{
		//������ʽƥ��������
		QString StrEmoji;
		StrEmoji = "\\[em_[0-9]{1,2}\\]";
		content.replace(QRegExp(StrEmoji), "[����]");
		m_BulletScreen->ReciverStudent(name, content, chatid);
	}
		
}

void UIWindowSet::SendTeacherBullet(QString name, QString content, QString chatid)
{
	if (m_BulletScreen)
	{
		//������ʽƥ��������
		QString StrEmoji;
		StrEmoji = "\\[em_[0-9]{1,2}\\]";
		content.replace(QRegExp(StrEmoji), "[����]");
		m_BulletScreen->ReciverTeacher(name, content, chatid);
	}

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
	ui.Bullet1v1_checkBox->setCheckState(Qt::CheckState::Unchecked);
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
		m_PullCamera = CameraIsClose;
		//����
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
			//m_CameraInfo->SetPauseVideo();
			if (m_curTags->GetLessonType() != d_1V1Lesson)
			{
				m_LiveStatusManager->SendCameraSwitchMsg(1, m_EnumStatus);
			}
		}
	}
	else
	{
		m_PullCamera = CameraIsOpen;
		//��ʾ
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
			//m_CameraInfo->SetResumeVideo();
			if (m_curTags->GetLessonType() != d_1V1Lesson)
			{
				m_LiveStatusManager->SendCameraSwitchMsg(1, m_EnumStatus);
			}		
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
	if (m_CameraInfo)
	{
		m_CameraInfo->SetChangeVideo(index);
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
	if (ratio=="����")
	{
		m_VideoInfo->setRatio(1);
	}
	else
	{
		m_VideoInfo->setRatio(0);
	}
}

void UIWindowSet::SendStopLive()
{
	// ���ͽ���ֱ����Ϣ�ٹر�
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
		QString("����ʱ��"),
		QString(sError),
		QString("ȷ��"));
}

bool UIWindowSet::IsLiving()
{
	if (m_VideoInfo->IsCurrentLiving())
	{
		m_ScreenTip->setErrorTip("���Ƚ���ֱ�����ٹرմ��ڣ�");
		return m_VideoInfo->IsCurrentLiving();
	}

	if (m_bLiving1v1)
	{
		m_ScreenTip->setErrorTip("���Ƚ���ֱ�����ٹرմ��ڣ�");
		return m_bLiving1v1;
	}

	return false;
}

void UIWindowSet::ReturnLogin()
{
	m_parent->returnClick();
}

void UIWindowSet::StopSuccess(QWidget* widget)
{
	if (m_VideoInfo == widget)
	{
		if (IsHasCamera() && m_EnumStatus != CameraStatusClose)
			m_CameraInfo->StopLiveVideo();
	}
}

// ������Ļ����
void UIWindowSet::MathScreenSize()
{
	QDesktopWidget* dsk = QApplication::desktop();
	int iWidth = dsk->width();
	int iHeight = dsk->height();
	if (iWidth / 16 * 9 == iHeight)
		m_ScreenType = SCREEN_16_9;
	else if (iWidth / 16 * 10 == iHeight)
		m_ScreenType = SCREEN_16_10;
	else if (iWidth / 4 * 3 == iHeight)
		m_ScreenType = SCREEN_4_3;
}

/***************************************************************************/
/*																		   */
/*					  ��ʼ�����š��װ塢���ص�							   */
/*																		   */
/***************************************************************************/
void UIWindowSet::initCallBack()
{
	IMInterface::getInstance()->initVChat();
	IMInterface::getInstance()->initVChatCallback();
	IMInterface::getInstance()->initRtsCallback();
	// ��ʼ���װ�
	initWhiteBoardWidget();
	initConnection();

 	IMInterface::getInstance()->EnumDeviceDevpath(AudioDevice);
	IMInterface::getInstance()->EnumDeviceDevpath(Video);
	IMInterface::getInstance()->EnumDeviceDevpath(AudioOut);

	// ������Ϣ�ص�
	nim::Talk::RegReceiveCb(&CallbackReceiveMsg);
	// ������Ϣ״̬�ص�
	nim::Talk::RegSendMsgCb(&CallbackSendMsgArc);

	InitAudioCallBack();
}

// ������Ϣ�ص�
void CallbackReceiveMsg(const nim::IMMessage& msg)
{
	nim::IMMessage* pMsg = new nim::IMMessage;
	pMsg->session_type_ = msg.session_type_;
	pMsg->receiver_accid_ = msg.receiver_accid_;
	pMsg->sender_accid_ = msg.sender_accid_;
	pMsg->readonly_sender_client_type_ = msg.readonly_sender_client_type_;
	pMsg->readonly_sender_device_id_ = msg.readonly_sender_device_id_;
	pMsg->readonly_sender_nickname_ = msg.readonly_sender_nickname_;
	pMsg->timetag_ = msg.timetag_;

	pMsg->type_ = msg.type_;
	pMsg->content_ = msg.content_;
	pMsg->attach_ = msg.attach_;
	pMsg->client_msg_id_ = msg.client_msg_id_;
	pMsg->readonly_server_id_ = msg.readonly_server_id_;

	pMsg->local_res_path_ = msg.local_res_path_;
	pMsg->local_talk_id_ = msg.local_talk_id_;
	pMsg->local_res_id_ = msg.local_res_id_;
	pMsg->status_ = msg.status_;
	pMsg->sub_status_ = msg.sub_status_;
	
	PostMessage(m_This->GetParentWnd(), MSG_CLIENT_RECEIVE, (WPARAM)pMsg, 0);
}

// ������Ϣ�ص�
void CallbackSendMsgArc(const nim::SendMessageArc& arc)
{
	nim::SendMessageArc* arcNew = new nim::SendMessageArc;
	arcNew->msg_id_ = arc.msg_id_;
	arcNew->msg_timetag_ = arc.msg_timetag_;
	arcNew->rescode_ = arc.rescode_;
	arcNew->talk_id_ = arc.talk_id_;

	PostMessage(m_This->GetParentWnd(), MSG_SEND_MSG_STATUS, (WPARAM)arcNew, 0);
}

// ��ѯ��ʷ��¼�ص�
void UIWindowSet::QueryMsgOnlineCb(nim::NIMResCode code, const std::string& id, nim::NIMSessionType type, const nim::QueryMsglogResult& result)
{
	nim::QueryMsglogResult* pRes = new nim::QueryMsglogResult;
	pRes->msglogs_ = result.msglogs_;
	pRes->count_ = result.count_;

	PostMessage(m_This->GetParentWnd(), MSG_CLIENT_RECORD, (WPARAM)pRes, 0);
}

// ��һ�β�ѯ��ʷ��¼�ص�
void UIWindowSet::QueryFirstMsgOnlineCb(nim::NIMResCode code, const std::string& id, nim::NIMSessionType type, const nim::QueryMsglogResult& result)
{
	nim::QueryMsglogResult* pRes = new nim::QueryMsglogResult;
	pRes->msglogs_ = result.msglogs_;
	pRes->count_ = result.count_;

	PostMessage(m_This->GetParentWnd(), MSG_CLIENT_RECORD, (WPARAM)pRes, 1);
}

// ��ȡ��Ա�ص�
void UIWindowSet::OnGetTeamMemberCallback(const std::string& tid, int count, const std::list<nim::TeamMemberProperty>& team_member_info_list)
{
	std::list<nim::TeamMemberProperty> *pTeamList = new std::list<nim::TeamMemberProperty>;

	for (const auto& member : team_member_info_list)
		pTeamList->push_back(member);

	PostMessage(m_This->GetParentWnd(), MSG_MEMBERS_INFO, (WPARAM)pTeamList, 0);
}

HWND UIWindowSet::GetParentWnd()
{
	if (m_parent)
		return (HWND)m_parent->winId();
	else
		return NULL;
}

/***************************************************************************/
/*																		   */
/*								����ֱ��								   */
/*																		   */
/***************************************************************************/
//��ʼ���װ�
void UIWindowSet::initWhiteBoardWidget()
{
	mWhiteBoard = new Palette();
	ui.horizontalLayout_21->addWidget(mWhiteBoard);
	mWhiteBoard->setIsDraw(true);
	connect(mWhiteBoard, SIGNAL(PicData(QString, QString)), this, SLOT(PicData(QString,QString)));

	m_WhiteBoardTool = new UIWhiteBoardTool(ui.live1v1_widget);
	m_WhiteBoardTool->hide();
	connect(m_WhiteBoardTool, SIGNAL(selectColor(QColor&)), this, SLOT(selectColor(QColor&)));
	connect(m_WhiteBoardTool, SIGNAL(returnClick()), this, SLOT(returnClick()));
	connect(m_WhiteBoardTool, SIGNAL(deleteClick()), this, SLOT(deleteClick()));
	connect(m_WhiteBoardTool, SIGNAL(laserClick()), this, SLOT(laserClick()));
	connect(m_WhiteBoardTool, SIGNAL(drawClick()), this, SLOT(drawClick()));

	// ��ʦ�Լ�������ͷ
	m_Camera1v1Info = new UICamera1v1(ui.cameraT_widget);
	m_Camera1v1Info->setWindowFlags(Qt::FramelessWindowHint);
	ui.horizontalLayout_22->addWidget(m_Camera1v1Info);
	m_Camera1v1Info->show();

	// ѧ������ͷ
	m_CameraS1v1Info = new UICameraS1v1(ui.cameraS_widget);
	m_CameraS1v1Info->setWindowFlags(Qt::FramelessWindowHint);
	ui.horizontalLayout_23->addWidget(m_CameraS1v1Info);
	m_CameraS1v1Info->setParentWnd(ui.cameraS_widget);
	m_CameraS1v1Info->show();

	// 1v1ȫ������
	m_VideoInfo1v1 = new UIVideo1v1(ui.fullS1v1_widget);
	m_VideoInfo1v1->setWindowFlags(Qt::FramelessWindowHint);
	ui.horizontalLayout_21->addWidget(m_VideoInfo1v1);
	m_VideoInfo1v1->hide();
	connect(m_VideoInfo1v1, SIGNAL(sig_refreshWnd()), this, SLOT(slot_refreshWnd())); 
	connect(m_VideoInfo1v1, SIGNAL(sig_CustomVideoData(__int64, const char*, int, int, int)), this, SLOT(slot_CustomVideoData(__int64, const char*, int, int, int)));

	m_AppWndTool1v1 = new UIAppWndTool(ui.live1v1_widget);
	m_AppWndTool1v1->hide();
	connect(m_AppWndTool1v1, SIGNAL(sig_shiftWnd()), this, SLOT(slot_shiftWnd()));
	connect(m_AppWndTool1v1, SIGNAL(sig_CloseWnd()), this, SLOT(slot_CloseWnd()));

	// 1v1ѡ�񴰿ڽ���
	m_AppWnd1v1 = new UIAppWnd();
	m_AppWnd1v1->setWindowFlags(Qt::FramelessWindowHint);
	m_AppWnd1v1->hide();
	connect(m_AppWnd1v1, SIGNAL(sig_selectWnd(HWND)), this, SLOT(slot_selectWnd(HWND)));
	SetWindowPos((HWND)m_AppWnd1v1->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void UIWindowSet::slot_shiftWnd()
{
	if (m_AppWnd1v1)
	{
		m_AppWnd1v1->deleteWnd();
		m_AppWnd1v1->show();
		m_AppWnd1v1->move(QApplication::desktop()->width() / 2 - m_AppWnd1v1->width() / 2, QApplication::desktop()->height() / 2 - m_AppWnd1v1->height() / 2);

		CaptureWindowInfoList wnd_list;
		m_VideoInfo1v1->GetCaptureWindowList(&wnd_list);
		CaptureWindowInfo dest_info;
		dest_info.id = GetDesktopWindow();
		dest_info.title = L"ȫ������";
		wnd_list.insert(wnd_list.begin(), dest_info);
		for (auto& it : wnd_list)
		{
			m_AppWnd1v1->AddAppWnd(QString::fromStdWString(it.title), it.id);
		}
	}
}

void UIWindowSet::slot_CloseWnd()
{
	// �򿪰װ弰������
	mWhiteBoard->setVisible(true);
	m_WhiteBoardTool->setVisible(true);

	// ����ץȡ�Ĵ��ھ��Ϊ�ռ�����
	m_VideoInfo1v1->setCaptureWnd(NULL);
	m_VideoInfo1v1->setVisible(false);
	m_AppWndTool1v1->setVisible(false);

	// ���ͷ�����֪ͨ��ѧ����(�װ���Ϣ)
	mWhiteBoard->SendFullScreen(false);
	// ͨ���Զ���Ⱥ��֪ͨ���Ͱװ��л���Ϣ add by zbc 20170825
	if (m_curChatRoom)
	{
		QString mType, mContent;
		mType = "SwitchVedioObject";
		mContent = "Board";
		m_curChatRoom->SendSelfDefineNotification(mType, mContent);
		m_PublicCameraStatus = 1;
		SendFrameStatusToWebService(); // ֪ͨweb��ǰ��ʦ��ֱ��������Ļ
	}

	// ���÷�������ģʽΪ�Զ��巢��
	IMInterface::getInstance()->SetCustomData(false);

	// ��������ͷ
	m_Camera1v1Info->StartEndVideo(false);

	// �������
	ui.video1v1_checkBox->setEnabled(true);
}

void UIWindowSet::slot_refreshWnd()
{
	int iWidth = ui.full1v1_widget->width();
	int iHeight = ui.full1v1_widget->height();

	// ����ץȡ
	int iScreenWidth = iWidth;
	int iScreenHeight = ((double)iWidth / (double)m_VideoInfo1v1->ScreenWidth())*m_VideoInfo1v1->ScreenHeight();
	if (iScreenHeight > iHeight)
	{
		iScreenWidth = ((double)iHeight / (double)m_VideoInfo1v1->ScreenHeight())*m_VideoInfo1v1->ScreenWidth();
		m_VideoInfo1v1->setFixedSize(iScreenWidth, iHeight);
	}
	else
		m_VideoInfo1v1->setFixedSize(iScreenWidth, iScreenHeight);
}

void UIWindowSet::slot_CustomVideoData(__int64 time, const char* data, int size, int iwidth, int iheight)
{
	// ���¼���1��1��Ļ�ߴ�
	Math1v1Screen();

	IMInterface::getInstance()->CustomVideoData(time, data, size, iwidth, iheight);
}

void UIWindowSet::slot_selectWnd(HWND hwnd)
{
	// ���ذװ弰������
	mWhiteBoard->setVisible(false);
	m_WhiteBoardTool->setVisible(false);

	// ����ץȡ�Ĵ��ھ������ʾ
	m_VideoInfo1v1->setCaptureWnd(hwnd);
	m_VideoInfo1v1->setVisible(true);
	m_AppWndTool1v1->setVisible(true);

	// ���ͷ�����֪ͨ��ѧ����
	mWhiteBoard->SendFullScreen(true);
	// ͨ���Զ���Ⱥ��֪ͨ���Ͱװ��л���Ϣ add by zbc 20170825
	if (m_curChatRoom)
	{
		QString mType, mContent;
		mType = "SwitchVedioObject";
		mContent = "Desktop";
		m_curChatRoom->SendSelfDefineNotification(mType, mContent);
		m_PublicCameraStatus = 0;
		SendFrameStatusToWebService(); // ֪ͨweb��ǰ��ʦ��ֱ��������Ļ
	}

	// ���÷�������ģʽΪ�Զ��巢��
	IMInterface::getInstance()->SetCustomData(true);

	// �ر�����ͷ
	m_Camera1v1Info->StartEndVideo(true);

	// ��������ͷ
	ui.video1v1_checkBox->setEnabled(false);
}

void UIWindowSet::PicData(QString str,QString uid)
{
	std::string picData = str.toStdString();
	std::string strUid = uid.toStdString();
	IMInterface::getInstance()->SendData("", 1, picData,strUid);
}

void UIWindowSet::initConnection()
{
	IMInterface *instance = IMInterface::getInstance();
	if (NULL == instance)
	{
		return;
	}

	connect(instance, SIGNAL(createRtsRoomSuccessfully(const std::string&)), this, SLOT(joinRtsRoom(const std::string&)));
	connect(instance, SIGNAL(joinRtsRoomSuccessfully()),this, SLOT(joinRoomSuccessfully()));
	connect(instance, SIGNAL(createVChatRoomSuccessfully()), this, SLOT(joinVChatRoom()));
	connect(instance, SIGNAL(joinVChatSuccessfully(QString)),this, SLOT(joinVChatSuccessfully(QString)));
 	connect(instance, SIGNAL(hasError(const QString &)), this, SLOT(errorInfo(const QString &)));
	connect(instance, SIGNAL(deviceInfos(int)), this, SLOT(setDeviceInfos(int)));
	connect(instance, SIGNAL(startDeviceSuccessfully(int)), this, SLOT(startDeviceSuccessfully(int)));
	connect(instance, SIGNAL(VideoCapture(const char*, unsigned int, unsigned int, unsigned int)), m_Camera1v1Info, SLOT(VideoCapture(const char*, unsigned int, unsigned int, unsigned int)));
	connect(instance, SIGNAL(RecVideoCapture(const char*, unsigned int, unsigned int, unsigned int)), m_CameraS1v1Info, SLOT(VideoCapture(const char*, unsigned int, unsigned int, unsigned int)));
	connect(instance, SIGNAL(PeopleStatus(bool)), m_CameraS1v1Info, SLOT(StartEndVideo(bool)));
	connect(instance, SIGNAL(FunctionGetNetState(int)), this, SLOT(slot_GetNetState(int)));

	connect(instance, SIGNAL(rtsDataReceived(const std::string&, const std::string &)), this, SLOT(rtsDataReceived(const std::string&, const std::string &)));
	connect(instance, SIGNAL(requstError(QString)), this, SLOT(requstError(QString)));
	connect(instance, SIGNAL(sig_rtsTcpDiscontect()), this, SLOT(slot_rtsTcpDiscontect()));
}

void UIWindowSet::joinRtsRoom(const std::string &roomName)
{
	IMInterface::getInstance()->joinRtsRoom(roomName, false);
}

void UIWindowSet::joinRoomSuccessfully()
{
	if (m_curTags)
	{
		if (m_curTags->GetLessonType() == d_1V1Lesson)
		{
			IMInterface::getInstance()->m_BoardURL = m_BoardPushURL;
			IMInterface::getInstance()->m_LessonType = d_1V1Lesson;
		}
		else
		{
			IMInterface::getInstance()->m_LessonType = m_curTags->GetLessonType();
		}
		IMInterface::getInstance()->createVChatRoom(m_curTags->ChatID().toStdString());
	}
		
}

void UIWindowSet::joinVChatRoom()
{	
	if (m_curTags)
		IMInterface::getInstance()->joinVChatRoom(2,m_curTags->ChatID().toStdString());
}

// ��������Ƶ�ɹ��󣬿�ʼֱ������
void UIWindowSet::joinVChatSuccessfully(QString mChannelID)
{
	m_bLiving1v1 = true;
	m_LiveStatusManager->m_ChannelID = mChannelID;
	m_LiveStatusManager->SendStart1v1LiveHttpMsg(m_lessonid, m_curTags->ChatID(), m_Token);
	//������Ϣ��֪ͨ
	QString mOnline = "";
	QString mStartText = "ֱ����ʼ";
	mOnline = "ScheduledLesson";
	m_curChatRoom->SendSelfDefineMessageScreen(true, mOnline);
	m_curChatRoom->m_uitalk->InsertOneNotice(mStartText);
}

void UIWindowSet::requstError(QString error)
{
	m_bLiving1v1 = false;
	CMessageBox::showMessage(
		QString("����ʱ��"),
		QString(error),
		QString("ȷ��"),
		QString("ȡ��"));
}

void UIWindowSet::errorInfo(const QString & error)
{
	m_bLiving1v1 = false;
	CMessageBox::showMessage(
		QString("����ʱ��"),
		QString("���뷿����ִ��󣡴��룺") + error,
		QString("ȷ��"),
		QString("ȡ��"));
}

void UIWindowSet::setDeviceInfos(int type)
{
	switch (type)
	{
	case AudioDevice:
	{
		foreach(const DevInfo &info, IMInterface::getInstance()->getDeviceInfos())
		{
			if (info.type == type)
			{
				m_AudioChangeInfo1v1->setAudioParam(info.name, info.path);
			}
		}
	}
	break;
	case AudioOut:
	{
		foreach(const DevInfo &info, IMInterface::getInstance()->getDeviceInfos())
		{
			if (info.type == type)
			{
				m_AudioOutChangeInfo1v1->setAudioParam(info.name, info.path);
			}
		}
	}
	break;
	case Video:
		foreach(const DevInfo &info, IMInterface::getInstance()->getDeviceInfos())
		{
			if (info.type == type)
			{
				m_VideoChangeInfo1v1->setVideoParam(info.name,info.path);
			}
		}
		break;
	default:
		break;
	}
}

void UIWindowSet::InitSetParamWnds()
{
	m_AudioChangeInfo1v1 = new UIAudioChange1v1(this);
	m_AudioChangeInfo1v1->setWindowFlags(Qt::FramelessWindowHint);
	m_AudioChangeInfo1v1->setAudioChange(this);
	m_AudioChangeInfo1v1->hide();

	m_AudioOutChangeInfo1v1 = new UIAudioOutChange1v1(this);
	m_AudioOutChangeInfo1v1->setWindowFlags(Qt::FramelessWindowHint);
	m_AudioOutChangeInfo1v1->setAudioChange(this);
	m_AudioOutChangeInfo1v1->hide();

	m_VideoChangeInfo1v1 = new UIVideoChange1v1(this);
	m_VideoChangeInfo1v1->setWindowFlags(Qt::FramelessWindowHint);
	m_VideoChangeInfo1v1->setVideoChange(this);
	m_VideoChangeInfo1v1->hide();

	// ������˷���ʽ
	ui.Audio1v1_checkBox->setStyleSheet("QCheckBox::indicator{width: 24px;height: 19px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/mic_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/mic_close.png);}");
	ui.Audio1v1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/Arrow.png);}");

	// ��������ͷ��ƵԴ��ʽ
	ui.video1v1_checkBox->setStyleSheet("QCheckBox::indicator{width: 29px;height: 19px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/camera_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/camera_close.png);}");
	ui.video1v1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/Arrow.png);}");

	// ���õ�Ļ��ʽ
	ui.Bullet1v1_checkBox->setStyleSheet("QCheckBox::indicator{width: 26px;height: 19px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/bullet_close.png);}"
		"QCheckBox::indicator:checked{image: url(./images/bullet_open.png);}");
	ui.Bullet1v1_pushButton->setStyleSheet("QPushButton{border-image:url(./images/Arrow.png);}");

	// ����������������ʽ
	ui.AudioOut_checkBox->setStyleSheet("QCheckBox::indicator{width: 29px;height: 19px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/audioout_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/audioout_close.png);}");
	ui.AudioOut_pushButton->setStyleSheet("QPushButton{border-image:url(./images/Arrow.png);}");

	ui.shapeScreen_pushButton->setStyleSheet("QPushButton{border-image:url(./images/shapeScreen.png);}");

	connect(ui.Audio1v1_checkBox, SIGNAL(stateChanged(int)), this, SLOT(Audio1v1Status(int)));
	connect(ui.Audio1v1_pushButton, SIGNAL(clicked()), this, SLOT(clickAudio1v1Param()));
	connect(ui.video1v1_checkBox, SIGNAL(stateChanged(int)), this, SLOT(Video1v1Status(int)));
	connect(ui.video1v1_pushButton, SIGNAL(clicked()), this, SLOT(clickVideo1v1Param()));
	connect(ui.Bullet1v1_checkBox, SIGNAL(stateChanged(int)), this, SLOT(BulletStatus(int)));
	connect(ui.Bullet1v1_pushButton, SIGNAL(clicked()), this, SLOT(clickBulletParam()));
	connect(ui.AudioOut_checkBox, SIGNAL(stateChanged(int)), this, SLOT(AudioOut1v1Status(int)));
	connect(ui.AudioOut_pushButton, SIGNAL(clicked()), this, SLOT(clickAudioOut1v1Param()));
	connect(ui.Live1v1_pushBtn, SIGNAL(clicked()), this, SLOT(clickLive1v1()));
	connect(ui.time1v1_label, SIGNAL(clicked()), this, SLOT(clickLive1v1()));
	connect(ui.shapeScreen_pushButton, SIGNAL(clicked()), this, SLOT(clickShapeScreen1v1()));
}

void UIWindowSet::Audio1v1Status(int iStatus)
{
	if (iStatus)
	{
		IMInterface::getInstance()->endDevice(AudioDevice);
	}
	else
	{
		//�������ŵ�����ͷ�ɼ�
		QString dPath = m_AudioChangeInfo1v1->GetCurPath();
		if (!dPath.isNull())
			IMInterface::getInstance()->startDevice(AudioDevice, dPath.toStdString(), 0, 0, 0);
	}
}

void UIWindowSet::Video1v1Status(int iStatus)
{
	if (iStatus)
	{
		//�ر����ŵ�����ͷ�ɼ�
		IMInterface::getInstance()->endDevice(Video);
		m_Camera1v1Info->StartEndVideo(true);
	}
	else
	{
		//�������ŵ�����ͷ�ɼ�
		QString dPath = m_VideoChangeInfo1v1->GetCurPath();
		if (!dPath.isNull())
			IMInterface::getInstance()->startDevice(Video, dPath.toStdString(), VIDEO_FPS, 0, 0);
		m_Camera1v1Info->StartEndVideo(false);
	}
}

void UIWindowSet::AudioOut1v1Status(int iSatus)
{
	if (iSatus)
	{
		IMInterface::getInstance()->endDevice(AudioOut);
	}
	else
	{
		//�������ŵ�����ͷ�ɼ�
		QString dPath = m_AudioOutChangeInfo1v1->GetCurPath();
		if (!dPath.isNull())
			IMInterface::getInstance()->startDevice(AudioOut, dPath.toStdString(), 0, 0, 0);
	}
}

void UIWindowSet::clickVideo1v1Param()
{
	m_VideoChangeInfo1v1->show();
	m_VideoChangeInfo1v1->move(width() / 2 - m_VideoChangeInfo1v1->width() / 2, height() / 2 - m_VideoChangeInfo1v1->height() / 2);
	m_VideoChangeInfo1v1->setFocus();
}

void UIWindowSet::clickAudio1v1Param()
{
	m_AudioChangeInfo1v1->show();
	m_AudioChangeInfo1v1->move(width() / 2 - m_AudioChangeInfo1v1->width() / 2, height() / 2 - m_AudioChangeInfo1v1->height() / 2);
	m_AudioChangeInfo1v1->setFocus();
}

void UIWindowSet::clickAudioOut1v1Param()
{
	m_AudioOutChangeInfo1v1->show();
	m_AudioOutChangeInfo1v1->move(width() / 2 - m_AudioOutChangeInfo1v1->width() / 2, height() / 2 - m_AudioOutChangeInfo1v1->height() / 2);
	m_AudioOutChangeInfo1v1->setFocus();
}

void UIWindowSet::setAudioChange1v1(QString path)
{
	if (!ui.Audio1v1_checkBox->isChecked())
	{
		if (!path.isNull())
		{
			
			IMInterface::getInstance()->endDevice(AudioDevice);
			IMInterface::getInstance()->startDevice(AudioDevice, path.toStdString(), 0, 0, 0);
		}
	}
}

void UIWindowSet::setValueChange1v1(int iVolumn, bool capturn)
{
	IMInterface::getInstance()->setAudioChange(iVolumn, capturn);
}

void UIWindowSet::setVideoChange1v1(QString path)
{
	if (!ui.video1v1_checkBox->isChecked())
	{
		if (!path.isNull())
		{
			IMInterface::getInstance()->endDevice(Video);
			IMInterface::getInstance()->startDevice(Video, path.toStdString(), VIDEO_FPS, 0, 0);
		}
	}
}

void UIWindowSet::clickLive1v1()
{
	if (m_bLiving1v1)
	{
		int iStatus = CMessageBox::showMessage(
			QString("����ʱ��"),
			QString("�Ƿ�رյ�ǰֱ����"),
			QString("ȷ��"),
			QString("ȡ��"));
		if (iStatus == 1)
		{
			// �ѷ�����Ļ�ر�
			slot_CloseWnd();
			mWhiteBoard->stopSendMsg();
			ui.Live1v1_pushBtn->setText(LIVE_BUTTON_NAME);
			ui.Live1v1_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");

			// �����װ������Ƶ
			// ʵ�ֽ����װ������Ƶ����......��д
			IMInterface::getInstance()->EndLive(m_curTags->ChatID().toStdString());

			m_LiveStatusManager->SendStopLiveHttpMsg1v1();

			if (m_CountTimer->isActive())
			{
				m_CountTimer->stop();					// ֹͣ��ʱ
				m_iTimerCount = 0;						// ��������
				ui.time1v1_label->setVisible(false);	// ����
				ui.time1v1_label->setText("00:00:00");	// ����ʱ��
			}

			m_bLiving1v1 = false;
			//������Ϣ��֪ͨ
			QString mOnline = "";
			QString mStartText = "ֱ������";
			mOnline = "ScheduledLesson";
			m_curChatRoom->SendSelfDefineMessageScreen(false, mOnline);
			m_curChatRoom->m_uitalk->InsertOneNotice(mStartText);

		}
		else
			return;
	}
	else
	{
		if (m_curTags == NULL)
			return;

		m_LiveLesson->DeleteItem();
		m_LiveLesson->move(width() / 2 - m_LiveLesson->width() / 2, height() / 2 - m_LiveLesson->height() / 2);
		m_LiveLesson->show();
		m_LiveLesson->setFocus();	
		ShowLesson();
		RecordLive();
	}
}

// ���¿�ʼ1��1ֱ����ť����ʱ����״̬
void UIWindowSet::start1v1LiveStream()
{
	m_CountTimer->start(1000);
	ui.time1v1_label->setVisible(true);
	ui.Live1v1_pushBtn->setText("����ֱ��");
	ui.Live1v1_pushBtn->setStyleSheet("QPushButton{background-color:white;color: red;border-radius: 5px; border: 2px solid red;}");

	// ��ʼ��·ֱ��
//	RecordLive();
}

void UIWindowSet::RecordLive()
{
	if (m_curTags == NULL || !m_curTags->IsModle())
		return;

	QString strCourseID = "";
	if (m_curTags)
		strCourseID = m_curTags->CourseID();
	else
		return;

	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_courses/{id}/detail";
		strUrl.replace("{id}", strCourseID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_courses/{id}/detail";
		strUrl.replace("{id}", strCourseID);
	}

	HttpRequest http;
	http.setRawHeader("Remember-Token", m_Token.toUtf8());

	QByteArray result = http.httpGet(strUrl);
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1)
	{
		QJsonObject online = data["interactive_course"].toObject();
		QString mBoardPushURL = online["board_push_stream"].toString();
		qDebug() << "��·ֱ��������ַ:" << mBoardPushURL;
		m_BoardPushURL = mBoardPushURL;
//		m_VideoRecordInfo->setPlugFlowUrl(mBoardPushURL);
// 		m_VideoRecordInfo->StartLiveVideo();
	}
	else
	{
		qDebug() << "��ȡ��·ֱ��������ַʧ�ܣ�";
	}
}

void UIWindowSet::show1v1ErrorTip(QString sError)
{
	m_bLiving1v1 = false;
	CMessageBox::showMessage(
		QString("����ʱ��"),
		QString(sError),
		QString("ȷ��"));

	IMInterface::getInstance()->EndLive(m_curTags->ChatID().toStdString());
}

void UIWindowSet::selectColor(QColor& color)
{
	if (mWhiteBoard)
	{
		mWhiteBoard->setPenColor(color);
	}
}

void UIWindowSet::rtsDataReceived(const std::string& data, const std::string &uid)
{
	if (mWhiteBoard)
		mWhiteBoard->RecData(data, uid);
}

void UIWindowSet::returnClick()
{
	if (mWhiteBoard)
		mWhiteBoard->revocation();
}

void UIWindowSet::deleteClick()
{
	if (mWhiteBoard)
		mWhiteBoard->cleanUp();
}

void UIWindowSet::laserClick()
{
	if (mWhiteBoard)
		mWhiteBoard->setIsDraw(false);
}

void UIWindowSet::drawClick()
{
	if (mWhiteBoard)
		mWhiteBoard->setIsDraw(true);
}

void UIWindowSet::clickShapeScreen1v1()
{
	if (!m_bLiving1v1)
	{
		QToolTip::showText(QCursor::pos(), "�뿪��ֱ�����ٷ����ڣ�");
		return;
	}
	if (m_AppWnd1v1)
	{
		m_AppWnd1v1->deleteWnd();
		m_AppWnd1v1->show();
		m_AppWnd1v1->move(QApplication::desktop()->width() / 2 - m_AppWnd1v1->width() / 2, QApplication::desktop()->height() / 2 - m_AppWnd1v1->height() / 2);

		CaptureWindowInfoList wnd_list;
		m_VideoInfo1v1->GetCaptureWindowList(&wnd_list);
		CaptureWindowInfo dest_info;
		dest_info.id = GetDesktopWindow();
		dest_info.title = L"ȫ������";
		wnd_list.insert(wnd_list.begin(), dest_info);
		for (auto& it : wnd_list)
		{
			m_AppWnd1v1->AddAppWnd(QString::fromStdWString(it.title),it.id);
		}
	}
}

void UIWindowSet::ShowLesson()
{
	QString strUrl;

	if (m_curTags->GetLessonType() == d_ExclusiveLesson)
	{
		QString strCourseID = "";
		if (m_curTags)
			strCourseID = m_curTags->CourseID();
		else
			return;
		strUrl += m_homePage;
		strUrl += "/api/v2/live_studio/teachers/{teacher_id}/schedule_data";
		strUrl.replace("{teacher_id}", m_teacherID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/teachers/{teacher_id}/schedule";
		strUrl.replace("{teacher_id}", m_teacherID);
	}


	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	request.setRawHeader("Remember-Token", m_Token.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIWindowSet::LessonRequestFinished);
}

void UIWindowSet::LessonRequestFinished()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonArray courses = obj["data"].toArray();

	foreach(const QJsonValue & value, courses)
	{
		QJsonObject objValue;
		QJsonArray lessons;
		objValue = value.toObject();
		lessons = objValue["lessons"].toArray();
		foreach(const QJsonValue & value, lessons)
		{
			Lesson *lesson = new Lesson();
			lesson->readJson(value.toObject());
			QString mOffline;
			mOffline = value.toObject()["model_type"].toString();
			if (mOffline == "LiveStudio::OfflineLesson")
			{
				continue;
			}

			QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
			if (lesson->Date() == curTime)
			{
				if (m_curTags->GetLessonType() == d_ExclusiveLesson)
				{
					AddTodayToLesson(lesson->LessonID(), lesson->CourseID(), m_curTags->BoardStream(), m_curTags->CameraStream(), lesson->LessonTime(), lesson->ChinaLessonStatus(), lesson->name());
				}
				else
				{
					AddTodayToLesson(lesson->LessonID(), lesson->CourseID(), lesson->BoardUrl(), lesson->CameraUrl(), lesson->LessonTime(), lesson->ChinaLessonStatus(), lesson->name());
				}
				
			}
			delete lesson;
		}
	}
	if (m_curTags)
	{
		m_LiveLesson->SetURLBasicInfo(m_homePage, m_Token, m_curTags->BoardStream(), m_curTags->CameraStream());
		m_LiveLesson->setCourseID(m_curTags->CourseID(), m_curTags->GetLessonType());
	}
	
}

void UIWindowSet::EndDev()
{
	IMInterface::getInstance()->endDevice(AudioDevice);
	IMInterface::getInstance()->endDevice(AudioOut);
	IMInterface::getInstance()->endDevice(Video);
}

void UIWindowSet::QueryOnlinePersonNum()
{
	if (m_curTags == NULL || !m_curTags->IsModle())
		return;
	
	QString strCourseID = "";
	if (m_curTags)
		strCourseID = m_curTags->CourseID();
	else
		return;

	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/courses/{id}/status";
		strUrl.replace("{id}", strCourseID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/courses/{id}/status";
		strUrl.replace("{id}", strCourseID);
	}

	HttpRequest http;
	http.setRawHeader("Remember-Token", m_Token.toUtf8());

	QByteArray result = http.httpGet(strUrl);
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1)
	{
		QJsonArray online = data["online_users"].toArray();
		int iCount = online.size();
		ui.online_label->setText(QString::number(iCount));
	}
}

void UIWindowSet::startQueryOnlineNum()
{
	stopQueryOnlineNum();
	QueryOnlinePersonNum();

	if (m_QueryOnlieTimers)
		m_QueryOnlieTimers->start(30000);
}

void UIWindowSet::stopQueryOnlineNum()
{
	if (m_QueryOnlieTimers)
		m_QueryOnlieTimers->stop();
}
void UIWindowSet::slot_onOnlineTimeout()
{
	QueryOnlinePersonNum();
}

void UIWindowSet::slot_rtsTcpDiscontect()
{
	slot_CloseWnd();
	mWhiteBoard->stopSendMsg();
	//ֹͣ1��1ֱ��
	ui.Live1v1_pushBtn->setText(LIVE_BUTTON_NAME);
	ui.Live1v1_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");

	// �����װ������Ƶ
	// ʵ�ֽ����װ������Ƶ����.....
	IMInterface::getInstance()->EndLive(m_curTags->ChatID().toStdString());

	m_LiveStatusManager->SendStopLiveHttpMsg1v1();

	if (m_CountTimer->isActive())
	{
		m_CountTimer->stop();					// ֹͣ��ʱ
		m_iTimerCount = 0;						// ��������
		ui.time1v1_label->setVisible(false);	// ����
		ui.time1v1_label->setText("00:00:00");	// ����ʱ��
	}
	m_bLiving1v1 = false;

	QString strError;
	strError = QString("���粻�ȶ�,����������ʧ��\r\n�����¿���ֱ����");
	CMessageBox::showMessage(
		QString("����ʱ��"),
		QString(strError),
		QString("ȷ��"),
		QString("ȡ��"));
}

bool UIWindowSet::IsCaptureAudio()
{
	if (m_vecChatRoom.size() > 0)
	{
		std::vector<UIChatRoom*>::iterator it;
		for (it = m_vecChatRoom.begin(); it != m_vecChatRoom.end(); it++)
		{
			UIChatRoom *mRoom = *it;
			if (mRoom->IsCaptureAudio())
			{
				return true;
			}
		}
	}
	return false;
}

void UIWindowSet::SendAudio(std::string sid, std::string msgid, std::string mPath, long mSize, int dur, std::string mfileEx)
{
	if (m_vecChatRoom.size() > 0)
	{
		std::vector<UIChatRoom*>::iterator it;
		for (it = m_vecChatRoom.begin(); it != m_vecChatRoom.end(); it++)
		{
			UIChatRoom *mRoom = *it;
			//�ж��ǵ�ǰ�ػ�����
			if (strcmp(sid.c_str(), mRoom->GetCurChatID().c_str()) == 0)
			{
				mRoom->SendAudio(QString::fromStdString(msgid), QString::fromStdString(mPath), mSize, dur, mfileEx);
				return;
			}
		}
	}
}

// �����л�
void UIWindowSet::InitAudioCallBack()
{
	nim_audio::Audio::RegStartCaptureCb(&OnStartCaptureCallback);
	nim_audio::Audio::RegStopCaptureCb(&OnStopCaptureCallback);
}

void UIWindowSet::OnStartCaptureCallback(int code)
{
	if (code != 200)
	{
		//��ʾ¼��ʧ��
		QToolTip::showText(QCursor::pos(), "¼������ʧ�ܣ�");
	}
	else
	{
		if (m_This)
			m_This->StartCaptureAudio();
	}
}

void UIWindowSet::StartCaptureAudio()
{
	if (m_curChatRoom)
	{
		m_curChatRoom->m_AudioBar->CaptureAudio();
	}
}

void UIWindowSet::OnStopCaptureCallback(int rescode, const char *sid, const char *cid, const char *file_path, const char *file_ext, long file_size, int audio_duration)
{
	if (rescode == 200 && m_This)
	{
		MyAudioStruct *mAudio = new MyAudioStruct;
		mAudio->sSessionID = sid;
		mAudio->sMsgID = cid;
		mAudio->sFilePath = file_path;
		mAudio->sFileEx = file_ext;
		mAudio->fileSize = file_size;
		mAudio->duration = audio_duration;

		if (m_This->m_parent)
			PostMessage((HWND)m_This->m_parent->winId(), MSG_SEND_AUDIO_FINISH_MSG, (WPARAM)mAudio, 0);
	}
}

void UIWindowSet::RecordingVoice(std::string chatID, std::string msgID)
{
	nim_audio::Audio::StartCapture(chatID.c_str(), msgID.c_str(), nim_audio::AMR);
}

void UIWindowSet::StopRecord()
{
	nim_audio::Audio::StopCapture();
}

//�л���ǩʱ�ر������Զ����Ź��ܵ�ȫ�ֱ���
void UIWindowSet::StopAudioAutoPlay()
{
	m_AutoAudioState = 0;
	m_IsAudioPlaying = false;
	nim_audio::Audio::StopPlayAudio();
}

//�����жϵ�ǰ��ʾͼ������Ƿ��������������������false  add by zbc 2170719
//�ж�ԭ��ԭ��Ƶ��������������Ƶ���� �ı��ʹ�ϵ����2�����ж��쳣
bool RateError(double tWidth, double tHeigth, int tOriginalWidth, int tOriginalHeigth)
{
	double mCurrentRate, mOriginalRate = 0;
	mCurrentRate = tWidth / tHeigth;
	mOriginalRate = tOriginalWidth / tOriginalHeigth;
	if (((mOriginalRate / mCurrentRate)>= 2) || ((mOriginalRate / mCurrentRate)>=2))
	{
		return false;
	}
	return true;
}

//����1��1��Ļ����󣬴��ڵĴ�С
void UIWindowSet::Math1v1Screen()
{
	int iWidth = ui.full1v1_widget->width();
	int iHeight = ui.full1v1_widget->height();

	// ����ץȡ
	int iScreenWidth = iWidth;
	int iScreenHeight = ((double)iWidth / (double)m_VideoInfo1v1->ScreenWidth())*m_VideoInfo1v1->ScreenHeight();

	if (iScreenHeight > iHeight)
	{
		iScreenWidth = ((double)iHeight / (double)m_VideoInfo1v1->ScreenHeight())*m_VideoInfo1v1->ScreenWidth();
		if (RateError(iScreenWidth, iHeight, m_VideoInfo1v1->ScreenWidth(), m_VideoInfo1v1->ScreenHeight()) == true)
		{
			m_VideoInfo1v1->setFixedSize(iScreenWidth, iHeight);
		}
		else
		{
			qDebug() << __FILE__ << __LINE__ << "height:" << m_VideoInfo1v1->ScreenHeight() << "width:" << m_VideoInfo1v1->ScreenWidth(); //2017/07/20 zbc
			//�ж�ת�����ǿ��Ǹ��쳣
			//�����߼���������խ�����֣�˵��ֻ�п�͸�֮һ��������Ȼ����������Ϊ׼�����������������һ����
			if (iScreenWidth > iHeight)
			{
				iHeight = (iScreenWidth * m_VideoInfo1v1->ScreenHeight()) / m_VideoInfo1v1->ScreenWidth();
			}
			else
			if (iScreenWidth < iHeight)
			{
				iScreenWidth = (m_VideoInfo1v1->ScreenWidth() * iHeight) / m_VideoInfo1v1->ScreenHeight();
			}
			m_VideoInfo1v1->setFixedSize(iScreenWidth, iHeight);
		}
	}
	else
	{
		if (RateError(iScreenWidth, iHeight, m_VideoInfo1v1->ScreenWidth(), m_VideoInfo1v1->ScreenHeight()) == true)
		{
			m_VideoInfo1v1->setFixedSize(iScreenWidth, iScreenHeight);
		}
		else
		{
			qDebug() << __FILE__ << __LINE__ << "height:" << m_VideoInfo1v1->ScreenHeight() << "width:" << m_VideoInfo1v1->ScreenWidth(); //2017/07/20 zbc
			//�ж�ת�����ǿ��Ǹ��쳣
			//�����߼���������խ�����֣�˵��ֻ�п�͸�֮һ��������Ȼ����������Ϊ׼�����������������һ����
			if (iScreenWidth > iHeight)
			{
				iHeight = (iScreenWidth * m_VideoInfo1v1->ScreenHeight()) / m_VideoInfo1v1->ScreenWidth();
			}
			else
			if (iScreenWidth < iHeight)
			{
				iScreenWidth = (m_VideoInfo1v1->ScreenWidth() * iHeight) / m_VideoInfo1v1->ScreenHeight();
			}
			m_VideoInfo1v1->setFixedSize(iScreenWidth, iScreenHeight);
		}
	}
		
	
}


// ��������ʵʱ״̬����ͼ��
void UIWindowSet::slot_GetNetState(int mState)
{
	switch (mState)
	{
		case nim::kNIMVideoChatSessionNetStatVeryGood:
		{
			ui.label_NetState->setText("����");
			ui.label_NetState->setStyleSheet("color: rgb(0, 255, 0);");
			ui.label_NetState->setToolTip("�������ã���˳��ֱ��");
			break;
		}
		case nim::kNIMVideoChatSessionNetStatGood:
		{
			ui.label_NetState->setText("һ��");
			ui.label_NetState->setStyleSheet("color: rgb(255, 185, 15);");
			ui.label_NetState->setToolTip("����һ�㣬ֱ�����ܳ��ֿ���");
			break;
		}
		case nim::kNIMVideoChatSessionNetStatBad:
		{
			ui.label_NetState->setText("�ϲ�");
			ui.label_NetState->setStyleSheet("color: rgb(205, 38, 38);");
			ui.label_NetState->setToolTip("����ϲֱ�����ܳ��ֺ���");
			break;
		}
		case nim::kNIMVideoChatSessionNetStatVeryBad:
		{
			ui.label_NetState->setText("�ܲ�");
			ui.label_NetState->setStyleSheet("color: rgb(255, 0, 0);");
			ui.label_NetState->setToolTip("����ܲֱ�����ܻ�Ͽ�����");
			break;
		}
	}
}

void UIWindowSet::SendFrameStatusToWebService()
{
	ZWeb *mZWeb = new ZWeb;
	QString strUrl;
	QJsonObject mUrlJson;
	

	strUrl += m_homePage;
	strUrl += "/api/v1/live_studio/interactive_lessons/{LessonID}/live_switch";
	strUrl.replace("{LessonID}", m_lessonid);
	QByteArray append("camera=");
	append += QString::number(m_PublicCameraStatus);

	bool bPostUrl = false;
	bPostUrl = mZWeb->PostURLInfoBySignal(strUrl, append, m_Token);
	if (bPostUrl == false)
	{
		//delete mZWeb;
		return;
	}
	//delete mZWeb;
	mZWeb = NULL;
}