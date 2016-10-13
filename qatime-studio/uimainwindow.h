#ifndef UIMAINWINDOW_H
#define UIMAINWINDOW_H

#include <QWidget>
#include "ui_uimainwindow.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "user.h"
#include "UIOtherApp.h"
#include "UIVideo.h"
#include <windows.h>
#include "UIAuxiliaryPanel.h"
#include "UIAudiochange.h"
#include "UIVideochange.h"
#include "UIRatio.h"
#include "UIChatRoom.h"
#include "loginwindow.h"
#include "UILessontable.h"

#pragma execution_character_set("utf-8")
#define STARTLS_ASYNC

class LoginWindow;
class UIAuxiliaryPanel;
class UIAudioChange;
class UIVideoChange;
class UIRatio;
class UILessonTable;
class UIVideo;

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


	UIChatRoom*						m_charRoom;			//聊天室
	LoginWindow*					m_LoginWindow;

	UIAuxiliaryPanel*				m_AuxiliaryPanel;	// 老师课程面板
	UIOtherApp*					    m_OtherAppInfo;		// 其它应用信息窗口
	UILessonTable*					m_LessonTable;		// 课程表窗口
	UIVideo*						m_VideoInfo;		// 直播窗口
	QString							m_teacherID;		// 老师ID
	QTimer*							m_CountTimer;		// 计时器
	QTimer*							m_HeartTimer;		// 心跳
	QTimer*							m_ShowVideoTimer;	// 显示视频
	INT64							m_iTimerCount;		// 计时器秒数
	bool							m_bOtherApp;		// 播放其他应用

	UIAudioChange*					m_AudioChangeInfo;	// 麦克风选择窗口
	UIVideoChange*					m_VideoChangeInfo;	// 摄像头选择窗口
	UIRatio*						m_RatioChangeInfo;	// 分辨率窗口
	HWND							m_VideoWnd;			// 视频窗口句柄
	QString							m_liveToken;		// 直播心跳时需要（开始直播时返回token）
private slots :
	void MinDialog();									// 最小化对话框
	void MaxDialog();									// 最大化对话框
	void CloseDialog();									// 关闭对话框
	void Expansion();									// 收缩面板
	void slot_startOrStopLiveStream();					// 开始直播
	void VideoStatus(int iStatus);						// 视频状态（直播中暂停、继续的控制）
	void AudioStatus(int iStatus);						// 声音状态（直播中暂停、继续的控制）
	void FullScreenStatus(int iStatus);					// 切换全屏视频源
	void slot_onCountTimeout();							// 计时器 改变直播时间
	void slot_onHeartTimeout();							// 5分一次，发送心跳
	void OtherApp();									// 其它APP应用
	void clickChangeAudio(int);
	void clickChangeVideo(int);							// 弹出摄像头选择框
	void clickChangeRatio();							// 弹出分辨率框
	void clickLessonList();								// 弹出课程表
	void setVideoPos();									// 设置视频位置
protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // 添加caption
	virtual void resizeEvent(QResizeEvent *);			// 设置窗口圆角
	virtual void paintEvent(QPaintEvent *event);
	virtual void closeEvent(QCloseEvent *e);
	virtual void focusInEvent(QFocusEvent *e);

public:
	void setTeacherInfo(QJsonObject& data);					// 设置老师信息
	void setAutoTeacherInfo(QString teacherID, QString teacherName, 
		 QString teacherUrl, QString accid, QString token); // 记住密码时使用的方法
	void setRemeberToken(const QString &name);				// 设置Token
	void AuxiliaryRequestFinished();						// 辅导班http请求
	void LessonRequestFinished();							// 课程http请求
	void EnumAvailableMediaDevices();						// 枚举设备
	void SendStartLiveHttpMsg();							// 往服务器发送直播开始消息
	void SendStopLiveHttpMsg(bool bConnect=true);			// 往服务器发送直播停止消息
	void SendHeartBeatHttpMsg();							// 往服务器发送直播心跳消息（5分钟一次）
	void ShowAuxiliary();									// 显示辅导班
	void InitAudioList();
	void InitVideoList();
	void setAudioChangeIndex(int index);					//  改变麦克风
	void setVideoChangeIndex(int index);					//  改变视频头
	void setRatioChangeIndex(int index);					//  改变分辨率


	void HideOtherUI( QWidget* self=NULL);					//  隐藏其他界面
	void returnClick();
	void setLoginWindow(LoginWindow* parent);
	void setCurChatRoom(QString chatID, QString courseid);	// 进入聊天室
	void RequestKey();										// 申请云信key
	void returnKey();										// 返回key
	void RequestMember();									// 请求群成员
	void returnMember();									// 返回群成员
	void setVideoLesson(QString lessonName);				// 设置视频上显示的课程
	void showChatRoomWnd();									// 显示聊天会话
	void LessonTable_Auxiliary(QString sLessonID, QString sCourseID); //程表中选择课程——关联到辅导班
	void setLiveBtnEnable(bool bEnable);
	void SendVideoMsg(UINT iMsg);							// 往qatime_video发送消息
	void FinishStartLive();									// 返回开始直播请求的token
	void FinishStopLive();									// 返回结束直播请求的状态
	void RequestError(QJsonObject& error, bool bTrue=true);	// 请求返回错误提示
	void SendRequestStatus();
	void RequestStatus();
	void SendChangeStatusMsg(QString id);
	void returnChangeStatus();
};

#endif // UIMAINWINDOW_H
