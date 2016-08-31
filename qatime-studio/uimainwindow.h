#ifndef UIMAINWINDOW_H
#define UIMAINWINDOW_H

#include <QWidget>
#include "ui_uimainwindow.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "user.h"
#include "UIAuxiliary.h"
#include "UILesson.h"

#pragma execution_character_set("utf-8")

class UIMainWindow : public QWidget
{
	Q_OBJECT

public:
	UIMainWindow(QWidget *parent = 0);
	~UIMainWindow();

private:
	Ui::UIMainWindow		ui;
	QString					mRemeberToken;
	QNetworkAccessManager	manager;
	QNetworkReply			*reply;
	QPoint					last;

	UIAuxiliary*			m_AuxiliaryInfo;	// ��������Ϣ����
	UILesson*				m_LessonInfo;		// �γ���Ϣ����
	QString					m_teacherID;		// ��ʦID

private slots :
	void MinDialog();							// ��С���Ի���
	void CloseDialog();							// �رնԻ���
	void ShowAuxiliary();						// ��ʾ������
	void ShowLesson();							// ��ʾ�γ�
	void setNetworkPic(const QString &szUrl);	// ��ʾ����ͼƬ
	void Expansion();							// �������

public:
	void setTeacherInfo(QJsonObject& data);
	void setRemeberToken(const QString &name);
	void AuxiliaryRequestFinished();
	void LessonRequestFinished();

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
};

#endif // UIMAINWINDOW_H
