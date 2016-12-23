#ifndef UICHATROOM_H
#define UICHATROOM_H

#include <QWidget>
#include "ui_UIChatRoom.h"
#include "myemotionwidget.h"
#include <QLabel>
#include "uimainwindow.h"
#include "UIChatHtml.h"
#include <vector>

//---����
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
	QString			PhotoImg;		//����ͷ��
	QString			name;			//�ǳ�
	QString			time;			//ʱ��
	QString			ReceiverImg;	//���յ�ͼƬ·��
	std::string		chatID;			//�Ựid
};

class UIChatRoom : public QWidget
{
	Q_OBJECT

public:
	UIChatRoom(QWidget *parent = 0);
	~UIChatRoom();
	void stringToHtmlFilter(QString &str);					//QStringתhtlm
	void stringToHtml(QString &str, QColor crl);			//QStringתhtlm����ɫ
	void stringToHtmlPos(QString &str, QColor crl);			//QStringתhtml����ɫ��λ������
	void imgPathToHtml(QString &path);						//edit ����ͼƬ
	void setMainWindow(UIMainWindow* parent);
public slots:
	void setBrow(QString path);

private:
	Ui::UIChatRoom ui;
	
	QNetworkAccessManager manager;
	QNetworkReply *reply;
	UIChatHtml*						m_ChatHtml;			// ���촰��html

	//�����	
	MyEmotionWidget*				m_smallEmotionWidget;
	MyEmotionWidget*				m_normalEmotionWidget;
	QLabel*						    m_lableTitle;
	QStringList						m_borw;//��ǰѡ�еı���
	bool							m_isBorw;//�Ƿ��б���
	std::string						m_CurChatID;		// ��ǰ�Ự����ID 
	QString							m_CurCourseID;		// ��ǰ������ID
	QString							m_accid;			// �����û�ID
	QString							m_token;			// �����û�����
	QString							m_appKey;			// ����key
	bool							m_bLogin;			// �Ƿ��¼
	bool                            m_switchTime;		// �л�ʱ��
	QDateTime						m_RecordTime;		// ��Ϣ��¼ʱ�䣨����ʷ��¼ʱ�����Աȣ�������ʾ���죩
	QDateTime						m_ReceiveTime;		// �����¼ʱ�䣨��֮ǰ��¼���Աȣ�������ʾ���죩
	/************************************************************************/
	/*								�����¼����							*/
	long long						m_farst_msg_time;	// ��Զ����Ϣʱ��
	int								kMsgLogNumberShow;	// һ�λ�ȡ������
	/************************************************************************/
	QTextCursor*					m_TextCursor;
	QMap<QString, QString>			m_StudentInfo;		// key ��accid 
	QString							mRemeberToken;
	std::string						m_AddMemberID;		// �¼���ĳ�ԱID
	UIMainWindow*					m_parent;			// �����ڶ���
	int								m_studentNum;		// ��ǰѧ������
	int								m_proclamationHeight;// ����߶�
	std::vector<MyImageInfo>		m_VerReceiveImg;	// ���յ�ͼƬ��Ϣ����
	QTimer*							m_LoadImgTimer;		// ��ʱ������ͼƬ
	int								m_drawingWidth;		// �����ı�Ŀ��
	void initEmotion();
public:
	QString							m_TeachterName;		// ��ʦ����
protected:
	virtual bool eventFilter(QObject *watched, QEvent *event);
	void mousePressEvent(QMouseEvent *event);
private slots:
	void clickTalk();									// ���������
	void clickStudentList();							// ����ѧ���б�
	void clickProclamation();							// ��������
	void clickCleanText();								// ����
	void clickBrow();									// ���鰴ť
	void clickNotes();									// ��Ϣ��¼
	void closeNotes();									// �ر���Ϣ��¼
	void clickSendMseeage();							// ������Ϣ
	void chickChoseTime();								// ѡ��ʱ��ۿ���Ϣ��¼
	void choseTime(QDate);								// ���ĳһʱ��Ĳۺ���
	void forwardTime();									// �����ǰһ��Ĳۺ���
	void afterTime();									// �������һ��Ĳۺ���
	void RecordMoved(int iPos);							// ��Ϣ��¼������
	void announce();									// ������������桿��ť
	void putTalk();										// �������������ť
	void putTalkCancel();								// �����ȡ����������ť
	void proclamationTextChage();						// �����༭�������������÷�����ť�Ƿ���Ա������
	void colseCalendar();								// �ر������ۺ���
	void colseBrow();									// �رձ���ۺ���
	void calendaCurrentPageChanged(int year, int month);// ������ǰҳ�汻�ı�
	void clickPic();									// ѡ��ͼƬ
	void LoadImgTimeout();								// ����ͼƬ��ʱ��
private:
	void		initSDK();									// ��ʼ������SDK
	bool		LoadConfig(const std::string& app_data_dir,const std::string& app_install_dir, nim::SDKConfig &config); //��������
	std::string GetJsonStringWithNoStyled(const QJsonObject& values);				   //json����ת��
	void		PackageMsg(nim::IMMessage &msg);
	void        style(QTextBrowser *style);
	/**
	* ��¼����ص�
	* @param[in] json_params ���ص���Ϣ��Json����ת�����ַ���
	* @param[in] user_data ��¼ʱ�û��������ݣ�sdkԭ�ⲻ�������ص�
	* @return void	�޷���ֵ
	*/
	static void OnLoginCallback(const nim::LoginRes& login_res, const void* user_data);

	/**
	* Ⱥ�¼��ص�
	* @param[in] rescode ������
	* @param[in] notification_id Ⱥ�¼�����
	* @param[in] tid Ⱥid
	* @param[in] result �¼���Ϣ���ݣ� Json����ת���ɵ��ַ���
	* @return void	�޷���ֵ
	*/
	static void OnTeamEventCallback(const nim::TeamEvent& result);

	/**
	* ��ȡ�Ự��¼�Ļص�����
	* @param[in] code	������(200�����޴���)
	* @param[in] id		��ȡ�ĻỰ��¼�����ĻỰid
	* @param[in] type	��ȡ�ĻỰ��¼�����ĻỰ����
	* @param[in] result ��Ϣ��Json string,�����˻�ȡ�ĻỰ��¼����Ϣ
	* @return void �޷���ֵ
	*/
	static void QueryMsgOnlineCb(nim::NIMResCode code, const std::string& id, nim::NIMSessionType type, const nim::QueryMsglogResult& result);

	static void OnGetTeamInfoCb(const nim::TeamEvent& team_event);

	QString		UserAppdataPath();
	void		GenerateUploadImage(const std::wstring src, const std::wstring dest);
	std::string GetFileMD5(QString path);
	long		GetFileSize(QString path);
public:
	void	SendImage(const std::wstring src, QString &filename, QString msgid="");
	void	setChatInfo(QJsonObject &chatInfo, QString token);	// ���������˻���Ϣ
	bool	ReceiverMsg(nim::IMMessage* pMsg);					// ���շ��������͹�������Ϣ
	void	ReceiverRecordMsg(nim::QueryMsglogResult* pMsg);	// ������ʷ��Ϣ��¼
	void	ReceiverLoginMsg(nim::LoginRes* pRes);				// ���յ�¼���
	void	ReceiverMemberMsg(std::list<nim::TeamMemberProperty>* pMemberMsg); //����Ⱥ��Ա��Ϣ
	void	setCurChatID(QString chatID, QString courseid);		// ���õ�ǰ���ڻỰID,���ڽ�����Ϣʱ�Ƚ�
	void	setKeyAndLogin(QString key);						// ����appkey����¼����ȡ��Key֮�󣬾Ϳ���ֱ�ӵ�¼��
	bool	IsLogin();											// �Ƿ��¼
	bool	IsCurChatRoom(QString chatID);						// �Ƿ��ǵ�ǰ�ỰID

	void	ShowMsgs(const std::vector<nim::IMMessage> &msg);
	void	ShowMsg(nim::IMMessage pMsg);
	void	ParseFace(QString qContect);						// �������յ�����Ϣ
	bool	IsHasFace(QString qContect);						// �ж��Ƿ��б���
	QString BuildFaceToUrl(QString qFace);						// ͨ�����鷵��url·�������磺����[em_1]����./images/em_1.gif��
	void	SetStudentName(int iNum);
	void	setAdaptHeight(int iHeight);						// ����Ӧ�߶�
	void	setResize(int iWidth, int iHeight);					// ��� �߶�
	bool	IsFous();
	void	UpLoadPicProcess(double iProcess);					// �ϴ�ͼƬ����
	void	SendStatus(nim::SendMessageArc* arcNew);			// ������Ϣ����״̬
public slots:
	void chickChage(int, QString, QString);
	bool AddStudent(QString iconUrl, QString name, QString accid);		//��ӳ�Ա
	void AddStudentNumbers(int num);									//��ӳ�Ա����
	void AddAnnouncement(QString announcement, QString time);			//��ӹ���
	void QueryGroup();													//��ѯȺ��Ա��Ϣ
	void QueryRecord(QString dtstr);
	void stepDays(QDateTime date);										//��ʷ��¼����
	void stepMsgDays(QDateTime dateTime);								//�����¼����
	void clearAll();													//��������¼�����桢Ⱥ��Ա
	void OnSendAnnouncements(QString Announcements);					//����Ⱥ����
	void QueryMember();
	void returnMember();
private:
	QToolButton* pPreMonthButton1;
	QToolButton* pPreMonthButton;
	bool		 m_bZoom;	// ����ֱ���������ֺ��Ƿ����Ź� 
};

#endif // UICHATROOM_H
