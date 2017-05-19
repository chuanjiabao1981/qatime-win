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

//1��1
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
//---����
#include "nim_client_def.h"
#include "assert.h"
#include <string>
//IM SDK�ӿڶ���ͷ�ļ�
#include "nim_cpp_api.h"
#include "nim_cpp_client.h"
#include "nim_tools_http_cpp.h"

//ǰ������������Ϊ�յ��������߻�ͬ����Ϣ֪ͨ�������յ�������Ϣ֪ͨ������������Ϣ���֪ͨ������������֪ͨ��������˵�¼֪ͨ����������֪ͨ����������֪ͨ��������¼����֪ͨ������
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

//����ֱ��
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
		CameraStatusInit = 0,	//δֱ��
		CameraStatusTeaching,	//ֱ����
		CameraStatusClose,		//�ѹر�
	};

	enum SCREEN_TYPE {
		SCREEN_16_9,
		SCREEN_16_10,
		SCREEN_4_3
	};

	UIWindowSet(QWidget *parent = 0);
	~UIWindowSet();

	// ��갴���϶��Ĳ���
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

	std::vector<UITags*>			m_vecTags;				// ���ڱ�ǩ��
	std::vector<UIChatRoom*>		m_vecChatRoom;			// ���еĸ��������촰
	QMap<QString, UITags*>			m_mapTags;				// ��ǩmap
	QMap<QString, UIChatRoom*>		m_mapChatRoom;			// ���촰��map
	UITags*							m_curTags;				// ��ǰ��ǩ
	UIChatRoom*						m_curChatRoom;			// ��ǰ���촰

	UINoticeWnd*					m_NoticeWnd;			// ���洰��
	UICourseWnd*					m_CourseWnd;			// ��������
	UIPersonWnd*					m_PersonWnd;			// ��Ա�б�
	UILessonList*					m_LessonWnd;			// �γ��б�
	/********************ֱ��ȫ��������ͷ*******************/
	LiveStatusManager*				m_LiveStatusManager;	// ֱ��״̬������
	QString							m_lessonid;				// ��ǰֱ���Ŀγ�id
	QString							m_lessonName;			// ��ǰֱ���Ŀγ�����
	QString							m_boardUrl;				// ��ǰֱ���İװ��ַ
	QString							m_cameraUrl;			// ��ǰֱ��������ͷ��ַ
	QTimer*							m_tempTimers;			// ������ͷ��

	UIVideo*						m_VideoInfo;			// ֱ��ȫ������
	UICamera*						m_CameraInfo;			// ֱ������ͷ����
	bool							m_bHasCamera;			// ��������ͷ
	CameraStatus					m_EnumStatus;			// ����ͷֱ��״̬
	QTimer*							m_CountTimer;			// ֱ����ʱ��
	int								m_iTimerCount;			// ����
	UILiveLessonWnd*				m_LiveLesson;			// ѡ��γ̴���

	UIBulletScreen*					m_BulletScreen;			// ��Ļ����
	UIVideoChange*					m_VideoChangeInfo;		// ����ͷѡ�񴰿�
	UIAudioChange*					m_AudioChangeInfo;		// ��˷�ѡ�񴰿�
	UIRatio*						m_RatioChangeInfo;		// �ֱ���ѡ�񴰿�
	UIBulletParam*					m_BulletParamInfo;		// ��Ļ��������
	UIScreenTip*					m_ScreenTip;			// ȫ����ʾ��
	SCREEN_TYPE						m_ScreenType;			// ��Ļ����

	/***************************����ֱ��*****************************/
	bool							m_bLiving1v1;			// 1��1ֱ����ʶ
	Palette*						mWhiteBoard;			// ����
	UICamera1v1*					m_Camera1v1Info;		// 1��1����ͷ
	UICameraS1v1*					m_CameraS1v1Info;		// 1��1ѧ������ͷ
	UIVideoChange1v1*				m_VideoChangeInfo1v1;	// ����ͷѡ�񴰿�
	UIAudioChange1v1*				m_AudioChangeInfo1v1;	// ��˷�ѡ�񴰿�
	UIAudioOutChange1v1*			m_AudioOutChangeInfo1v1;// ������ѡ�񴰿�
	UIWhiteBoardTool*				m_WhiteBoardTool;		// �װ����
	UIVideo1v1*						m_VideoInfo1v1;			// 1v1ȫ������
	UIAppWnd*						m_AppWnd1v1;			// ѡ�������
	UIAppWndTool*					m_AppWndTool1v1;		// 1v1���ڷ�������
private:
	Ui::UIWindowSet ui;

signals:
	void sig_Modle(bool bModle);
private slots :
	void MaxDialog();										// ���
	void MinDialog();										// ��С��
	void CloseDialog();										// �رնԻ���
	void clickTag(UITags*);									// �����ǩ
	void clickLive();										// �����ʼֱ��
	void clickChange(bool checked);							// �л�
	void slots_Modle(bool bModle);							// �ı�ģʽ
	void slot_onCountTimeout();								// ֱ����ʱ��
	void clickNotice();										// ����
	void clickCourse();										// ����������
	void clickPerson();										// ��Ա
	void clickLesson();										// �γ��б�
	void slot_stateChange(bool, QString, QString);			// ���Թ���
	void AudioStatus(int iStatus);							// �����ر���˷�
	void BulletStatus(int iStatus);							// ��Ļ״̬
	void clickAudioParam();									// ��˷����
	void VideoStatus(int iStatus);							// �����ر�����ͷ
	void clickVideoParam();									// ����ͷ����
	void clickRatioParam();									// �ֱ��ʲ���
	void clickBulletParam();								// ��Ļ����
	void slot_PullStreaming(QString, QString, QString, QString, QString,bool);	// ��ʼ����
	void slot_changeLessonStatus(QString, QString);			// �ı�γ�״̬
	void DeleteTag(UITags* tag);							// ɾ����ǩ	
	void slot_onTempTimeout();								// ����ͷ����

	/*����ֱ��*/
	void joinRtsRoom(const std::string &);					// ����װ巿��
	void joinRoomSuccessfully(const std::string &, __int64, const std::string &);// ����װ巿��ɹ�
	void joinVChatRoom();									// ��������Ƶ
	void joinVChatSuccessfully();							// ��������Ƶ����ɹ�
	void errorInfo(const QString &);						// ����ʧ�ܴ�����Ϣ
	void PicData(QString);									// �װ�����
	void setDeviceInfos(int);								// �豸����
	void clickVideo1v1Param();								// ����ͷ����
	void clickAudio1v1Param();								// ��˷����
	void clickAudioOut1v1Param();							// ����������
	void Audio1v1Status(int iStatus);						// �����ر���˷�
	void Video1v1Status(int iStatus);						// �����ر�����ͷ
	void AudioOut1v1Status(int);							// �����ر�������
	void clickLive1v1();									// ����1v1ֱ��
	void selectColor(QColor&);								// ��ɫ��
	void returnClick();										// ������һ��
	void deleteClick();										// ��հװ�
	void laserClick();										// �����
	void drawClick();										// �滭��
	void rtsDataReceived(const std::string& data,const std::string &uid);			// ���հװ�����
	void clickShapeScreen1v1();								// ���������Ļ
	void slot_selectWnd(HWND);								// ѡ�񴰿�
	void slot_refreshWnd();									// ˢ�´���
	void slot_CustomVideoData(__int64, const char*, int, int, int);	// �����Զ�������

	void slot_shiftWnd();									// �л�����
	void slot_CloseWnd();									// �ر���Ļ����
	void requstError(QString);								// �װ弰����Ƶ������ʾ
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual bool eventFilter(QObject *target, QEvent *event);
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

public:
	/*************************ֱ��ȫ��**************************/
	void	initFont();											// ��ʼ�������С
	void	setMainWindow(UIMainNewWindow* parent);
	void	SetEnvironmental(bool bType);						// ��������;
	void	SetToken(QString token);
	void	setTeacherID(QString id, QString name);				// ��ʦID������
	void	setAccid(QString accid);							// ����ID
	void	AddChatRoom(QString chatID, QString courseid, QString teacherid, QString token, QString studentName, std::string strCurAudioPath,
					QString name, int UnreadCount, QString status, QString boardurl, QString cameraUrl, bool b1v1Lesson = false); // ��������
	bool	IsHasTag(QString chatID, QString status);			// �˸������Ƿ񴴽���ǩ
	UIChatRoom* IsHasRoom(QString chatID);						// �˸������������Ƿ񴴽�
	void	AddTag(QString chatID, QString name, QString ID, bool sel, UIChatRoom* room, QString status,bool b1v1Lesson, QString board, QString camera);// ��ӱ�ǩ����, ����sel�Ƿ�ѡ�д˱�ǩ
	void	titleButtonWidth();
	QPixmap TeacherPhotoPixmap();								// ��ʦͷ��	
	void    AgainSelectTag();									// ����ѡ���ǩ
	void	ChangeBtnStyle(bool bLive);							// �л�ģʽ
	bool	IsHasFoucs(BTN_TYPE type);							// �жϵ��ʱ�����Ƿ��ڿؼ���
	void	SendStopLive();										// �˳�ʱ���ͽ���ֱ����Ϣ
	QPushButton*	GetNoticeBtn();
	QPushButton*	GetLessonBtn();
	QPushButton*	GetPersonBtn();
	QPushButton*	GetCourseBtn();
	void	QueryNotice();										// ��ѯ����
	void	returnNotice();										// ���ع���
	void	QueryCourse();										// ��ѯ�������
	void	returnCourse();										// ���ؼ��
	void	QueryLesson();										// ��ѯ�γ�
	void	returnLesson();										// ���ؿγ�
	void	QueryPerson();										// ��ѯ��Ա
	void    setLiveBtnEnable(bool bEnable);						// ����ֱ����ť��ʽ״̬

	/*************************��������**************************/
	void	initCallBack();
	static void QueryFirstMsgOnlineCb(nim::NIMResCode code, const std::string& id, nim::NIMSessionType type, const nim::QueryMsglogResult& result);	// ��һ������
	static void QueryMsgOnlineCb(nim::NIMResCode code, const std::string& id, nim::NIMSessionType type, const nim::QueryMsglogResult& result);		// ������ʷ��¼����
	static void OnGetTeamMemberCallback(const std::string& tid, int count, const std::list<nim::TeamMemberProperty>& team_member_info_list);		// ��ȡ��Ա�ص�
	HWND	 GetParentWnd();
	void	ReceiverMsg(const nim::IMMessage* pIMsg);					// ������Ϣ
	void	ReceiverRecordMsg(nim::QueryMsglogResult* pIMsg);	// ������ʷ��¼��Ϣ
	void	ReceiverChatMsg(nim::IMMessage* pIMsg);				// ���ճ�ʼ����һ���������Ϣ
	void	ReceiverLoginMsg(const nim::LoginRes& pLogMsg);			// ���ص�¼���
	void	ReceiverMemberMsg(std::string sid, std::list<nim::TeamMemberProperty>* pMemberMsg);	// ���س�Ա
	void	SendStatus(nim::SendMessageArc* arcNew);			// ������Ϣ״̬
	void	OnStopPlayAudio(std::string sid, char* msgid);		// �������Ž�����Ϣ
	void    OnSendAnnouncements(QString text);					// ����Ⱥ����

	/*************************ֱ������**************************/
	void	AddTodayToLesson(QString  id, QString courseid, QString boardUrl, QString cameraUrl, QString time, QString status, QString name);// ��ѡ��γ�����ӽ��տγ�
	void	UpatateLiveStatus(QWidget* widget, bool bSuc);		// ����ֱ��״̬
	void	SendRequestStatus(QString status);					// ���µ�ǰ������״̬
	void	startLiveStream();									// ���¿�ʼֱ����ť����ʱ����״̬
	void	setCameraEnable();									// ��������ͷ��������
	void    ErrorStopLive(QWidget* pWidget);					// ֱ�������г��� 
	void	ErrorStop();										// ����ֹͣ
	bool	IsHasCamera();										// �Ƿ�������ͷ
	void    addVideoDevices(QString name, QString path);						// �����ƵԴ
	void	addAudioDevices(QString name, QString path);						// �����ƵԴ
	void	setAudioChangeIndex(int index);						// ѡ����ƵԴ
	void    setVideoChangeIndex(int index);						// ѡ����ƵԴ
	void    setRatioChangeIndex(QString ratio);					// ѡ��ֱ���
	void	showErrorTip(QString sError);						// ��ʾ����
	bool    IsLiving();											// ��ǰ�Ƿ���ֱ��
	void	SendStudentBullet(QString name, QString content,QString chatid);	// ����ѧ����Ļ
	void	SendTeacherBullet(QString name, QString content, QString chatid);	// ������ʦ��Ļ
	void	BulletDelay(int iDelay);							// ���
	void	setTriggerType(bool bType);							// ��������
	void	CloseBullet();										// ���ص�Ļ
	void	ReturnLogin();										// ���µ�¼
	void	StopSuccess(QWidget* widget);						// ֱ��ֹͣ�ɹ�
	void    MathScreenSize();									// ������Ļ����
	int		mathVideoWidth(int iwidth, SCREEN_TYPE type);		// ����video��ʾ�Ŀ��
	int		mathVideoHeight(int iheight, SCREEN_TYPE type);		// ����video��ʾ�ĸ߶�
	/***************************����ֱ��*****************************/
	void	OpenCourse(QString chatID, QString courseid, QString teacherid, QString token, QString studentName,
				std::string strCurAudioPath, QString courseName, int UnreadCount, QString status,
				QString boardurl, QString cameraUrl, bool b1v1Lesson);// �򿪸�����
	void	OpenCourse1v1(QString chatID, QString courseid, QString teacherid, QString token, QString studentName,
				std::string strCurAudioPath, QString courseName, int UnreadCount, QString status,
				QString boardurl, QString cameraUrl, bool b1v1Lesson);// �򿪻���ֱ��
	void	createRtsRoom(const QString &roomName, const QString &roomInfo = "");// �����װ�����
	void	InitSetParamWnds();										  // ������ͷ��������˷�ȴ���
	void	initWhiteBoardWidget();									  // ��ʼ���װ�
	void	initConnection();										  // ��ʼ���װ�ӿ�
	void	setAudioChange1v1(QString path);						  // ����һ��һ��˷�
	void	setValueChange1v1(int iVolumn, bool capturn);			  // ������˷�����
	void	setVideoChange1v1(QString path);						  // ����һ��һ����ͷ
	void	start1v1LiveStream();
	void	show1v1ErrorTip(QString sError);
	void	ShowLesson();											  // ��ѯ���տγ�
	void	LessonRequestFinished();								  // ���ؽ��տγ�
};

#endif // UIWINDOWSET_H
