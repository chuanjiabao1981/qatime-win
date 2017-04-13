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

#define MAINWINDOW_X_MARGIN 2
#define MAINWINDOW_Y_MARGIN 2
#define MAINWINDOW_TITLE_HEIGHT 0
#define MAINWINDOW_MAXHEIGHT	900		//��Ļ�߶�
#define MAINWINDOW_MAXWIDTH		528		//���������
#define MAINWINDOW_MINWIDTH		342		//������С���
#define MAINWINDOW_MINHEIGHT	528		//������С�߶�

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
	, m_iSucCount(0)
	, m_HelpBtnPos(290)
	, m_BulletScreen(NULL)
	, m_RefreshTimer(NULL)
{
	ui.setupUi(this);
	setFocusPolicy(Qt::ClickFocus);
	setMaximumWidth(MAINWINDOW_MAXWIDTH);
	setMinimumWidth(MAINWINDOW_MINWIDTH);
	setMinimumHeight(MAINWINDOW_MINHEIGHT);

	QDesktopWidget *dsk = QApplication::desktop();
	int iHeight = dsk->height();
	//��ȥ�������ĸ߶�
	if (iHeight < MAINWINDOW_MAXHEIGHT)
	{
		setAdaptHeight(iHeight*0.92);
		this->move(dsk->width() - 345, 0);
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
	connect(ui.time_label, SIGNAL(clicked()), this, SLOT(slot_startOrStopLiveStream()));
	ui.time_label->hide();
	
	m_SeeLiveWnd = new UISeeLiveWnd();
	m_SeeLiveWnd->setWindowFlags(Qt::FramelessWindowHint);
	m_SeeLiveWnd->SetMainWnd(this);
	m_SeeLiveWnd->hide();

	m_SetParam = new UISetParam();
	m_SetParam->setWindowFlags(Qt::FramelessWindowHint);
	m_SetParam->setParent(this);
	m_SetParam->hide();
	SetWindowPos((HWND)m_SetParam->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	m_VideoInfo = new UIVideo(ui.childVideo_widget);
	m_VideoInfo->setWindowFlags(Qt::FramelessWindowHint);
//	m_VideoInfo->SetMainWnd(this);
	m_VideoInfo->InitDeviceParam();
	m_VideoInfo->resize(316, 179);
	m_VideoInfo->show();

	m_AuxiliaryPanel = new UIAuxiliaryPanel(ui.Auxiliary_widget);
	m_AuxiliaryPanel->setWindowFlags(Qt::FramelessWindowHint);
	m_AuxiliaryPanel->setParent(this);
	m_AuxiliaryPanel->resize(ui.Auxiliary_widget->size().width()+2, ui.Auxiliary_widget->size().height()+5);
	
	m_AudioChangeInfo = new UIAudioChange(this);
	m_AudioChangeInfo->setWindowFlags(Qt::FramelessWindowHint);
//	m_AudioChangeInfo->setAudioChange(this);
	m_AudioChangeInfo->hide();

	m_VideoChangeInfo = new UIVideoChange(this);
	m_VideoChangeInfo->setWindowFlags(Qt::FramelessWindowHint);
//	m_VideoChangeInfo->setVideoChange(this);
	m_VideoChangeInfo->hide();

	m_RatioChangeInfo = new UIRatio(this);
	m_RatioChangeInfo->setWindowFlags(Qt::FramelessWindowHint);
//	m_RatioChangeInfo->setVideoChange(this);
	m_RatioChangeInfo->hide();

	m_LessonTable = new UILessonTable(this);
	m_LessonTable->setWindowFlags(Qt::FramelessWindowHint);
	m_LessonTable->SetMainWindow(this);
	m_LessonTable->hide();

	m_BulletScreen = new UIBulletScreen();
	m_BulletScreen->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
//	m_BulletScreen->setMainWindow(this);
	m_BulletScreen->hide();
	SetWindowPos((HWND)m_BulletScreen->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	m_HoverWnd = new UIHoverWindow();
	m_HoverWnd->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	m_HoverWnd->setParentWindow(this);
	m_HoverWnd->hide();

	m_LiveStatusManager = new LiveStatusManager(this);
//	m_LiveStatusManager->setMainWindow(this);

	// ֱ����ť
	ui.Live_pushBtn->setText("��ʼֱ��");
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");

	// ��ʱ�� �ı�ֱ����ʱʱ��
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

	m_RefreshTimer = new QTimer(this);
	connect(m_RefreshTimer, SIGNAL(timeout()), this, SLOT(onTimerResize()));

	// ��������ͷ��ƵԴ��ʽ
	ui.video_checkBox->setStyleSheet("QCheckBox::indicator{width: 62px;height: 20px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/camera_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/camera_close.png);}");

	// ������˷���ʽ
	ui.Audio_checkBox->setStyleSheet("QCheckBox::indicator{width: 55px;height: 20px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/mic_open.png);}"
		"QCheckBox::indicator:checked{image: url(./images/mic_close.png);}");

	// ���ô����л���ʽ
	ui.switchScreen_checkBox->setStyleSheet("QCheckBox::indicator{width: 69px;height: 20px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/switch.png);}"
		"QCheckBox::indicator:checked{image: url(./images/switch.png);}");

	// ���ò�������
	ui.set_checkBox->setStyleSheet("QCheckBox::indicator{width: 50px;height: 20px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/set.png);}"
		"QCheckBox::indicator:checked{image: url(./images/set.png);}");

	// �رմ��ڲɼ���ʽ
	ui.close_radioButton->setStyleSheet("QRadioButton::indicator{width: 62px;height: 22px;}"
		"QRadioButton::indicator:unchecked{image: url(./images/captureclose_uncheck.png);}"
		"QRadioButton::indicator:checked{image: url(./images/captureclose_check.png);}"
		"QRadioButton::indicator:checked:disabled{image:url(./images/captureclose_disabled.png);}"
		"QRadioButton::indicator:unchecked:disabled{image:url(./images/captureclose_disabled.png);}");

	// ���ڲɼ���ʽ
	ui.rect_radioButton->setStyleSheet("QRadioButton::indicator{width: 62px;height: 22px;}"
		"QRadioButton::indicator:unchecked{image: url(./images/capturerect_uncheck.png);}"
		"QRadioButton::indicator:checked{image: url(./images/capturerect_check.png);}");

	// ȫ���ɼ���ʽ
	ui.full_radioButton->setStyleSheet("QRadioButton::indicator{width: 62px;height: 22px;}"
		"QRadioButton::indicator:unchecked{image: url(./images/capturefull_uncheck.png);}"
		"QRadioButton::indicator:checked{image: url(./images/capturefull_check.png);}");

	QPixmap pixmap(QCoreApplication::applicationDirPath() + "/images/login_min.png");
	QPixmap pixmap1(QCoreApplication::applicationDirPath() + "/images/login_close.png");
	ui.mainmin_pushBtn->setPixmap(pixmap, 1);
	ui.mainclose_pushBtn->setPixmap(pixmap1, 1);
	
	m_charRoom = new UIChatRoom(ui.chat_widget);
	m_charRoom->setWindowFlags(Qt::FramelessWindowHint);
//	m_charRoom->setMainWindow(this);
 	m_charRoom->move(10, 0);
	m_charRoom->resize(m_charRoom->size().width(), ui.chat_widget->size().height());
	m_charRoom->setAdaptHeight(ui.chat_widget->size().height());
	m_charRoom->show();

	ui.titel_pushButton->installEventFilter(this);
	ui.titel1_pushButton->installEventFilter(this);

	// TabWidget�����任
	ui.tabWidget->setStyleSheet("QTabWidget:pane{border: 1px solid white;background-color:white;}");

	// TabBar����ͼƬ
	ui.tabWidget->setStyleSheet("QTabBar::tab { height: 25px; width: 170px; }"
		"QTabBar::tab:!selected {color:rgb(204,204,204);background: transparent;}"
		"QTabBar::tab:selected {color:rgb(5, 158, 213);}");

	QFont font;
	font.setPointSize(12);
	font.setFamily(("΢���ź�"));
	ui.tabWidget->setFont(font);

	ui.close_radioButton->installEventFilter(this);
	ui.rect_radioButton->installEventFilter(this);
	ui.full_radioButton->installEventFilter(this);

	ui.help_pushButton->setIcon(QIcon("./images/help.png"));
	ui.help_pushButton->setText("����");
	connect(ui.help_pushButton, SIGNAL(clicked()), this, SLOT(HelpDialog()));

	// ��ʱ����ȫ��
	ui.groupBox->setVisible(false);
	ui.video_widget->setVisible(false);
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

	//ɾ����ʱ��
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

void UIMainWindow::setMainTitle(QString Version)
{
	QString sVersion = "  ����ʱ��ֱ������{version}";
	sVersion.replace("{version}", Version);
	ui.titel_pushButton->setText(sVersion);
}
void UIMainWindow::setAdaptHeight(int iHeight)
{
	if (iHeight < 560)
		iHeight = MAINWINDOW_MINHEIGHT;

	setMaximumSize(QSize(342, iHeight));
	ui.tabWidget->setMaximumHeight(iHeight - 71);
	ui.chat_widget->setMaximumHeight(iHeight - 382);
}
void UIMainWindow::WhiteBoard()
{

}

void UIMainWindow::MinDialog()
{
	QRect rc = ui.title_widget->geometry();
	QRect rc2 = ui.help_widget->geometry();
	QRect rc3 = ui.welcome_widget->geometry();
	QRect rc4 = ui.videoCamera_widget->geometry();
	QRect rc5 = ui.right_label->geometry();
	QRect rc6 = ui.btn_widget->geometry();
	QRect rc7 = ui.tabWidget->geometry();
	QRect rc8 = ui.time_label->geometry();
	QRect rc9 = ui.chat_widget->geometry();
	QRect rc10 = m_charRoom->geometry();

	QRect rc1 = this->geometry();
	//�������Ϊ�������С�������ش˴��ڣ�����������
	this->hide();
	QDesktopWidget *dsk = QApplication::desktop();
	SetWindowPos((HWND)m_HoverWnd->winId(), HWND_TOPMOST, dsk->width() - 45, dsk->height()*0.6, 0, 0, SWP_NOSIZE);
	m_HoverWnd->resize(44, m_HoverWnd->size().height());
	m_HoverWnd->SetNumber();
	m_HoverWnd->show();
}

void UIMainWindow::CloseDialog()
{
	if (m_VideoInfo->IsCurrentLiving())
	{
		CMessageBox::showMessage(
			QString("����ʱ��"),
			QString("���Ƚ���ֱ�����ٹرյ�ǰ����"),
			QString("ȷ��"));
		return;
	}

	int iStatus = CMessageBox::showMessage(
		QString("����ʱ��"),
		QString("�Ƿ�رյ�ǰӦ�ó���"),
		QString("ȷ��"),
		QString("ȡ��"));
	if (iStatus == 1)
	{
		// ���ͽ���ֱ����Ϣ�ٹر�
		if (m_VideoInfo->IsCurrentLiving())
			m_LiveStatusManager->SendStopLiveHttpMsg(false);

		//�������ô���
		if (m_SetParam)
			m_SetParam->setVisible(false);

		if (m_ScreenTip)
			m_ScreenTip->setVisible(false);

		if (m_HelpWord)
			m_HelpWord->setVisible(false);

		if (m_BulletScreen)
			m_BulletScreen->setVisible(false);

		m_LoginWindow->CloseTray();
		
		exit(0);
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
	ui.teacherName_label->setText(teacherName);

	// ������ʦͷ��
	QString teacherPhoto_url = data["avatar_url"].toString();
	setNetworkPic(teacherPhoto_url);

	QString strWelcome = "��ӭ ";
	strWelcome += teacherName;
	strWelcome += " ��ʦ��¼����ʱ�䣡";
	ui.lesson_label->setText(strWelcome);

	// ��ʦ������Ϣ
	m_charRoom->setChatInfo(data["chat_account"].toObject(), mRemeberToken);
	m_charRoom->m_TeachterName = teacherName;

	m_LessonTable->InitToken(mRemeberToken, m_teacherID);
}

void UIMainWindow::setAutoTeacherInfo(QString teacherID, QString teacherName, QString teacherUrl, QString accid, QString token)
{
	m_teacherID = teacherID;
	m_AuxiliaryPanel->setTeacherID(m_teacherID);

	// ������ʦ����
	ui.teacherName_label->setText(teacherName);

	// ������ʦͷ��
	setNetworkPic(teacherUrl);

	QString strWelcome = "��ӭ ";
	strWelcome += teacherName;
	strWelcome += " ��ʦ��¼����ʱ�䣡";
	ui.lesson_label->setText(strWelcome);

	// ��ʦ������Ϣ
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
	strUrl = "https://qatime.cn/api/v1/live_studio/teachers/{teacher_id}/courses/full?status=teaching";
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
//	m_mutex.lock();
 	int w, h;
	w = e->size().width() - e->oldSize().width();
	h = e->size().height() - e->oldSize().height();
	if (m_icount > 0 || e == NULL)
	{
		// tab����ı�
		if (w != 0)
		{
			ui.title_widget->resize(ui.title_widget->width() + w, ui.title_widget->height());
			ui.help_widget->resize(ui.help_widget->width() + w, ui.help_widget->height());
			ui.welcome_widget->resize(ui.welcome_widget->width() + w, ui.welcome_widget->height());
			ui.btn_widget->resize(ui.btn_widget->width() + w, ui.btn_widget->height());
			ui.videoCamera_widget->resize(ui.videoCamera_widget->width() + w, ui.videoCamera_widget->height());
			ui.right_label->move(ui.right_label->geometry().left() + w, ui.right_label->geometry().top());
		}
		// tab2����ı�
		m_AuxiliaryPanel->resize(m_AuxiliaryPanel->width() + w, m_AuxiliaryPanel->height() + h);
		ui.Auxiliary_widget->resize(ui.Auxiliary_widget->width() + w, ui.Auxiliary_widget->height() + h);

		if (m_charRoom)
		{
			int iWidth = ui.tabWidget->width() + w;
			ui.tabWidget->resize(iWidth, ui.tabWidget->height() + h);
			// TabBar����ͼƬ
			QString sheet;
			sheet = QString().sprintf("QTabBar::tab { height: 25px; width: %dpx; } QTabBar::tab:!selected {color:rgb(204,204,204);background: transparent;} QTabBar::tab:selected {color:rgb(5, 158, 213);}", iWidth / 2);
			ui.tabWidget->setStyleSheet(sheet);
			// ��ʱ��λ��
			ui.time_label->move((iWidth - 50) / 2, ui.time_label->geometry().top());

			ui.chat_widget->resize(ui.chat_widget->width() + w, ui.chat_widget->height() + h);
			m_charRoom->resize(m_charRoom->width() + w, m_charRoom->height() + h);
			m_charRoom->setResize(w,h);
		}

		if (m_AuxiliaryPanel && m_AuxiliaryPanel->m_teacher_treewidget && w != 0)
		{
			int iWidth = m_AuxiliaryPanel->m_teacher_treewidget->columnWidth(0);
			m_AuxiliaryPanel->m_teacher_treewidget->setColumnWidth(0, iWidth+w);
		}

		if (m_LessonTable)
			m_LessonTable->setResize(w);

		// ������ťλ�øı�
		m_HelpBtnPos += w;

		m_RefreshTimer->start(1000);
	}
	m_icount++;
//	m_mutex.unlock();
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
				m_iSucCount = 0;
				ui.Live_pushBtn->setText("��ʼֱ��");
				ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");
				
				m_VideoInfo->StopLiveVideo();
				if (IsHasCamera() && m_EnumStatus != CameraStatusClose)
					m_CameraInfo->StopLiveVideo();
				m_LiveStatusManager->SendStopLiveHttpMsg();

				if (m_CountTimer->isActive())
				{
					m_CountTimer->stop();					// ֹͣ��ʱ
					m_iTimerCount = 0;						// ��������
					ui.time_label->setVisible(false);		// ����
					ui.time_label->setText("00:00:00");		// ����ʱ��
					
					if (m_HoverWnd)
						m_HoverWnd->SetLiveTimer("00:00:00");
				}

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
					QString("����ʱ��"),
					QString("��ѡ����Ҫ��ʼ�Ŀγ̽���ֱ����"),
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

			if (ui.close_radioButton->isChecked())
			{
				CMessageBox::showMessage(
					QString("����ʱ��"),
					QString("�뿪���ɼ�ģʽ����ֱ����"),
					QString("ȷ��"));
				return;
			}

			
			if (!IsHasCamera())
				m_EnumStatus = CameraStatusClose;
			
			// ��ʼ��ֱ��״̬�������������������ɹ������㿪��ֱ��
			m_iSucCount = 0;
			// �ưװ�
			QString url = m_AuxiliaryPanel->getURL();
			qDebug() << "�װ�������ַ��"+ url;
			m_VideoInfo->setPlugFlowUrl(url);
			m_VideoInfo->StartLiveVideo();
			// ������ͷ
			QString CameraUrl = m_AuxiliaryPanel->getCameraURL();
			qDebug() << "����ͷ������ַ��" + CameraUrl;
			m_CameraInfo->setPlugFlowUrl(CameraUrl);
			if (IsHasCamera() && m_EnumStatus != CameraStatusClose)
			{
				qDebug() << "ֱ������ͷ";
				m_CameraInfo->StartLiveVideo();
			}
		}
	}
}

// ���¿�ʼֱ����ť����ʱ����״̬
void UIMainWindow::startLiveStream()
{	
	m_CountTimer->start(1000);
	ui.time_label->setVisible(true);
	ui.Live_pushBtn->setText("����ֱ��");
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: red;border-radius: 5px; border: 2px solid red;}");
	m_AuxiliaryPanel->setPreview(true);

	this->hide();
	QDesktopWidget* dsk = QApplication::desktop();
	SetWindowPos((HWND)m_HoverWnd->winId(), HWND_TOPMOST, dsk->width() - 45, dsk->height()*0.6, 0, 0, SWP_NOSIZE);
	m_HoverWnd->resize(44, m_HoverWnd->size().height());
	m_HoverWnd->SetNumber();
	m_HoverWnd->show();
}

void UIMainWindow::VideoStatus(int iStatus)
{
	if (iStatus)
	{
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
			m_LiveStatusManager->SendCameraSwitchMsg(1, m_EnumStatus);
		}
	}
	else
	{
		//��ʾ
		m_CameraInfo->setVisible(true);
		if (m_VideoInfo)
		{
			m_EnumStatus = CameraStatusTeaching;

			if (!IsHasCamera())
				return;

			if (!m_VideoInfo->IsCurrentLiving())
				return;

			m_iSucCount++;
			ui.video_checkBox->setEnabled(false);
			m_CameraInfo->StartLiveVideo();
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

// ���ò�������
void UIMainWindow::SetParamWindow()
{
//	m_SeeLiveWnd->show();
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

	m_CameraInfo = new UICamera(ui.childCamera_widget);
	m_CameraInfo->setWindowFlags(Qt::FramelessWindowHint);
//	m_CameraInfo->SetMainWnd(this);
	m_CameraInfo->show();
	m_CameraInfo->resize(317, 210);
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
		case MSG_CLIENT_RECEIVE:  // ����������Ϣ
		{
			qDebug() << "���յ���Ϣ";
			MSG* Msg = pMsg;
			nim::IMMessage* pIMsg = (nim::IMMessage*)Msg->wParam;

			if (m_charRoom)
			{
				if (m_charRoom->ReceiverMsg(pIMsg) && !m_BulletScreen->isVisible())
					m_HoverWnd->UpdateChatNumber();
			}
				
			qDebug() << "�������";
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
		case MSG_SEND_AUDIO_MSG: // �������Ž���
		{
			MSG* Msg = pMsg;
			char* msgid = (char*)Msg->lParam;
			int   sid = Msg->wParam;
			if (m_charRoom)
				m_charRoom->OnStopPlayAudio(msgid);
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
			ShowWindow(m_VideoWnd, SW_SHOW);
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
		case MSG_PICTURE_PROCESS:
		{
			double iUpSize = (double)pMsg->wParam;
			double iFileSize = (double)pMsg->lParam;
			double diff = iUpSize / iFileSize;
			if (m_charRoom)
			{
				m_charRoom->UpLoadPicProcess(diff);
			}
		}
		break;
		case  MSG_SEND_MSG_STATUS:
		{
			nim::SendMessageArc* arcNew = (nim::SendMessageArc*)pMsg->wParam;
			if (arcNew)
			{
				if (m_charRoom && arcNew->rescode_ !=200)
				{
					m_charRoom->SendStatus(arcNew);
				}

				delete arcNew;
				arcNew = NULL;
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
				if (ui.mainclose_pushBtn && ui.mainmin_pushBtn && ui.help_pushButton && ui.return_pushButton)
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

					Rect = ui.return_pushButton->geometry();
					if (x > Rect.left() && x < Rect.right() && y > Rect.top() && y < Rect.bottom())
					{
						return false;
					}

					Rect = QRect(m_HelpBtnPos, 40, ui.help_pushButton->width(), ui.help_pushButton->height());
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
	strUrl = "https://qatime.cn/api/v1/live_studio/teachers/{teacher_id}/courses/{id}";
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

void UIMainWindow::setAudioChangeIndex(int index)
{
	if (m_VideoInfo)
	{
		m_VideoInfo->SetChangeAudio(index);
	}
}

// void UIMainWindow::setRadioIndex()
// {
// 
// }

void UIMainWindow::setVideoChangeIndex(int index)
{
	if (m_CameraInfo)
	{
		m_CameraInfo->SetChangeVideo(index);
	}
}

void UIMainWindow::setRatioChangeIndex(int index)
{
	if (m_VideoInfo)
	{

	}
}

void UIMainWindow::setLoginWindow(LoginWindow* parent)
{
	m_LoginWindow = parent;
}

void UIMainWindow::returnClick()
{
	if (m_VideoInfo->IsCurrentLiving())
	{
		CMessageBox::showMessage(
			QString("����ʱ��"),
			QString("���Ƚ���ֱ�����ٹرյ�ǰ����"),
			QString("ȷ��"));
		return;
	}

	int iStatus = CMessageBox::showMessage(
		QString("����ʱ��"),
		QString("�Ƿ��˳���ǰ�˺ţ�"),
		QString("ȷ��"),
		QString("ȡ��"));
	if (iStatus == 1)
	{
		if (m_BulletScreen)
			m_BulletScreen->setVisible(false);

		// ���ͽ���ֱ����Ϣ�ٹر�
		m_LiveStatusManager->SendStopLiveHttpMsg(false);

		if (m_LoginWindow)
			m_LoginWindow->ReturnLogin();
	}
}

void UIMainWindow::setCurChatRoom(QString chatID, QString courseid)
{
	if (m_charRoom)
	{
		// ����ǵ�ǰ�Ự���ڣ�����Ҫ�ٴ�����Ⱥ��Ա
		if (!m_charRoom->IsCurChatRoom(m_AuxiliaryPanel->getChatID()))
		{
//			m_charRoom->setCurChatID(chatID, courseid);
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
	strUrl = "https://qatime.cn/api/v1/app_constant/im_app_key";
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
	strUrl = "https://qatime.cn/api/v1/live_studio/courses/{id}/realtime";
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
		m_charRoom->SetStudentName(i);

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

	CMessageBox::showMessage(QString("����ʱ��"), QString("���ѳɹ�����ֱ�����ң�"), QString("ȷ��"), QString(""), NULL, true);
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

// �γ̱���ѡ��γ̡���������������
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
	}
}

void UIMainWindow::setCameraPos()
{
	if (m_CameraWnd)
	{
		if (m_ShowCameraTimer->isActive())
			m_ShowCameraTimer->stop();
		MoveWindow(m_CameraWnd, 0, 0, 317, 178, true);
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
	strUrl = "https://qatime.cn/api/v1/live_studio/lessons/{id}/finish";
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
	ui.Live_pushBtn->setText("����ֱ��");
	ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");
}

// �϶��������Ĵ���
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
	}
	else if (target == ui.rect_radioButton)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			if (ui.rect_radioButton->isChecked())
				return false;
		}
	}
	else if (target == ui.close_radioButton)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
 			if (!ui.close_radioButton->isEnabled())
				return false;

			if (ui.close_radioButton->isChecked())
			{
				ui.close_radioButton->setCheckable(false);
				ShowWindow(m_VideoWnd, SW_SHOW);
			}
			else
			{
				ui.close_radioButton->setCheckable(true);
				ShowWindow(m_VideoWnd, SW_HIDE);
			}
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
		move(rc.x(), 0);
	}
}

void UIMainWindow::leaveEvent(QEvent *)
{
	QRect rc;
 	QRect rect;
 	rect = this->geometry();
	int iRight = rect.right();
	int i = QCursor::pos().x();
	if (rect.right() - 10 <= QCursor::pos().x() && rect.right() + 10 >= QCursor::pos().x())
	{
		return;
	}

	QDesktopWidget *dsk = QApplication::desktop();
	rc.setRect(rect.x(), rect.y(), rect.width(), rect.height());
// 	if (rect.top() <= 0)
// 	{
// 		if (m_charRoom->IsFous())
// 		{
// 			return;
// 		}
// 		this->hide();
// 
// 		SetWindowPos((HWND)m_HoverWnd->winId(), HWND_TOPMOST, dsk->width() - 45, dsk->height()*0.6, 0, 0, SWP_NOSIZE);
// 		m_HoverWnd->resize(44, m_HoverWnd->size().height());
// 		m_HoverWnd->SetNumber();
// 		m_HoverWnd->show();
// 	}
	if (rect.right() >= dsk->width()-1)
	{
		if (m_charRoom->IsFous() || m_charRoom->IsClickPic())
		{
			return;
		}
		this->hide();

		SetWindowPos((HWND)m_HoverWnd->winId(), HWND_TOPMOST, dsk->width() - 45, dsk->height()*0.6, 0, 0, SWP_NOSIZE);
		m_HoverWnd->resize(44, m_HoverWnd->size().height());
		m_HoverWnd->SetNumber();
		m_HoverWnd->show();
	}
}

void UIMainWindow::setNetworkPic(const QString &szUrl)
{
	QUrl url(szUrl);
	QNetworkAccessManager manager;
	QEventLoop loop;

	QNetworkReply *reply = manager.get(QNetworkRequest(url));
	//���������������ɺ��˳����¼�ѭ�� 
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//�������¼�ѭ�� 
	loop.exec();

	QByteArray jpegData = reply->readAll();
	QPixmap pixmap;
	pixmap.loadFromData(jpegData);
	QPixmap scaledPixmap = pixmap.scaled(QSize(30, 30), Qt::KeepAspectRatio);
	ui.teacherPhoto_Label->setPixmap(scaledPixmap);
	m_teacherPix = scaledPixmap;
}

// �л���ֱ��ҳ
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

QPixmap UIMainWindow::TeacherPhotoPixmap()
{
	return m_teacherPix;
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
		QString("����ʱ��"),
		QString(sError),
		QString("ȷ��"));
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

void UIMainWindow::ShowMain()
{
	if (m_HoverWnd && m_HoverWnd->isVisible())
	{
		this->setVisible(true);
		m_HoverWnd->setVisible(false);
	}
}

void UIMainWindow::UpatateLiveStatus(QWidget* widget, bool bSuc)
{	
	// ÿ��������������m_iSucCount=0��
	if (bSuc)
		m_iSucCount++;
	else
	{
		// ��ֹ����һ��ʧ�ܣ���һ��ҲҪ����
		if (m_VideoInfo == widget)
		{
			if (IsHasCamera() && m_EnumStatus != CameraStatusClose)
				m_CameraInfo->StopLiveVideo();
		}
		else
		{
			m_VideoInfo->StopLiveVideo();
		}
	}

	int iAllSucCount = 2;
	// �������ͷ�رգ�����û������ͷ���ɹ�һ�ξ͹���
	if (!IsHasCamera() || m_EnumStatus == CameraStatusClose)
		iAllSucCount = 1;

	if (m_iSucCount == iAllSucCount)
	{
		m_LiveStatusManager->SendStartLiveHttpMsg(1, m_EnumStatus, m_AuxiliaryPanel->getLessonID(), mRemeberToken);
	}
}

// ���ÿ�ʼֱ����ť״̬
void UIMainWindow::setLiveBtnEnable(bool bEnable)
{
	ui.Live_pushBtn->setEnabled(bEnable);
}

// �����ƵԴ
void UIMainWindow::addVideoDevices(QString path)
{
	bHasCamera = true;
	m_SetParam->setVideoParam(path, path);
}

// �����˷�
void UIMainWindow::addAudioDevices(QString path)
{
	m_SetParam->setAudioParam(path, path);
}

// ֱ�������г���ֱֹͣ��
void UIMainWindow::ErrorStopLive(QWidget* pWidget)
{
	if (m_CameraInfo && m_VideoInfo && pWidget)
	{
		if (!m_VideoInfo->IsCurrentLiving() && !m_CameraInfo->IsCurrentLiving())
		{
			return;
		}

		m_VideoInfo->StopLiveVideo();
		if (IsHasCamera() && m_EnumStatus != CameraStatusClose)
			m_CameraInfo->StopLiveVideo();

		m_iSucCount = 0;
		ui.Live_pushBtn->setText("��ʼֱ��");
		ui.Live_pushBtn->setStyleSheet("QPushButton{background-color:white;color: #059ed5;border-radius: 5px; border: 2px solid #059ed5;}");
		if (m_CountTimer->isActive())
		{
			m_CountTimer->stop();					// ֹͣ��ʱ
			m_iTimerCount = 0;						// ��������
			ui.time_label->setVisible(false);		// ����
			ui.time_label->setText("00:00:00");		// ����ʱ��

			if (m_HoverWnd)
				m_HoverWnd->SetLiveTimer("00:00:00");
		}

		m_AuxiliaryPanel->setPreview(false);

		m_ScreenTip->setErrorTip("ֱ��������������ִ���������\n����ֱ����");
		m_LiveStatusManager->SendStopLiveHttpMsg();
	}
	else
	{
		qDebug() << "m_CameraInfo && m_VideoInfo && pWidget == NULL";
	}
}

LoginWindow* UIMainWindow::GetLoginWnd()
{
	if (m_LoginWindow)
	{
		return m_LoginWindow;
	}
	else
		return NULL;
}

void UIMainWindow::SendStudentBullet(QString name, QString content)
{
	if (m_BulletScreen)
	{
//		m_BulletScreen->ReciverStudent(name, content);
	}
}

void UIMainWindow::SendTeacherBullet(QString name, QString content)
{
	if (m_BulletScreen)
	{
//		m_BulletScreen->ReciverTeacher(name, content);
	}
}

void UIMainWindow::SetBullet(int iStatus)
{
	if (m_BulletScreen)
	{
		if (iStatus)
		{
//			m_BulletScreen->showDialog();
			if (m_HoverWnd)
			{
				m_HoverWnd->SetNumber();
				m_HoverWnd->SetLiveTimer("00:00:00");
			}
		}
		else
			m_BulletScreen->hide();
	}
}

void UIMainWindow::CloseBullet()
{
	if (m_SetParam)
		m_SetParam->CloseBulletSet();
}

void UIMainWindow::setCameraEnable()
{
	ui.video_checkBox->setEnabled(true);
}

void UIMainWindow::MenuClose()
{
	emit ui.mainclose_pushBtn->clicked();
}

void UIMainWindow::MenuRetrun()
{
	emit ui.return_pushButton->clicked();
}

void UIMainWindow::PosInWindow()
{
	QRect rc = this->geometry();
	if (rc.contains(QCursor::pos()))
		return;
	
	QDesktopWidget* dsk = QApplication::desktop();
	if (rc.right()+1 != dsk->width())
		return;
	
	this->hide();

	SetWindowPos((HWND)m_HoverWnd->winId(), HWND_TOPMOST, dsk->width() - 45, dsk->height()*0.6, 0, 0, SWP_NOSIZE);
	m_HoverWnd->resize(44, m_HoverWnd->size().height());
	m_HoverWnd->SetNumber();
	m_HoverWnd->show();
}

void UIMainWindow::BulletDelay(int iDelay)
{
	if (m_BulletScreen)
		m_BulletScreen->setTriggerDelay(iDelay);
}

void UIMainWindow::setTriggerType(bool bType)
{
	if (m_BulletScreen)
		m_BulletScreen->setBulletTriggerType(bType);
}

void UIMainWindow::ChatRoomDown()
{
	if (m_charRoom)
		m_charRoom->TalkDown();
}

void UIMainWindow::ErrorStop()
{
	if (m_CameraInfo)
		m_CameraInfo->StopLiveVideo();

	if (m_VideoInfo)
		m_VideoInfo->StopLiveVideo();
}

void UIMainWindow::onTimerResize()
{
	m_RefreshTimer->stop();
	int width = this->width();
	int height = this->height();
	int tabWidth = width - 2;
	int tabHeight = height - 71;
	if (tabWidth == ui.tabWidget->width() && tabHeight == ui.tabWidget->height())
		return;
	int w = tabWidth - ui.tabWidget->width();
	int h = tabHeight - ui.tabWidget->height();
	// tab����ı�
	if (w != 0)
	{
		ui.title_widget->resize(ui.title_widget->width() + w, ui.title_widget->height());
		ui.help_widget->resize(ui.help_widget->width() + w, ui.help_widget->height());
		ui.welcome_widget->resize(ui.welcome_widget->width() + w, ui.welcome_widget->height());
		ui.btn_widget->resize(ui.btn_widget->width() + w, ui.btn_widget->height());
		ui.videoCamera_widget->resize(ui.videoCamera_widget->width() + w, ui.videoCamera_widget->height());
		ui.right_label->move(ui.right_label->geometry().left() + w, ui.right_label->geometry().top());
	}
	// tab2����ı�
	m_AuxiliaryPanel->resize(m_AuxiliaryPanel->width() + w, m_AuxiliaryPanel->height() + h);
	ui.Auxiliary_widget->resize(ui.Auxiliary_widget->width() + w, ui.Auxiliary_widget->height() + h);

	if (m_charRoom)
	{
		int iWidth = ui.tabWidget->width() + w;
		ui.tabWidget->resize(iWidth, ui.tabWidget->height() + h);
		// TabBar����ͼƬ
		QString sheet;
		sheet = QString().sprintf("QTabBar::tab { height: 25px; width: %dpx; } QTabBar::tab:!selected {color:rgb(204,204,204);background: transparent;} QTabBar::tab:selected {color:rgb(5, 158, 213);}", iWidth / 2);
		ui.tabWidget->setStyleSheet(sheet);
		// ��ʱ��λ��
		ui.time_label->move((iWidth - 50) / 2, ui.time_label->geometry().top());

		ui.chat_widget->resize(ui.chat_widget->width() + w, ui.chat_widget->height() + h);
		m_charRoom->resize(m_charRoom->width() + w, m_charRoom->height() + h);
		m_charRoom->setResize(w, h);
	}

	if (m_AuxiliaryPanel && m_AuxiliaryPanel->m_teacher_treewidget && w != 0)
	{
		int iWidth = m_AuxiliaryPanel->m_teacher_treewidget->columnWidth(0);
		m_AuxiliaryPanel->m_teacher_treewidget->setColumnWidth(0, iWidth + w);
	}

	if (m_LessonTable)
		m_LessonTable->setResize(w);

	// ������ťλ�øı�
	m_HelpBtnPos += w;
}