#include "UIMenu.h"
#include <QPainter>
#include <iosfwd>
#include <sstream>
#include <string.h>
#include "windows.h"

UIMenu::UIMenu(QWidget *parent)
	: QWidget(parent)
	, m_parent(NULL)
	, m_EnvironmentalTyle(true)
	, m_uiversion(NULL)
{
	ui.setupUi(this);
	ui.label->setStyleSheet("border-image: url(./images/t_photo.png);");
	connect(ui.return_pushButton, SIGNAL(clicked()), this, SLOT(clickReturn()));
	connect(ui.check_pushButton, SIGNAL(clicked()), this, SLOT(clickCheck()));
	connect(ui.about_pushButton, SIGNAL(clicked()), this, SLOT(clickAbout()));

	ui.return_pushButton->setStyleSheet("QPushButton{border-image:url(./images/return_n.png);}"
						"QPushButton:hover{border-image:url(./images/return_h.png);}");
	ui.check_pushButton->setStyleSheet("QPushButton{border-image:url(./images/check_nor.png);}"
		"QPushButton:hover{border-image:url(./images/check_hover.png);}");
	ui.about_pushButton->setStyleSheet("QPushButton{border-image:url(./images/about_nor.png);}"
		"QPushButton:hover{border-image:url(./images/about_hover.png);}");
	ui.label->setEnabled(false);

	m_uiversion = new UIAboutVersion();
	m_uiversion->setWindowFlags(Qt::FramelessWindowHint);
}

UIMenu::~UIMenu()
{
	if (m_uiversion)
	{
		m_uiversion->hide();
		delete m_uiversion;
		m_uiversion = NULL;
	}
	
}

void UIMenu::focusOutEvent(QFocusEvent* e)
{
	QPoint pt = mapFromGlobal(QCursor::pos());
	pt.setY(pt.y() + geometry().y());
	QRect rc = this->geometry();
	if (rc.contains(pt))
	{
		setFocus();
		return;
	}

	this->hide();
}

void UIMenu::SetMainParent(UIAuxiliaryWnd* pWnd)
{
	m_parent = pWnd;
}

void UIMenu::setPic(QPixmap pix)
{
	ui.pic_label->setPixmap(pix);
}

void UIMenu::setName(QString name)
{
	ui.name_label->setText(name);

	int fontSize = ui.name_label->fontMetrics().width(name);//获取之前设置的字符串的像素大小
	if (fontSize >= ui.name_label->width()) //与label自身相比较
	{
		QString  tmpStr = ui.name_label->fontMetrics().elidedText(name, Qt::ElideRight, ui.name_label->width());
		ui.name_label->setText(tmpStr);
		ui.name_label->setToolTip(name);
	}
}

void UIMenu::clickPic()
{
//	hide();
}

void UIMenu::clickReturn()
{
	if (m_parent)
		m_parent->ReturnAccount();
}

void UIMenu::paintEvent(QPaintEvent *event)
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QColor color(255, 255, 255);
	for (int i = 0; i < 4; i++)
	{
		path.setFillRule(Qt::WindingFill);

		path.addRect(0, 0, this->width(), this->height());

		painter.setPen(color);
		painter.drawPath(path);
	}
}

void UIMenu::leaveEvent(QEvent *e)
{
	hide();
}

void UIMenu::clickCheck()
{
	checkVersion();
}

void UIMenu::clickAbout()
{
	m_uiversion->show();
	SetWindowPos((HWND)m_uiversion->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void UIMenu::StartCheck(QString enforce, QString version, QString downpath)
{
	// 启动更新程序
	TCHAR szTempPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, szTempPath);
	lstrcat(szTempPath, L"\\QtUpdate.exe");
//	lstrcat(szTempPath, L"C:\\Users\\lenovo\\qatime\\qatime-studio\\qatime-studio\\Bin\\Release\\QtUpdate.exe");
	TCHAR szDownPath[_MAX_PATH] = { 0 }; //	下载路径
	TCHAR szVersion[_MAX_PATH] = { 0 };	 //	版本号
	TCHAR szUpdate[_MAX_PATH] = { 0 };	 //	更新

	lstrcat(szDownPath, (LPCWSTR)downpath.toStdWString().c_str());
	lstrcat(szVersion, (LPCWSTR)version.toStdWString().c_str());

	if (enforce == "")
		lstrcat(szUpdate, L"0");
	else
		lstrcat(szUpdate, L"1");
	
	/************************************************************************/
	/* 	启动更新进程														*/
	/*	参数说明：															*/
	/*	-downpath		下载路径											*/
	/*	-version		版本号												*/
	/*	-update			是否更新：0不更新，1更新							*/
	/************************************************************************/
	std::wstring wszCmdLine;
	std::wstringstream wstream;
	wstream << L" -downpath " << szDownPath
		<< L" -version " << szVersion
		<< L" -update " << szUpdate
		<< std::endl;
	wszCmdLine = wstream.str();

	STARTUPINFOW	si = { 0 };
	si.cb = sizeof(si);

	PROCESS_INFORMATION	pi;

	BOOL bIsSuc = CreateProcess(szTempPath, (LPWSTR)wszCmdLine.c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
}

void UIMenu::setVersion(QString version)
{
	m_version = version;
	m_uiversion->setVersion(m_version);
}

void UIMenu::checkVersion()
{
	QString strUrl;

	strUrl += m_homePage;
	strUrl += "/api/v1/system/check_update?category=teacher_live&version={version}&platform=windows";
	strUrl.replace("{version}", m_version);
	

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	request.setRawHeader("Remember-Token", m_token.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIMenu::returnVersion);
}

void UIMenu::returnVersion()
{
	QString strError;
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();

	if (obj["status"].toInt() == 1)
	{
		QString version = data["version"].toString();
		QString down_links = data["cdn_url"].toString();
		bool bEnforce = data["enforce"].toBool();
		
		if (version == "")
			StartCheck("", version,down_links);
		else 
			StartCheck("true", version, down_links);
	}
}

void UIMenu::SetEnvironmental(bool Environmental, QString homePage)
{
	m_homePage = homePage;
	m_EnvironmentalTyle = Environmental;
}

void UIMenu::setToken(QString token)
{
	m_token = token;
}