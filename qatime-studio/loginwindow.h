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

#pragma execution_character_set("utf-8")

class UIMainWindow;

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
	void OnLogIn();				// ������½
	void BrowseHomePage();		// ������ҳ
	void RegisterAccount();		// ע���û�
	void FindPassword();		// �����һ�����
	void MinDialog();			// ��С���Ի���
	void CloseDialog();			// �رնԻ���
	void changedRemeber(int i);	// ��ס�˺�
private:
	void loginFinished();		// ��½��ɵķ����¼�

protected:
	void RemeberPassword();
	void InitUserName();
	void AutoLogin();			// �Զ���¼����ס���빦�ܣ�

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

	// �Զ���¼ʱ��Ҫ
	QString m_teacherID;
	QString m_teacherName;
	QString m_teacherToken;
	QString m_teacherUrl;
	QString m_accid;
	QString m_accidToken;

public:
	void ReturnLogin();			// ���µ�½
	void ReadSetting();			// ��ȡ�����ļ�·����Ϣ
	bool IsAutoLogin();			// �Ƿ��Զ���¼
	void Checking();			// ��֤��ǰtoken�Ƿ����
	void CheckingFinished();
};

#endif // LOGINWINDOW_H
