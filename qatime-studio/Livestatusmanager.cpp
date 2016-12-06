#include "Livestatusmanager.h"
#include "define.h"

#define GET_RTMP_FAIL_COUNT		5	// 获取推流地址失败重试次数
#define HEARTBEAT_FAIL_COUNT	5	// 直播心跳失败重试次数
#define STOP_LIVE_FAIL_COUNT	5	// 停止推流失败重试次数

#ifdef TEST
#define _DEBUG
#else
#endif
LiveStatusManager::LiveStatusManager(QObject *parent)
	: QObject(parent)
	, m_parent(NULL)
	, m_TGetRtmpTimer(NULL)
	, m_HeartTimer(NULL)
	, m_HeartFailTimer(NULL)
	, m_StopFailTimer(NULL)
	, m_SwitchFailTimer(NULL)
{
	m_iGetRtmpCount = GET_RTMP_FAIL_COUNT;
	m_TGetRtmpTimer = new QTimer(this);
	connect(m_TGetRtmpTimer, SIGNAL(timeout()), this, SLOT(GetRtmpFailTimer()));
	
	m_HeartTimer = new QTimer(this);
	connect(m_HeartTimer, SIGNAL(timeout()), this, SLOT(HeartBeatTimer()));

	m_iHeartCount = HEARTBEAT_FAIL_COUNT;
	m_HeartFailTimer = new QTimer(this);
	connect(m_HeartFailTimer, SIGNAL(timeout()), this, SLOT(HeartBeatFailTimer()));

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

void LiveStatusManager::setMainWindow(UIMainWindow* parent)
{
	m_parent = parent;
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
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{id}/live_info";
	strUrl.replace("{id}", lessonID);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/lessons/{id}/live_info";
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
		
		if (m_parent)
			m_parent->setBoradCamera(sBoardRtmp, sCameraRtmp);

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

			if (m_parent)
				m_parent->showErrorTip(TIP_GET_RTMP_ERROR);
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
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
	strUrl.replace("{lessons_id}", m_lessonID);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
	strUrl.replace("{lessons_id}", m_lessonID);
#endif

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
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
	strUrl.replace("{lessons_id}", m_lessonID);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
	strUrl.replace("{lessons_id}", m_lessonID);
#endif

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	QByteArray append("live_token=");
	append += m_sLiveToken;
	append.append("&beat_step=");
	qInfo(append);
	append += m_iBeatStep;
	append.append("&timestamp=");
	append += m_Time;

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

#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_start";
	strUrl.replace("{lessons_id}", m_lessonID);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_start";
	strUrl.replace("{lessons_id}", m_lessonID);
#endif

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
	if (obj["status"].toInt() == 1 && data.contains("live_token"))
	{
		QString status = data["status"].toString();
		m_sLiveToken = data["live_token"].toString();
		m_iBeatStep = data["beat_step"].toInt();
		if (status == "teaching")
		{
			if (m_parent)
			{
				// 更新开始直播按钮、计时器等状态
				m_parent->startLiveStream();
				// 更新课程状态
				m_parent->SendRequestStatus();
			}
		}
		else
		{
			if (m_parent)
				m_parent->showErrorTip(TIP_START_LIVE_ERROR);
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		if (m_parent)
			m_parent->showErrorTip(TIP_START_LIVE_ERROR);
	}
}

// 停止直播推流消息
void LiveStatusManager::SendStopLiveHttpMsg(bool bConnect)
{
	StopTimer();
	QString strUrl;

#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_end";
	strUrl.replace("{lessons_id}", m_lessonID);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_end";
	strUrl.replace("{lessons_id}", m_lessonID);
#endif

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
		if (m_parent)
		{
			// 更新课程状态
			m_parent->SendRequestStatus();

			m_iStopLiveCount = STOP_LIVE_FAIL_COUNT;
		}
	}
	else if (obj["status"].toInt() == 0)
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
		m_HeartFailTimer && m_SwitchFailTimer)
	{
		m_TGetRtmpTimer->stop();
		m_HeartTimer->stop();
		m_HeartFailTimer->stop();
		m_SwitchFailTimer->stop();
	}
}

// 摄像头状态切换
void LiveStatusManager::SendCameraSwitchMsg(int iBoard, int iCamera)
{
	m_SwitchFailTimer->stop();
	m_iBoardStatus = iBoard;
	m_iCameraStatus = iCamera;

	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_switch";
	strUrl.replace("{lessons_id}", m_lessonID);
#else
	strUrl = "http://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_switch";
	strUrl.replace("{lessons_id}", m_lessonID);
#endif

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
		m_SwitchFailTimer->start(1000);
	}
}

void LiveStatusManager::SwitchFailTimer()
{
	m_SwitchFailTimer->stop();

	SendCameraSwitchMsg(m_iBoardStatus,m_iCameraStatus);
}