#include "UITalk.h"
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QTime>
#include <QScrollBar>
#include "windows.h"
#include "UIMessageBox.h"
#include "UIHomework.h"


//自动播放语音，全局状态标识
extern int		m_AutoAudioState;
//是否有语音播放中，全局状态标识
extern bool		m_IsAudioPlaying;


void sleep(int secs)
{
	QTime dieTime = QTime::currentTime().addMSecs(secs);

	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 20);
}

#define TIME_DELAY 150

UITalk::UITalk(QWidget *parent)
	: QWidget(parent)
	, m_spacer(NULL)
	, m_mainView(NULL)
	, m_Ver(NULL)
	, m_parent(NULL)
	, m_vecAudio(NULL)
	, m_pFirstAudio(NULL)
{
	ui.setupUi(this);

	m_view = new QScrollArea;
	m_view->setFocusPolicy(Qt::NoFocus);
	m_view->setFrameStyle(QFrame::NoFrame);
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_mainView = new QWidget();
	m_VerAll = new QVBoxLayout(m_mainView);

	m_Ver = new QVBoxLayout();
	m_VerAll->addLayout(m_Ver);
	m_Ver->setSpacing(0);

	m_view->setWidget(m_mainView);
	m_view->setWidgetResizable(true);

	ui.verticalLayout->addWidget(m_view);

	style(m_view);

	QScrollBar * mBar = m_view->verticalScrollBar();
	connect(mBar, SIGNAL(rangeChanged(int, int)), this, SLOT(slot_ScrollDownBottom(int, int)));
}

UITalk::~UITalk()
{
	if (m_Ver)
	{
		delete m_Ver;
		m_Ver = NULL;
	}

	if (m_VerAll)
	{
		delete m_VerAll;
		m_VerAll = NULL;
	}

	if (m_mainView)
	{
		delete m_mainView;
		m_mainView = NULL;
	}

	if (m_view)
	{
		delete m_view;
		m_view = NULL;
	}
}

// 插入文字聊天信息
void UITalk::InsertChat(QPixmap* pixmap, QString name, QString time, QString text, bool bTeacher)
{
	InsertSpacer();

	if (!pixmap)
		pixmap = &QPixmap("./images/teacherPhoto.png");

	QFont font;
	font.setPointSize(10);
	font.setFamily(("微软雅黑"));

	QFont font1;
	font1.setPointSize(9);
	font1.setFamily(("微软雅黑"));

	// 第一行（头像、名字、时间）
	QHBoxLayout* FirstRow = new QHBoxLayout();
	FirstRow->setObjectName("firstRow");

	CBtnPix* head = new CBtnPix(*pixmap, this);
	head->setFixedSize(24, 24);

	QLabel* LName = new QLabel();
	if (name.count()>7)
		name = name.mid(0, 7);
	LName->setText(name);
	LName->setFont(font);
	if (bTeacher)
		LName->setStyleSheet("color: rgb(190, 11, 11);"); //老师名字颜色
	else
		LName->setStyleSheet("color: rgb(135, 195, 237);"); //学生名字颜色
	LName->setFixedWidth(LName->fontMetrics().width(name));

	QLabel* LTime = new QLabel();
	LTime->setText(time);
	LTime->setFont(font1);
	LTime->setStyleSheet("color: rgb(153, 153, 153);");
	FirstRow->addWidget(head);
	FirstRow->addWidget(LName);
	FirstRow->addWidget(LTime);

	// 第二行（聊天内容）
	QHBoxLayout* SecRow = new QHBoxLayout();
	SecRow->setContentsMargins(30, 0, 10, 0);

	if (isDigitStr(text))
	{
		AnimatedTextBrowserA* mText = new AnimatedTextBrowserA(true, this);
		mText->setOpenLinks(true);
		mText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);	//设置滚动条不可见 add by zbc 20171013
		mText->setFrameShape(QFrame::NoFrame);
		mText->setMinimumWidth(245);
		mText->setFont(font);
		mText->setStyleSheet("color: rgb(85, 85, 85);");
		mText->append(text);
		SecRow->addWidget(mText);
		mText->autoHeight();
		connect(mText, SIGNAL(sig_scrollDown()), this, SLOT(slot_scrollDown()));
	}
	else
	{
		QLabel* LText = new QLabel();
		LText->setWordWrap(true);
		LText->setText(text);
		LText->setFont(font);
		LText->setStyleSheet("color: rgb(85, 85, 85);");
		SecRow->addWidget(LText);
	}
	
	m_Ver->addLayout(FirstRow);
	m_Ver->addLayout(SecRow);

	// 添加到布局里
	if (m_spacer == NULL)
	{
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}
	else
	{
		m_Ver->removeItem(m_spacer);
		m_spacer = NULL;
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}

}

void UITalk::InsertHomeworkInfo(QPixmap *mHeadPic, QString mName, QString mContent, QString mType, QString mTime, QString mURL, bool bTeacher)
{
	InsertSpacer();
	if (!mHeadPic)
	{
		mHeadPic = &QPixmap("./images/teacherPhoto.png");
	}
	// 第一行（头像、名字、时间）
	QHBoxLayout* FirstRow = new QHBoxLayout();
	FirstRow->setObjectName("firstRow");
	QFont font;
	font.setPointSize(10);
	font.setFamily(("微软雅黑"));
	CBtnPix* head = new CBtnPix(*mHeadPic, this);
	head->setFixedSize(24, 24);

	QFont font1;
	font1.setPointSize(9);
	font1.setFamily(("微软雅黑"));

	QLabel* LName = new QLabel();
	if (mName.count() > 7)
		mName = mName.mid(0, 7);
	LName->setText(mName);
	LName->setFont(font);
	if (bTeacher)
		LName->setStyleSheet("color: rgb(190, 11, 11);"); //老师名字颜色
	else
		LName->setStyleSheet("color: rgb(135, 195, 237);"); //学生名字颜色
	LName->setFixedWidth(LName->fontMetrics().width(mName));

	QLabel* LTime = new QLabel();
	LTime->setStyleSheet("color: rgb(153, 153, 153);");
	LTime->setText(mTime);
	LTime->setFont(font1);
	FirstRow->addWidget(head);
	FirstRow->addWidget(LName);
	FirstRow->addWidget(LTime);
	

	// 第二行（聊天内容）
	QHBoxLayout* SecRow = new QHBoxLayout();
	SecRow->setContentsMargins(30, 0, 0, 0);
	
	QWidget *mHomeWidget = new QWidget;
	UIHomework *mHomeworkObject = new UIHomework(mHomeWidget);
	//mHomeworkObject->show();
	mHomeworkObject->ShowPage(mContent, mType, mURL);
	mHomeWidget->setMinimumHeight(68);
	

	SecRow->addWidget(mHomeWidget);
	m_Ver->addLayout(FirstRow);
	m_Ver->addLayout(SecRow);
	
	// 添加到布局里
	if (m_spacer == NULL)
	{
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}
	else
	{
		m_Ver->removeItem(m_spacer);
		m_spacer = NULL;
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}
}

// 插入语音聊天信息
void UITalk::InsertAudioChat(QPixmap* pixmap, QString name, QString time, QString text, std::string path, std::string sid, std::string msgid, bool bTeacher, bool bRead)
{
	InsertSpacer();

	if (!pixmap)
		pixmap = &QPixmap("./images/teacherPhoto.png");

	QFont font;
	font.setPointSize(10);
	font.setFamily(("微软雅黑"));

	QFont font1;
	font1.setPointSize(9);
	font1.setFamily(("微软雅黑"));

	// 第一行（头像、名字、时间）
	QHBoxLayout* FirstRow = new QHBoxLayout();
	FirstRow->setObjectName("firstRow");

	CBtnPix* head = new CBtnPix(*pixmap, this);
	head->setFixedSize(24, 24);

	QLabel* LName = new QLabel();
	if (name.count() > 7)
		name = name.mid(0, 7);
	LName->setText(name);
	LName->setFont(font);
	if (bTeacher)
		LName->setStyleSheet("color: rgb(190, 11, 11);"); //老师名字颜色
	else
		LName->setStyleSheet("color: rgb(135, 195, 237);"); //学生名字颜色
	LName->setFixedWidth(LName->fontMetrics().width(name));

	QLabel* LTime = new QLabel();
	LTime->setText(time);
	LTime->setFont(font1);
	LTime->setStyleSheet("color: rgb(153, 153, 153);");
	FirstRow->addWidget(head);
	FirstRow->addWidget(LName);
	FirstRow->addWidget(LTime);
	// 第二行（聊天内容）
	QHBoxLayout* SecRow = new QHBoxLayout();
	SecRow->setContentsMargins(30, 0, 0, 0);
	CBtnAudio* pAudio = new CBtnAudio(path, sid,msgid,this,bRead);
	pAudio->setFixedSize(80, 25);
	connect(pAudio, SIGNAL(sig_Audioclicked(std::string, std::string, std::string, bool)), this, SLOT(slot_Audioclicked(std::string, std::string, std::string, bool)));
	m_vecAudio.push_back(pAudio);

	//因为是教师端，所以bTeacher 为true，代表老师自己发的语音，所以不插入语音链表
	if (bTeacher == false)
	{
		if (m_pFirstAudio == NULL)
		{
			m_pFirstAudio = pAudio;
		}
		else
		{
			m_pFirstAudio->setLastAudio(pAudio);
			m_pFirstAudio = pAudio;
		}
	}
	
	SecRow->addWidget(pAudio);

	QLabel* LDur = new QLabel(); // 显示时长
	LDur->setText(text + "\"");
	LDur->setFont(font);
	LDur->setStyleSheet("color:rgb(153,153,153);}");
	SecRow->addWidget(LDur);
	// 加个弹簧，把按钮往左顶
	QSpacerItem* spacer = new QSpacerItem(5, 5, QSizePolicy::Expanding, QSizePolicy::Minimum);
	SecRow->addSpacerItem(spacer);

	m_Ver->addLayout(FirstRow);
	m_Ver->addLayout(SecRow);

	// 添加到布局里
	if (m_spacer == NULL)
	{
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}
	else
	{
		m_Ver->removeItem(m_spacer);
		m_spacer = NULL;
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}


	if (m_parent && (m_parent->isVisible()) && (bTeacher == false))
	{
		if ((m_IsAudioPlaying == false) && (m_AutoAudioState == 1))
		{
			pAudio->clicked(true);
			//AutoPlayAudio();  //插入语音后，判断当前自动读取状态来决定是否自动读取语音
		}
	}


}

// 插入语音聊天信息(未下载)
void UITalk::InsertAudioChatNew(QPixmap* pixmap, QString name, QString time, QString text, std::string path, std::string sid, std::string msgid, bool bTeacher, bool bRead)
{
	CBtnAudio* pAudio = new CBtnAudio(path, sid, msgid, this, bRead);
	pAudio->setMsgAll(pixmap, name, time, text, path, sid, msgid, bTeacher, bRead);
	connect(pAudio, SIGNAL(sig_AudioLoadEnd(QPixmap*, QString, QString, QString, std::string, std::string, std::string, bool, bool,CBtnAudio*)), 
		this, SLOT(slot_AudioLoadEnd(QPixmap*, QString, QString, QString, std::string, std::string, std::string, bool, bool, CBtnAudio*)));
	pAudio->hide();
}

void UITalk::slot_AudioLoadEnd(QPixmap* pixmap, QString name, QString time, QString text, std::string path, std::string sid, std::string msgid, bool bTeacher, bool bRead, CBtnAudio* pAudio)
{
	try
	{
		if (pAudio)
		{	
			qDebug() << __FILE__ << __LINE__ << "删除语音1！";
			delete pAudio;	
			qDebug() << __FILE__ << __LINE__ << "删除语音2！";
			pAudio = NULL;
		}
	}
	catch (...)
	{
		qDebug() << __FILE__ << __LINE__ << "当前删除语音失败！";
	}

	try
	{
		InsertAudioChat(pixmap, name, time, text, path, sid, msgid, bTeacher, bRead);
	}
	catch (...)
	{
		qDebug() << __FILE__ << __LINE__ << "name" << name << "time" << time << "text" << text << "path" << QString::fromStdString(path) << "sid" << QString::fromStdString(sid)
			<< "msgid" << QString::fromStdString(msgid) << "bTeacher" << bTeacher << "bRead" << bRead;
		QString error = "接收消息失败，请重新进入房间！";
		CMessageBox::showMessage(QString("答疑时间"), error, QString("确定"), QString());
		return;
	}
	
}

// 插入通知消息等
void UITalk::InsertNotice(QString text)
{
	InsertSpacer();

	QFont font;
	font.setPointSize(10);
	font.setFamily(("微软雅黑"));

	// 第一行（消息）
	QHBoxLayout* FirstRow = new QHBoxLayout();
	QLabel* LNotice = new QLabel();
	LNotice->setText(text);
	LNotice->setFont(font);
	LNotice->setStyleSheet("color: rgb(85, 85, 85);"); //学生名字颜色
	LNotice->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	LNotice->setFixedHeight(35);
	FirstRow->addWidget(LNotice);

	m_Ver->addLayout(FirstRow);

	// 添加到布局里
	if (m_spacer == NULL)
	{
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}
	else
	{
		m_Ver->removeItem(m_spacer);
		m_spacer = NULL;
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}

//	m_timerDelay->start(TIME_DELAY);
// 	sleep(50);
// 	ScrollDown();
}

// 插入图片聊天信息
void UITalk::InsertPic(QPixmap* pixmap, QString name, QString time, QString url, QString sMsgID, bool bTeacher)
{
	InsertSpacer();

	if (!pixmap)
		pixmap = &QPixmap("./images/teacherPhoto.png");

	QFont font;
	font.setPointSize(10);
	font.setFamily(("微软雅黑"));

	QFont font1;
	font1.setPointSize(9);
	font1.setFamily(("微软雅黑"));

	// 第一行（头像、名字、时间）
	QHBoxLayout* FirstRow = new QHBoxLayout();

	CBtnPix* head = new CBtnPix(*pixmap, this);
	head->setFixedSize(24, 24);

	QLabel* LName = new QLabel();
	LName->setText(name);
	LName->setFont(font);
	if (bTeacher)
		LName->setStyleSheet("color: rgb(190, 11, 11);"); //老师名字颜色
	else
		LName->setStyleSheet("color: rgb(135, 195, 237);"); //学生名字颜色
	LName->setFixedWidth(LName->fontMetrics().width(name));

	QLabel* LTime = new QLabel();
	LTime->setText(time);
	LTime->setFont(font1);
	LTime->setStyleSheet("color: rgb(153, 153, 153);");
	FirstRow->addWidget(head);
	FirstRow->addWidget(LName);
	FirstRow->addWidget(LTime);

	// 第二行（聊天内容）
	QHBoxLayout* SecRow1 = new QHBoxLayout();
	SecRow1->setContentsMargins(30, 0, 0, 0);

	QVBoxLayout* SecRow = new QVBoxLayout();
//	SecRow->setContentsMargins(30, 0, 0, 0);

	// 读取数据
	QFile theFile(url);
	theFile.open(QIODevice::ReadOnly);
	QByteArray ba = theFile.readAll();

	// 生成QPixmap类型文件
	QPixmap img;
	img.loadFromData(ba);
	CBtnPix* pBtn = new CBtnPix(url, img, this);
	if (pBtn)
	{
		connect(pBtn, SIGNAL(sig_idclicked(QString, QPixmap, bool)), this, SLOT(slot_btnclicked(QString, QPixmap, bool)));
		connect(pBtn, SIGNAL(sig_faildclicked(CBtnPix*)), this, SLOT(slot_faildclicked(CBtnPix*)));

		QPushButton* pImgProcess = new QPushButton();
		pImgProcess->setFixedSize(0, 0);
		pImgProcess->setStyleSheet("QPushButton{border-image:url(./images/process.png);");
		pBtn->SetImgProcess(pImgProcess);
		pBtn->SetMsgID(sMsgID);

		SecRow->addWidget(pBtn);
		SecRow->addWidget(pImgProcess);
		
		m_vecImgProcess.push_back(pBtn);
	}

	QSpacerItem* spacer = new QSpacerItem(5, 5, QSizePolicy::Expanding, QSizePolicy::Minimum);
	SecRow1->addLayout(SecRow);
	SecRow1->addItem(spacer);

	m_Ver->addLayout(FirstRow);
	m_Ver->addLayout(SecRow1);

	// 添加到布局里
	if (m_spacer == NULL)
	{
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}
	else
	{
		m_Ver->removeItem(m_spacer);
		m_spacer = NULL;
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}

}

// 插入图片聊天信息
void UITalk::InsertPicUrl(QPixmap* pixmap, QString name, QString time, QString url_, QString sMsgID, bool bTeacher)
{
	InsertSpacer();

	if (!pixmap)
		pixmap = &QPixmap("./images/teacherPhoto.png");

	QFont font;
	font.setPixelSize(13);
	font.setFamily(("微软雅黑"));

	QFont font1;
	font1.setPixelSize(12);
	font1.setFamily(("微软雅黑"));

	// 第一行（头像、名字、时间）
	QHBoxLayout* FirstRow = new QHBoxLayout();

	CBtnPix* head = new CBtnPix(*pixmap, this);
	head->setFixedSize(24, 24);

	QLabel* LName = new QLabel();
	if (name.count() > 7)
		name = name.mid(0, 7);
	LName->setText(name);
	LName->setFont(font);
	if (bTeacher)
		LName->setStyleSheet("color: rgb(190, 11, 11);"); //老师名字颜色
	else
		LName->setStyleSheet("color: rgb(135, 195, 237);"); //学生名字颜色
	LName->setFixedWidth(LName->fontMetrics().width(name));

	QLabel* LTime = new QLabel();
	LTime->setText(time);
	LTime->setFont(font1);
	LTime->setStyleSheet("color: rgb(153, 153, 153);");
	FirstRow->addWidget(head);
	FirstRow->addWidget(LName);
	FirstRow->addWidget(LTime);

	// 第二行（聊天内容）
	QHBoxLayout* SecRow1 = new QHBoxLayout();
	SecRow1->setContentsMargins(30, 0, 0, 0);

	QVBoxLayout* SecRow = new QVBoxLayout();
	// 读取数据
	QUrl url(url_);
	QNetworkAccessManager manager;
	QEventLoop loop;

	QNetworkReply *reply = manager.get(QNetworkRequest(url));
	//请求结束并下载完成后，退出子事件循环 
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//开启子事件循环 
	loop.exec();
	QByteArray jpegData = reply->readAll();
	QPixmap pixmapload;

	// 加载成功则显示
	pixmapload.loadFromData(jpegData);
	CBtnPix* pBtn = new CBtnPix(url_, pixmapload, this);
	if (pBtn)
	{
		connect(pBtn, SIGNAL(sig_idclicked(QString, QPixmap, bool)), this, SLOT(slot_btnclicked(QString, QPixmap, bool)));
		connect(pBtn, SIGNAL(sig_faildclicked(CBtnPix*)), this, SLOT(slot_faildclicked(CBtnPix*)));

		QPushButton* pImgProcess = new QPushButton();
		pImgProcess->setFixedSize(0, 0);
		pImgProcess->setStyleSheet("QPushButton{border-image:url(./images/process.png);");
		pBtn->SetImgProcess(pImgProcess);
		pBtn->SetMsgID(sMsgID);

		SecRow->addWidget(pBtn);
		SecRow->addWidget(pImgProcess);

		m_vecImgProcess.push_back(pBtn);
	}

	QSpacerItem* spacer = new QSpacerItem(5, 5, QSizePolicy::Expanding, QSizePolicy::Minimum);
	SecRow1->addLayout(SecRow);
	SecRow1->addItem(spacer);

	m_Ver->addLayout(FirstRow);
	m_Ver->addLayout(SecRow1);

	// 添加到布局里
	if (m_spacer == NULL)
	{
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}
	else
	{
		m_Ver->removeItem(m_spacer);
		m_spacer = NULL;
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}

	// 	sleep(50);
	// 	ScrollDown();
}

// 点击图片获取图片本地url
void UITalk::slot_btnclicked(QString imgPath, QPixmap pixmap, bool b)
{
	imgPath.replace("/", "\\");
	TCHAR path[MAX_PATH] = { 0 };
	TCHAR Dir[MAX_PATH] = { 0 };
	GetSystemDirectory(Dir, MAX_PATH);
	lstrcat(path, Dir);
	lstrcat(path, L"\\rundll32.exe");

	QString sDir = QString::fromStdWString(Dir);;
	QString strParm;
	strParm.append(QString("%1\\shimgvw.dll imageview_fullscreen %2").arg(sDir).arg(imgPath));
	LPCTSTR lpstrParm = (LPCTSTR)strParm.utf16();
	HINSTANCE hNewExe = ShellExecute(NULL, L"Open", path, lpstrParm, NULL, SW_SHOW);
	if ((DWORD)hNewExe <= 32)
	{
		int error = (DWORD)hNewExe;
	}
}

// 插入表情聊天信息
void UITalk::InsertEmoji(QPixmap* pixmap, QString name, QString time, QString text, bool bTeacher)
{
	InsertSpacer();

	if (!pixmap)
		pixmap = &QPixmap("./images/teacherPhoto.png");

	QFont font;
	font.setPointSize(10);
	font.setFamily(("微软雅黑"));

	QFont font1;
	font1.setPointSize(9);
	font1.setFamily(("微软雅黑"));

	// 第一行（头像、名字、时间）
	QHBoxLayout* FirstRow = new QHBoxLayout();

	CBtnPix* head = new CBtnPix(*pixmap, this);
	head->setFixedSize(24, 24);

	QLabel* LName = new QLabel();
	LName->setText(name);
	LName->setFont(font);
	if (bTeacher)
		LName->setStyleSheet("color: rgb(190, 11, 11);"); //老师名字颜色
	else
		LName->setStyleSheet("color: rgb(135, 195, 237);"); //学生名字颜色
	LName->setFixedWidth(LName->fontMetrics().width(name));

	QLabel* LTime = new QLabel();
	LTime->setText(time);
	LTime->setFont(font1);
	LTime->setStyleSheet("color: rgb(153, 153, 153);");
	FirstRow->addWidget(head);
	FirstRow->addWidget(LName);
	FirstRow->addWidget(LTime);

	// 第二行（聊天内容）
	QHBoxLayout* SecRow = new QHBoxLayout();
	SecRow->setContentsMargins(30, 0, 0, 0);

	AnimatedTextBrowserA* Anim = new AnimatedTextBrowserA(true,this);
	Anim->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);	//设置滚动条不可见 add by zbc 20171013
	Anim->setFrameShape(QFrame::NoFrame);
	Anim->setOpenLinks(true);
	Anim->setMinimumWidth(240);
	Anim->setFont(font);
	SecRow->addWidget(Anim);
	ParseFace(Anim, text);
	Anim->autoHeight();
	connect(Anim, SIGNAL(sig_scrollDown()), this, SLOT(slot_scrollDown()));

	m_Ver->addLayout(FirstRow);
	m_Ver->addLayout(SecRow);

	// 添加到布局里
	if (m_spacer == NULL)
	{
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}
	else
	{
		m_Ver->removeItem(m_spacer);
		m_spacer = NULL;
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}

//	m_timerDelay->start(TIME_DELAY);
// 	sleep(50);
// 	ScrollDown();
}

void UITalk::ParseFace(AnimatedTextBrowserA* Anim, QString qContect)
{
	Anim->append("");
	QString qFace = "";			// 表情
	bool    bHas = false;		// 判断当前是否有表情
	int iCount = qContect.size();
	for (int i = 0; i < iCount; i++)
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
			Anim->insertHtml("<img src='" + strFace + "'/>");  //   此处的test 即 url
			Anim->addAnimation(QUrl(strFace), strFace);  //添加一个动画.	

			qFace = "";
			bHas = false;
			continue;
		}

		if (!bHas)
		{
			stringToHtml(str,QColor(85,85,85));
			Anim->insertHtml(str);
		}
	}
}

QString UITalk::BuildFaceToUrl(QString qFace)
{
	qFace.replace("[", "");
	qFace.replace("]", "");

	QString qFacePath = "./images/em.gif";
	qFacePath.replace("em", qFace);
	return qFacePath;
}

void UITalk::stringToHtml(QString &str, QColor crl)
{
	QByteArray array;
	array.append(crl.red());
	array.append(crl.green());
	array.append(crl.blue());
	QString strC(array.toHex());
	str = QString("<span style=\" color:#%1;\">%2</span>").arg(strC).arg(str);
}

// 滚动到最底部
void UITalk::ScrollDown()
{
	if (m_view)
		m_view->verticalScrollBar()->setValue(m_view->verticalScrollBar()->maximum());
}

void UITalk::PicProcess(double iProcess)
{
	if (m_vecImgProcess.size() > 0)
	{
		std::vector<CBtnPix*>::iterator it;
		it = m_vecImgProcess.begin();
		CBtnPix* img = *it;

		int iPicProcess = 0;
		if (iProcess != 1)
			iPicProcess = 120 * iProcess;
		else
			m_vecImgProcess.erase(it);

		QPushButton* imgProcess = img->GetImgProcess();
		if (imgProcess)
		{
			imgProcess->setFixedSize(iPicProcess, 3);
		}
	}
}

void UITalk::SendStatus(QString msgid)
{
	if (m_vecImgProcess.size() > 0)
	{
		std::vector<CBtnPix*>::iterator it;
		for (it = m_vecImgProcess.begin(); it != m_vecImgProcess.end(); it++)
		{
			CBtnPix* img = *it;
			if (img->GetMsgID() == msgid)
			{
				img->AddFailBtn();

				m_vecImgProcess.erase(it);
				return;
			}

		}
	}
}

void UITalk::setMainWindow(UIChatRoom* parent)
{
	m_parent = parent;
}

void UITalk::slot_faildclicked(CBtnPix* img)
{
	img->DeleteFailBtn();

	QString msgId;
	if (m_parent)
		m_parent->SendImage(img->GetUrl().toStdWString(), msgId, img->GetMsgID());

	m_vecImgProcess.push_back(img);
}

void UITalk::slot_scrollDown()
{
	ScrollDown();
}

// 判断是否有连续的10个数字或者英文
bool UITalk::isDigitStr(QString str)
{
	int nCountChina = 0;
	int nCount = str.count();
	for (int i = 0; i < nCount; i++)
	{
		QChar cha = str.at(i);
		ushort uni = cha.unicode();
		if (uni >= 0x4E00 && uni <=0x9FA5)	//这个字符是中文  
		{
			nCountChina=0;
		}
		else
		{
			nCountChina++;
			if (nCountChina == 10)
				return true;
		}
	}

	return false;
}

void UITalk::style(QScrollArea *style)
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



void UITalk::slot_Audioclicked(std::string path, std::string sid, std::string msgid, bool isPlay)
{
	if (m_parent)
		m_parent->OnPlayAudio(path, sid, msgid, isPlay);
}

void UITalk::stopAudio(std::string msgid)
{
	QString strMsgid = QString::fromStdString(msgid);
	//QString sMsgid = strMsgid.mid(0, 32);
	//如果语音播放停止，且自动播放状态也关闭时
	if (m_AutoAudioState == 0)
	{
		if (m_parent)
		{
			m_parent->GetNowAutoAudioID()->setStyleSheet("QPushButton{border-image:url(./images/AutoAudio2.png);}");
		}
	}

	if (m_vecAudio.size() > 0)
	{
		std::vector<CBtnAudio*>::iterator it;
		for (it = m_vecAudio.begin(); it != m_vecAudio.end(); it++)
		{		
			CBtnAudio* img = *it;
			QString mTestID = img->GetMsgID();
			if (img->GetMsgID() == strMsgid)
			{
				img->stopPlay();
				return;
			}
		}
	}
}



void UITalk::slot_ScrollDownBottom(int mMax, int mMin)
{
	ScrollDown();
}

void UITalk::AutoPlayAudio()
{
	if (m_parent && !m_parent->isVisible())
	{
		return;
	}
	//判断当前自动语音播放状态及语音消息列表是否为空
	if ((m_parent && (m_AutoAudioState == 1))&&(m_vecAudio.size() > 0))
	{
		std::vector<CBtnAudio*>::reverse_iterator it;
		for (it = m_vecAudio.rbegin(); it != m_vecAudio.rend(); it++)
		{
			CBtnAudio* Now_Audio = *it;
			//如果最后一条信息已读，则退出
			if ((Now_Audio == *(m_vecAudio.rbegin())) && (Now_Audio->m_bRead))
			{
				return;
			}
			//寻找到最后一条已读信息,从此条信息的下一条信息开始读取
			if (Now_Audio->m_bRead)
			{
				it--;
				Now_Audio = *it;
				Now_Audio->clicked(true);
				return;
			}
			//假如所有信息都未读，从第一条开始读
			if ((Now_Audio == m_vecAudio[0]) && (Now_Audio->m_bRead == false))
			{
				emit Now_Audio->clicked(true);
				return;
			}
		}
	}
}



// 插入格式化通知消息等
void UITalk::InsertOneNotice(QString mText)
{
	InsertSpacer();

	QFont font;
	font.setPixelSize(12);
	font.setFamily(("微软雅黑"));

	// 第一行（消息）
	QVBoxLayout* ver = new QVBoxLayout();
	ver->setSpacing(0);

	QVBoxLayout* FirstRow = new QVBoxLayout();
	FirstRow->setContentsMargins(30, 0, 30, 0);
	QLabel* LName = new QLabel();
	LName->setText(mText);
	LName->setFont(font);
	LName->setStyleSheet("color: rgb(153,153,153);border-radius:5px;border-image:url(./images/notice_back.png);"); //字体颜色 
	LName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	LName->setWordWrap(true);
	FirstRow->addWidget(LName);


	m_Ver->addLayout(FirstRow);
	m_Ver->addLayout(ver);

	// 添加到布局里
	if (m_spacer == NULL)
	{
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}
	else
	{
		m_Ver->removeItem(m_spacer);
		m_spacer = NULL;
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}
}


// 插入格式化通知消息等
void UITalk::InsertNewNotice(QString name, QString text)
{
	InsertSpacer();

	QFont font;
	font.setPixelSize(12);
	font.setFamily(("微软雅黑"));

	QFont font1;
	font1.setPixelSize(13);
	font1.setFamily(("微软雅黑"));

	// 第一行（消息）
	QVBoxLayout* ver = new QVBoxLayout();
	ver->setSpacing(0);

	QVBoxLayout* FirstRow = new QVBoxLayout();
	FirstRow->setContentsMargins(30, 0, 30, 0);
	QLabel* LName = new QLabel();
	LName->setText(name);
	LName->setFont(font);
	LName->setStyleSheet("color: rgb(153,153,153);border-radius:5px;border-image:url(./images/notice_back.png);"); //学生名字颜色 
	LName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	LName->setWordWrap(true);
	FirstRow->addWidget(LName);

	QHBoxLayout* SecRow = new QHBoxLayout();
	SecRow->setContentsMargins(30, 0, 30, 0);

	AnimatedTextBrowserA* EditNotice = new AnimatedTextBrowserA(true, this);
	EditNotice->setOpenLinks(true);
	EditNotice->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);	//设置滚动条不可见 add by zbc 20171013
	EditNotice->setText(text);
	EditNotice->setFont(font1);
	EditNotice->setStyleSheet("color: rgb(102,102,102);border-radius:5px;border-image:url(./images/notice_back.png);"); //学生名字颜色
	EditNotice->setWordWrapMode(QTextOption::WrapAnywhere);
	EditNotice->autoHeight();
	EditNotice->AutoLeftOrEnter();
	EditNotice->setEnabled(false);
	connect(EditNotice, SIGNAL(sig_scrollDown()), this, SLOT(slot_scrollDown()));
	SecRow->addWidget(EditNotice);

// 	QSpacerItem* spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
// 	ver->addLayout(FirstRow);
// 	ver->addLayout(SecRow);
// 	ver->addSpacerItem(spacer);

	m_Ver->addLayout(FirstRow);
	m_Ver->addLayout(SecRow);
	m_Ver->addLayout(ver);

	// 添加到布局里
	if (m_spacer == NULL)
	{
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}
	else
	{
		m_Ver->removeItem(m_spacer);
		m_spacer = NULL;
		m_spacer = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver->addSpacerItem(m_spacer);
	}
}

// 插入间距
void UITalk::InsertSpacer()
{
	QLabel* pLabel = new QLabel();
	pLabel->setFixedHeight(10);
	m_Ver->addWidget(pLabel);
}