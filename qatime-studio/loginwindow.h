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
#include "UIMainwindow.h"
#include <QSystemTrayIcon>
#include "UITraymenu.h"

#pragma execution_character_set("utf-8")

class UIMainWindow;
class UITrayMenu;

class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = 0);
	~LoginWindow();

	// 鼠标按下拖动的操作
	QPoint							m_startPos;
	QPoint							m_clickPos;
	QPoint							m_WndCurPos;

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	virtual bool eventFilter(QObject *target, QEvent *event);

private slots :
	void OnLogIn();				// 立即登陆
	void BrowseHomePage();		// 访问主页
	void RegisterAccount();		// 注册用户
	void FindPassword();		// 快速找回密码
	void MinDialog();			// 最小化对话框
	void CloseDialog();			// 关闭对话框
	void changedRemeber(int i);	// 记住账号
	void trayiconActivated(QSystemTrayIcon::ActivationReason);
	void ShowMain();
	void CloseWindow();
	void ReturnAccount();

private:
	void loginFinished();		// 登陆完成的返回事件

protected:
	void RemeberPassword();
	void InitUserName();
	void AutoLogin();			// 自动登录（记住密码功能）

private:
	CButton* m_MinBtn;
	CButton* m_CloseBtn;

private:
	Ui::LoginWindowClass ui;
	QPoint last;
	QUrl url;
	QNetworkAccessManager manager;
	QNetworkReply *reply;
	UIMainWindow* mainWin;
	UITrayMenu*	  m_TrayMenu;

	// 自动登录时需要
	QString m_teacherID;
	QString m_teacherName;
	QString m_teacherToken;
	QString m_teacherUrl;
	QString m_accid;
	QString m_accidToken;
	QString m_version;
	QSystemTrayIcon *trayIcon;
	QMenu * menu;

public:
	void ReturnLogin();			// 重新登陆
	void ReadSetting();			// 读取配置文件路径信息
	bool IsAutoLogin();			// 是否自动登录
	void Checking();			// 验证当前token是否可用
	void CheckingFinished();
	void CreateTray();			// 创建托盘
	void CloseTray();
	QString parse(QString str);	// 解析密码
};

#endif // LOGINWINDOW_H
