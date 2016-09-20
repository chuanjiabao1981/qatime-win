#ifndef UICHATROOM_H
#define UICHATROOM_H

#include <QWidget>
#include "ui_UIChatRoom.h"
#include "myemotionwidget.h"
#include <QLabel>

//---云信
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
	void stringToHtmlFilter(QString &str);					//QString转htlm
	void stringToHtml(QString &str, QColor crl);			//QString转htlm带颜色
	void imgPathToHtml(QString &path);						//edit 设置图片
public slots:
	void setBrow(QString path);

private:
	Ui::UIChatRoom ui;
	
	//表情框	
	MyEmotionWidget*				m_smallEmotionWidget;
	MyEmotionWidget*				m_normalEmotionWidget;
	QLabel*						    m_lableTitle;
	QString							m_borw;//当前选中的表情
	bool							m_isBorw;//是否有表情
	std::string						m_CurChatID;		// 当前会话窗口ID 
	QString							m_accid;			// 云信用户ID
	QString							m_token;			// 云信用户密码
	QString							m_appKey;			// 云信key
	bool							m_bLogin;			// 是否登录
	void initEmotion();
	
private slots:
	void clickTalk();									// 弹出聊天框
	void clickStudentList();							// 弹出学生列表
	void clickProclamation();							// 弹出讨论框
	void clickCleanText();								// 清屏
	void clickBrow();									// 表情按钮
	void clickNotes();									// 消息记录
	void clickSendMseeage();							// 发送消息

private:
private:
	void		initSDK();									// 初始化云信SDK
	bool		LoadConfig(const std::string& app_data_dir,
		const std::string& app_install_dir, nim::SDKConfig &config); //加载配置
	std::string GetJsonStringWithNoStyled(const QJsonObject& values);				   //json类型转换
	void		PackageMsg(nim::IMMessage &msg);
	/**
	* 登录结果回调
	* @param[in] json_params 返回的信息，Json数据转化的字符串
	* @param[in] user_data 登录时用户传的数据，sdk原封不动传给回调
	* @return void	无返回值
	*/
	static void OnLoginCallback(const nim::LoginRes& login_res, const void* user_data);

public:
	void setChatInfo(QJsonObject &chatInfo);				// 设置云信账户信息
	void ReceiverMsg(nim::IMMessage* pMsg);					// 接收服务器发送过来的消息
	void setCurChatID(QString chatID);						// 设置当前窗口会话ID,用于接收消息时比较
	void setKeyAndLogin(QString key);						// 设置appkey并登录（获取完Key之后，就可以直接登录）
	bool IsLogin();											// 是否登录
	bool IsCurChatRoom(QString chatID);						// 是否是当前会话ID
};

#endif // UICHATROOM_H
