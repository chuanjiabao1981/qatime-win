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
#include "UIMenuTool.h"
#include "UICamera.h"
#include <QMutex>
#include "Rangecapture.h"
#include "UISetparam.h"
#include "UIHoverwindow.h"
#include "UISeelivewnd.h"
#include "UIScreentip.h"
#include "UIPersonNum.h"
#pragma execution_character_set("utf-8")
#define STARTLS_ASYNC

class LoginWindow;
class UIAuxiliaryPanel;
class UIAudioChange;
class UIVideoChange;
class UIRatio;
class UILessonTable;
class UIVideo;
class UICamera;
class RangeCapture;
class UISetParam;
class UIHoverWindow;
class UISeeLiveWnd;
class UIScreenTip;
class UIPersonNum;

struct StructVideo
{
	std::string url;
	HWND hwnd;
};
struct MEDIA_DEVICE_DRIVE_INFO1
{
	char* device_path_;
	char* friendly_name_;
};

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
	UICamera*						m_CameraInfo;		// 摄像头窗口
	UISetParam*						m_SetParam;			// 设置参数窗口
	UIHoverWindow*					m_HoverWnd;			// 悬浮窗口
	UISeeLiveWnd*					m_SeeLiveWnd;		// 直播窗口放大
	UIScreenTip*					m_ScreenTip;		// 全屏提示框
	UIPersonNum*					m_PersonNum;		// 观看人数

	QString							m_teacherID;		// 老师ID
	QTimer*							m_CountTimer;		// 计时器
	QTimer*							m_HeartTimer;		// 心跳
	QTimer*							m_ShowVideoTimer;	// 显示视频
	QTimer*							m_ShowCameraTimer;	// 显示摄像头
	INT64							m_iTimerCount;		// 计时器秒数
	bool							m_bOtherApp;		// 播放其他应用

	UIAudioChange*					m_AudioChangeInfo;	// 麦克风选择窗口
	UIVideoChange*					m_VideoChangeInfo;	// 摄像头选择窗口
	UIRatio*						m_RatioChangeInfo;	// 分辨率窗口
	HWND							m_VideoWnd;			// 视频窗口句柄
	HWND							m_CameraWnd;		// 摄像头窗口句柄
	QString							m_liveToken;		// 直播心跳时需要（开始直播时返回token）

	UIMenuTool*						m_MenuTool;			// 工具按钮
	QTimer*							m_tempTimers;		// 临时的定时器，用完记得关掉
	QTimer*							m_ScreenTipTimer;	// 启动全屏提示
	RangeCapture*					m_RangeCapture;		// 区域抓取窗口
	int a;
	int chat_X;	//聊天位置x
	int chat_Y; //聊天位置y
	int video_Width; //播放区大小
	int video_Heigth;//播放区大小
	int chat_Width;//聊天区大小宽度
	int chat_Heigth;//聊天区大小 高度
	QMutex	m_mutex;
	bool  showMax;

	int								m_SwichScreenTimerId;	// 切屏定时器
	int                             m_ShowChatRoomTimerId;	// 显示会话窗口定时器
	QRect							m_videoRect;
	QRect							m_cameraRect;
	QWidget*						m_VideoOrCamera;		// 初始对象白板
	QWidget*						m_CameraOrVideo;		// 初始对象摄像头

	// 鼠标按下拖动的操作
	QPoint							m_startPos;
	QPoint							m_clickPos;
	QPoint							m_WndCurPos;
private slots :
	void MinDialog();									// 最小化对话框
	void CloseDialog();									// 关闭对话框
	void slot_startOrStopLiveStream();					// 开始直播
	void VideoStatus(int iStatus);						// 视频状态（直播中暂停、继续的控制）
	void AudioStatus(int iStatus);						// 声音状态（直播中暂停、继续的控制）
	void SwitchScreenStatus(int iStatus);					// 切换全屏视频源
	void slot_onCountTimeout();							// 计时器 改变直播时间
	void slot_onHeartTimeout();							// 5分一次，发送心跳
	void slot_onTempTimeout();							// 临时应用
	void slot_ScreenTipTimeout();
	void SetParamWindow();								// 设置参数窗口
	void clickLessonList();								// 弹出课程表
	void setVideoPos();									// 设置视频位置
	void setCameraPos();								// 设置视频头位置
	void HideSideScreen();								// 隐藏摄像头屏幕
	void WhiteBoard();									// 切换白板
	void returnClick();									// 切换账号

private:
	void setNetworkPic(const QString &szUrl);			// 显示网络图片

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // 添加caption
	virtual void resizeEvent(QResizeEvent *);			// 设置窗口圆角
	virtual void paintEvent(QPaintEvent *event);
	virtual void closeEvent(QCloseEvent *e);
	virtual void focusInEvent(QFocusEvent *e);
	virtual void timerEvent(QTimerEvent *event);
	virtual bool eventFilter(QObject *target, QEvent *event);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);

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
	void setAudioChangeIndex(QString path);					//  改变麦克风
	void setVideoChangeIndex(QString path);					//  改变视频头
	void setRatioChangeIndex(int index);					//  改变分辨率


	void HideOtherUI( QWidget* self=NULL);					//  隐藏其他界面
	void setLoginWindow(LoginWindow* parent);
	void setCurChatRoom(QString chatID, QString courseid);	// 进入聊天室
	void RequestKey();										// 申请云信key
	void returnKey();										// 返回key
	void RequestMember();									// 请求群成员
	void returnMember();									// 返回群成员
	void setVideoLesson(QString lessonName);				// 设置视频上显示的课程
	void showChatRoomWnd();									// 显示聊天会话
	void LessonTable_Auxiliary(QString sLessonID, QString sCourseID); //程表中选择课程——关联到辅导班
	void SendVideoMsg(UINT iMsg);							// 往win_video发送消息
	void SendCameraMsg(UINT iMsg);							// 往camera_video发送消息
	void FinishStartLive();									// 返回开始直播请求的token
	void FinishStopLive();									// 返回结束直播请求的状态
	void RequestError(QJsonObject& error, bool bTrue=true);	// 请求返回错误提示
	void SendRequestStatus();
	void RequestStatus();
	void SendChangeStatusMsg(QString id);
	void returnChangeStatus();	
	void setPausedBtn();									// 改变直播按钮图片
	void LivePage();										// 切换到直播页
	void setComeBack();										// 从放大的窗口返回到小窗口
};

#endif // UIMAINWINDOW_H
