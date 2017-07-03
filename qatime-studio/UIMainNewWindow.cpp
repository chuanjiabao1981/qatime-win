#include "UIMainNewWindow.h"
#include "windows.h"
#include "lesson.h"
#include "course.h"
#include "course1v1.h"
#include <QDir>
#include "UIMessageBox.h"
#include "IMInterface.h"

UIMainNewWindow* m_This = NULL;
UIMainNewWindow::UIMainNewWindow(QWidget *parent)
	: QWidget(parent)
	, m_LoginWindow(NULL)
	, m_AuxiliaryWnd(NULL)
	, m_WindowSet(NULL)
{
	ui.setupUi(this);
	m_This = this;
	setWindowTitle(QApplication::translate("UIMainWindow", "UIMainWindow", 0));

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
			m_AuxiliaryWnd->AddTodayNoLesson(UIAuxiliaryWnd::EN_TODAY_LESSON);
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

	QByteArray append("?status=teaching&per_page=");//
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
	int iCount = 0;
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
			mRemeberToken, m_TeacherName, m_AudioPath, course->status(),course->url(), course->CameraURL());

		iCount++;
		delete course;
	}

	if (m_AuxiliaryWnd)
		m_AuxiliaryWnd->LoadPic();

	if (iCount == 0)
	{
		if (m_AuxiliaryWnd)
			m_AuxiliaryWnd->AddTodayNoLesson(UIAuxiliaryWnd::EN_ALL_LESSON);
	}

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
	int iCount = 0;
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

		iCount++;
		delete course;
	}

	if (m_AuxiliaryWnd)
		m_AuxiliaryWnd->LoadPic1v1();

	if (iCount == 0)
	{
		if (m_AuxiliaryWnd)
			m_AuxiliaryWnd->AddTodayNoLesson(UIAuxiliaryWnd::EN_1V1_LESSON);
	}
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
	nim::SDKConfig config;
	config.database_encrypt_key_ = "Netease";
	//sdk能力参数（必填）
	bool bSuc = nim::Client::Init(key.toStdString(), "", "", config);

	nim_http::Init(); // 初始化云信http

	auto Mcb = std::bind(OnMultispotLoginCallback, std::placeholders::_1);
	nim::Client::RegMultispotLoginCb(Mcb);

	auto Kcb = std::bind(OnKickoutCallback, std::placeholders::_1);
	nim::Client::RegKickoutCb(Kcb);

	InitAudio();	  // 初始化语音
	m_WindowSet->initCallBack();

	auto cb = std::bind(OnLoginCallback, std::placeholders::_1, nullptr);
	nim::Client::Login(key.toStdString(), m_accid.toStdString(), m_token.toStdString(), cb);

	// 查询课程
	ShowLesson();
}

void UIMainNewWindow::OnLoginCallback(const nim::LoginRes& login_res, const void* user_data)
{
	m_This->m_WindowSet->ReceiverLoginMsg(login_res);
	if (login_res.res_code_ == nim::kNIMResSuccess) // 登录成功
	{
		if (login_res.login_step_ == nim::kNIMLoginStepLogin)
		{
		}
	}
	else
	{
		int ErrorCode = login_res.res_code_;
		QString sError = "云信登录失败，错误码";
		sError += QString::number(ErrorCode);
		CMessageBox::showMessage(QString("答疑时间"), QString(sError), QString("确定"), QString("取消"));
	}
}
//多端
void UIMainNewWindow::OnMultispotLoginCallback(const nim::MultiSpotLoginRes& res)
{
	bool online = res.notify_type_ == nim::kNIMMultiSpotNotifyTypeImIn;
	if (!res.other_clients_.empty())
	{
	}
}
void UIMainNewWindow::OnKickoutCallback(const nim::KickoutRes& res)
{
	int i = 0;
	if (res.client_type_ == nim::kNIMClientTypePCWindows)
	{
		i++;
	}
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

	LogOut();
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

void UIMainNewWindow::CreateRoom(QString chatID, QString courseID, QString teacherID, QString token, QString studentName, std::string audioPath, QString courseName, 
						int UnreadCount, QString status, QString boardurl, QString cameraUrl, bool b1v1Lesson)
{
	if (m_WindowSet)
		m_WindowSet->AddChatRoom(chatID, courseID, teacherID, token, studentName, audioPath, courseName, UnreadCount, status, boardurl, cameraUrl, b1v1Lesson);
}

QPixmap UIMainNewWindow::TeacherPhotoPixmap()
{
	return m_TeacherPix;
}

void UIMainNewWindow::InitAudio()
{
	// 语音消息初始化 audio
	std::string acc = m_accid.toStdString();
	std::string app_data_audio_path = nim::Tool::GetUserAppdataDir(acc.c_str());
	QString audio_path = QString::fromStdString(app_data_audio_path);
	QDir Dir(audio_path);
	if (!Dir.exists())
		Dir.mkdir(audio_path);
	std::wstring res_audio_path = audio_path.toStdWString();
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
	
	PostMessage((HWND)m_This->winId(), MSG_SEND_AUDIO_MSG, (WPARAM)(int)strSid.toInt(), (LPARAM)pData);
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
				m_WindowSet->ReceiverMsg(pIMsg);
			}

			delete pIMsg;
			pIMsg = NULL;
		}
		break;
		case MSG_CLIENT_RECORD:  // 接收历史消息
		{
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

			m_WindowSet->m_bQueryMsg = false;
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
// 		case MSG_LOGIN:  // 接收登录返回结果
// 		{
// 			MSG* Msg = pMsg;
// 			nim::LoginRes* pLogMsg = (nim::LoginRes*)Msg->wParam;
// 
// 			if (m_WindowSet)
// 				m_WindowSet->ReceiverLoginMsg(pLogMsg);
// 
// 			delete pLogMsg;
// 			pLogMsg;
// 		}
// 		break;
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
void UIMainNewWindow::changeLessonStatus(QString id, QString status)
{
	m_AuxiliaryWnd->ChangeAuxiliaryTodayStatus(id, status);
}

void UIMainNewWindow::changeMsgNumber(QString chid)
{
	if (m_AuxiliaryWnd)
		m_AuxiliaryWnd->ReceiverNumber(chid);
}

void UIMainNewWindow::LogOut()
{
	nim::VChat::Cleanup();
	nim_http::Uninit();
	exit(0);

// 	auto cb = std::bind(OnLogOutCallback, std::placeholders::_1);
// 	nim::Client::Logout(nim::kNIMLogoutAppExit, cb);
}

void UIMainNewWindow::OnLogOutCallback(nim::NIMResCode res_code)
{
	qInfo() << "OnLogoutCallback:" << res_code;
	
	nim::VChat::Cleanup();
	nim_http::Uninit();
	exit(0);
}