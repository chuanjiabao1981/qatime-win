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
#include "UIVideo.h"
#include <windows.h>
#include "UIAuxiliaryPanel.h"

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
	UIVideo*						m_VideoInfo;		// 直播窗口
	QString							m_teacherID;		// 老师ID
	QTimer*							m_CountTimer;		// 计时器
	QTimer*							m_HeartTimer;		// 心跳
	INT64							m_iTimerCount;		// 计时器秒数

	UIAuxiliaryPanel*				m_AuxiliaryPanel;	// 老师面板

private slots :
	void MinDialog();									// 最小化对话框
	void MaxDialog();									// 最大化对话框
	void CloseDialog();									// 关闭对话框
	void Expansion();									// 收缩面板
	void slot_startOrStopLiveStream();					// 开始直播
	void VideoSourceChange(int index);					// 视频源变化
	void VideoAppChange(int index);						// APP应用发生变化
	void VideoStatus(int iStatus);						// 视频状态（直播中暂停、继续的控制）
	void AudioStatus(int iStatus);						// 声音状态（直播中暂停、继续的控制）
	void slot_onCountTimeout();							// 计时器 改变直播时间
	void slot_onHeartTimeout();							// 5分一次，发送心跳

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // 添加标题caption
	virtual void resizeEvent(QResizeEvent *);			// 设置窗口圆角
	virtual void paintEvent(QPaintEvent *event);

private:
	void SetSourceAppPath();							// 当视频源选中其他应用时，设置APP路径名称

public:
	void setTeacherInfo(QJsonObject& data);					// 设置老师信息
	void setRemeberToken(const QString &name);				// 设置Token
	void AuxiliaryRequestFinished();						// 辅导班http请求
	void LessonRequestFinished();							// 课程http请求
	void EnumAvailableMediaDevices();						// 枚举设备
	void SendStartLiveHttpMsg();							// 往服务器发送直播开始消息
	void SendStopLiveHttpMsg();								// 往服务器发送直播停止消息
	void SendHeartBeatHttpMsg();							// 往服务器发送直播心跳消息（5分钟一次）
	void ShowAuxiliary();									// 显示辅导班
};

#endif // UIMAINWINDOW_H
