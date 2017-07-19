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

//---����
#include "assert.h"
#include <string>
#include "nim_cpp_api.h"
#include "nim_cpp_client.h"
#include "nim_cpp_tool.h"

// ����¼����ɵĲ���	add by zbc 20170704
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
	bool							m_EnvironmentalTyle;	// ��ǰ����
	UIAuxiliaryWnd*					m_AuxiliaryWnd;			// �����ര��	
	UIWindowSet*					m_WindowSet;			// ���ڼ�
	QString							mRemeberToken;			// token
	QString							m_teacherID;			// ��ʦID
	QPixmap							m_TeacherPix;			// ��ʦͷ��
	QString							m_TeacherName;			// ��ʦ����
	QString							m_accid;				// �����Լ���ID
	QString							m_token;				// ��������
	std::string						m_AudioPath;			// ������Ϣ·��
	QString							m_homePage;				// ������

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // ���caption

public:
	/*������Ź���*/
	void    InitAudio();									// ��ʼ������
	void	setLoginWindow(LoginWindow* parent);			// ��¼����
	void	setVersion(QString version);					// ���ð汾��
	void	SetEnvironmental(bool EnvironmentalTyle, QString m_homePage);
	void	setRemeberToken(const QString &token);
	void	setTeacherInfo(QJsonObject &data);
	void    returnClick();									// �л��˺�
	void    setAutoTeacherInfo(QString studentID, QString studentName, QString studentUrl, QString accid, QString token);
	void	ShowLesson();									// ���տγ�
	void    LessonRequestFinished();						// ���ؽ��տγ�
	void    ShowAuxiliary();								// ȫ���γ�
	void	AuxiliaryRequestFinished();						// ����ȫ���γ�
	void    Show1v1Auxiliary();								// 1v1�γ�
	void    Return1v1AuxiliaryRequestFinished();			// ����1v1�γ�
	void    CloseDialog();									// �رմ���
	bool	IsMayClose();									// �Ƿ���Թرմ���
	void    ShowCourse();									// ��ʾ������
	QPixmap TeacherPhotoPixmap();							// ��ʦͷ��
	void	RequestKey();									// ����Key
	void    returnKey();									// ����Key
	void	setKeyAndLogin(QString key);					// ��¼����
	static void OnLoginCallback(const nim::LoginRes& login_res, const void* user_data);
	static void OnMultispotLoginCallback(const nim::MultiSpotLoginRes& res);
	static void OnKickoutCallback(const nim::KickoutRes& res);
	static void OnStopAudioCallback(int code, const char* file_path, const char* sid, const char* cid);// ֹͣ����
	static void OnLogOutCallback(nim::NIMResCode res_code);
	void	CreateRoom(QString chatID, QString courseID, QString teacherID, QString token, QString studentName, std::string audioPath, 
						QString courseName, int UnreadCount, QString status, QString boardurl, QString cameraUrl,bool b1v1Lesson=false);
	void    changeLessonStatus(QString id, QString status);
	void	changeMsgNumber(QString chid);
	void	LogOut();										// �˳����ŵ�¼

signals:
	void sig_QueryLesson();									// ��ѯ�γ�

private slots:
	void slot_QueryLesson();								// ��ѯ�γ�
};

#endif // UIMAINNEWWINDOW_H
