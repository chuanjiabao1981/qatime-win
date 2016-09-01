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
	void OnLogIn();				// ������½
	void BrowseHomePage();		// ������ҳ
	void RegisterAccount();		// ע���û�
	void FindPassword();		// �����һ�����
	void MinDialog();			// ��С���Ի���
	void CloseDialog();			// �رնԻ���

private:
	void loginFinished();		// ��½��ɵķ����¼�

protected:
	void ReadSetting();			// ��ȡ�����ļ�·����Ϣ

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
