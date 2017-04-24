#ifndef UIWINDOWSET_H
#define UIWINDOWSET_H

#include <QWidget>
#include "ui_UIWindowset.h"
#include "UIMainNewWindow.h"
#include "UITags.h"
#include "UIVideo.h"
#include "UICamera.h"
#include "UINoticewnd.h"
#include "UICoursewnd.h"
#include "UIPersonwnd.h"
#include "UILessonlist.h"
#include "UIAudiochange.h"
#include "UIRatio.h"
#include "UILivelessonwnd.h"
#include "Livestatusmanager.h"
#include "UIScreentip.h"
#include "UIChatRoom.h"
#include "UIBulletparam.h"
#include "UIBulletscreen.h"

//---云信
#include "nim_client_def.h"
#include "assert.h"
#include <string>
#include "YxChat/nim_tools_http_cpp_wrapper.h"
#include "YxChat/nim_client_helper.h"
#include "YxChat/nim_cpp_talk.h"
#include "YxChat/nim_cpp_team.h"
#include "YxChat/nim_cpp_msglog.h"
#include "YxChat/nim_cpp_nos.h"
#include "YxChat/nim_tools_audio_cpp_wrapper.h"
#include "YxChat/nim_sdk_helper.h"
#include "YxChat/session_callback.h"
#include "YxChat/nim_cpp_client.h"
#pragma execution_character_set("utf-8")

class UITags;
class UIMainNewWindow;
class UIVideo;
class UICamera;
class UIVideoChange;
class UIAudioChange;
class UIRatio;
class LiveStatusManager;
class UIChatRoom;
class UIBulletParam;
class UIBulletScreen;

class UIPersonWnd;
class UINoticeWnd;
class UICourseWnd;
class UILessonList;
class UIWindowSet : public QWidget
{
	Q_OBJECT

public:
	enum BTN_TYPE {
		NoticeWnd,
		CourseWnd,
		PersonWnd,
		LessonWnd
	};

	enum CameraStatus{
		CameraStatusInit = 0,	//未直播
		CameraStatusTeaching,	//直播中
		CameraStatusClose,		//已关闭
	};

	enum SCREEN_TYPE {
		SCREEN_16_9,
		SCREEN_16_10,
		SCREEN_4_3
	};

	UIWindowSet(QWidget *parent = 0);
	~UIWindowSet();

	// 鼠标按下拖动的操作
	QPoint							m_startPos;
	QPoint							m_clickPos;
	QPoint							m_WndCurPos;

	QNetworkReply*					reply;
	QNetworkAccessManager			manager;

	UIMainNewWindow*				m_parent;
	bool							m_EnvironmentalTyle;
	QString							m_Token;
	QString							m_teacherID;
	QString							m_teacherName;
	QString							m_accid;

	std::vector<UITags*>			m_vecTags;				// 窗口标签集
	std::vector<UIChatRoom*>		m_vecChatRoom;			// 所有的辅导班聊天窗
	QMap<QString, UITags*>			m_mapTags;				// 标签map
	QMap<QString, UIChatRoom*>		m_mapChatRoom;			// 聊天窗口map
	UITags*							m_curTags;				// 当前标签
	UIChatRoom*						m_curChatRoom;			// 当前聊天窗

	UINoticeWnd*					m_NoticeWnd;			// 公告窗口
	UICourseWnd*					m_CourseWnd;			// 辅导班简介
	UIPersonWnd*					m_PersonWnd;			// 成员列表
	UILessonList*					m_LessonWnd;			// 课程列表
	/********************直播全屏和摄像头*******************/
	LiveStatusManager*				m_LiveStatusManager;	// 直播状态更新类
	QString							m_lessonid;				// 当前直播的课程id
	QString							m_lessonName;			// 当前直播的课程名称
	QString							m_boardUrl;				// 当前直播的白板地址
	QString							m_cameraUrl;			// 当前直播的摄像头地址
	QTimer*							m_tempTimers;			// 推摄像头流

	UIVideo*						m_VideoInfo;			// 直播全屏窗口
	UICamera*						m_CameraInfo;			// 直播摄像头窗口
	bool							m_bHasCamera;			// 有无摄像头
	CameraStatus					m_EnumStatus;			// 摄像头直播状态
	QTimer*							m_CountTimer;			// 直播计时器
	int								m_iTimerCount;			// 秒数
	UILiveLessonWnd*				m_LiveLesson;			// 选择课程窗口

	UIBulletScreen*					m_BulletScreen;			// 弹幕窗口
	UIVideoChange*					m_VideoChangeInfo;		// 摄像头选择窗口
	UIAudioChange*					m_AudioChangeInfo;		// 麦克风选择窗口
	UIRatio*						m_RatioChangeInfo;		// 分辨率选择窗口
	UIBulletParam*					m_BulletParamInfo;		// 弹幕参数窗口
	UIScreenTip*					m_ScreenTip;			// 全屏提示框
	SCREEN_TYPE						m_ScreenType;			// 屏幕比例
private:
	Ui::UIWindowSet ui;

signals:
	void sig_Modle(bool bModle);
private slots :
	void MaxDialog();										// 最大化
	void MinDialog();										// 最小化
	void CloseDialog();										// 关闭对话框
	void clickTag(UITags*);									// 点击标签
	void clickLive();										// 点击开始直播
	void clickChange(bool checked);							// 切换
	void slots_Modle(bool bModle);							// 改变模式
	void slot_onCountTimeout();								// 直播计时器
	void clickNotice();										// 公告
	void clickCourse();										// 辅导班详情
	void clickPerson();										// 成员
	void clickLesson();										// 课程列表
	void slot_stateChange(bool, QString, QString);			// 禁言功能
	void AudioStatus(int iStatus);							// 开启关闭麦克风
	void BulletStatus(int iStatus);							// 弹幕状态
	void clickAudioParam();									// 麦克风参数
	void VideoStatus(int iStatus);							// 开启关闭摄像头
	void clickVideoParam();									// 摄像头参数
	void clickRatioParam();									// 分辨率参数
	void clickBulletParam();								// 弹幕参数
	void slot_PullStreaming(QString, QString, QString, QString, QString);	// 开始推流
	void slot_changeLessonStatus(QString, QString);					// 改变课程状态
	void DeleteTag(UITags* tag);								// 删除标签	
	void slot_onTempTimeout();								// 摄像头推流
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual bool eventFilter(QObject *target, QEvent *event);
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

public:
	/*************************直播全屏**************************/
	void	initFont();											// 初始化字体大小
	void	setMainWindow(UIMainNewWindow* parent);
	void	SetEnvironmental(bool bType);						// 环境变量;
	void	SetToken(QString token);
	void	setTeacherID(QString id, QString name);				// 老师ID和名字
	void	setAccid(QString accid);							// 云信ID
	void	AddChatRoom(QString chatID, QString courseid, QString teacherid, QString token, QString studentName, std::string strCurAudioPath,
					QString name, int UnreadCount, QString status, QString boardurl, QString cameraUrl, bool b1v1Lesson = false); // 创建聊天
	bool	IsHasTag(QString chatID, QString status);			// 此辅导班是否创建标签
	UIChatRoom* IsHasRoom(QString chatID);						// 此辅导班聊天室是否创建
	void	AddTag(QString chatID, QString name, QString ID, bool sel, UIChatRoom* room, QString status,bool b1v1Lesson, QString board, QString camera);// 添加标签窗口, 参数sel是否选中此标签
	void	titleButtonWidth();
	QPixmap TeacherPhotoPixmap();								// 老师头像	
	void    AgainSelectTag();									// 重新选择标签
	void	ChangeBtnStyle(bool bLive);							// 切换模式
	bool	IsHasFoucs(BTN_TYPE type);							// 判断点击时鼠标点是否在控件内
	void	SendStopLive();										// 退出时发送结束直播消息
	QPushButton*	GetNoticeBtn();
	QPushButton*	GetLessonBtn();
	QPushButton*	GetPersonBtn();
	QPushButton*	GetCourseBtn();
	void	QueryNotice();										// 查询公告
	void	returnNotice();										// 返回公告
	void	QueryCourse();										// 查询辅导简介
	void	returnCourse();										// 返回简介
	void	QueryLesson();										// 查询课程
	void	returnLesson();										// 返回课程
	void	QueryPerson();										// 查询成员
	void    setLiveBtnEnable(bool bEnable);						// 设置直播按钮样式状态

	/*************************云信聊天**************************/
	bool	ReceiverMsg(nim::IMMessage* pIMsg);					// 接收消息
	void	ReceiverRecordMsg(nim::QueryMsglogResult* pIMsg);	// 接收历史记录消息
	void	ReceiverChatMsg(nim::IMMessage* pIMsg);				// 接收初始化第一次请求的消息
	void	ReceiverLoginMsg(nim::LoginRes* pLogMsg);			// 返回登录结果
	void	ReceiverMemberMsg(std::string sid, std::list<nim::TeamMemberProperty>* pMemberMsg);	// 返回成员
	void	SendStatus(nim::SendMessageArc* arcNew);			// 接收消息状态
	void	OnStopPlayAudio(std::string sid, char* msgid);		// 语音播放结束消息
	void    OnSendAnnouncements(QString text);					// 发送群公告

	/*************************直播推流**************************/
	void	AddTodayToLesson(QString  id, QString courseid, QString boardUrl, QString cameraUrl, QString time, QString status, QString name);// 往选择课程中添加今日课程
	void	UpatateLiveStatus(QWidget* widget, bool bSuc);		// 更新直播状态
	void	SendRequestStatus(QString status);					// 更新当前辅导班状态
	void	startLiveStream();									// 更新开始直播按钮、计时器等状态
	void	setCameraEnable();									// 设置摄像头禁用启用
	void    ErrorStopLive(QWidget* pWidget);					// 直播过程中出错 
	void	ErrorStop();										// 错误停止
	bool	IsHasCamera();										// 是否有摄像头
	void    addVideoDevices(QString path);						// 添加视频源
	void	addAudioDevices(QString path);						// 添加音频源
	void	setAudioChangeIndex(int index);						// 选择音频源
	void    setVideoChangeIndex(int index);						// 选择视频源
	void    setRatioChangeIndex(QString ratio);					// 选择分辨率
	void	showErrorTip(QString sError);						// 显示错误
	bool    IsLiving();											// 当前是否有直播
	void	SendStudentBullet(QString name, QString content,QString chatid);	// 发送学生弹幕
	void	SendTeacherBullet(QString name, QString content, QString chatid);	// 发送老师弹幕
	void	BulletDelay(int iDelay);							// 间隔
	void	setTriggerType(bool bType);							// 触发类型
	void	CloseBullet();										// 隐藏弹幕
	void	ReturnLogin();										// 重新登录
	void	StopSuccess(QWidget* widget);						// 直播停止成功
	void    MathScreenSize();									// 计算屏幕比例
	int		mathVideoWidth(int iwidth);							// 计算video显示的宽度
	int		mathVideoHeight(int iheight);						// 计算video显示的高度
};

#endif // UIWINDOWSET_H
