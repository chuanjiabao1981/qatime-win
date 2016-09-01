#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QUrl>
#include "ui_loginwindow.h"
#include "UIButton.h"
#include <string.h>
#include <wtypes.h>

#pragma execution_character_set("utf-8")

class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = 0);
	~LoginWindow();

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private slots :
	void OnLogIn();				// 立即登陆
	void BrowseHomePage();		// 访问主页
	void RegisterAccount();		// 注册用户
	void FindPassword();		// 快速找回密码
	void MinDialog();			// 最小化对话框
	void CloseDialog();			// 关闭对话框

private:
	void loginFinished();		// 登陆完成的返回事件

protected:
	void ReadSetting();			// 读取配置文件路径信息

private:
	CButton* m_MinBtn;
	CButton* m_CloseBtn;

private:
	Ui::LoginWindowClass ui;
	QPoint last;
	QUrl url;
	QNetworkAccessManager manager;
	QNetworkReply *reply;
};

#endif // LOGINWINDOW_H
