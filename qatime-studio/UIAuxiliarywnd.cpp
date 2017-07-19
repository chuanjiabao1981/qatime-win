#include "UIAuxiliarywnd.h"
#include <QScrollBar>
#include <QPainter>
#include <QMouseEvent>

UIAuxiliaryWnd::UIAuxiliaryWnd(QWidget *parent)
	: QWidget(parent)
	, m_pWorker(NULL)
	, m_pWorker1v1(NULL)
	, m_parent(NULL)
{
	ui.setupUi(this);
	setAutoFillBackground(true);;
	QPalette p = palette();
	p.setColor(QPalette::Window, QColor(255, 255, 255));
	setPalette(p);
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);

	// 设置全部辅导垂直布局
	m_mainAllView = new QWidget();
	ui.scrollArea->setWidget(m_mainAllView);
	m_VerAll = new QVBoxLayout(m_mainAllView);
	m_VerAll->setSpacing(0);
	m_VerAll->setContentsMargins(5, 5, 0, 0);
	ui.scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	style(ui.scrollArea);

	// 设置今日课程垂直布局
	m_mainTodayView = new QWidget();
	ui.today_scrollArea->setWidget(m_mainTodayView);
	m_VerToday = new QVBoxLayout(m_mainTodayView);
	m_VerToday->setSpacing(0);
	m_VerToday->setContentsMargins(5, 5, 0, 0);
	ui.today_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	style(ui.today_scrollArea);

	// 设置1v1课程垂直布局
	m_main1v1View = new QWidget();
	ui.live1V1_scrollArea->setWidget(m_main1v1View);
	m_Ver1v1 = new QVBoxLayout(m_main1v1View);
	m_Ver1v1->setSpacing(0);
	m_Ver1v1->setContentsMargins(5, 5, 0, 0);
	ui.live1V1_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	style(ui.live1V1_scrollArea);

	ui.scrollArea->installEventFilter(this);
	ui.today_scrollArea->installEventFilter(this);
	ui.title_pushButton->installEventFilter(this);
	ui.pic_widget->installEventFilter(this);
	ui.all_widget->setVisible(false);

	connect(ui.today_radioButton, SIGNAL(clicked()), this, SLOT(clickToday()));
	connect(ui.all_radioButton, SIGNAL(clicked()), this, SLOT(clickAll()));
	connect(ui.live1V1_pushButton, SIGNAL(clicked()), this, SLOT(click1V1()));
	connect(ui.min_pushButton, SIGNAL(clicked()), this, SLOT(MinDialog()));
	connect(ui.close_pushButton, SIGNAL(clicked()), this, SLOT(CloseDialog()));
//	connect(ui.pic_btn, SIGNAL(clicked()), this, SLOT(clickPic()));

	m_pWorker = new WorkerPic();
	connect(this, SIGNAL(sig_StartLoading()), m_pWorker, SLOT(slot_StartLoading()));
	connect(this, SIGNAL(sig_Close()), m_pWorker, SLOT(slot_Close()));

	m_pWorker1v1 = new WorkerPic();
	connect(this, SIGNAL(sig_Start1v1Loading()), m_pWorker1v1, SLOT(slot_StartLoading()));
	connect(this, SIGNAL(sig_Close()), m_pWorker1v1, SLOT(slot_Close()));

	ui.live1V1_widget->setVisible(false);
	ui.all_widget->setVisible(false);
	m_UIMenu = new UIMenu(this);
	m_UIMenu->SetMainParent(this);
	m_UIMenu->hide();
}

UIAuxiliaryWnd::~UIAuxiliaryWnd()
{

}

void UIAuxiliaryWnd::paintEvent(QPaintEvent *event)
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
bool UIAuxiliaryWnd::eventFilter(QObject *target, QEvent *event)
{
	if (target == ui.title_pushButton)
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
	}
	else if (target == ui.scrollArea || target == ui.today_scrollArea)
	{
		QMouseEvent* pMe = static_cast<QMouseEvent*>(event);
		if (event->type() == QEvent::Resize)
		{
			m_mainAllView->setMaximumWidth(ui.scrollArea->width());
			m_mainTodayView->setMaximumWidth(ui.today_scrollArea->width());
		}
	}
	else if (target == ui.pic_widget)
	{
		QMouseEvent* pMe = static_cast<QMouseEvent*>(event);
		if (event->type() == QEvent::Enter)
		{
			clickPic();
		}
	}
	return false;
}

void UIAuxiliaryWnd::style(QScrollArea *style)
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

void UIAuxiliaryWnd::MinDialog()
{
	showMinimized();
	hide();
}

void UIAuxiliaryWnd::CloseDialog()
{
	if (m_parent->IsMayClose())
	{
		emit sig_Close();
		m_parent->CloseDialog();
	}
}

void UIAuxiliaryWnd::clickToday()
{
	ui.today_widget->setVisible(true);
	ui.live1V1_widget->setVisible(false);
	ui.all_widget->setVisible(false);
}

void UIAuxiliaryWnd::clickAll()
{
	ui.live1V1_widget->setVisible(false);
	ui.today_widget->setVisible(false);
	ui.all_widget->setVisible(true);
}

void UIAuxiliaryWnd::click1V1()
{
	ui.today_widget->setVisible(false);
	ui.all_widget->setVisible(false);
	ui.live1V1_widget->setVisible(true);
}

void UIAuxiliaryWnd::AddTodayAuxiliary(QString lessonName, QString courseID, QString courseName, QString time, QString status, QString lessonid)
{
	UIAuxiliaryToday* auxiliary = new UIAuxiliaryToday(ui.today_widget);
	auxiliary->AddLesson(lessonName, courseID, courseName, time, status);
	connect(auxiliary, SIGNAL(clickAuxiliaryToday(UIAuxiliaryToday*)), this, SLOT(clickAuxiliaryToday(UIAuxiliaryToday*)));
	m_VerToday->addWidget(auxiliary);
	m_mapAuxiliaryLesson.insert(lessonid, auxiliary);

	// 添加到布局里
	if (m_spacerToday == NULL)
	{
		m_spacerToday = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_VerToday->addSpacerItem(m_spacerToday);
	}
	else
	{
		m_VerToday->removeItem(m_spacerToday);
		m_spacerToday = NULL;
		m_spacerToday = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_VerToday->addSpacerItem(m_spacerToday);
	}
}

void UIAuxiliaryWnd::AddAuxiliary(QString picUrl, QString courseName, QString grade, QString teacherName, QString chatID, QString courseID, QString teacherID, QString token,
	QString studentName, std::string AudioPath, QString status, QString url, QString cameraUrl)
{
	UIAuxiliaryList* auxiliary = new UIAuxiliaryList(ui.all_widget);
	QLabel* pic = auxiliary->AddCourse(picUrl, courseName, grade, teacherName, chatID, courseID, teacherID, token, studentName, AudioPath, status,url,cameraUrl);
	connect(auxiliary, SIGNAL(clickAuxiliary(UIAuxiliaryList*)), this, SLOT(clickAuxiliary(UIAuxiliaryList*)));
	m_VerAll->addWidget(auxiliary);

	QLabel* line = new QLabel();
	line->setFixedHeight(1);
	line->setStyleSheet("background-color: rgb(229, 229, 229);");
	m_VerAll->addWidget(line);

	m_mapAuxiliaryChatID.insert(chatID, auxiliary);
	m_mapAuxiliaryCourseID.insert(courseID, auxiliary);
	m_pWorker->m_mapUrl.insert(pic, picUrl);

	// 添加到布局里
	if (m_spacerAll == NULL)
	{
		m_spacerAll = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_VerAll->addSpacerItem(m_spacerAll);
	}
	else
	{
		m_VerAll->removeItem(m_spacerAll);
		m_spacerAll = NULL;
		m_spacerAll = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_VerAll->addSpacerItem(m_spacerAll);
	}
}

void UIAuxiliaryWnd::Add1v1Auxiliary(QString picUrl, QString courseName, QString grade, QString teacherName, QString chatID, QString courseID, QString teacherID, QString token,
	QString studentName, std::string AudioPath, QString status)
{
	UIAuxiliaryList* auxiliary = new UIAuxiliaryList(ui.all_widget);
	QLabel* pic = auxiliary->AddCourse(picUrl, courseName, grade, teacherName, chatID, courseID, teacherID, token, studentName, AudioPath, status,"","",true);
	connect(auxiliary, SIGNAL(clickAuxiliary(UIAuxiliaryList*)), this, SLOT(clickAuxiliary(UIAuxiliaryList*)));
	m_Ver1v1->addWidget(auxiliary);

	QLabel* line = new QLabel();
	line->setFixedHeight(1);
	line->setStyleSheet("background-color: rgb(229, 229, 229);");
	m_Ver1v1->addWidget(line);

	m_mapAuxiliaryChatID.insert(chatID, auxiliary);
	m_mapAuxiliaryCourseID.insert(courseID, auxiliary);
	m_pWorker1v1->m_mapUrl.insert(pic, picUrl);

	// 添加到布局里
	if (m_spacer1v1 == NULL)
	{
		m_spacer1v1 = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver1v1->addSpacerItem(m_spacer1v1);
	}
	else
	{
		m_Ver1v1->removeItem(m_spacer1v1);
		m_spacer1v1 = NULL;
		m_spacer1v1 = new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
		m_Ver1v1->addSpacerItem(m_spacer1v1);
	}
}

void UIAuxiliaryWnd::SetToken(QString mRemeberToken)
{
	m_UIMenu->setToken(mRemeberToken);
}

void UIAuxiliaryWnd::setTeacherName(QString TeacherName)
{
	m_UIMenu->setName(TeacherName);
}

QPixmap UIAuxiliaryWnd::setTeacherUrl(QString Url)
{
	QUrl url(Url);
	QNetworkAccessManager manager;
	QEventLoop loop;

	QNetworkReply *reply = manager.get(QNetworkRequest(url));
	//请求结束并下载完成后，退出子事件循环 
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//开启子事件循环 
	loop.exec();

	QByteArray jpegData = reply->readAll();
	QPixmap pixmap;
	if (!pixmap.loadFromData(jpegData))
		pixmap = QPixmap("./images/avatar_teacher.png");
	QPixmap scaledPixmap = pixmap.scaled(QSize(30, 30));

	ui.pic_widget->setAutoFillBackground(true);
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(scaledPixmap));
	ui.pic_widget->setPalette(palette);

	m_UIMenu->setPic(scaledPixmap);
	return scaledPixmap;
}

void UIAuxiliaryWnd::AddTodayNoLesson(EN_LESSON_TYPE type)
{
	QHBoxLayout* layout = new QHBoxLayout();
	QPixmap pix = QPixmap("./images/nolesson.png");
	QLabel* noLesson = new QLabel();
	noLesson->setFixedSize(180, 180);
	noLesson->setPixmap(pix);
	layout->addWidget(noLesson);
	switch (type)
	{
	case EN_TODAY_LESSON:
		m_VerToday->addLayout(layout);
		break;
	case EN_ALL_LESSON:
		m_VerAll->addLayout(layout);
		break;
	case EN_1V1_LESSON:
		m_Ver1v1->addLayout(layout);
		break;
	default:
		break;
	}
}

void UIAuxiliaryWnd::LoadPic()
{
	emit sig_StartLoading();
}

void UIAuxiliaryWnd::LoadPic1v1()
{
	emit sig_Start1v1Loading();
}

void UIAuxiliaryWnd::clickPic()
{
	if (m_UIMenu)
	{
		m_UIMenu->move(165, 0);
		m_UIMenu->show();
		m_UIMenu->setFocus();
	}
}

void UIAuxiliaryWnd::setMainWindow(UIMainNewWindow* parent)
{
	m_parent = parent;
}

void UIAuxiliaryWnd::ReturnAccount()
{
	if (m_parent->IsMayClose())
	{
		emit sig_Close();

		if (m_parent)
			m_parent->returnClick();
	}
}

void UIAuxiliaryWnd::setVersion(QString version)
{
	m_UIMenu->setVersion(version);
}

void UIAuxiliaryWnd::clickAuxiliary(UIAuxiliaryList* auxiliary)
{
	if (m_parent)
	{
		qDebug() << __FILE__ << __LINE__ << "点击--" << "辅导班ID：" << auxiliary->CourseID() << "辅导班名字：" << auxiliary->CourseName();
		m_parent->CreateRoom(auxiliary->ChatID(), auxiliary->CourseID(), auxiliary->TeacherID(), auxiliary->Token(), auxiliary->StudentName(), 
							auxiliary->AudioPath(), auxiliary->CourseName(), auxiliary->UnreadMsgCount(), auxiliary->Status(), auxiliary->BoardUrl(), auxiliary->CameraUrl(),auxiliary->Is1v1Lesson());
		auxiliary->ClearMsgNumber();
	}
}

void UIAuxiliaryWnd::clickAuxiliaryToday(UIAuxiliaryToday* auxiliaryToday)
{
	QMap<QString, UIAuxiliaryList*>::iterator it;
	UIAuxiliaryList* auxiliary = NULL;
	auxiliary = m_mapAuxiliaryCourseID.value(auxiliaryToday->GetCourseID(),NULL);
	if (auxiliary)
	{
		if (m_parent)
		{
			qDebug() << __FILE__ << __LINE__ << "点击--"<<"辅导班ID：" << auxiliary->CourseID() << "辅导班名字：" << auxiliary->CourseName();
			m_parent->CreateRoom(auxiliary->ChatID(), auxiliary->CourseID(), auxiliary->TeacherID(), auxiliary->Token(), auxiliary->StudentName(), auxiliary->AudioPath(),
				auxiliary->CourseName(), auxiliary->UnreadMsgCount(), auxiliary->Status(), auxiliary->BoardUrl(), auxiliary->CameraUrl(), auxiliary->Is1v1Lesson());
			
			auxiliary->ClearMsgNumber();
		}
	}
}

void UIAuxiliaryWnd::SetEnvironmental(bool bType, QString homePage)
{
	m_UIMenu->SetEnvironmental(bType,homePage);
}

void UIAuxiliaryWnd::ReceiverNumber(QString chatID)
{
	UIAuxiliaryList* auxiliary = NULL;
	auxiliary = m_mapAuxiliaryChatID.value(chatID,NULL);
	if (auxiliary)
	{
		auxiliary->AddMsgCount();
	}
}

void UIAuxiliaryWnd::ChangeAuxiliaryTodayStatus(QString lessonid, QString status)
{
	UIAuxiliaryToday* auxiliary = NULL;
	auxiliary = m_mapAuxiliaryLesson.value(lessonid,NULL);
	if (auxiliary)
	{
		auxiliary->setStatue(status);
	}
}