#ifndef UIMAINWINDOW_H
#define UIMAINWINDOW_H

#include <QWidget>
#include "ui_uimainwindow.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "user.h"
#include "UIVideo.h"
#include <windows.h>
#include "UIAuxiliaryPanel.h"
#include "UIAudiochange.h"
#include "UIVideochange.h"
#include "UIRatio.h"
#include "UIChatRoom.h"
#include "loginwindow.h"
#include "UILessontable.h"
#include "UICamera.h"
#include <QMutex>
#include "UISetparam.h"
#include "UIHoverwindow.h"
#include "UISeelivewnd.h"
#include "UIScreentip.h"
//#include "UIPersonNum.h"
#include "Livestatusmanager.h"
#include "UIHelpword.h"
#include "UIBulletscreen.h"
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
class UISetParam;
class UIHoverWindow;
class UISeeLiveWnd;
class UIScreenTip;
class UIPersonNum;
class UIChatRoom;
class LiveStatusManager;
class UIBulletScreen;

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
enum CameraStatus
{
	CameraStatusInit=0,		//未直播
	CameraStatusTeaching,	//直播中
	CameraStatusClose,		//已关闭
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
	UILessonTable*					m_LessonTable;		// 课程表窗口
	UIVideo*						m_VideoInfo;		// 直播窗口
	UICamera*						m_CameraInfo;		// 摄像头窗口
	UISetParam*						m_SetParam;			// 设置参数窗口
	UIHoverWindow*					m_HoverWnd;			// 悬浮窗口
	UISeeLiveWnd*					m_SeeLiveWnd;		// 直播窗口放大
	UIScreenTip*					m_ScreenTip;		// 全屏提示框
	UIPersonNum*					m_PersonNum;		// 观看人数
	LiveStatusManager*				m_LiveStatusManager;// 直播状态更新类
	UIHelpWord*						m_HelpWord;			// 帮助文档
	UIBulletScreen*					m_BulletScreen;		// 弹幕

	QString							m_teacherID;		// 老师ID
	QTimer*							m_CountTimer;		// 计时器
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

	QTimer*							m_tempTimers;		// 临时的定时器，用完记得关掉
	QTimer*							m_ScreenTipTimer;	// 启动全屏提示
	int m_icount;
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

	QPixmap							m_teacherPix;
	// 直播需要的参数
	QString							m_sBoardRtmp;			// 白板推流地址
	QString							m_sCemeraRtmp;			// 摄像头推流地址
	
	bool							bHasCamera;				// 有无摄像头
	CameraStatus					m_EnumStatus;			// 摄像头直播状态
	int								m_iSucCount;			// 成功两次才能正式直播
	int								m_HelpBtnPos;			// 帮助按钮的位置
	QTimer*							m_RefreshTimer;			// 刷新界面
private slots :
	void MinDialog();									// 最小化对话框
	void CloseDialog();									// 关闭对话框
	void slot_startOrStopLiveStream();					// 开始直播
	void VideoStatus(int iStatus);						// 视频状态（直播中暂停、继续的控制）
	void AudioStatus(int iStatus);						// 声音状态（直播中暂停、继续的控制）
	void SwitchScreenStatus(int iStatus);					// 切换全屏视频源
	void slot_onCountTimeout();							// 计时器 改变直播时间
	void slot_onTempTimeout();							// 临时应用
	void slot_ScreenTipTimeout();
	void SetParamWindow();								// 设置参数窗口
	void clickLessonList();								// 弹出课程表
	void setVideoPos();									// 设置视频位置
	void setCameraPos();								// 设置视频头位置
	void HideSideScreen();								// 隐藏摄像头屏幕
	void WhiteBoard();									// 切换白板
	void returnClick();									// 切换账号
	void HelpDialog();									// 帮助文档
	void onTimerResize();								// 改变大小后，定时器重新设置位置

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
	void SendStopLiveHttpMsg(bool bConnect=true);			// 往服务器发送直播停止消息
	void ShowAuxiliary();									// 显示辅导班
	void setAudioChangeIndex(int index);					//  改变麦克风
	void setVideoChangeIndex(int index);					//  改变视频头
	void setRatioChangeIndex(int index);					//  改变分辨率


	void HideOtherUI( QWidget* self=NULL);					//  隐藏其他界面
	void setLoginWindow(LoginWindow* parent);
	void setCurChatRoom(QString chatID, QString courseid);	// 进入聊天室
	void RequestKey();										// 申请云信key
	void returnKey();										// 返回key
	void RequestMember();									// 请求群成员
	void returnMember();									// 返回群成员
	void setVideoLesson(QString lessonName);				// 设置视频上显示的课程
	void LessonTable_Auxiliary(QString sLessonID, QString sCourseID); //程表中选择课程——关联到辅导班
	void SendVideoMsg(UINT iMsg);							// 往win_video发送消息
	void SendCameraMsg(UINT iMsg);							// 往camera_video发送消息
	void RequestError(QJsonObject& error, bool bTrue=true);	// 请求返回错误提示
	void SendRequestStatus();
	void RequestStatus();
	void SendChangeStatusMsg(QString id);
	void returnChangeStatus();	
	void setPausedBtn();									// 改变直播按钮图片
	void LivePage();										// 切换到直播页
	void setComeBack();										// 从放大的窗口返回到小窗口
	QPixmap TeacherPhotoPixmap();							// 老师头像的pixmap
	void showErrorTip(QString sError);						// 显示错误信息
	void setBoradCamera(QString sBoard, QString sCamera);	// 设置推流信息

	// RangeCapture窗口抓取交互函数
	void clickFullRadio();
	void clickRectRadio();

	void startLiveStream();									// 服务器返回teaching后，才开始正式推流
	bool IsHasCamera();										// 有无摄像头
	void setAdaptHeight(int iHeight);						// 设置自适应高度
	void setMainTitle(QString sVersion);					// 设置版本号
	void ShowMain();										// 显示主窗口
	void UpatateLiveStatus(QWidget* widget, bool bSuc);		// 直播成功
	void setLiveBtnEnable(bool bEnable);					// 设置直播按钮禁用状态
	void addVideoDevices(QString path);						// 设置视频源
	void addAudioDevices(QString path);						// 设置麦克风
	void ErrorStopLive(QWidget* pWidget);					// 直播过程出现错误，停止直播
	LoginWindow* UIMainWindow::GetLoginWnd();				// 获取登录窗口
	void SendTeacherBullet(QString name, QString content);	// 老师消息
	void SendStudentBullet(QString name, QString content);	// 学生消息
	void SetBullet(int iStatus);							// 开启弹幕设置
	void CloseBullet();		
	void setCameraEnable();									// 设置摄像头禁用
	void MenuClose();										// 托盘菜单发送过来的关闭消息
	void MenuRetrun();										// 托盘菜单发送过来的切换账号消息
	void PosInWindow();										
	void BulletDelay(int iDelay);							// 浮层延迟显示
	void setTriggerType(bool b = true);						// true为整体触发 false为按钮触发	
	void ChatRoomDown();									// 聊天置底
	void ErrorStop();										// 出现错误，停止直播
};

#endif // UIMAINWINDOW_H
