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

	UIAuxiliary*			m_AuxiliaryInfo;	// 辅导班信息窗口
	UILesson*				m_LessonInfo;		// 课程信息窗口
	QString					m_teacherID;		// 老师ID

private slots :
	void MinDialog();							// 最小化对话框
	void CloseDialog();							// 关闭对话框
	void ShowAuxiliary();						// 显示辅导班
	void ShowLesson();							// 显示课程
	void setNetworkPic(const QString &szUrl);	// 显示网络图片
	void Expansion();							// 收缩面板

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
