#ifndef UICHATROOM_H
#define UICHATROOM_H

#include <QWidget>
#include "ui_UIChatRoom.h"
#include "myemotionwidget.h"
#include <QLabel>
#include "uimainwindow.h"
#include "UITalk.h"
#include "UITalkRecord.h"
#include <vector>
#include "UIWindowset.h"

//---云信
//#include "nim_client_def.h"
#include "assert.h"
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>

#include "nim_cpp_api.h"
#include "nim_cpp_client.h"
#include "nim_cpp_tool.h"
#include "nim_audio_cpp.h"


class UIMainWindow;
class UITalk;
class UITalkRecord;
class UIWindowSet;
struct MyImageInfo
{
	QPixmap			PhotoImg;		//聊天头像
	QString			name;			//昵称
	QString			time;			//时间
	QString			ReceiverImg;	//接收的图片路径
	std::string		chatID;			//会话id
};

class UIChatRoom : public QWidget
{
	Q_OBJECT

public:
	UIChatRoom(QWidget *parent = 0);
	~UIChatRoom();
	void stringToHtmlFilter(QString &str);					//QString转htlm
	void stringToHtml(QString &str, QColor crl);			//QString转htlm带颜色
	void stringToHtmlPos(QString &str, QColor crl);			//QString转html带颜色切位置上移
	void imgPathToHtml(QString &path);						//edit 设置图片
	void setMainWindow(UIWindowSet* parent);
public slots:
	void setBrow(QString path);
signals:
	void sig_StartLoading();
private:
	Ui::UIChatRoom ui;
	
	QNetworkAccessManager manager;
	QNetworkReply *reply;
	UITalk*							m_uitalk;			// 聊天窗的自定义聊天控件
	UITalkRecord*					m_uitalkRecord;		// 聊天记录

	//表情框	
	MyEmotionWidget*				m_smallEmotionWidget;
	MyEmotionWidget*				m_normalEmotionWidget;
	QLabel*						    m_lableTitle;
	QStringList						m_borw;//当前选中的表情
	bool							m_isBorw;//是否有表情
	std::string						m_CurChatID;		// 当前会话窗口ID 
	QString							m_CurCourseID;		// 当前辅导班ID
	QString							m_token;			// 云信用户密码
	QString							m_appKey;			// 云信key
	bool							m_bLogin;			// 是否登录
	bool                            m_switchTime;		// 切换时间
	QDateTime						m_RecordTime;		// 消息记录时间（和历史记录时间做对比，用来显示跨天）
	QDateTime						m_ReceiveTime;		// 聊天记录时间（和之前记录做对比，用来显示跨天）
	/************************************************************************/
	/*								聊天记录参数							*/
	long long						m_farst_msg_time;	// 最远得消息时间
	int								kMsgLogNumberShow;	// 一次获取的条数
	/************************************************************************/
	QTextCursor*					m_TextCursor;
	QMap<QString, QString>			m_StudentInfo;		// key ：accid 
	QString							mRemeberToken;
	std::string						m_AddMemberID;		// 新加入的成员ID
	UIWindowSet*					m_parent;			// 主窗口对象
	int								m_studentNum;		// 当前学生数量
	int								m_proclamationHeight;// 公告高度
	std::vector<MyImageInfo>		m_VerReceiveImg;	// 接收的图片消息队列
	QTimer*							m_LoadImgTimer;		// 定时器加载图片
	int								m_drawingWidth;		// 拉动改变的宽度
	bool							m_bClickPic;		// 当前是否点击图片
	std::string						m_AudioPath;		// 语音消息路径
	QString							m_CurTeacherID;		// 老师当前ID
	QString							m_teacherName;		// 老师名字
	QString							m_accid;			// 老师当前云信ID
	int								m_UnreadCount;		// 消息未读数
	bool							m_bPerson;			// 是否请求完成员
	bool							m_EnvironmentalTyle;// 环境变量
	bool							m_b1v1Lesson;
	void initEmotion();
public:
	QString							m_TeachterName;		// 老师名字
protected:
	virtual bool eventFilter(QObject *watched, QEvent *event);
	void mousePressEvent(QMouseEvent *event);
public slots:
	void clickTalk();									// 弹出聊天框
	void clickStudentList();							// 弹出学生列表
	void clickProclamation();							// 弹出公告
	void clickCleanText();								// 清屏
	void clickBrow();									// 表情按钮
	void clickNotes();									// 消息记录
	void closeNotes();									// 关闭消息记录
	void clickSendMseeage();							// 发送消息
	void chickChoseTime();								// 选择时间观看消息记录
	void choseTime(QDate);								// 点击某一时间的槽函数
	void forwardTime();									// 点击往前一天的槽函数
	void afterTime();									// 点击往后一天的槽函数
	void RecordMoved();									// 消息记录滚动条
	void announce();									// 点击【发布公告】按钮
	void putTalk();										// 点击【发布】按钮
	void putTalkCancel();								// 点击【取消发布】按钮
	void proclamationTextChage();						// 发布编辑框输入文字设置发布按钮是否可以被点击。
	void colseCalendar();								// 关闭日历槽函数
	void colseBrow();									// 关闭表情槽函数
	void calendaCurrentPageChanged(int year, int month);// 日历当前页面被改变
	void clickPic();									// 选择图片
	void LoadImgTimeout();								// 加载图片定时器
private:
//	void		initSDK();									// 初始化云信SDK
//	bool		LoadConfig(const std::string& app_data_dir,const std::string& app_install_dir, nim::SDKConfig &config); //加载配置
//	std::string GetJsonStringWithNoStyled(const QJsonObject& values);				   //json类型转换
	void		PackageMsg(nim::IMMessage &msg);
	void        style(QTextBrowser *style);
	/**
	* 登录结果回调
	* @param[in] json_params 返回的信息，Json数据转化的字符串
	* @param[in] user_data 登录时用户传的数据，sdk原封不动传给回调
	* @return void	无返回值
	*/
	static void OnLoginCallback(const nim::LoginRes& login_res, const void* user_data);

	/**
	* 群事件回调
	* @param[in] rescode 返回码
	* @param[in] notification_id 群事件类型
	* @param[in] tid 群id
	* @param[in] result 事件信息内容， Json数据转化成的字符串
	* @return void	无返回值
	*/
	static void OnTeamEventCallback(const nim::TeamEvent& result);

	static void OnGetTeamInfoCb(const nim::TeamEvent& team_event);

	QString		UserAppdataPath();
	void		GenerateUploadImage(const std::wstring src, const std::wstring dest);
	std::string GetFileMD5(QString path);
	long		GetFileSize(QString path);
public:
	void		SendImage(const std::wstring src, QString &filename, QString msgid="");
	void		setChatInfo(QJsonObject &chatInfo, QString token);	// 设置云信账户信息
	bool		ReceiverMsg(const nim::IMMessage* pMsg);					// 接收服务器发送过来的消息
	void		ReceiverRecordMsg(nim::QueryMsglogResult* pMsg);	// 接收历史消息记录
	void		ReceiverLoginMsg(const nim::LoginRes& pRes);				// 接收登录结果
	void		ReceiverMemberMsg(std::list<nim::TeamMemberProperty>* pMemberMsg); //接收群成员信息
	void		setCurChatID(QString chatID, QString courseid, QString teacherid, QString token, QString studentName, QString accid, int UnreadCount,bool b1v1=false);		// 设置当前窗口会话ID,用于接收消息时比较
	std::string	GetCurChatID();
	void		setKeyAndLogin(QString key);						// 设置appkey并登录（获取完Key之后，就可以直接登录）
	bool		IsLogin();											// 是否登录
	bool		IsCurChatRoom(QString chatID);						// 是否是当前会话ID

	void		ShowMsgs(const std::vector<nim::IMMessage> &msg);
	void		ShowMsg(nim::IMMessage pMsg);
	void		ParseFace(QString qContect, QString name, QString time);						// 解析接收到的消息
	bool		IsHasFace(QString qContect);						// 判断是否有表情
	QString		BuildFaceToUrl(QString qFace);						// 通过表情返回url路径（例如：传入[em_1]返回./images/em_1.gif）
	void		SetStudentName(int iNum);
	void		setResize(int iWidth, int iHeight);					// 宽度 高度
	bool		IsFous();
	void		UpLoadPicProcess(double iProcess);					// 上传图片进度
	void		SendStatus(nim::SendMessageArc* arcNew);			// 发送消息返回状态
	bool		IsClickPic();										// 当前是否点击图片
	void		TalkDown();											// 聊天内容置底
	void		OnPlayAudio(std::string path, std::string sid, std::string msgid, bool isPlay);
	void		OnStopPlayAudio(char* msgid);
	/**
	* 开始播放语音消息的回调（播放语音消息需要和sdk交互，并非一定播放成功）
	* @param[in] code 返回码
	* @param[in] file_path 语音消息文件路径
	* @param[in] sid 会话id
	* @param[in] cid 消息id
	* @return void 无返回值
	*/
	static void	OnPlayAudioCallback(int code, const char* file_path, const char* sid, const char* cid);

	/**
	* 停止播放语音消息的回调
	* @param[in] code 返回码
	* @param[in] file_path 语音消息文件路径
	* @param[in] sid 会话id
	* @param[in] cid 消息id
	* @return void 无返回值
	*/
	static void OnStopAudioCallback(int code, const char* file_path, const char* sid, const char* cid);
	std::vector<personListBuddy*>  GetBuddy();						// 获取成员
	bool		IsPerson();											// 是否请求完成员
	void		ResultMsg();										// 第一次请求
	void		ShowChatMsg(nim::IMMessage pMsg);					// 第一次请求回来的消息
	
public slots:
	void		chickChage(int, QString, QString);
	bool		AddStudent(QString iconUrl, QString name, QString accid);		//添加成员
	void		MuteStudent(bool state, QString id);							//禁言学生
	void		AddStudentNumbers(int num);										//添加成员数量
	void		AddAnnouncement(QString announcement, QString time);			//添加公告
	void		QueryGroup();													//查询群成员信息
	void		QueryRecord(QString dtstr);
	void		stepDays(QDateTime date);										//历史记录跨天
	void		stepMsgDays(QDateTime dateTime);								//聊天记录跨天
	void		clearAll();														//清除聊天记录、公告、群成员
	void		OnSendAnnouncements(QString Announcements);						//发送群公告
	void		ReturnAnnouncements();											//返回公告状态
	void		RequestError(QJsonObject& error);								//错误
	void		QueryMember();
	void		returnMember();
	void		RequestMember();												//请求成员
	void		returnAllMember();												//返回成员
	void		Request1v1Member();												//请求1v1成员
	void		SetEnvironmental(bool EnvironmentalTyle);						//设置当前环境
	void        SetCurAudioPath(std::string path);								//设置当前语音路径
	QString		parse(QString str);
private:
	QToolButton* pPreMonthButton1;
	QToolButton* pPreMonthButton;
	bool		 m_bZoom;	// 当垂直滚动条出现后，是否缩放过 
};

#endif // UICHATROOM_H
