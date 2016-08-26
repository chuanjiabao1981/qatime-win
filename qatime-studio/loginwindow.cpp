#include "loginwindow.h"
#include "uimainwindow.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "user.h"
#include "windows.h"
#include "ShellApi.h"

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
	if (ui.UserName_Edit->text().isEmpty() || ui.UserPass_Edit->text().isEmpty())
	{
		ui.ErrorTip_Label->setText(QString("用户名或者密码为空"));
		ui.UserName_Edit->setText(QString(""));
		ui.UserPass_Edit->setText(QString(""));
		return;
	}

	url = QUrl("http://qatime.cn/api/v1/sessions");
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
		User *user = new User();
		user->readJson(data);
		UIMainWindow* mainWin = new UIMainWindow();
		mainWin->show();
		mainWin->setWindowFlags(Qt::FramelessWindowHint);
		this->destroy();
	}
	else
	{
		QString str = QString("用户名或者密码不匹配");
		ui.ErrorTip_Label->setText(str);
		ui.UserName_Edit->setText(QString(""));
		ui.UserPass_Edit->setText(QString(""));
	}
}

void LoginWindow::BrowseHomePage()
{
	ShellExecute(NULL, L"open", L"http://qatime.cn", NULL, NULL, SW_SHOW);
}

void LoginWindow::RegisterAccount()
{
	ShellExecute(NULL, L"open", L"http://qatime.cn/teachers/new", NULL, NULL, SW_SHOW);
}

void LoginWindow::FindPassword()
{
	ShellExecute(NULL, L"open", L"http://cn.bing.com", NULL, NULL, SW_SHOW);
}

void LoginWindow::MinDialog()
{
	showMinimized();
}

void LoginWindow::CloseDialog()
{
	close();
}