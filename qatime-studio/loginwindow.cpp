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

#ifdef TEST
	#define _DEBUG
#else
#endif
TCHAR m_pathHomePage[MAX_PATH] = {0};
TCHAR m_pathUserName[MAX_PATH] = { 0 };
TCHAR m_pathTeacherName[MAX_PATH] = { 0 };
TCHAR m_pathTeacherID[MAX_PATH] = { 0 };
TCHAR m_pathTeacherToken[MAX_PATH] = { 0 };
TCHAR m_pathTeacherUrl[MAX_PATH] = { 0 };
TCHAR m_pathAccid[MAX_PATH] = { 0 };
TCHAR m_pathAccidToken[MAX_PATH] = { 0 };
int	  m_iRemeber = 0;

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
	connect(ui.remember_checkBox, SIGNAL(stateChanged(int)), this, SLOT(changedRemeber(int)));

	ui.UserName_Edit->setTextMargins(30, 3, 20, 3);
	ui.UserPass_Edit->setTextMargins(30, 3, 20, 3);

	ReadSetting();
	InitUserName();

	QShortcut *key = new QShortcut(QKeySequence(Qt::Key_Return | Qt::Key_Enter), this);//创建一个快捷键"Key_Return"键
	connect(key, SIGNAL(activated()), this, SLOT(OnLogIn()));//连接到指定槽函数
	setTabOrder(ui.UserName_Edit, ui.UserPass_Edit);
	setTabOrder(ui.UserPass_Edit, ui.login_pushBtn);

	QPixmap pixmap(QCoreApplication::applicationDirPath() + "/images/btn_07.png");
	QPixmap pixmap1(QCoreApplication::applicationDirPath() + "/images/btn_off.png");
	ui.min_pushBtn->setPixmap(pixmap, 4);
	ui.close_pushBtn->setPixmap(pixmap1, 4);

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
	append.append("&login_account=");
	append += ui.UserName_Edit->text();
	append.append("&password=");
	qInfo(append);
	append += ui.UserPass_Edit->text();
	append.append("&client_cate=teacher_live");
	QNetworkRequest request(url);
	reply = manager.post(request, append);
	connect(reply, &QNetworkReply::finished, this, &LoginWindow::loginFinished);
}

// 返回登陆结果
void LoginWindow::loginFinished()
{
	QString strError;
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
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

		mainWin = new UIMainWindow();
		mainWin->setWindowFlags(Qt::FramelessWindowHint);
		mainWin->setAttribute(Qt::WA_DeleteOnClose, false);
		mainWin->setRemeberToken(m_teacherToken);
		mainWin->setTeacherInfo(data["user"].toObject());
		mainWin->ShowAuxiliary();
		mainWin->setLoginWindow(this);		
		mainWin->show();
		SetWindowPos((HWND)mainWin->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		this->hide();
	}
	else if (error["code"].toInt() == 1001)
		strError = QString("用户未登录");
	else if (error["code"].toInt() == 1002)
		strError = QString("授权过期");
	else if (error["code"].toInt() == 1003)
		strError = QString("没有权限访问");
	else if (error["code"].toInt() == 1004)
		strError = QString("授权失败");
	else if (error["code"].toInt() == 2001)
		strError = QString("客户端版本过低");
	else if (error["code"].toInt() == 2002)
		strError = QString("不支持的客户端");
	else if (error["code"].toInt() == 3001)
		strError = QString("参数错误");
	else if (error["code"].toInt() == 3002)
		strError = QString("数据不合法");
	else if (error["code"].toInt() == 4001)
		strError = QString("找不到资源");
	else if (error["code"].toInt() == 9999)
		strError = QString("服务器错误");
	else
		strError = QString("用户名或密码不正确");

	ui.ErrorTip_Label->setText(strError);
	ui.UserPass_Edit->setText(QString(""));

	RemeberPassword();
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
	GetPrivateProfileString(L"CONFIG_PATH", L"USERNAME", L"",    m_pathUserName, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"TEACHERNAME", L"", m_pathTeacherName, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"TEACHERID", L"", m_pathTeacherID, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"TOKEN", L"", m_pathTeacherToken, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"TEACHERURL", L"", m_pathTeacherUrl, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"ACCID", L"", m_pathAccid, MAX_PATH, szTempPath);
	GetPrivateProfileString(L"CONFIG_PATH", L"ACCIDTOKEN", L"", m_pathAccidToken, MAX_PATH, szTempPath);
	m_iRemeber = GetPrivateProfileInt(L"CONFIG_PATH", L"REMEBER", 0, szTempPath);

	m_teacherName = QString::fromStdWString(m_pathTeacherName);
	m_teacherID = QString::fromStdWString(m_pathTeacherID);
	m_teacherToken = QString::fromStdWString(m_pathTeacherToken);
	m_teacherUrl = QString::fromStdWString(m_pathTeacherUrl);
	m_accid = QString::fromStdWString(m_pathAccid);
	m_accidToken = QString::fromStdWString(m_pathAccidToken);
}

void LoginWindow::changedRemeber(int i)
{
	TCHAR szTempPath[MAX_PATH] = { 0 };

	GetCurrentDirectory(MAX_PATH, szTempPath);
	lstrcat(szTempPath, L"\\config.ini");

	if (i == 0)
		WritePrivateProfileString(L"CONFIG_PATH", L"REMEBER", L"0",szTempPath);
	else
		WritePrivateProfileString(L"CONFIG_PATH", L"REMEBER", L"1",szTempPath);
}

void LoginWindow::RemeberPassword()
{
	TCHAR szTempPath[MAX_PATH] = { 0 };

	GetCurrentDirectory(MAX_PATH, szTempPath);
	lstrcat(szTempPath, L"\\config.ini");

	QString strName = ui.UserName_Edit->text();
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
	}
}

void LoginWindow::InitUserName()
{
	if (m_iRemeber == 1)
	{
		ui.UserName_Edit->setText(QString::fromStdWString(m_pathUserName));
		ui.remember_checkBox->setCheckState(Qt::Checked);

		Checking();
	}
	else
		this->show();
}

void LoginWindow::ReturnLogin()
{
	if (mainWin)
	{
		delete mainWin;
		mainWin = NULL;
	}
	this->show();
}

void LoginWindow::AutoLogin()
{
	mainWin = new UIMainWindow();
	mainWin->setWindowFlags(Qt::FramelessWindowHint);
	mainWin->setAttribute(Qt::WA_DeleteOnClose, false);
	mainWin->setRemeberToken(m_teacherToken);
	mainWin->setAutoTeacherInfo(m_teacherID, m_teacherName,m_teacherUrl,m_accid,m_accidToken);
	mainWin->ShowAuxiliary();
	mainWin->setLoginWindow(this);
	mainWin->show();
//	SetWindowPos((HWND)mainWin->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
}

bool LoginWindow::IsAutoLogin()
{
	return (bool)m_iRemeber;
}

void LoginWindow::Checking()
{
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/teachers/{teacher_id}/courses";
	strUrl.replace("{teacher_id}", m_teacherID);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/teachers/{teacher_id}/courses";
	strUrl.replace("{teacher_id}", m_teacherID);
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = m_teacherToken;

	request.setRawHeader("Remember-Token", m_teacherToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &LoginWindow::CheckingFinished);
}

void LoginWindow::CheckingFinished()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	if (obj["status"].toInt() == 0)
	{
		QString sError;
		QJsonObject error = obj["error"].toObject();
		if (error["code"].toInt() == 1002)
			sError = QString("授权已过期，请重新输入密码");

		ui.ErrorTip_Label->setText(sError);
		this->show();
	}
	else
	{
		AutoLogin();
	}
}