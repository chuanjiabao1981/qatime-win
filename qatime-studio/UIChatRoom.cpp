#include "UIChatRoom.h"
#include <QDir>
#include <QTime>
#include <QMovie>
#include "UIMessageBox.h"

#include "YxChat/nim_sdk_helper.h"
#include "YxChat/session_callback.h"
#include "YxChat/nim_cpp_client.h"
#include "define.h"

typedef bool(*nim_client_init)(const char *app_data_dir, const char *app_install_dir, const char *json_extension);
typedef void(*nim_client_cleanup)(const char *json_extension);
typedef void(*nim_client_login)(const char *app_token, const char *account, const char *password, const char *json_extension, nim_json_transport_cb_func cb, const void* user_data);
typedef void(*nim_client_relogin)(const char *json_extension);
typedef void(*nim_client_logout)(NIMLogoutType logout_type, const char *json_extension, nim_json_transport_cb_func cb, const void* user_data);
typedef void(*nim_client_kick_other_client)(const char *json_extension);
typedef void(*nim_client_reg_auto_relogin_cb)(const char *json_extension, nim_json_transport_cb_func cb, const void* user_data);
typedef void(*nim_client_reg_kickout_cb)(const char *json_extension, nim_json_transport_cb_func cb, const void* user_data);
typedef void(*nim_client_reg_disconnect_cb)(const char *json_extension, nim_json_transport_cb_func cb, const void* user_data);
typedef void(*nim_client_reg_multispot_login_notify_cb)(const char *json_extension, nim_json_transport_cb_func cb, const void *user_data);
typedef void(*nim_client_reg_kickout_other_client_cb)(const char *json_extension, nim_json_transport_cb_func cb, const void *user_data);
typedef void(*nim_client_reg_sync_multiport_push_config_cb)(const char *json_extension, nim_client_multiport_push_config_cb_func cb, const void *user_data);
typedef void(*nim_client_set_multiport_push_config)(const char *switch_content, const char *json_extension, nim_client_multiport_push_config_cb_func cb, const void *user_data);
typedef void(*nim_client_get_multiport_push_config)(const char *json_extension, nim_client_multiport_push_config_cb_func cb, const void *user_data);

UIChatRoom::UIChatRoom(QWidget *parent)
	: QWidget(parent)
	, m_CurChatID("")
	, m_bLogin(false)
{
	ui.setupUi(this);
	setWindowTitle("QATIME");
	connect(ui.button_talk, SIGNAL(clicked()), this, SLOT(clickTalk()));
	connect(ui.button_proclamation, SIGNAL(clicked()), this, SLOT(clickProclamation()));
	connect(ui.button_studentList, SIGNAL(clicked()), this, SLOT(clickStudentList()));
	connect(ui.button_cleanText, SIGNAL(clicked()), this, SLOT(clickCleanText()));
	connect(ui.button_brow, SIGNAL(clicked()), this, SLOT(clickBrow()));
	connect(ui.button_notes, SIGNAL(clicked()), this, SLOT(clickNotes()));
	connect(ui.button_sendMseeage, SIGNAL(clicked()), this, SLOT(clickSendMseeage()));
	connect(ui.student_list, SIGNAL(signalChickChage(bool, QString, QString)), this, SLOT(chickChage(bool, QString,QString)));
	initEmotion();
	this->clickTalk();	
	m_isBorw = false;
	
	initSDK();
}

UIChatRoom::~UIChatRoom()
{

}
// 弹出聊天框
void UIChatRoom::clickTalk()
{
	ui.text_proclamation->setHidden(true);
	ui.text_talk->setHidden(false);
	ui.student_list->setHidden(true);
	ui.text_talk->moveCursor(QTextCursor::End);
}
// 弹出学生列表
void UIChatRoom::clickStudentList()
{
	//ui.student_list->
	ui.text_proclamation->setHidden(true);
	ui.text_talk->setHidden(true);
	ui.student_list->setHidden(false);
	ui.student_list->initMenu();
}
// 弹出讨论框
void UIChatRoom::clickProclamation()
{
	ui.text_proclamation->setHidden(false);
	ui.text_talk->setHidden(true);
	ui.student_list->setHidden(true);
}
// 清屏
void UIChatRoom::clickCleanText()
{
	ui.text_talk->clear();
	ui.student_list->cleanStudents();
}
// 表情按钮
void UIChatRoom::clickBrow()
{
	if (m_smallEmotionWidget->isHidden())
	{
		m_smallEmotionWidget->show();
	}
	else
	{
		m_smallEmotionWidget->setHidden(true);
	}
	
	m_smallEmotionWidget->move(60, 440);
}
// 消息记录
void UIChatRoom::clickNotes()
{

}
//加载所有表情
void UIChatRoom::initEmotion()
{
	// 初始化小表情框;
	m_smallEmotionWidget = new MyEmotionWidget(this);
	connect(m_smallEmotionWidget, SIGNAL(emitFileName(QString)), this, SLOT(setBrow(QString)));
	m_smallEmotionWidget->setRowAndColumn(10, 8);
	m_smallEmotionWidget->setEmotionSize(QSize(32, 32));
	m_smallEmotionWidget->setEmotionMovieSize(QSize(24, 24));
	m_smallEmotionWidget->setMaxRow(4);
	m_smallEmotionWidget->initTableWidget();
	QString path = "./images/em_";
	QString path1 = ".gif";
	for (int i = 1; i <= 75; i++)
	{
		m_smallEmotionWidget->addEmotionItem(path + QString::number(i) + path1, "");
		//		m_smallEmotionWidget->addEmotionItem(dir.currentPath()+"./images/em_1.gif", "");
	}
	m_smallEmotionWidget->setVisible(false);
	// 初始化正常表情框;
// 	m_normalEmotionWidget = new MyEmotionWidget;
// 	m_normalEmotionWidget->setRowAndColumn(10, 14);
// 	m_normalEmotionWidget->setEmotionSize(QSize(32, 32));
// 	m_normalEmotionWidget->setEmotionMovieSize(QSize(24, 24));
// 	m_normalEmotionWidget->setMaxRow(6);
// 	m_normalEmotionWidget->initTableWidget();
// 	m_normalEmotionWidget->setVisible(false);
// 	for (int i = 1; i <= 75; i++)
// 	{
// 		//		m_normalEmotionWidget->addEmotionItem(path.arg(i + 1), "");
// 	}	

	// 	m_lableTitle = new QLabel;
	// 	QVBoxLayout* vLayout = new QVBoxLayout;
	// 	vLayout->addWidget(m_lableTitle);
	// 	vLayout->addWidget(m_smallEmotionWidget);
	// 	vLayout->addWidget(m_normalEmotionWidget);
	// 	this->setLayout(vLayout);

}


void UIChatRoom::stringToHtmlFilter(QString &str)
{
	//注意这几行代码的顺序不能乱，否则会造成多次替换
	str.replace("&", "&amp;");
	str.replace(">", "&gt;");
	str.replace("<", "&lt;");
	str.replace("\"", "&quot;");
	str.replace("\'", "&#39;");
	str.replace(" ", "&nbsp;");
	str.replace("\n", "<br>");
	str.replace("\r", "<br>");
}

void UIChatRoom::stringToHtml(QString &str, QColor crl)
{
	QByteArray array;
	array.append(crl.red());
	array.append(crl.green());
	array.append(crl.blue());
	QString strC(array.toHex());
	str = QString("<span style=\" color:#%1;\">%2</span>").arg(strC).arg(str);
}

void UIChatRoom::imgPathToHtml(QString &path)
{
	path = QString("<img src=\"%1\"/>").arg(path);
}

void UIChatRoom::setBrow(QString path)
{
	m_smallEmotionWidget->hide();
	m_borw.append(path);
	ui.textEdit->insertHtml("<img src='" + path + "'/>");  //   此处的test 即 url
	ui.textEdit->addAnimation(QUrl(path), path);  //添加一个动画.
	m_isBorw = true;
}

void UIChatRoom::clickSendMseeage()
{
	QString tempText = ui.textEdit->toPlainText();
	QStringList textList = tempText.split("?");
	QString sendText,sendMsg;
	for (int i = 0; i < textList.size(); i++)
	{
		if (i == textList.size() - 1)
		{
			sendText.append(textList.at(i));
			sendMsg.append(textList.at(i));
			break;
		}
		sendText.append(textList.at(i));
		sendMsg.append(textList.at(i));
		sendMsg.append("[" + m_borw.at(i).split("/").at(2).split(".").at(0) + "]");
	}
	if (textList.size() >= 2 || !sendText.isEmpty())
	{
		QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
		QString timeStr = time.toString("MM-dd hh:mm:ss");
		QString imgPath = "./images/username.png";
		imgPathToHtml(imgPath);
		stringToHtmlFilter(sendText);
		if (m_isBorw != false)
		{
			ui.text_talk->append("(自己) " + timeStr);
			ui.text_talk->append("");
			for (int i = 0; i < m_borw.count(); i++)
			{
				ui.text_talk->insertHtml("<img src='" + m_borw.at(i) + "'/>");  //   此处的test 即 url
				ui.text_talk->addAnimation(QUrl(m_borw.at(i)), m_borw.at(i));  //添加一个动画.				
			}
			ui.text_talk->insertHtml(sendText);			
			m_isBorw = false;
		}
		else
		{
			QString qName = "(自己)";
			ui.text_talk->append(qName + " " + timeStr);
			ui.text_talk->append(sendText);
		}

		ui.text_talk->append("");
		ui.text_talk->acceptRichText();
		ui.text_talk->moveCursor(QTextCursor::End);
		ui.textEdit->clear();
	}
	else
	{
		CMessageBox::showMessage(
			QString("答疑时间"),
			QString("不能发送空数据！"),
			QString("确定"),
			QString());
		return;
	}

	// 发往服务器
	nim::IMMessage msg;
	PackageMsg(msg);
	msg.type_ = nim::kNIMMessageTypeText;
	msg.content_ = sendMsg.toStdString();
	
	nim::MessageSetting setting;
	std::string json_msg = nim::Talk::CreateTextMessage(msg.receiver_accid_, msg.session_type_, msg.client_msg_id_, msg.content_, setting, msg.timetag_);
	nim::Talk::SendMsg(json_msg);
	m_borw.clear();
}

void UIChatRoom::PackageMsg(nim::IMMessage &msg)
{
	msg.session_type_ = nim::kNIMSessionTypeTeam;
	msg.receiver_accid_ = m_CurChatID;//会话ID
	msg.sender_accid_ = m_accid.toStdString();//用户名
	msg.client_msg_id_ = nim::Client::GetUuid();
	msg.msg_setting_.resend_flag_ = nim::BS_FALSE;

	//base获取的时间单位是s，服务器的时间单位是ms
	msg.timetag_ = QDateTime::currentMSecsSinceEpoch();

	msg.status_ = nim::kNIMMsgLogStatusSending;
}

void UIChatRoom::ReceiverMsg(nim::IMMessage* pMsg)
{
	// 判断当前过来的消息，是不是此会话窗口
	if (strcmp(pMsg->local_talk_id_.c_str(), m_CurChatID.c_str()) == 0)
	{
		std::string strName = pMsg->readonly_sender_nickname_;
		std::string strContent = pMsg->content_;

		__int64 time = pMsg->timetag_;
		QString qTime = QDateTime::fromMSecsSinceEpoch(time).toString("MM-dd hh:mm:ss");// 原型yyyy-MM-dd hh:mm:ss
		QString qName = QString::fromStdString(strName);
		QString qContent = QString::fromStdString(strContent);

		ui.text_talk->append(qName + " " + qTime);
		ui.text_talk->append(qContent);
		ui.text_talk->append("");
	}
}

//////////////////////////////添加云信功能////////////////////////////////
void UIChatRoom::initSDK()
{
	nim::SDKConfig config;

	//sdk能力参数（必填）
	config.database_encrypt_key_ = "Netease"; //string（db key必填，目前只支持最多32个字符的加密密钥！建议使用32个字符）
	bool ret = false;
	ret = nim::SDKFunction::LoadSdkDll();
	ret = LoadConfig("Netease", "", config);

	if (!ret)
		assert(0);// 初始化失败

	nim_http::Init(); // 初始化云信http

	// 接受消息回调
	nim::Talk::RegReceiveCb(&nim_comp::TalkCallback::OnReceiveMsgCallback);
}

void UIChatRoom::OnLoginCallback(const nim::LoginRes& login_res, const void* user_data)
{
	bool bsuc = true;

	if (login_res.res_code_ == nim::kNIMResSuccess)
		bsuc = true;
	else
		bsuc = false;
}

bool UIChatRoom::LoadConfig(const std::string& app_data_dir, const std::string& app_install_dir, nim::SDKConfig &config)
{
	QJsonObject config_root;
	QJsonObject config_values;
	config_values[kNIMDataBaseEncryptKey] = config.database_encrypt_key_.c_str();//string（db key必填，目前只支持最多32个字符的加密密钥！建议使用32个字符）
	config_values[kNIMPreloadAttach] = config.preload_attach_;        //bool （选填，是否需要预下载附件缩略图， sdk默认预下载）
	config_values[kNIMSDKLogLevel] = config.sdk_log_level_;
	config_root[kNIMGlobalConfig] = config_values;

	return NIM_SDK_GET_FUNC(nim_client_init)(app_data_dir.c_str(), app_install_dir.c_str(), GetJsonStringWithNoStyled(config_root).c_str());
}

std::string UIChatRoom::GetJsonStringWithNoStyled(const QJsonObject& values)
{
	QJsonDocument document;
	document.setObject(values);

	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	QString json_str(byte_array);
	return json_str.toStdString();
}

void UIChatRoom::setCurChatID(QString chatID)
{
	m_CurChatID = chatID.toStdString();
}

void UIChatRoom::setChatInfo(QJsonObject &chatInfo)
{
	m_accid = chatInfo["accid"].toString();
	m_token = chatInfo["token"].toString();
}

void UIChatRoom::setKeyAndLogin(QString key)
{
	m_appKey = key;

	// 登陆
	auto cb = std::bind(OnLoginCallback, std::placeholders::_1, nullptr);
	bool bFail = nim::Client::Login(m_appKey.toStdString(), m_accid.toStdString(), m_token.toStdString(), cb);
	if (!bFail)
	{
		CMessageBox::showMessage(QString("答疑时间"),QString("失败！"),QString("确定"),QString("取消"));
		return;
	}

	m_bLogin = true;
}

bool UIChatRoom::IsLogin()
{
	return m_bLogin;
}

bool UIChatRoom::IsCurChatRoom(QString chatID)
{
	bool bChatRoom = false;
	std::string strChatID = chatID.toStdString();

	// 判断当前过来的消息，是不是此会话窗口
	if (strcmp(strChatID.c_str(), m_CurChatID.c_str()) == 0)
	{
		bChatRoom = true;
	}

	return bChatRoom;
}

void UIChatRoom::OnTeamEventCallback(const nim::TeamEvent& result)
{
}

//?????????
void UIChatRoom::chickChage(bool b, QString qAccid, QString name)
{
	std::string accid = qAccid.toStdString();
	auto cb = std::bind(OnTeamEventCallback, std::placeholders::_1);
	nim::Team::MuteMemberAsync(m_CurChatID, accid, b, cb);

	// ?????????
	if (b)
		ui.text_talk->append(name + "  ??????");
	else
		ui.text_talk->append(name + "  ?????????");

	ui.text_talk->append("");
}

void UIChatRoom::AddStudent(QString iconUrl, QString name, QString accid)
{
	ui.student_list->addStrdent(iconUrl, name, accid);
}