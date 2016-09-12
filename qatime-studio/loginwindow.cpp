#include "loginwindow.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "user.h"
#include "windows.h"
#include "ShellApi.h"
#include <QHBoxLayout>

//#define _DEBUG
TCHAR m_pathHomePage[MAX_PATH] = {0};
TCHAR m_pathUserName[MAX_PATH] = { 0 };
int	  m_nRemember = 0;

LoginWindow::LoginWindow(QWidget *parent)
	: QMainWindow(parent)
	, mainWin(NULL)
{
	ui.setupUi(this);

	connect(ui.login_pushBtn, SIGNAL(clicked()), this, SLOT(OnLogIn()));
	connect(ui.homepage_pushBtn, SIGNAL(clicked()), this, SLOT(BrowseHomePage()));
	connect(ui.register_pushBtn, SIGNAL(clicked()), this, SLOT(RegisterAccount()));
	connect(ui.findPassword_pushBtn, SIGNAL(clicked()), this, SLOT(FindPassword()));
	connect(ui.min_pushBtn, SIGNAL(clicked()), this, SLOT(MinDialog()));
	connect(ui.close_pushBtn, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.remember_checkBox, SIGNAL(stateChanged(int)), this, SLOT(RememberChanged(int)));

	ui.UserName_Edit->setTextMargins(30, 3, 20, 3);
	ui.UserPass_Edit->setTextMargins(30, 3, 20, 3);

	ReadSetting();

	setTabOrder(ui.UserName_Edit, ui.UserPass_Edit);
	setTabOrder(ui.UserPass_Edit, ui.login_pushBtn);
}

LoginWindow::~LoginWindow()
{
	if (mainWin)
	{
		delete mainWin;
		mainWin = NULL;
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

// 开始登陆
void LoginWindow::OnLogIn()
{
	if (ui.UserName_Edit->text().isEmpty())
	{
		ui.ErrorTip_Label->setText(QString("请输入用户名"));
		ui.UserName_Edit->setText(QString(""));
		ui.UserPass_Edit->setText(QString(""));
		return;
	}
	else if (ui.UserPass_Edit->text().isEmpty())
	{
		ui.ErrorTip_Label->setText(QString("请输入密码"));
		ui.UserName_Edit->setText(QString(""));
		ui.UserPass_Edit->setText(QString(""));
		return;
	}

#ifdef _DEBUG
	url = QUrl("http://testing.qatime.cn/api/v1/sessions");
#else
	url = QUrl("http://qatime.cn/api/v1/sessions");
#endif
	
	QByteArray append("client_type=pc");
	append.append("&email=");
	append += ui.UserName_Edit->text();
	append.append("&password=");
//	append.append("&client_cate=teacher_live");
	qInfo(append);
	append += ui.UserPass_Edit->text();
	QNetworkRequest request(url);
	reply = manager.post(request, append);
	connect(reply, &QNetworkReply::finished, this, &LoginWindow::loginFinished);

	RememberPassword();
}

// 返回登陆结果
void LoginWindow::loginFinished()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	if (obj["status"].toInt() == 1 && data.contains("remember_token"))
	{
		mainWin = new UIMainWindow();
		mainWin->setWindowFlags(Qt::FramelessWindowHint);
		mainWin->setAttribute(Qt::WA_DeleteOnClose, false);
		mainWin->setTeacherInfo(data["user"].toObject());
		mainWin->setRemeberToken(data["remember_token"].toString());
		mainWin->ShowAuxiliary();
		mainWin->setFixedSize(1024,720);
		mainWin->show();
		this->hide();
	}
	else
	{
		QString str = QString("用户名或密码不正确");
		ui.ErrorTip_Label->setText(str);
		ui.UserName_Edit->setText(QString(""));
		ui.UserPass_Edit->setText(QString(""));
	}
}

void LoginWindow::BrowseHomePage()
{
	ShellExecute(NULL, L"open", m_pathHomePage, NULL, NULL, SW_SHOW);
}

void LoginWindow::RegisterAccount()
{
	lstrcat(m_pathHomePage, L"//teachers//new");
	ShellExecute(NULL, L"open", m_pathHomePage, NULL, NULL, SW_SHOW);
}

void LoginWindow::FindPassword()
{
	lstrcat(m_pathHomePage, L"//passwords//new");
	ShellExecute(NULL, L"open", m_pathHomePage, NULL, NULL, SW_SHOW);
}

void LoginWindow::MinDialog()
{
	showMinimized();
}

void LoginWindow::CloseDialog()
{
	close();
}

void LoginWindow::ReadSetting()
{
	TCHAR szTempPath[MAX_PATH] = { 0 };

	GetCurrentDirectory(MAX_PATH, szTempPath);
	lstrcat(szTempPath, L"\\config.ini");

	GetPrivateProfileString(L"CONFIG_PATH", L"HOMEPAGE", L"", m_pathHomePage, MAX_PATH, szTempPath);				//访问主页路径
	GetPrivateProfileString(L"CONFIG_PATH", L"USERNAME", L"", m_pathUserName, MAX_PATH, szTempPath);
	m_nRemember = GetPrivateProfileInt(L"CONFIG_PATH", L"REMEMBER", 0, szTempPath);

	if (m_nRemember == 1)
	{
		ui.remember_checkBox->setCheckState(Qt::Checked);

		QString name = QString::fromStdWString(m_pathUserName);
		ui.UserName_Edit->setText(name);
	}
}

void LoginWindow::RememberPassword()
{
	if (ui.remember_checkBox->isChecked())
	{
		QString strUserName = ui.UserName_Edit->text();
		
		TCHAR szTempPath[MAX_PATH] = { 0 };
		GetCurrentDirectory(MAX_PATH, szTempPath);
		lstrcat(szTempPath, L"\\config.ini");

		WritePrivateProfileString(L"CONFIG_PATH", L"USERNAME", (LPCTSTR)strUserName.utf16(), szTempPath);
	}
}

void LoginWindow::RememberChanged(int i)
{
	TCHAR szTempPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, szTempPath);
	lstrcat(szTempPath, L"\\config.ini");

	if (i==0)
		WritePrivateProfileString(L"CONFIG_PATH", L"REMEMBER", L"0", szTempPath);
	else
		WritePrivateProfileString(L"CONFIG_PATH", L"REMEMBER", L"1", szTempPath);
}