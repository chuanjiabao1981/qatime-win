#ifndef UICHATROOM_H
#define UICHATROOM_H

#include <QWidget>
#include "ui_UIChatRoom.h"
#include "myemotionwidget.h"
#include <QLabel>
#include "uimainwindow.h"
#include "UIChatHtml.h"
#include <vector>

//---云信
#include "nim_client_def.h"
#include "assert.h"
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include "YxChat/nim_tools_http_cpp_wrapper.h"
#include "YxChat/nim_client_helper.h"
#include "YxChat/nim_cpp_talk.h"
#include "YxChat/nim_cpp_team.h"
#include "YxChat/nim_cpp_msglog.h"
#include <QNetworkAccessManager>

class UIMainWindow;
class UIChatHtml;

struct MyImageInfo
{
	QString			PhotoImg;		//聊天头像
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
	void setMainWindow(UIMainWindow* parent);
public slots:
	void setBrow(QString path);

private:
	Ui::UIChatRoom ui;
	
	QNetworkAccessManager manager;
	QNetworkReply *reply;
	UIChatHtml*						m_ChatHtml;			// 聊天窗的html

	//表情框	
	MyEmotionWidget*				m_smallEmotionWidget;
	MyEmotionWidget*				m_normalEmotionWidget;
	QLabel*						    m_lableTitle;
	QStringList						m_borw;//当前选中的表情
	bool							m_isBorw;//是否有表情
	std::string						m_CurChatID;		// 当前会话窗口ID 
	QString							m_CurCourseID;		// 当前辅导班ID
	QString							m_accid;			// 云信用户ID
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
	UIMainWindow*					m_parent;			// 主窗口对象
	int								m_studentNum;		// 当前学生数量
	int								m_proclamationHeight;// 公告高度
	std::vector<MyImageInfo>		m_VerReceiveImg;	// 接收的图片消息队列
	QTimer*							m_LoadImgTimer;		// 定时器加载图片
	int								m_drawingWidth;		// 拉动改变的宽度
	void initEmotion();
public:
	QString							m_TeachterName;		// 老师名字
protected:
	virtual bool eventFilter(QObject *watched, QEvent *event);
	void mousePressEvent(QMouseEvent *event);
private slots:
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
	void RecordMoved(int iPos);							// 消息记录滚动条
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
	void		initSDK();									// 初始化云信SDK
	bool		LoadConfig(const std::string& app_data_dir,const std::string& app_install_dir, nim::SDKConfig &config); //加载配置
	std::string GetJsonStringWithNoStyled(const QJsonObject& values);				   //json类型转换
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

	/**
	* 获取会话记录的回调函数
	* @param[in] code	错误码(200代表无错误)
	* @param[in] id		获取的会话记录所属的会话id
	* @param[in] type	获取的会话记录所属的会话类型
	* @param[in] result 消息体Json string,包含了获取的会话记录的信息
	* @return void 无返回值
	*/
	static void QueryMsgOnlineCb(nim::NIMResCode code, const std::string& id, nim::NIMSessionType type, const nim::QueryMsglogResult& result);

	static void OnGetTeamInfoCb(const nim::TeamEvent& team_event);

	QString		UserAppdataPath();
	void		GenerateUploadImage(const std::wstring src, const std::wstring dest);
	std::string GetFileMD5(QString path);
	long		GetFileSize(QString path);
public:
	void	SendImage(const std::wstring src, QString &filename, QString msgid="");
	void	setChatInfo(QJsonObject &chatInfo, QString token);	// 设置云信账户信息
	bool	ReceiverMsg(nim::IMMessage* pMsg);					// 接收服务器发送过来的消息
	void	ReceiverRecordMsg(nim::QueryMsglogResult* pMsg);	// 接收历史消息记录
	void	ReceiverLoginMsg(nim::LoginRes* pRes);				// 接收登录结果
	void	ReceiverMemberMsg(std::list<nim::TeamMemberProperty>* pMemberMsg); //接收群成员信息
	void	setCurChatID(QString chatID, QString courseid);		// 设置当前窗口会话ID,用于接收消息时比较
	void	setKeyAndLogin(QString key);						// 设置appkey并登录（获取完Key之后，就可以直接登录）
	bool	IsLogin();											// 是否登录
	bool	IsCurChatRoom(QString chatID);						// 是否是当前会话ID

	void	ShowMsgs(const std::vector<nim::IMMessage> &msg);
	void	ShowMsg(nim::IMMessage pMsg);
	void	ParseFace(QString qContect);						// 解析接收到的消息
	bool	IsHasFace(QString qContect);						// 判断是否有表情
	QString BuildFaceToUrl(QString qFace);						// 通过表情返回url路径（例如：传入[em_1]返回./images/em_1.gif）
	void	SetStudentName(int iNum);
	void	setAdaptHeight(int iHeight);						// 自适应高度
	void	setResize(int iWidth, int iHeight);					// 宽度 高度
	bool	IsFous();
	void	UpLoadPicProcess(double iProcess);					// 上传图片进度
	void	SendStatus(nim::SendMessageArc* arcNew);			// 发送消息返回状态
public slots:
	void chickChage(int, QString, QString);
	bool AddStudent(QString iconUrl, QString name, QString accid);		//添加成员
	void AddStudentNumbers(int num);									//添加成员数量
	void AddAnnouncement(QString announcement, QString time);			//添加公告
	void QueryGroup();													//查询群成员信息
	void QueryRecord(QString dtstr);
	void stepDays(QDateTime date);										//历史记录跨天
	void stepMsgDays(QDateTime dateTime);								//聊天记录跨天
	void clearAll();													//清除聊天记录、公告、群成员
	void OnSendAnnouncements(QString Announcements);					//发送群公告
	void QueryMember();
	void returnMember();
private:
	QToolButton* pPreMonthButton1;
	QToolButton* pPreMonthButton;
	bool		 m_bZoom;	// 当垂直滚动条出现后，是否缩放过 
};

#endif // UICHATROOM_H
