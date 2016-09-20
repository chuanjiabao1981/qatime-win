#ifndef UICHATROOM_H
#define UICHATROOM_H

#include <QWidget>
#include "ui_UIChatRoom.h"
#include "myemotionwidget.h"
#include <QLabel>

//---����
#include "nim_client_def.h"
#include "assert.h"
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include "YxChat/nim_tools_http_cpp_wrapper.h"
#include "YxChat/nim_client_helper.h"
#include "YxChat/nim_cpp_talk.h"

class UIChatRoom : public QWidget
{
	Q_OBJECT

public:
	UIChatRoom(QWidget *parent = 0);
	~UIChatRoom();
	void stringToHtmlFilter(QString &str);					//QStringתhtlm
	void stringToHtml(QString &str, QColor crl);			//QStringתhtlm����ɫ
	void imgPathToHtml(QString &path);						//edit ����ͼƬ
public slots:
	void setBrow(QString path);

private:
	Ui::UIChatRoom ui;
	
	//�����	
	MyEmotionWidget*				m_smallEmotionWidget;
	MyEmotionWidget*				m_normalEmotionWidget;
	QLabel*						    m_lableTitle;
	QString							m_borw;//��ǰѡ�еı���
	bool							m_isBorw;//�Ƿ��б���
	std::string						m_CurChatID;		// ��ǰ�Ự����ID 
	QString							m_accid;			// �����û�ID
	QString							m_token;			// �����û�����
	QString							m_appKey;			// ����key
	bool							m_bLogin;			// �Ƿ��¼
	void initEmotion();
	
private slots:
	void clickTalk();									// ���������
	void clickStudentList();							// ����ѧ���б�
	void clickProclamation();							// �������ۿ�
	void clickCleanText();								// ����
	void clickBrow();									// ���鰴ť
	void clickNotes();									// ��Ϣ��¼
	void clickSendMseeage();							// ������Ϣ

private:
private:
	void		initSDK();									// ��ʼ������SDK
	bool		LoadConfig(const std::string& app_data_dir,
		const std::string& app_install_dir, nim::SDKConfig &config); //��������
	std::string GetJsonStringWithNoStyled(const QJsonObject& values);				   //json����ת��
	void		PackageMsg(nim::IMMessage &msg);
	/**
	* ��¼����ص�
	* @param[in] json_params ���ص���Ϣ��Json����ת�����ַ���
	* @param[in] user_data ��¼ʱ�û��������ݣ�sdkԭ�ⲻ�������ص�
	* @return void	�޷���ֵ
	*/
	static void OnLoginCallback(const nim::LoginRes& login_res, const void* user_data);

public:
	void setChatInfo(QJsonObject &chatInfo);				// ���������˻���Ϣ
	void ReceiverMsg(nim::IMMessage* pMsg);					// ���շ��������͹�������Ϣ
	void setCurChatID(QString chatID);						// ���õ�ǰ���ڻỰID,���ڽ�����Ϣʱ�Ƚ�
	void setKeyAndLogin(QString key);						// ����appkey����¼����ȡ��Key֮�󣬾Ϳ���ֱ�ӵ�¼��
	bool IsLogin();											// �Ƿ��¼
	bool IsCurChatRoom(QString chatID);						// �Ƿ��ǵ�ǰ�ỰID
};

#endif // UICHATROOM_H
