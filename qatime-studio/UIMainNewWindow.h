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

class UIWindowSet;
class UIAuxiliaryWnd;
class LoginWindow;
class UIMainNewWindow : public QWidget
{
	Q_OBJECT

public:
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

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // 添加caption

public:
	/*添加云信功能*/
	void	initSDK();										// 初始化云信
	void    InitAudio();									// 初始化语音
	bool	LoadConfig(const std::string& app_data_dir, const std::string& app_install_dir, nim::SDKConfig &config);// 加载配置
	std::string GetJsonStringWithNoStyled(const QJsonObject& values); // json类型转换

	void	setLoginWindow(LoginWindow* parent);			// 登录窗口
	void	setVersion(QString version);					// 设置版本号
	void	SetEnvironmental(bool EnvironmentalTyle);
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
	static void OnStopAudioCallback(int code, const char* file_path, const char* sid, const char* cid);// 停止语音
	void	CreateRoom(QString chatID, QString courseID, QString teacherID, QString token, QString studentName, std::string audioPath, 
						QString courseName, int UnreadCount, QString status, bool b1v1Lesson=false);
	void    changeLessonStatus(QString id, QString status);
};

#endif // UIMAINNEWWINDOW_H
