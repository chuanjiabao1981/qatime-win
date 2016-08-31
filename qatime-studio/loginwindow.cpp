#include "loginwindow.h"
#include "uimainwindow.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "user.h"
#include "windows.h"
#include "ShellApi.h"
#include <QHBoxLayout>

TCHAR m_pathHomePage[MAX_PATH] = {0};
TCHAR m_pathRegisterAccount[MAX_PATH] = { 0 };
TCHAR m_pathFindPassword[MAX_PATH] = { 0 };

LoginWindow::LoginWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.login_pushBtn, SIGNAL(clicked()), this, SLOT(OnLogIn()));
	connect(ui.homepage_pushBtn, SIGNAL(clicked()), this, SLOT(BrowseHomePage()));
	connect(ui.register_pushBtn, SIGNAL(clicked()), this, SLOT(RegisterAccount()));
	connect(ui.findPassword_pushBtn, SIGNAL(clicked()), this, SLOT(FindPassword()));
	connect(ui.min_pushBtn, SIGNAL(clicked()), this, SLOT(MinDialog()));
	connect(ui.close_pushBtn, SIGNAL(clicked()), this, SLOT(CloseDialog()));

	ui.UserName_Edit->setTextMargins(30, 3, 20, 3);
	ui.UserPass_Edit->setTextMargins(30, 3, 20, 3);

	ReadSetting();
}

LoginWindow::~LoginWindow()
{

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
	qInfo(append);
	append += ui.UserPass_Edit->text();
	QNetworkRequest request(url);
	reply = manager.post(request, append);
	connect(reply, &QNetworkReply::finished, this, &LoginWindow::loginFinished);
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
		UIMainWindow* mainWin = new UIMainWindow();
		mainWin->setWindowFlags(Qt::FramelessWindowHint);
		mainWin->setTeacherInfo(data["user"].toObject());
		mainWin->setRemeberToken(data["remember_token"].toString());
		mainWin->show();
		this->destroy();
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
	ShellExecute(NULL, L"open", m_pathRegisterAccount, NULL, NULL, SW_SHOW);
}

void LoginWindow::FindPassword()
{
	ShellExecute(NULL, L"open", m_pathFindPassword, NULL, NULL, SW_SHOW);
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

	GetPrivateProfileString(L"CONFIG_PATH", L"HOMEPAGE", L"", m_pathHomePage, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"REGISTERACCOUNT", L"", m_pathRegisterAccount, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"FINDPASSWORD", L"", m_pathFindPassword, MAX_PATH, szTempPath);
}
