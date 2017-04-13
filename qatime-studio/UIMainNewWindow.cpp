#include "UIMainNewWindow.h"
#include "windows.h"
#include "lesson.h"
#include "course.h"
#include "course1v1.h"
#include <QDir>
#include "define.h"
#include "UIMessageBox.h"

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

typedef	const wchar_t * (*nim_tool_get_user_appdata_dir)(const char * app_account);
typedef	void(*nim_global_free_buf)(void *data);

UIMainNewWindow::UIMainNewWindow(QWidget *parent)
	: QWidget(parent)
	, m_LoginWindow(NULL)
	, m_AuxiliaryWnd(NULL)
	, m_WindowSet(NULL)
{
	ui.setupUi(this);
	setWindowTitle(QApplication::translate("UIMainWindow", "UIMainWindow", 0));
	initSDK();

	m_WindowSet = new UIWindowSet();
	m_WindowSet->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	m_WindowSet->setMainWindow(this);
	m_WindowSet->hide();

	m_AuxiliaryWnd = new UIAuxiliaryWnd();
	m_AuxiliaryWnd->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	m_AuxiliaryWnd->setMainWindow(this);
	m_AuxiliaryWnd->show();
	SetWindowPos((HWND)m_AuxiliaryWnd->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

}

UIMainNewWindow::~UIMainNewWindow()
{
	if (m_AuxiliaryWnd)
	{
		delete m_AuxiliaryWnd;
		m_AuxiliaryWnd = NULL;
	}

	if (m_WindowSet)
	{
		delete m_WindowSet;
		m_WindowSet = NULL;
	}
}

void UIMainNewWindow::SetEnvironmental(bool EnvironmentalTyle)
{
	m_EnvironmentalTyle = EnvironmentalTyle;
	m_WindowSet->SetEnvironmental(m_EnvironmentalTyle);
	m_AuxiliaryWnd->SetEnvironmental(m_EnvironmentalTyle);
}

void UIMainNewWindow::setRemeberToken(const QString &token)
{
	mRemeberToken = token;
	m_WindowSet->SetToken(mRemeberToken);
	m_AuxiliaryWnd->SetToken(mRemeberToken);
}

void UIMainNewWindow::setTeacherInfo(QJsonObject &data)
{
	// 设置老师名字
	QString teacherName = data["name"].toString();
	m_AuxiliaryWnd->setTeacherName(teacherName);
	m_TeacherName = teacherName;

	int iID = data["id"].toInt();
	m_teacherID = QString::number(iID);
	m_WindowSet->setTeacherID(m_teacherID, teacherName);

	// 设置老师头像
	QString studentPhoto_url = data["avatar_url"].toString();
	m_TeacherPix = m_AuxiliaryWnd->setTeacherUrl(studentPhoto_url);

	// 云信信息
	QJsonObject obj = data["chat_account"].toObject();
	m_accid = obj["accid"].toString();
	m_token = obj["token"].toString();

	m_WindowSet->setAccid(m_accid);

	InitAudio();
}

void UIMainNewWindow::setAutoTeacherInfo(QString studentID, QString studentName, QString studentUrl, QString accid, QString token)
{
	m_teacherID = studentID;
	m_WindowSet->setTeacherID(m_teacherID, studentName);
	m_AuxiliaryWnd->setTeacherName(studentName);
	m_TeacherName = studentName;

	// 设置头像
	m_TeacherPix = m_AuxiliaryWnd->setTeacherUrl(studentUrl);

	// 学生云信信息
	m_accid = accid;
	m_token = token;

	m_WindowSet->setAccid(m_accid);

	InitAudio();
}

void UIMainNewWindow::ShowLesson()
{
	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl = "https://qatime.cn/api/v1/live_studio/teachers/{teacher_id}/schedule";
		strUrl.replace("{teacher_id}", m_teacherID);
	}
	else
	{
		strUrl = "http://testing.qatime.cn/api/v1/live_studio/teachers/{teacher_id}/schedule";
		strUrl.replace("{teacher_id}", m_teacherID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIMainNewWindow::LessonRequestFinished);
}

void UIMainNewWindow::LessonRequestFinished()
{
	int iCount = 0;
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonArray courses = obj["data"].toArray();
	foreach(const QJsonValue & value, courses)
	{
		QJsonObject obj = value.toObject();
		QJsonArray lessons = obj["lessons"].toArray();
		foreach(const QJsonValue & value, lessons)
		{
			Lesson *lesson = new Lesson();
			lesson->readJson(value.toObject());

			QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
			//		curTime = "2017-03-01";
			if (lesson->Date() == curTime)
			{
				if (m_AuxiliaryWnd)
					m_AuxiliaryWnd->AddTodayAuxiliary(lesson->name(), lesson->CourseID(), lesson->CourseName(), lesson->LessonTime(), lesson->ChinaLessonStatus(), lesson->LessonID());

				if (m_WindowSet)
					m_WindowSet->AddTodayToLesson(lesson->LessonID(), lesson->CourseID(), lesson->BoardUrl(), lesson->CameraUrl(),lesson->LessonTime(),lesson->ChinaLessonStatus(),lesson->name());
				
				iCount++;
			}

			delete lesson;
		}
	}

	if (iCount == 0)
	{
		if (m_AuxiliaryWnd)
			m_AuxiliaryWnd->AddTodayNoLesson();
	}

	ShowAuxiliary();
}

void UIMainNewWindow::ShowAuxiliary()
{
	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl = "https://qatime.cn/api/v1/live_studio/teachers/{teacher_id}/courses/full";
		strUrl.replace("{teacher_id}", m_teacherID);
	}
	else
	{
		strUrl = "http://testing.qatime.cn/api/v1/live_studio/teachers/{teacher_id}/courses/full";
		strUrl.replace("{teacher_id}", m_teacherID);
	}

	QByteArray append("?status=teaching&per_page=");
	append += "100";

	strUrl += append;
	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIMainNewWindow::AuxiliaryRequestFinished);
}

void UIMainNewWindow::AuxiliaryRequestFinished()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonArray courses = obj["data"].toArray();
	foreach(const QJsonValue & value, courses)
	{
		QJsonObject obj = value.toObject();
		Course *course = new Course();
		course->readJson(value.toObject());

		if (m_AuxiliaryWnd)
			m_AuxiliaryWnd->AddAuxiliary(course->PicUrl(), course->name(), course->Grade(), course->TeacherName(), course->ChatId(), course->id(), "",
			mRemeberToken, m_TeacherName, m_AudioPath, course->status());

		delete course;
	}

	if (m_AuxiliaryWnd)
		m_AuxiliaryWnd->LoadPic();
	Show1v1Auxiliary();
}

void UIMainNewWindow::Show1v1Auxiliary()
{
	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl = "https://qatime.cn/api/v1/live_studio/teachers/{teacher_id}/interactive_courses";
		strUrl.replace("{teacher_id}", m_teacherID);
	}
	else
	{
		strUrl = "http://testing.qatime.cn/api/v1/live_studio/teachers/{teacher_id}/interactive_courses";
		strUrl.replace("{teacher_id}", m_teacherID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIMainNewWindow::Return1v1AuxiliaryRequestFinished);
}

void UIMainNewWindow::Return1v1AuxiliaryRequestFinished()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonArray courses = obj["data"].toArray();
	foreach(const QJsonValue & value, courses)
	{
		QJsonObject obj = value.toObject();
		Course1v1 *course = new Course1v1();
		course->readJson(value.toObject());

		if (m_AuxiliaryWnd)
			m_AuxiliaryWnd->Add1v1Auxiliary(course->PicUrl(), course->name(), course->Grade(), m_TeacherName, course->ChatId(), course->id(), "",
			mRemeberToken, m_TeacherName, m_AudioPath, course->status());

		delete course;
	}

	if (m_AuxiliaryWnd)
		m_AuxiliaryWnd->LoadPic1v1();
	RequestKey();
}

void UIMainNewWindow::RequestKey()
{
	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl = "https://qatime.cn/api/v1/app_constant/im_app_key";
	}
	else
	{
		strUrl = "http://testing.qatime.cn/api/v1/app_constant/im_app_key";
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QString str = this->mRemeberToken;

	request.setRawHeader("Remember-Token", this->mRemeberToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &UIMainNewWindow::returnKey);
}

void UIMainNewWindow::returnKey()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1 && data.contains("im_app_key"))
	{
		QString key = data["im_app_key"].toString();
		setKeyAndLogin(key);
	}
	else if (obj["status"].toInt() == 0)
	{
//		RequestError(error);
	}
}

void UIMainNewWindow::setKeyAndLogin(QString key)
{
	// 登陆
	auto cb = std::bind(OnLoginCallback, std::placeholders::_1, nullptr);
	bool bFail = nim::Client::Login(key.toStdString(), m_accid.toStdString(), m_token.toStdString(), cb);
	if (!bFail)
	{
		CMessageBox::showMessage(QString("答疑时间"), QString("云信登录失败！"), QString("确定"), QString("取消"));
		return;
	}
}

void UIMainNewWindow::OnLoginCallback(const nim::LoginRes& login_res, const void* user_data)
{
}
void UIMainNewWindow::setLoginWindow(LoginWindow* parent)
{
	m_LoginWindow = parent;
}

void UIMainNewWindow::setVersion(QString version)
{
	if (m_AuxiliaryWnd)
		m_AuxiliaryWnd->setVersion(version);
}
void UIMainNewWindow::returnClick()
{
	if (m_LoginWindow)
		m_LoginWindow->ReturnLogin();
}

bool UIMainNewWindow::IsMayClose()
{
	if (m_WindowSet->IsLiving())
		return false;
	else
		return true;
}

void UIMainNewWindow::CloseDialog()
{
	if (m_LoginWindow)
		m_LoginWindow->CloseTray();

	exit(0);
}

void UIMainNewWindow::ShowCourse()
{
	if (m_AuxiliaryWnd)
	{
		if (m_AuxiliaryWnd->isVisible())
		{
			m_AuxiliaryWnd->showMinimized();
			m_AuxiliaryWnd->hide();
			return;
		}
		m_AuxiliaryWnd->showNormal();
	}
}

void UIMainNewWindow::CreateRoom(QString chatID, QString courseID, QString teacherID, QString token, QString studentName, std::string audioPath, QString courseName, int UnreadCount, QString status, bool b1v1Lesson)
{
	if (m_WindowSet)
		m_WindowSet->AddChatRoom(chatID, courseID, teacherID, token, studentName, audioPath, courseName, UnreadCount, status,b1v1Lesson);
}

QPixmap UIMainNewWindow::TeacherPhotoPixmap()
{
	return m_TeacherPix;
}

void UIMainNewWindow::InitAudio()
{
	// 语音消息初始化 audio
	std::string acc = m_accid.toStdString();
	const wchar_t *dir = NIM_SDK_GET_FUNC(nim_tool_get_user_appdata_dir)(acc.c_str());
	std::wstring app_data_audio_path = (std::wstring)dir;
	NIM_SDK_GET_FUNC(nim_global_free_buf)((void*)dir);
	QString audio_path = QString::fromStdWString(app_data_audio_path);
	QDir Dir(audio_path);
	if (!Dir.exists())
		Dir.mkdir(audio_path);
	std::string res_audio_path = audio_path.toStdString();
	bool ret = nim_audio::Audio::Init(res_audio_path);

	nim_audio::Audio::RegStopPlayCb(&UIMainNewWindow::OnStopAudioCallback);

	audio_path.append("audio\\");
	m_AudioPath = audio_path.toStdString();
}

void UIMainNewWindow::OnStopAudioCallback(int code, const char* file_path, const char* sid, const char* cid)
{
	QString strSid = QString(QLatin1String(sid));
	char* pData = new char[strlen(cid)];
	memcpy(pData, cid, strlen(cid));
	HWND hWnd = FindWindow(L"Qt5QWindowToolSaveBits", L"UIMainWindow");
	PostMessage(hWnd, MSG_SEND_AUDIO_MSG, (WPARAM)(int)strSid.toInt(), (LPARAM)pData);
}

bool UIMainNewWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	if ("windows_generic_MSG" == eventType)
	{
		PMSG pMsg = static_cast<PMSG>(message);
		switch (pMsg->message)
		{
		case MSG_UPDATE_MSG:
		{
			CloseDialog();
		}
		break;
		case MSG_CLIENT_RECEIVE:  // 接收聊天消息
		{
			MSG* Msg = pMsg;
			nim::IMMessage* pIMsg = (nim::IMMessage*)Msg->wParam;

			if (m_WindowSet)
			{
				if (!m_WindowSet->ReceiverMsg(pIMsg))
				{
					if (m_AuxiliaryWnd)
					{
						m_AuxiliaryWnd->ReceiverNumber(QString::fromStdString(pIMsg->local_talk_id_));
					}
				}
			}

			delete pIMsg;
			pIMsg = NULL;
		}
		break;
		case MSG_CLIENT_RECORD:  // 接收历史消息
		{
			qInfo("接收到历史消息");
			MSG* Msg = pMsg;
			nim::QueryMsglogResult* pIMsg = (nim::QueryMsglogResult*)Msg->wParam;
			int iTyle = Msg->lParam;

			if (m_WindowSet)
			{
				if (iTyle == 1)
				{
					std::vector<nim::IMMessage> vec;
					for (auto& msg : pIMsg->msglogs_)
					{
						vec.push_back(msg);
					}

					//msg倒序排列
					size_t len = vec.size();
					for (size_t i = len; i > 0; i--)
					{
						m_WindowSet->ReceiverChatMsg(&vec[i - 1]);
					}
				}
				else
				{
					m_WindowSet->ReceiverRecordMsg(pIMsg);
				}
			}

			delete pIMsg;
			pIMsg = NULL;
		}
		break;
		case MSG_SEND_AUDIO_MSG: // 语音播放结束
		{
			MSG* Msg = pMsg;
			char* msgid = (char*)Msg->lParam;
			int   sid = Msg->wParam;
			std::string strSid = QString::number(sid).toStdString();

			if (m_WindowSet)
				m_WindowSet->OnStopPlayAudio(strSid, msgid);
		}
		break;
		case MSG_LOGIN:  // 接收登录返回结果
		{
			MSG* Msg = pMsg;
			nim::LoginRes* pLogMsg = (nim::LoginRes*)Msg->wParam;

			if (m_WindowSet)
				m_WindowSet->ReceiverLoginMsg(pLogMsg);

			delete pLogMsg;
			pLogMsg;
		}
		break;
		case MSG_MEMBERS_INFO:  // 接收群成员信息
		{
			MSG* Msg = pMsg;
			std::list<nim::TeamMemberProperty>* pMemberMsg = (std::list<nim::TeamMemberProperty>*)Msg->wParam;
			if (pMemberMsg->size() == 0)
			{
				delete pMemberMsg;
				return false;
			}

			std::string sid = pMemberMsg->front().GetTeamID();

			if (m_WindowSet)
				m_WindowSet->ReceiverMemberMsg(sid, pMemberMsg);

			delete pMemberMsg;
			pMemberMsg = NULL;
		}
		break;
		case MSG_PICTURE_PROCESS:
		{
			double iUpSize = (double)pMsg->wParam;
			double iFileSize = (double)pMsg->lParam;
			double diff = iUpSize / iFileSize;
			// 			if (m_charRoom)
			// 			{
			// 				m_charRoom->UpLoadPicProcess(diff);
			// 			}
		}
		break;
		case  MSG_SEND_MSG_STATUS:
		{
			nim::SendMessageArc* arcNew = (nim::SendMessageArc*)pMsg->wParam;
			if (arcNew)
			{
				if (m_WindowSet)
					m_WindowSet->SendStatus(arcNew);

				delete arcNew;
				arcNew = NULL;
			}
		}
		break;
// 		case MSG_SEND_AUDIO_FINISH_MSG:
// 		{
// 			MyAudioStruct* audio = (MyAudioStruct*)pMsg->wParam;
// 			if (audio)
// 			{
// 				if (m_WindowSet)
// 					m_WindowSet->SendAudio(audio->sSessionID, audio->sMsgID, audio->sFilePath, audio->fileSize, audio->duration, audio->sFileEx);
// 
// 				delete audio;
// 				audio = NULL;
// 			}
// 		}
//		break;
// 		case MSG_LOAD_AUDIO_SUC_MSG:
// 		{
// 			char* chatid = (char*)pMsg->wParam;
// 			char* msgid = (char*)pMsg->lParam;
// 
// 			if (m_WindowSet)
// 				m_WindowSet->ReceiverAudioStatus(chatid, msgid, true);
// 		}
// 		break;
// 		case MSG_LOAD_AUDIO_FAIL_MSG:
// 		{
// 			char* chatid = (char*)pMsg->wParam;
// 			char* msgid = (char*)pMsg->lParam;
// 
// 			if (m_WindowSet)
// 				m_WindowSet->ReceiverAudioStatus(chatid, msgid, false);
// 		}
// 		break;
		default:
			return false;
			break;
		}
	}
	return false;
}
//////////////////////////////添加云信功能////////////////////////////////
void UIMainNewWindow::initSDK()
{
	nim::SDKConfig config;

	//sdk能力参数（必填）
	config.database_encrypt_key_ = "Netease"; //string（db key必填，目前只支持最多32个字符的加密密钥！建议使用32个字符）
	bool ret = false;
	ret = nim::SDKFunction::LoadSdkDll();
	if (!ret)
		CMessageBox::showMessage(QString("答疑时间"), QString("加载云信SDK失败！"), QString("确定"), QString("取消"));

	ret = LoadConfig("Netease", "", config);
	if (!ret)
		CMessageBox::showMessage(QString("答疑时间"), QString("加载云信Config失败！"), QString("确定"), QString("取消"));

	nim_http::Init(); // 初始化云信http

	// 接受消息回调
	nim::Talk::RegReceiveCb(&nim_comp::TalkCallback::OnReceiveMsgCallback);
	// 发送消息状态回调
	nim::Talk::RegSendMsgCb(&nim_comp::TalkCallback::OnSendMsgCallback);
}

bool UIMainNewWindow::LoadConfig(const std::string& app_data_dir, const std::string& app_install_dir, nim::SDKConfig &config)
{
	QJsonObject config_root;
	QJsonObject config_values;
	config_values[kNIMDataBaseEncryptKey] = config.database_encrypt_key_.c_str();//string（db key必填，目前只支持最多32个字符的加密密钥！建议使用32个字符）
	config_values[kNIMPreloadAttach] = config.preload_attach_;        //bool （选填，是否需要预下载附件缩略图， sdk默认预下载）
	config_values[kNIMSDKLogLevel] = config.sdk_log_level_;
	config_root[kNIMGlobalConfig] = config_values;

	return NIM_SDK_GET_FUNC(nim_client_init)(app_data_dir.c_str(), app_install_dir.c_str(), GetJsonStringWithNoStyled(config_root).c_str());
}

std::string UIMainNewWindow::GetJsonStringWithNoStyled(const QJsonObject& values)
{
	QJsonDocument document;
	document.setObject(values);

	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	QString json_str(byte_array);
	return json_str.toStdString();
}

void UIMainNewWindow::changeLessonStatus(QString id, QString status)
{
	m_AuxiliaryWnd->ChangeAuxiliaryTodayStatus(id, status);
}