#ifndef UIWINDOWSET_H
#define UIWINDOWSET_H

#include <QWidget>
#include "ui_UIWindowset.h"
#include "UIMainNewWindow.h"
#include "UITags.h"
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

//1对1
#include "palette.h"
#include "UICamera1v1.h"
#include "UICameraS1v1.h"
#include "UIVideochange1v1.h"
#include "UIAudiochange1v1.h"
#include "UIAudioOutchange1v1.h"
#include "UIWhiteBoardTool.h"
#include "UIVideo1v1.h"
#include "UIAppWnd.h"
#include "UIAppWndTool.h"
#include "UIVideochange.h"
#include "UIVideo.h"
//---云信
#include "nim_client_def.h"
#include "assert.h"
#include <string>
//IM SDK接口定义头文件
#include "nim_cpp_api.h"
#include "nim_cpp_client.h"
#include "nim_tools_http_cpp.h"

//前置声明，依次为收到批量离线或同步消息通知函数、收到在线消息通知函数、发送消息结果通知函数、踢他端通知函数、多端登录通知函数、断线通知函数、被踢通知函数、登录重连通知函数。
void CallbackReceiveMsgs(const std::list<nim::IMMessage>& msgs);
void CallbackReceiveMsg(const nim::IMMessage& msg);
void CallbackSendMsgArc(const nim::SendMessageArc& arc);
void CallbackKickOther(const nim::KickOtherRes& res);
void CallbackMultiSpotLogin(const nim::MultiSpotLoginRes& res);
void CallbackDisconnect();
void CallbackKickout(const nim::KickoutRes& res);
void CallbackLogin(const nim::LoginRes& res);

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

//互动直播
class Palette;
class UICamera1v1;
class UICameraS1v1;
class UIVideoChange1v1;
class UIAudioChange1v1;
class UIAudioOutChange1v1;
class UIWhiteBoardTool;
class UIVideo1v1;
class UIAppWnd;
class UIAppWndTool;
//class UIVideoRecord;
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
	QString							m_homePage;
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
	QString							m_BoardPushURL;			// 1v1白板推流地址
	QString							m_cameraUrl;			// 当前直播的摄像头地址
	QTimer*							m_tempTimers;			// 推摄像头流
	int								m_PullCamera;			// 摄像头推流状态三个状态

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

	/***************************互动直播*****************************/
	bool							m_bLiving1v1;			// 1对1直播标识
	Palette*						mWhiteBoard;			// 画板
	UICamera1v1*					m_Camera1v1Info;		// 1对1摄像头
	UICameraS1v1*					m_CameraS1v1Info;		// 1对1学生摄像头
	UIVideoChange1v1*				m_VideoChangeInfo1v1;	// 摄像头选择窗口
	UIAudioChange1v1*				m_AudioChangeInfo1v1;	// 麦克风选择窗口
	UIAudioOutChange1v1*			m_AudioOutChangeInfo1v1;// 扬声器选择窗口
	UIWhiteBoardTool*				m_WhiteBoardTool;		// 白板操作
	UIVideo1v1*						m_VideoInfo1v1;			// 1v1全屏桌面
	UIAppWnd*						m_AppWnd1v1;			// 选择分享窗口
	UIAppWndTool*					m_AppWndTool1v1;		// 1v1窗口分享工具条
	QTimer*							m_QueryOnlieTimers;		// 定时查询在线人数
	//UIVideoRecord*					m_VideoRecordInfo;		// 旁路直播

public:
	bool							m_bQueryMsg;		// 是否在查询消息记录
	bool							m_bEveryTime;		// 是否是即时直播，否的话是普通线上直播
private:
	Ui::UIWindowSet ui;

signals:
	void sig_Modle(bool bModle);
	void sig_AddAnnouncement(QString, QString);				// 添加云信通知信号
public slots :
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
	void slot_PullStreaming(QString, QString, QString, QString, QString,int, bool);	// 开始推流
	void slot_changeLessonStatus(QString, QString);			// 改变课程状态
	void DeleteTag(UITags* tag);							// 删除标签	
	void slot_onTempTimeout();								// 摄像头推流

	/*互动直播*/
	void joinRtsRoom(const std::string &);					// 加入白板房间
	void joinRoomSuccessfully();							// 加入白板房间成功
	void joinVChatRoom();									// 加入音视频
	void joinVChatSuccessfully(QString mChannelID);			// 加入音视频房间成功（1v1时，参数为回放的ChannelID）
	void errorInfo(const QString &);						// 加入失败错误信息
	void PicData(QString,QString);									// 白板数据
	void setDeviceInfos(int);								// 设备参数
	void clickVideo1v1Param();								// 摄像头参数
	void clickAudio1v1Param();								// 麦克风参数
	void clickAudioOut1v1Param();							// 扬声器参数
	void Audio1v1Status(int iStatus);						// 开启关闭麦克风
	void Video1v1Status(int iStatus);						// 开启关闭摄像头
	void AudioOut1v1Status(int);							// 开启关闭扬声器
	void clickLive1v1();									// 开启1v1直播
	void selectColor(QColor&);								// 颜色器
	void returnClick();										// 撤销上一步
	void deleteClick();										// 清空白板
	void laserClick();										// 激光笔
	void drawClick();										// 绘画笔
	void rtsDataReceived(const std::string& data,const std::string &uid);			// 接收白板数据
	void clickShapeScreen1v1();								// 点击分享屏幕
	void slot_selectWnd(HWND);								// 选择窗口
	void slot_refreshWnd();									// 刷新窗口
	void slot_CustomVideoData(__int64, const char*, int, int, int);	// 发送自定义数据

	void slot_shiftWnd();									// 切换窗口
	void slot_CloseWnd();									// 关闭屏幕共享
	void requstError(QString);								// 白板及音视频错误提示
	void slot_onOnlineTimeout();							// 查询在线人数
	void slot_rtsTcpDiscontect();							// 白板链接断开
	void slot_AddAnnouncement(QString, QString);			// 添加云信通知槽函数
	void slot_GetNetState(int);								// 设置实时网络状态
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual bool eventFilter(QObject *target, QEvent *event);
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

public:
	/*************************直播全屏**************************/
	void	initFont();											// 初始化字体大小
	void	setMainWindow(UIMainNewWindow* parent);
	void	SetEnvironmental(bool bType, QString homePage);		// 环境变量;
	void	SetToken(QString token);
	void	setTeacherID(QString id, QString name);				// 老师ID和名字
	void	setAccid(QString accid);							// 云信ID
	void	AddChatRoom(QString chatID, QString courseid, QString teacherid, QString token, QString studentName, std::string strCurAudioPath,
					QString name, int UnreadCount, QString status, QString boardurl, QString cameraUrl, int mLessonType = 1); // 创建聊天
	bool	IsHasTag(QString chatID, QString status);			// 此辅导班是否创建标签
	UIChatRoom* IsHasRoom(QString chatID);						// 此辅导班聊天室是否创建
	void	AddTag(QString chatID, QString name, QString ID, bool sel, UIChatRoom* room, QString status, int mLessonType, QString board, QString camera);// 添加标签窗口, 参数sel是否选中此标签
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
	void	QueryNoticeFromYX();								// 从云信查询公告
	static void	QueryTeamInfoCallback(const std::string& tid, const nim::TeamInfo& result);	//使用回调函数添加从云信查来的公告
	void	returnNotice();										// 返回公告
	void	QueryCourse();										// 查询辅导简介
	void	returnCourse();										// 返回简介
	void	QueryLesson();										// 查询课程
	void	returnLesson();										// 返回课程
	void	QueryPerson();										// 查询成员
	void    setLiveBtnEnable(bool bEnable);						// 设置直播按钮样式状态
	void	SendFrameStatusToWebService();						// 1v1切换分享屏幕时，发送切换状态到web服务器

	/*************************云信聊天**************************/
	void	initCallBack();
	static void QueryFirstMsgOnlineCb(nim::NIMResCode code, const std::string& id, nim::NIMSessionType type, const nim::QueryMsglogResult& result);	// 第一次请求
	static void QueryMsgOnlineCb(nim::NIMResCode code, const std::string& id, nim::NIMSessionType type, const nim::QueryMsglogResult& result);		// 正常历史记录请求
	static void OnGetTeamMemberCallback(const std::string& tid, int count, const std::list<nim::TeamMemberProperty>& team_member_info_list);		// 获取成员回调
	HWND	 GetParentWnd();
	void	ReceiverMsg(const nim::IMMessage* pIMsg);					// 接收消息
	void	ReceiverRecordMsg(nim::QueryMsglogResult* pIMsg);	// 接收历史记录消息
	void	ReceiverChatMsg(nim::IMMessage* pIMsg);				// 接收初始化第一次请求的消息
	void	ReceiverLoginMsg(const nim::LoginRes& pLogMsg);			// 返回登录结果
	void	ReceiverMemberMsg(std::string sid, std::list<nim::TeamMemberProperty>* pMemberMsg);	// 返回成员
	void	SendStatus(nim::SendMessageArc* arcNew);			// 接收消息状态
	void	OnStopPlayAudio(std::string sid, char* msgid);		// 语音播放结束消息
	void    OnSendAnnouncements(QString text);					// 发送群公告
	//语音完成并发送 add by zbc 20170704
	void	SendAudio(std::string sid, std::string msgid, std::string mPath, long mSize, int dur, std::string mfileEx);
	/*************************语音处理**************************/
	bool	IsCaptureAudio();									// 判断当前是否有录制语音
	
	void	InitAudioCallBack();								// 语音录制
	static void OnStartCaptureCallback(int code);				// 开始采集回调
	static void OnStopCaptureCallback(int rescode, const char* sid, const char* cid, const char* file_path, const char* file_ext, long file_size, int audio_duration);	// 停止采集回调
	void	StartCaptureAudio();
	void	RecordingVoice(std::string chatID, std::string msgID);  // 录制语音
	void	StopRecord();											// 停止录制语音
	void	StopAudioAutoPlay();									// 关闭自动语音播放中的相关功能
	/*************************直播推流**************************/
	void	AddTodayToLesson(QString  id, QString courseid, QString boardUrl, QString cameraUrl, QString time, QString status, QString name);// 往选择课程中添加今日课程
	void	UpatateLiveStatus(QWidget* widget, bool bSuc);		// 更新直播状态
	void	SendRequestStatus(QString status);					// 更新当前辅导班状态
	void	startLiveStream();									// 更新开始直播按钮、计时器等状态
	void	setCameraEnable();									// 设置摄像头禁用启用
	void    ErrorStopLive(QWidget* pWidget);					// 直播过程中出错 
	void	ErrorStop();										// 错误停止
	bool	IsHasCamera();										// 是否有摄像头
	void    addVideoDevices(QString name, QString path);						// 添加视频源
	void	addAudioDevices(QString name, QString path);						// 添加音频源
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
	int		mathVideoWidth(int iwidth, SCREEN_TYPE type);		// 计算video显示的宽度
	int		mathVideoHeight(int iheight, SCREEN_TYPE type);		// 计算video显示的高度
	/***************************互动直播*****************************/
	void	OpenCourse(QString chatID, QString courseid, QString teacherid, QString token, QString studentName,
				std::string strCurAudioPath, QString courseName, int UnreadCount, QString status,
				QString boardurl, QString cameraUrl, int mLessonType);// 打开辅导班
	void	OpenCourse1v1(QString chatID, QString courseid, QString teacherid, QString token, QString studentName,
				std::string strCurAudioPath, QString courseName, int UnreadCount, QString status,
				QString boardurl, QString cameraUrl, int mLessonType);// 打开互动直播
	void	createRtsRoom(const QString &roomName, const QString &roomInfo = "");// 创建白板链接
	void	InitSetParamWnds();										  // 打开摄像头参数、麦克风等窗口
	void	initWhiteBoardWidget();									  // 初始化白板
	void	initConnection();										  // 初始化白板接口
	void	setAudioChange1v1(QString path);						  // 设置一对一麦克风
	void	setValueChange1v1(int iVolumn, bool capturn);			  // 设置麦克风音量
	void	setVideoChange1v1(QString path);						  // 设置一对一摄像头
	void	start1v1LiveStream();
	void	show1v1ErrorTip(QString sError);
	void	ShowLesson();											  // 查询今日课程
	void	LessonRequestFinished();								  // 返回今日课程
	void    EndDev();												  // 关闭所有设备
	void    QueryOnlinePersonNum();									  // 查询在线人数
	void	startQueryOnlineNum();									  // 开始查询在线人数
	void    stopQueryOnlineNum();									  // 停止查询在线人数
	void	RecordLive();											  // 1对1录播
	void	Math1v1Screen();										  // 计算1对1屏幕分享后，窗口的大小
};

#endif // UIWINDOWSET_H
