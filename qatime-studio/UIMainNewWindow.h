#ifndef UIMAINNEWWINDOW_H
#define UIMAINNEWWINDOW_H

#include <QWidget>
#include "ui_UIMainNewWindow.h"
#include "UIAuxiliarywnd.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "loginWindow.h"
#include "UIWindowSet.h"
#include "define.h"

//---云信
#include "assert.h"
#include <string>
#include "nim_cpp_api.h"
#include "nim_cpp_client.h"
#include "nim_cpp_tool.h"

// 语音录制完成的参数	add by zbc 20170704
struct MyAudioStruct
{
	std::string sSessionID;
	std::string sMsgID;
	std::string sFilePath;
	std::string sFileEx;
	long	fileSize;
	int		duration;
};


class UIWindowSet;
class UIAuxiliaryWnd;
class LoginWindow;
class UIMainNewWindow : public QWidget
{
	Q_OBJECT

public:
	struct CourseStruct
	{
		QString		chatID;
		QString		courseID;
		QString		teacherID;
		QString		Token;
		QString		teacherName;
		std::string	audioPath;
		QString		courseName;
		int			UnreadMsgCount;
		QString		Status;
		bool		bIs1v1Lesson;
		QString		boardUrl;
		QString		cameraUrl;
	};
	CourseStruct*	m_structCousre;
	UIMainNewWindow(QWidget *parent = 0);
	~UIMainNewWindow();

	QNetworkAccessManager			manager;
	QNetworkReply*					reply;

private:
	Ui::UIMainNewWindow ui;
	LoginWindow*					m_LoginWindow;
	bool							m_EnvironmentalTyle;	// 当前环境
	UIAuxiliaryWnd*					m_AuxiliaryWnd;			// 辅导班窗口	
	UIWindowSet*					m_WindowSet;			// 窗口集
	QString							mRemeberToken;			// token
	QString							m_teacherID;			// 老师ID
	QPixmap							m_TeacherPix;			// 老师头像
	QString							m_TeacherName;			// 老师名字
	QString							m_accid;				// 云信自己的ID
	QString							m_token;				// 云信密码
	std::string						m_AudioPath;			// 语音消息路径
	QString							m_homePage;				// 服务器

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // 添加caption

public:
	/*添加云信功能*/
	void    InitAudio();									// 初始化语音
	void	setLoginWindow(LoginWindow* parent);			// 登录窗口
	void	setVersion(QString version);					// 设置版本号
	void	SetEnvironmental(bool EnvironmentalTyle, QString m_homePage);
	void	setRemeberToken(const QString &token);
	void	setTeacherInfo(QJsonObject &data);
	void    returnClick();									// 切换账号
	void    setAutoTeacherInfo(QString studentID, QString studentName, QString studentUrl, QString accid, QString token);
	void	ShowLesson();									// 今日课程
	void    LessonRequestFinished();						// 返回今日课程
	void    ShowAuxiliary();								// 全部课程
	void	AuxiliaryRequestFinished();						// 返回全部课程
	void    Show1v1Auxiliary();								// 1v1课程
	void    Return1v1AuxiliaryRequestFinished();			// 返回1v1课程
	void    CloseDialog();									// 关闭窗口
	bool	IsMayClose();									// 是否可以关闭窗口
	void    ShowCourse();									// 显示辅导班
	QPixmap TeacherPhotoPixmap();							// 老师头像
	void	RequestKey();									// 请求Key
	void    returnKey();									// 返回Key
	void	setKeyAndLogin(QString key);					// 登录云信
	static void OnLoginCallback(const nim::LoginRes& login_res, const void* user_data);
	static void OnMultispotLoginCallback(const nim::MultiSpotLoginRes& res);
	static void OnKickoutCallback(const nim::KickoutRes& res);
	static void OnStopAudioCallback(int code, const char* file_path, const char* sid, const char* cid);// 停止语音
	static void OnLogOutCallback(nim::NIMResCode res_code);
	void	CreateRoom(QString chatID, QString courseID, QString teacherID, QString token, QString studentName, std::string audioPath, 
						QString courseName, int UnreadCount, QString status, QString boardurl, QString cameraUrl,bool b1v1Lesson=false);
	void    changeLessonStatus(QString id, QString status);
	void	changeMsgNumber(QString chid);
	void	LogOut();										// 退出云信登录

signals:
	void sig_QueryLesson();									// 查询课程

private slots:
	void slot_QueryLesson();								// 查询课程
};

#endif // UIMAINNEWWINDOW_H
