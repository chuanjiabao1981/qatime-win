#include "Livestatusmanager.h"
#include "define.h"
#include "UIMessageBox.h"

#define GET_RTMP_FAIL_COUNT		5	// 获取推流地址失败重试次数
#define HEARTBEAT_FAIL_COUNT	5	// 直播心跳失败重试次数
#define STOP_LIVE_FAIL_COUNT	5	// 停止推流失败重试次数

#ifdef TEST
#define _DEBUG
#else
#endif
LiveStatusManager::LiveStatusManager(QObject *parent)
	: QObject(parent)
	, m_TGetRtmpTimer(NULL)
	, m_HeartTimer(NULL)
	, m_HeartFailTimer(NULL)
	, m_StopFailTimer(NULL)
	, m_SwitchFailTimer(NULL)
	, m_iBeatStep(0)
	, m_newParent(NULL)
	, m_EnvironmentalTyle(true)
	, m_1v1HeartTimer(NULL)
{
	m_iGetRtmpCount = GET_RTMP_FAIL_COUNT;
	m_TGetRtmpTimer = new QTimer(this);
	connect(m_TGetRtmpTimer, SIGNAL(timeout()), this, SLOT(GetRtmpFailTimer()));
	
	m_HeartTimer = new QTimer(this);
	connect(m_HeartTimer, SIGNAL(timeout()), this, SLOT(HeartBeatTimer()));

	m_1v1HeartTimer = new QTimer(this);
	connect(m_1v1HeartTimer, SIGNAL(timeout()), this, SLOT(HeartBeat1v1Timer()));

	m_iHeartCount = HEARTBEAT_FAIL_COUNT;
	m_HeartFailTimer = new QTimer(this);
	connect(m_HeartFailTimer, SIGNAL(timeout()), this, SLOT(HeartBeatFailTimer()));

	m_1v1HeartFailTimer = new QTimer(this);
	connect(m_1v1HeartFailTimer, SIGNAL(timeout()), this, SLOT(HeartBeatFailTimer1v1()));

	m_iStopLiveCount = STOP_LIVE_FAIL_COUNT;
	m_StopFailTimer = new QTimer(this);
	connect(m_StopFailTimer, SIGNAL(timeout()), this, SLOT(StopLiveFailTimer()));

	m_SwitchFailTimer = new QTimer(this);
	connect(m_SwitchFailTimer, SIGNAL(timeout()), this, SLOT(SwitchFailTimer()));
}

LiveStatusManager::~LiveStatusManager()
{
	//删除定时器
	if (m_TGetRtmpTimer)
	{
		if (m_TGetRtmpTimer->isActive())
			m_TGetRtmpTimer->stop();
		delete m_TGetRtmpTimer;
		m_TGetRtmpTimer = NULL;
	}

	//删除定时器
	if (m_HeartTimer)
	{
		if (m_HeartTimer->isActive())
			m_HeartTimer->stop();
		delete m_HeartTimer;
		m_HeartTimer = NULL;
	}

	//删除定时器
	if (m_HeartFailTimer)
	{
		if (m_HeartFailTimer->isActive())
			m_HeartFailTimer->stop();
		delete m_HeartFailTimer;
		m_HeartFailTimer = NULL;
	}

	//删除定时器
	if (m_StopFailTimer)
	{
		if (m_StopFailTimer->isActive())
			m_StopFailTimer->stop();
		delete m_StopFailTimer;
		m_StopFailTimer = NULL;
	}

	//删除定时器
	if (m_SwitchFailTimer)
	{
		if (m_SwitchFailTimer->isActive())
			m_SwitchFailTimer->stop();
		delete m_SwitchFailTimer;
		m_SwitchFailTimer = NULL;
	}
}

void LiveStatusManager::setMainWindow(UIWindowSet* parent)
{
	m_newParent = parent;
}

void LiveStatusManager::GetRtmpFailTimer()
{
	if (m_TGetRtmpTimer)
	{
		m_TGetRtmpTimer->stop();
		GetRtmpAddressAndHeartBeat(m_lessonID, m_sToken);
	}
}

// 获取白板、摄像头、心跳间隔
void LiveStatusManager::GetRtmpAddressAndHeartBeat(QString lessonID, QString sToken)
{
	m_lessonID = lessonID;
	m_sToken = sToken;
	QString strUrl;
#ifdef _DEBUG
	strUrl += m_homePage;
	strUrl = "/api/v1/live_studio/lessons/{id}/live_info";
	strUrl.replace("{id}", lessonID);
#else
	strUrl += m_homePage;
	strUrl = "/api/v1/live_studio/lessons/{id}/live_info";
	strUrl.replace("{id}", lessonID);
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	request.setRawHeader("Remember-Token", sToken.toUtf8());
	reply = manager.get(request);
	connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::ReturnRtmpAddressAndHeartBeat);
}

void LiveStatusManager::ReturnRtmpAddressAndHeartBeat()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1 )
	{
		QString sBoardRtmp = data["board_push_stream"].toString();
		QString sCameraRtmp = data["camera_push_stream"].toString();
		m_iBeatStep = data["beat_step"].toInt();
		
// 		if (m_parent)
// 			m_parent->setBoradCamera(sBoardRtmp, sCameraRtmp);

		// 成功以后次数重置
		m_iGetRtmpCount = GET_RTMP_FAIL_COUNT;
	}
	else if (obj["status"].toInt() == 0)
	{
		// 重试5次后放弃，提示ERROR
		m_iGetRtmpCount--;
		if (m_iGetRtmpCount < 0)
		{
			m_iGetRtmpCount = GET_RTMP_FAIL_COUNT;

// 			if (m_parent)
// 				m_parent->showErrorTip(TIP_GET_RTMP_ERROR);
			return;
		}

		m_TGetRtmpTimer->start(300);
	}
}

// 开始启动心跳
void LiveStatusManager::StartHeartBeat()
{
	m_HeartTimer->start(m_iBeatStep*1000);
}

// 直播心跳
void LiveStatusManager::HeartBeatTimer()
{
	QString strUrl;

	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	qint64 date = QDateTime::currentMSecsSinceEpoch();
	qint64 currentSec = date / 1000;
	QString time = QString::number(currentSec);
	m_Time = time;

	QByteArray append("live_token=");
	append += m_sLiveToken;
	append.append("&beat_step=");
	qInfo(append);
	append += QString::number(m_iBeatStep);
	append.append("&timestamp=");
	append += time;

	QString strHeartBeat = "HearBeat:";
	strHeartBeat += append;
//	qDebug()<<strHeartBeat;
	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	reply = manager.post(request, append);
	connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::ReturnHeartBeat);
}

// 心跳返回的结果
void LiveStatusManager::ReturnHeartBeat()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1)
	{
		m_sLiveToken = data["live_token"].toString();
		// 成功以后次数重置
		m_iHeartCount = HEARTBEAT_FAIL_COUNT;
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
	else
	{
		// 重试5次后放弃
		m_iHeartCount--;
		if (m_iHeartCount < 0)
		{
			m_iHeartCount = HEARTBEAT_FAIL_COUNT;
			return;
		}

		m_HeartFailTimer->start(300);
	}
}

// 心跳失败重试
void LiveStatusManager::HeartBeatFailTimer()
{
	m_HeartFailTimer->stop();

	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	QByteArray append("live_token=");
	append += m_sLiveToken;
	append.append("&beat_step=");
	qInfo(append);
	append += QString::number(m_iBeatStep);
	append.append("&timestamp=");
	append += m_Time;

	QString strHeartBeat = "HearBeat:";
	strHeartBeat += append;
//	qDebug() << strHeartBeat;
	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	reply = manager.post(request, append);
	connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::ReturnHeartBeat);
}

// 发送直播开始状态(0:未直播 1:直播中 2:已关闭)
void LiveStatusManager::SendStartLiveHttpMsg(int iBoard, int iCamera, QString sLessonid, QString sToken)
{
	m_lessonID = sLessonid;
	m_sToken = sToken;
	QString strUrl;

	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/lessons/{lessons_id}/live_start";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/lessons/{lessons_id}/live_start";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	QByteArray append("board=");
	append += QString::number(iBoard);
	append.append("&camera=");
	qInfo(append);
	append += QString::number(iCamera);

	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	reply = manager.post(request,append);
	connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::FinishStartLive);
}

void LiveStatusManager::FinishStartLive()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1 && data.contains("live_token"))
	{
		QString status = data["status"].toString();
		m_sLiveToken = data["live_token"].toString();
		m_iBeatStep = data["beat_step"].toInt();
		if (status == "teaching")
		{
			if (m_newParent)
			{
				// 更新课程状态
				m_newParent->startLiveStream();
				m_newParent->SendRequestStatus("直播中");

				StartHeartBeat();
			}
		}
		else
		{
			if (m_newParent)
			{
				m_newParent->showErrorTip(TIP_START_LIVE_ERROR);
				qDebug() << "ErrorStopLive 1";
				m_newParent->ErrorStopLive(NULL);
			}
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);	
	}
	else
	{
		if (m_newParent)
		{
			m_newParent->showErrorTip(TIP_START_LIVE_ERROR);
			qDebug() << "ErrorStopLive 2";
			m_newParent->ErrorStopLive(NULL);
		}
	}
}

// 停止直播推流消息
void LiveStatusManager::SendStopLiveHttpMsg(bool bConnect)
{
	qDebug() << "位置3";
	StopTimer();
	QString strUrl;

	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/lessons/{lessons_id}/live_end";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/lessons/{lessons_id}/live_end";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	reply = manager.post(request,"");
	if (bConnect)
		connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::FinishStopLive);
}

void LiveStatusManager::FinishStopLive()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1)
	{
		if (m_newParent)
		{
			// 更新课程状态
			m_newParent->SendRequestStatus("已直播");

			m_iStopLiveCount = STOP_LIVE_FAIL_COUNT;
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
	else
	{
		// 重试5次后放弃
		m_iStopLiveCount--;
		if (m_iStopLiveCount < 0)
		{
			m_iStopLiveCount = STOP_LIVE_FAIL_COUNT;
			return;
		}

		m_StopFailTimer->start(300);
	}
}

void LiveStatusManager::StopLiveFailTimer()
{
	m_StopFailTimer->stop();
	SendStopLiveHttpMsg(true);
}

void LiveStatusManager::StopTimer()
{
	if (m_TGetRtmpTimer && m_HeartTimer &&
		m_HeartFailTimer && m_SwitchFailTimer&&
		m_1v1HeartTimer && m_1v1HeartFailTimer)
	{
		m_TGetRtmpTimer->stop();
		m_HeartTimer->stop();
		m_HeartFailTimer->stop();
		m_SwitchFailTimer->stop();
		m_1v1HeartTimer->stop();
		m_1v1HeartFailTimer->stop();
	}
}

// 摄像头状态切换
void LiveStatusManager::SendCameraSwitchMsg(int iBoard, int iCamera)
{
	m_SwitchFailTimer->stop();
	m_iBoardStatus = iBoard;
	m_iCameraStatus = iCamera;

	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/lessons/{lessons_id}/live_switch";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/lessons/{lessons_id}/live_switch";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	QByteArray append("board=");
	append += QString::number(iBoard);
	append.append("&camera=");
	qInfo(append);
	append += QString::number(iCamera);

	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	reply = manager.post(request,append);
	connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::FinishCameraSwitch);
}

void LiveStatusManager::FinishCameraSwitch()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1 && obj.contains("data"))
	{
		QString statusOK = obj["data"].toString();
		if (statusOK == "ok")
		{
			return;
		}
		else
		{
			m_SwitchFailTimer->start(1000);
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
	else
	{
		m_SwitchFailTimer->start(1000);
	}
}

void LiveStatusManager::SwitchFailTimer()
{
	m_SwitchFailTimer->stop();

	SendCameraSwitchMsg(m_iBoardStatus,m_iCameraStatus);
}

void LiveStatusManager::RequestError(QJsonObject& error, bool bTrue)
{
	QString strError;
	if (error["code"].toInt() == 1002)
	{
		if (m_newParent)
			m_newParent->ErrorStop();
		StopTimer();
		strError = QString("授权过期,请重新登录！");

		int iStatus = CMessageBox::showMessage(
			QString("答疑时间"),
			QString(strError),
			QString("确定"),
			QString());
		if (iStatus == 1 || iStatus == 0)
		{
			if (m_newParent)
				m_newParent->ReturnLogin();
		}
		return;
	}
	else if (error["code"].toInt() == 1003)
		strError = QString("没有权限访问！");
	else if (error["code"].toInt() == 1004)
		strError = QString("授权失败,请重新登录！");
	else if (error["code"].toInt() == 3001)
		strError = QString("参数错误,请重新登录！");
	else if (error["code"].toInt() == 3002)
		strError = QString("数据不合法,请重新登录！");
	else if (error["code"].toInt() == 4001)
		strError = QString("找不到资源,请重新登录！");
	else if (error["code"].toInt() == 9999)
		strError = QString("服务器错误,请重新登录！");
	else
		return;

	int iStatus = CMessageBox::showMessage(
		QString("答疑时间"),
		QString(strError),
		QString("确定"),
		QString());
	if (iStatus == 1 || iStatus == 0)
	{
		if (m_newParent)
			m_newParent->ReturnLogin();
	}
}

void LiveStatusManager::SetEnvironmental(bool bTyle, QString homePage)
{
	m_homePage = homePage;
	m_EnvironmentalTyle = bTyle;
}

/***********************************************************/
/*					互动直播							   */
/***********************************************************/
// 发送1v1直播开始状态
void LiveStatusManager::SendStart1v1LiveHttpMsg(QString sLessonid,QString chatid, QString sToken)
{
	m_lessonID = sLessonid;
	m_sToken = sToken;
	QString strUrl;

	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_lessons/{lessons_id}/live_start";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_lessons/{lessons_id}/live_start";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	QByteArray append("room_id=");
	append += chatid;

	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	reply = manager.post(request, append);
	connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::Finish1v1StartLive);
}

void LiveStatusManager::Finish1v1StartLive()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1 && data.contains("live_token"))
	{
		QString status = data["status"].toString();
		m_sLiveToken = data["live_token"].toString();
		m_iBeatStep = data["beat_step"].toInt();

		if (status == "teaching")
		{
			if (m_newParent)
			{
				// 更新课程状态
				m_newParent->start1v1LiveStream();
				m_newParent->SendRequestStatus("直播中");

				Start1v1HeartBeat();
			}
		}
		else
		{
			if (m_newParent)
			{
				m_newParent->show1v1ErrorTip(TIP_START_LIVE_ERROR);
			}
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
	else
	{
		if (m_newParent)
		{
			m_newParent->show1v1ErrorTip(TIP_START_LIVE_ERROR);
		}
	}
}

// 开始启动1对1心跳
void LiveStatusManager::Start1v1HeartBeat()
{
	m_1v1HeartTimer->start(m_iBeatStep * 1000);
}

// 1对1直播心跳
void LiveStatusManager::HeartBeat1v1Timer()
{
	QString strUrl;

	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/interactive_lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/interactive_lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	qint64 date = QDateTime::currentMSecsSinceEpoch();
	qint64 currentSec = date / 1000;
	QString time = QString::number(currentSec);
	m_Time = time;

	QByteArray append("live_token=");
	append += m_sLiveToken;
	append.append("&beat_step=");
	append += QString::number(m_iBeatStep);
	append.append("&timestamp=");
	append += time;

	QString strHeartBeat = "HearBeat:";
	strHeartBeat += append;
	qDebug() << __FILE__ << __LINE__ << strHeartBeat;
	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	reply = manager.post(request, append);
	connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::ReturnHeartBeat1v1);
}

// 心跳返回的结果
void LiveStatusManager::ReturnHeartBeat1v1()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1)
	{
		m_sLiveToken = data["live_token"].toString();
		// 成功以后次数重置
		m_iHeartCount = HEARTBEAT_FAIL_COUNT;
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
	else
	{
		// 重试5次后放弃
		m_iHeartCount--;
		if (m_iHeartCount < 0)
		{
			m_iHeartCount = HEARTBEAT_FAIL_COUNT;
			return;
		}

		m_1v1HeartFailTimer->start(300);
	}
}

// 心跳失败重试
void LiveStatusManager::HeartBeatFailTimer1v1()
{
	m_1v1HeartFailTimer->stop();

	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/interactive_lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/interactive_lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	QByteArray append("live_token=");
	append += m_sLiveToken;
	append.append("&beat_step=");
	qInfo(append);
	append += QString::number(m_iBeatStep);
	append.append("&timestamp=");
	append += m_Time;

	QString strHeartBeat = "HearBeat:";
	strHeartBeat += append;
	qDebug() << "失败"<<strHeartBeat;
	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	reply = manager.post(request, append);
	connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::ReturnHeartBeat1v1);
}

// 停止直播推流消息
void LiveStatusManager::SendStopLiveHttpMsg1v1(bool bConnect)
{
	StopTimer();
	QString strUrl;

	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl = "/api/v1/live_studio/interactive_lessons/{lessons_id}/live_end";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl = "/api/v1/live_studio/interactive_lessons/{lessons_id}/live_end";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	reply = manager.post(request, "");
// 	if (bConnect)
// 	{
// 		if (m_newParent)
// 		{
// 			// 更新课程状态
// 			m_newParent->SendRequestStatus("已直播");
// 		}
// 	}
	if (bConnect)
		connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::FinishStopLive1v1);
}

void LiveStatusManager::FinishStopLive1v1()
{
	QByteArray result = reply->readAll();
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1)
	{
		if (m_newParent)
		{
			// 更新课程状态
			m_newParent->SendRequestStatus("已直播");

			m_iStopLiveCount = STOP_LIVE_FAIL_COUNT;
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
}