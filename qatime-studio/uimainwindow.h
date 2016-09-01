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
#include <QMouseEvent>
#include "nlss_type.h"
#include "nlss_api.h"
#include <QTimer>
#include "UIViedeo.h"

#pragma execution_character_set("utf-8")
#define STARTLS_ASYNC

class UIMainWindow : public QWidget
{
	Q_OBJECT

public:
	UIMainWindow(QWidget *parent = 0);
	~UIMainWindow();

private:
	Ui::UIMainWindow				ui;
	QString							mRemeberToken;
	QNetworkAccessManager			manager;
	QNetworkReply*					reply;
	QPoint							last;

	UIAuxiliary*					m_AuxiliaryInfo;	// 辅导班信息窗口
	UILesson*						m_LessonInfo;		// 课程信息窗口
	UIViedeo*						m_VideoInfo;		// 直播窗口
	QString							m_teacherID;		// 老师ID

	_HNLSSERVICE					m_hNlssService;

private slots :
	void MinDialog();							// 最小化对话框
	void CloseDialog();							// 关闭对话框
	void ShowAuxiliary();						// 显示辅导班
	void ShowLesson();							// 显示课程
	void setNetworkPic(const QString &szUrl);	// 显示网络图片
	void Expansion();							// 收缩面板
	void slot_startOrStopLiveStream();			// 直播

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

public:
	void setTeacherInfo(QJsonObject& data);					// 设置老师信息
	void setRemeberToken(const QString &name);				// 设置Token
	void AuxiliaryRequestFinished();						// 辅导班http请求
	void LessonRequestFinished();							// 课程http请求
	void SetNlsService(_HNLSSERVICE hNlssService);
};

#endif // UIMAINWINDOW_H
