#include "loginwindow.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "user.h"
#include "windows.h"
#include "ShellApi.h"
#include <QHBoxLayout>
#include "QShortcut"
#include <QDesktopWidget>
#include <QApplication>

#ifdef TEST
	#define _DEBUG
#else
#endif
TCHAR m_pathHomePage[MAX_PATH] = {0};
TCHAR m_pathUserName[MAX_PATH] = { 0 };
TCHAR m_pathUserPass[MAX_PATH] = { 0 };
TCHAR m_pathTeacherName[MAX_PATH] = { 0 };
TCHAR m_pathTeacherID[MAX_PATH] = { 0 };
TCHAR m_pathTeacherToken[MAX_PATH] = { 0 };
TCHAR m_pathTeacherUrl[MAX_PATH] = { 0 };
TCHAR m_pathAccid[MAX_PATH] = { 0 };
TCHAR m_pathAccidToken[MAX_PATH] = { 0 };
TCHAR m_pathVersion[MAX_PATH] = { 0 };
int	  m_iRemeber = 0;

#define MAINWINDOW_MAXHEIGHT 832	//客户端最大高度
LoginWindow::LoginWindow(QWidget *parent)
	: QMainWindow(parent)
	, mainWin(NULL)
	, trayIcon(NULL)
	, m_TrayMenu(NULL)
	, m_EnvironmentalFormally(true)
{
	
	ui.setupUi(this);
	setAutoFillBackground(true);;
	QPalette p = palette();
	p.setColor(QPalette::Window, QColor(255,255,255));
	setPalette(p);

	// 检查环境
	SetEnvironmental();
	
	connect(ui.login_pushBtn, SIGNAL(clicked()), this, SLOT(OnLogIn()));
	connect(ui.homepage_pushBtn, SIGNAL(clicked()), this, SLOT(BrowseHomePage()));
	connect(ui.register_pushBtn, SIGNAL(clicked()), this, SLOT(RegisterAccount()));
	connect(ui.findPassword_pushBtn, SIGNAL(clicked()), this, SLOT(FindPassword()));
	connect(ui.min_pushBtn, SIGNAL(clicked()), this, SLOT(MinDialog()));
	connect(ui.close_pushBtn, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.remember_checkBox, SIGNAL(stateChanged(int)), this, SLOT(changedRemeber(int)));
	
	ui.UserName_Edit->setTextMargins(45, 3, 20, 3); 
	ui.UserName_Edit->setStyleSheet("QLineEdit:hover{ border: 1px solid #57cff8 }"
		"QLineEdit{ border: 1px solid #cccccc }");
	ui.UserPass_Edit->setTextMargins(45, 3, 20, 3);
	ui.UserPass_Edit->setStyleSheet("QLineEdit:hover{ border: 1px solid #57cff8 }"
		"QLineEdit{ border: 1px solid #cccccc }");

	ReadSetting();
	InitUserName();

	QShortcut *key = new QShortcut(QKeySequence(Qt::Key_Return | Qt::Key_Enter), this);//创建一个快捷键"Key_Return"键
	connect(key, SIGNAL(activated()), this, SLOT(OnLogIn()));//连接到指定槽函数
	setTabOrder(ui.UserName_Edit, ui.UserPass_Edit);
	setTabOrder(ui.UserPass_Edit, ui.login_pushBtn);

	QPixmap pixmap(QCoreApplication::applicationDirPath() + "/images/login_min.png");
	QPixmap pixmap1(QCoreApplication::applicationDirPath() + "/images/login_close.png");
	ui.min_pushBtn->setPixmap(pixmap, 1);
	ui.close_pushBtn->setPixmap(pixmap1, 1);
	ui.homepage_pushBtn->installEventFilter(this);

	ui.remember_checkBox->setStyleSheet("QCheckBox::indicator{width: 65px;height: 13px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/remenber_uncheck.png);}"
		"QCheckBox::indicator:checked{image: url(./images/remenber_check.png);}");

	ui.back_label->setStyleSheet("border: 1px solid #059ed5;");
	CreateTray();

	m_TrayMenu = new UITrayMenu();
	m_TrayMenu->setWindowFlags(Qt::FramelessWindowHint);
	m_TrayMenu->SetMainWindow(this);
	m_TrayMenu->hide();
	
}

LoginWindow::~LoginWindow()
{
	if (mainWin)
	{
		delete mainWin;
		mainWin = NULL;
	}

	if (trayIcon)
	{
		delete trayIcon;
		trayIcon = NULL;
	}
}

void LoginWindow::mousePressEvent(QMouseEvent *e)
{
	last = e->globalPos();
}

void LoginWindow::mouseMoveEvent(QMouseEvent *e)
{
	int dx = e->globalX() - last.x();
	int dy = e->globalY() - last.y();

	last = e->globalPos();
	move(x() + dx, y() + dy);
}

void LoginWindow::mouseReleaseEvent(QMouseEvent *e)
{
	int dx = e->globalX() - last.x();
	int dy = e->globalY() - last.y();

	move(x() + dx, y() + dy);
}

// 开始登录
void LoginWindow::OnLogIn()
{
	Logining(true);
	if (ui.UserName_Edit->text().isEmpty())
	{
		ui.ErrorTip_Label->setText(QString("请输入用户名"));
		ui.UserName_Edit->setText(QString(""));
		ui.UserPass_Edit->setText(QString(""));
		Logining(false);
		return;
	}
	else if (ui.UserPass_Edit->text().isEmpty())
	{
		ui.ErrorTip_Label->setText(QString("请输入密码"));
		ui.UserPass_Edit->setText(QString(""));
		Logining(false);
		return;
	}

	QString str = ui.UserPass_Edit->text();
	str = parse(str);

	QString strUrl;
	strUrl += m_homePage;
	strUrl += "/api/v1/sessions";
	url = QUrl(strUrl);
	

	QByteArray append("client_type=pc");
	append.append("&login_account=");
	append += ui.UserName_Edit->text();
	append.append("&password=");
	qInfo(append, __FILE__, __LINE__);
	append += str;
	append.append("&client_cate=teacher_live");
	
	QNetworkRequest request(url);
	reply = manager.post(request, append);
	connect(reply, &QNetworkReply::finished, this, &LoginWindow::loginFinished);
}

// 返回登录结果
void LoginWindow::loginFinished()
{
	QString strError;
	QByteArray result = reply->readAll();
	int mNetErrorCode = reply->error();
	if (mNetErrorCode != QNetworkReply::NoError)
	{
		strError = QString("登录超时，请检查网络或者防火墙设置！");
		qDebug() << __FILE__ << __LINE__ << mNetErrorCode;
		ui.ErrorTip_Label->setText(strError);
		RemeberPassword();
		Logining(false);
		return;
	}
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	int mErrorCode = 0;
	mErrorCode = error["code"].toInt();
	// 记住老师信息，用于自动登录
	if (obj["status"].toInt() == 1 && data.contains("remember_token"))
	{
		// 记住老师信息，用于自动登录
		m_teacherToken = data["remember_token"].toString();
		QJsonObject objInfo = data["user"].toObject();
		m_teacherID = QString::number(objInfo["id"].toInt());
		m_teacherName = objInfo["name"].toString();
		m_teacherUrl = objInfo["avatar_url"].toString();
		m_accid = objInfo["chat_account"].toObject()["accid"].toString();
		m_accidToken = objInfo["chat_account"].toObject()["token"].toString();

		mainWin = new UIMainNewWindow();
		mainWin->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
		mainWin->setAttribute(Qt::WA_DeleteOnClose, false);
		mainWin->SetEnvironmental(m_EnvironmentalFormally,m_homePage);
		mainWin->setRemeberToken(m_teacherToken);
		mainWin->setTeacherInfo(data["user"].toObject());
		mainWin->RequestKey();
		mainWin->setLoginWindow(this);
		mainWin->setVersion(m_version);
		mainWin->resize(1, 1);
		mainWin->show();
		this->hide();
	}
	else if (mErrorCode == 1001)
		strError = QString("用户未登录");
	else if (mErrorCode == 1002)
		strError = QString("授权过期");
	else if (mErrorCode == 1003)
		strError = QString("没有权限访问");
	else if (mErrorCode == 1004)
		strError = QString("授权失败");
	else if (mErrorCode == 2001)
		strError = QString("客户端版本过低");
	else if (mErrorCode == 2002)
		strError = QString("不支持的客户端");
	else if (mErrorCode == 3001)
		strError = QString("参数错误");
	else if (mErrorCode == 3002)
		strError = QString("数据不合法");
	else if (mErrorCode == 4001)
		strError = QString("找不到资源");
	else if (mErrorCode == 9999)
		strError = QString("服务器错误");
	else 
		strError = QString("用户名或密码不正确");

	ui.ErrorTip_Label->setText(strError);

	RemeberPassword();
	Logining(false);
}

void LoginWindow::BrowseHomePage()
{
	ShellExecute(NULL, L"open", m_pathHomePage, NULL, NULL, SW_SHOW);
}

void LoginWindow::RegisterAccount()
{
	TCHAR m_pathRegister[MAX_PATH] = { 0 };
	lstrcat(m_pathRegister, m_pathHomePage);
	lstrcat(m_pathRegister, L"//teachers//new");
	ShellExecute(NULL, L"open", m_pathRegister, NULL, NULL, SW_SHOW);
}

void LoginWindow::FindPassword()
{
	TCHAR m_pathPass[MAX_PATH] = { 0 };
	lstrcat(m_pathPass, m_pathHomePage);
	lstrcat(m_pathPass, L"//passwords//new");
	ShellExecute(NULL, L"open", m_pathPass, NULL, NULL, SW_SHOW);
}

void LoginWindow::MinDialog()
{
	showMinimized();
}

void LoginWindow::CloseDialog()
{
	CloseTray();	// 关闭小图标
	close();
}

void LoginWindow::ReadSetting()
{
	TCHAR szTempPath[MAX_PATH] = { 0 };

	GetCurrentDirectory(MAX_PATH, szTempPath);
	lstrcat(szTempPath, L"\\config.ini");

	GetPrivateProfileString(L"CONFIG_PATH", L"HOMEPAGE", L"", m_pathHomePage, MAX_PATH, szTempPath);				//访问主页路径
	GetPrivateProfileString(L"CONFIG_PATH", L"USERNAME", L"",    m_pathUserName, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"TEACHERNAME", L"", m_pathTeacherName, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"TEACHERID", L"", m_pathTeacherID, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"TOKEN", L"", m_pathTeacherToken, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"TEACHERURL", L"", m_pathTeacherUrl, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"ACCID", L"", m_pathAccid, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"ACCIDTOKEN", L"", m_pathAccidToken, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"VERSION", L"", m_pathVersion, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"USERPASSWORD", L"", m_pathUserPass, MAX_PATH, szTempPath);
	m_iRemeber = GetPrivateProfileInt(L"CONFIG_PATH", L"REMEBER", 0, szTempPath);

	m_teacherName = QString::fromStdWString(m_pathTeacherName);
	m_teacherID = QString::fromStdWString(m_pathTeacherID);
	m_teacherToken = QString::fromStdWString(m_pathTeacherToken);
	m_teacherUrl = QString::fromStdWString(m_pathTeacherUrl);
	m_accid = QString::fromStdWString(m_pathAccid);
	m_accidToken = QString::fromStdWString(m_pathAccidToken);
	m_password = QString::fromStdWString(m_pathUserPass);
	m_version = QString::fromStdWString(m_pathVersion);
	m_homePage = QString::fromStdWString(m_pathHomePage);
	
	QString sVersion = "  答疑时间直播助手{version}";
	sVersion.replace("{version}", m_version);
	ui.title_label->setText(sVersion);
}

void LoginWindow::changedRemeber(int i)
{
	TCHAR szTempPath[MAX_PATH] = { 0 };

	GetCurrentDirectory(MAX_PATH, szTempPath);
	lstrcat(szTempPath, L"\\config.ini");

	if (i == 0)
	{
		WritePrivateProfileString(L"CONFIG_PATH", L"REMEBER", L"0", szTempPath);
	}
	else
	{
		WritePrivateProfileString(L"CONFIG_PATH", L"REMEBER", L"1", szTempPath);
	}
}

void LoginWindow::RemeberPassword()
{
	TCHAR szTempPath[MAX_PATH] = { 0 };

	GetCurrentDirectory(MAX_PATH, szTempPath);
	lstrcat(szTempPath, L"\\config.ini");

	QString strName = ui.UserName_Edit->text();
	QString strPassWord = ui.UserPass_Edit->text();
	QByteArray bPassWord = strPassWord.toLatin1().toBase64();
	m_password = bPassWord;
	WritePrivateProfileString(L"CONFIG_PATH", L"USERNAME", (LPCTSTR)strName.utf16(), szTempPath);

	if (ui.remember_checkBox->isChecked())
	{
		// TOKEN
		WritePrivateProfileString(L"CONFIG_PATH", L"TOKEN", (LPCTSTR)m_teacherToken.utf16(), szTempPath);
		// 老师ID
		WritePrivateProfileString(L"CONFIG_PATH", L"TEACHERID", (LPCTSTR)m_teacherID.utf16(), szTempPath);
		// 老师名字
		WritePrivateProfileString(L"CONFIG_PATH", L"TEACHERNAME", (LPCTSTR)m_teacherName.utf16(), szTempPath);
		// 老师头像URL
		WritePrivateProfileString(L"CONFIG_PATH", L"TEACHERURL", (LPCTSTR)m_teacherUrl.utf16(), szTempPath);
		// 老师accid
		WritePrivateProfileString(L"CONFIG_PATH", L"ACCID", (LPCTSTR)m_accid.utf16(), szTempPath);
		// 老师accidToken
		WritePrivateProfileString(L"CONFIG_PATH", L"ACCIDTOKEN", (LPCTSTR)m_accidToken.utf16(), szTempPath);
		// 老师密码
		WritePrivateProfileString(L"CONFIG_PATH", L"USERPASSWORD", (LPCTSTR)m_password.utf16(), szTempPath);
	}
	ui.UserPass_Edit->setText(QString(""));
}

void LoginWindow::InitUserName()
{
	ui.UserName_Edit->setText(QString::fromStdWString(m_pathUserName));
	if (m_iRemeber == 1)
	{
		ui.remember_checkBox->setCheckState(Qt::Checked);

		QByteArray bPassword = m_password.toLatin1();
		QString password = QByteArray::fromBase64(bPassword);
		ui.UserPass_Edit->setText(password);
	}

	this->show();
}

void LoginWindow::ReturnLogin()
{
	if (mainWin)
	{
		delete mainWin;
		mainWin = NULL;
	}
	//this->show();
	QApplication::exit(773);
}

bool LoginWindow::IsAutoLogin()
{
	return (bool)m_iRemeber;
}

// 拖动标题做的处理
bool LoginWindow::eventFilter(QObject *target, QEvent *event)
{
	if (target == ui.homepage_pushBtn)
	{
		static int i = 0;
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
	return false;
}

void LoginWindow::CreateTray()
{
	QAction* returnAction = new QAction(tr("切换账号"), this);
	connect(returnAction, SIGNAL(triggered()), this, SLOT(ReturnAccount()));
	QAction* closeAction = new QAction(tr("关闭软件"), this);
	connect(closeAction, SIGNAL(triggered()), this, SLOT(CloseWindow()));


	menu = new QMenu;
	menu->addAction(returnAction);
	menu->addAction(closeAction);

	QIcon icon = QIcon("./images/favicon.png");
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(menu);
	trayIcon->setIcon(icon);
	trayIcon->setToolTip("答疑时间");
	trayIcon->show();

	// 添加单 / 双击鼠标相应
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(trayiconActivated(QSystemTrayIcon::ActivationReason)));
}

void LoginWindow::trayiconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Unknown:
		{
			int i = 0;
			i++;
		}
		break;
	case QSystemTrayIcon::Context:
		{
		}
		break;
	case QSystemTrayIcon::DoubleClick:
		{
			if (mainWin)
				mainWin->ShowCourse();
		}
		break;
	case QSystemTrayIcon::Trigger:
		{
		}
		break;
	case QSystemTrayIcon::MiddleClick:
		{
			int i = 0;
			i++;
		}
		break;
	default:
		break;
	}
}

void LoginWindow::CloseTray()
{
	if (trayIcon)
	{
		delete trayIcon;
		trayIcon = NULL;
	}
}

void LoginWindow::ShowMain()
{
}

void LoginWindow::CloseWindow()
{
	if (mainWin->IsMayClose())
	{
		mainWin->CloseDialog();
	}
}

void LoginWindow::ReturnAccount()
{
	if (mainWin->IsMayClose())
	{
		ReturnLogin();
	}
}

QString LoginWindow::parse(QString str)
{
	QString encode;
	int iCount = str.count();
	for (int i = 0; i < iCount;i++ )
	{
		QString s = str.at(i);
		if (s == "~")
			s = "%7E";
		else if(s == "`")
			s = "%60";
		else if (s == "!")
			s = "%21";
		else if (s == "@")
			s = "%40";
		else if (s == "#")
			s = "%23";
		else if (s == "$")
			s = "%24";
		else if (s == "%")
			s = "%25";
		else if (s == "^")
			s = "%5E";
		else if (s == "&")
			s = "%26";
		else if (s == "*")
			s = "%2A";
		else if (s == "(")
			s = "%28";
		else if (s == ")")
			s = "%29";
		else if (s == "_")
			s = "%5F";
		else if (s == "+")
			s = "%2B";
		else if (s == "=")
			s = "%3D";
		else if (s == "\\")
			s = "%5C";
		else if (s == "|")
			s = "%7C";
		else if (s == "[")
			s = "%5B";
		else if (s == "]")
			s = "%5D";
		else if (s == ":")
			s = "%3A";
		else if (s == ";")
			s = "%3B";
		else if (s == "'")
			s = "%27";
		else if (s == "\"")
			s = "%22";
		else if (s == ",")
			s = "%2C";
		else if (s == "<")
			s = "%3C";
		else if (s == ".")
			s = "%2E";
		else if (s == ">")
			s = "%3E";
		else if (s == "/")
			s = "%2F";
		else if (s == "?")
			s = "%3F";

		encode += s;
	}
	return encode;
}

void LoginWindow::SetEnvironmental()
{
	QFile file("./EnvironmentalTest.txt");
	if (file.exists())
		m_EnvironmentalFormally = false;
	else
		m_EnvironmentalFormally = true;
}

void LoginWindow::Logining(bool bLogining)
{
	if (bLogining)
	{
		ui.login_pushBtn->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(190, 190, 190); ");
		ui.login_pushBtn->setText("正在登录中...");
		ui.login_pushBtn->setEnabled(false);
		ui.UserName_Edit->setEnabled(false);
		ui.UserPass_Edit->setEnabled(false);
	}
	else
	{
		ui.login_pushBtn->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(87, 207, 248); ");
		ui.login_pushBtn->setText("登录");
		ui.login_pushBtn->setEnabled(true);
		ui.UserName_Edit->setEnabled(true);
		ui.UserPass_Edit->setEnabled(true);
	}
}