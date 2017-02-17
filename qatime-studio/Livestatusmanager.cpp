#include "Livestatusmanager.h"
#include "define.h"
#include "UIMessageBox.h"

#define GET_RTMP_FAIL_COUNT		5	// ��ȡ������ַʧ�����Դ���
#define HEARTBEAT_FAIL_COUNT	5	// ֱ������ʧ�����Դ���
#define STOP_LIVE_FAIL_COUNT	5	// ֹͣ����ʧ�����Դ���

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
	, m_iBeatStep(0)
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
	//ɾ����ʱ��
	if (m_TGetRtmpTimer)
	{
		if (m_TGetRtmpTimer->isActive())
			m_TGetRtmpTimer->stop();
		delete m_TGetRtmpTimer;
		m_TGetRtmpTimer = NULL;
	}

	//ɾ����ʱ��
	if (m_HeartTimer)
	{
		if (m_HeartTimer->isActive())
			m_HeartTimer->stop();
		delete m_HeartTimer;
		m_HeartTimer = NULL;
	}

	//ɾ����ʱ��
	if (m_HeartFailTimer)
	{
		if (m_HeartFailTimer->isActive())
			m_HeartFailTimer->stop();
		delete m_HeartFailTimer;
		m_HeartFailTimer = NULL;
	}

	//ɾ����ʱ��
	if (m_StopFailTimer)
	{
		if (m_StopFailTimer->isActive())
			m_StopFailTimer->stop();
		delete m_StopFailTimer;
		m_StopFailTimer = NULL;
	}

	//ɾ����ʱ��
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

// ��ȡ�װ塢����ͷ���������
void LiveStatusManager::GetRtmpAddressAndHeartBeat(QString lessonID, QString sToken)
{
	m_lessonID = lessonID;
	m_sToken = sToken;
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{id}/live_info";
	strUrl.replace("{id}", lessonID);
#else
	strUrl = "https://qatime.cn/api/v1/live_studio/lessons/{id}/live_info";
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

		// �ɹ��Ժ��������
		m_iGetRtmpCount = GET_RTMP_FAIL_COUNT;
	}
	else if (obj["status"].toInt() == 0)
	{
		// ����5�κ��������ʾERROR
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

// ��ʼ��������
void LiveStatusManager::StartHeartBeat()
{
	m_HeartTimer->start(m_iBeatStep*1000);
}

// ֱ������
void LiveStatusManager::HeartBeatTimer()
{
	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
	strUrl.replace("{lessons_id}", m_lessonID);
#else
	strUrl = "https://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
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

	QString strHeartBeat = "HearBeat:";
	strHeartBeat += append;
//	qDebug()<<strHeartBeat;
	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	reply = manager.post(request, append);
	connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::ReturnHeartBeat);
}

// �������صĽ��
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
		// �ɹ��Ժ��������
		m_iHeartCount = HEARTBEAT_FAIL_COUNT;
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
	else
	{
		// ����5�κ����
		m_iHeartCount--;
		if (m_iHeartCount < 0)
		{
			m_iHeartCount = HEARTBEAT_FAIL_COUNT;
			return;
		}

		m_HeartFailTimer->start(300);
	}
}

// ����ʧ������
void LiveStatusManager::HeartBeatFailTimer()
{
	m_HeartFailTimer->stop();

	QString strUrl;
#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
	strUrl.replace("{lessons_id}", m_lessonID);
#else
	strUrl = "https://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
	strUrl.replace("{lessons_id}", m_lessonID);
#endif

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

// ����ֱ����ʼ״̬(0:δֱ�� 1:ֱ���� 2:�ѹر�)
void LiveStatusManager::SendStartLiveHttpMsg(int iBoard, int iCamera, QString sLessonid, QString sToken)
{
	m_lessonID = sLessonid;
	m_sToken = sToken;
	QString strUrl;

#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_start";
	strUrl.replace("{lessons_id}", m_lessonID);
#else
	strUrl = "https://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_start";
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
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1 && data.contains("live_token"))
	{
		QString status = data["status"].toString();
		m_sLiveToken = data["live_token"].toString();
		m_iBeatStep = data["beat_step"].toInt();
		if (status == "teaching")
		{
			if (m_parent)
			{
				// ���¿γ�״̬
				m_parent->startLiveStream();
				m_parent->SendRequestStatus();

				StartHeartBeat();
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
		RequestError(error);	
	}
	else
	{
		if (m_parent)
			m_parent->showErrorTip(TIP_START_LIVE_ERROR);
	}
}

// ֱֹͣ��������Ϣ
void LiveStatusManager::SendStopLiveHttpMsg(bool bConnect)
{
	StopTimer();
	QString strUrl;

#ifdef _DEBUG
	strUrl = "http://testing.qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_end";
	strUrl.replace("{lessons_id}", m_lessonID);
#else
	strUrl = "https://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_end";
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
			// ���¿γ�״̬
			m_parent->SendRequestStatus();

			m_iStopLiveCount = STOP_LIVE_FAIL_COUNT;
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
	else
	{
		// ����5�κ����
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

// ����ͷ״̬�л�
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
	strUrl = "https://qatime.cn/api/v1/live_studio/lessons/{lessons_id}/live_switch";
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
		if (m_parent)
			m_parent->ErrorStop();
		StopTimer();
		strError = QString("��Ȩ����,�����µ�¼��");
	}
	else if (error["code"].toInt() == 1003)
		strError = QString("û��Ȩ�޷��ʣ�");
	else if (error["code"].toInt() == 1004)
		strError = QString("��Ȩʧ��,�����µ�¼��");
	else if (error["code"].toInt() == 3001)
		strError = QString("��������,�����µ�¼��");
	else if (error["code"].toInt() == 3002)
		strError = QString("���ݲ��Ϸ�,�����µ�¼��");
	else if (error["code"].toInt() == 4001)
		strError = QString("�Ҳ�����Դ,�����µ�¼��");
	else if (error["code"].toInt() == 9999)
		strError = QString("����������,�����µ�¼��");
	else
		return;

	int iStatus = CMessageBox::showMessage(
		QString("����ʱ��"),
		QString(strError),
		QString("ȷ��"),
		QString());
	if (iStatus == 1 || iStatus == 0)
	{
		if (m_parent && bTrue)
			m_parent->GetLoginWnd()->ReturnLogin();
	}
}