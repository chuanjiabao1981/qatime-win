#include "Livestatusmanager.h"
#include "define.h"
#include "UIMessageBox.h"
#include "ZPublicDefine.h"

#define GET_RTMP_FAIL_COUNT		5	// ��ȡ������ַʧ�����Դ���
#define HEARTBEAT_FAIL_COUNT	5	// ֱ������ʧ�����Դ���
#define STOP_LIVE_FAIL_COUNT	5	// ֹͣ����ʧ�����Դ���

extern int m_PublicCameraStatus;

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
	, m_mLessonType(1)
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
		{
			qDebug() << __FILE__ << __LINE__ << "�رն�ʱ��1";
			m_HeartTimer->stop();
		}
		qDebug() << __FILE__ << __LINE__ << "ɾ����ʱ��1";
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

// ��ȡ�װ塢����ͷ���������
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

// 			if (m_parent)
// 				m_parent->showErrorTip(TIP_GET_RTMP_ERROR);
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

	if (m_mLessonType == d_ExclusiveLesson)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/events/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	qint64 date = QDateTime::currentMSecsSinceEpoch();
	qint64 currentSec = date / 1000;
	QString time = QString::number(currentSec);
	m_Time = time;

	//strUrl = strUrl + "?t=" + time;
	QUrl mUrl = QUrl(strUrl);
	QNetworkRequest mRequest(mUrl);

	QByteArray append("live_token=");
	append += m_sLiveToken;
	append.append("&beat_step=");
	qInfo(append, __FILE__, __LINE__);
	append += QString::number(m_iBeatStep);
	append.append("&timestamp=");
	append += time;

	QString strHeartBeat = "HearBeat:";
	strHeartBeat += append;
//	qDebug()<<strHeartBeat;
	mRequest.setRawHeader("Remember-Token", m_sToken.toUtf8());
	qDebug() << __FILE__ << __LINE__ << m_sToken;
	m_HeartReply = manager.post(mRequest, append);
	connect(m_HeartReply, &QNetworkReply::finished, this, &LiveStatusManager::ReturnHeartBeat);
}

// �������صĽ��
void LiveStatusManager::ReturnHeartBeat()
{
	QByteArray mResult = m_HeartReply->readAll();
	int mNetErrorCode = m_HeartReply->error();
	// �����߼�Ϊ������״̬��������ѭ������
	if (mNetErrorCode != QNetworkReply::NoError)
	{
		// ����5�κ����
		m_iHeartCount--;
		if (m_iHeartCount < 0)
		{
			m_iHeartCount = HEARTBEAT_FAIL_COUNT;		
		}
		m_HeartFailTimer->start(300);
		return;
	}
	QJsonDocument document(QJsonDocument::fromJson(mResult));
	QJsonObject mObj = document.object();
	QJsonObject mData = mObj["data"].toObject();
	QJsonObject mError = mObj["error"].toObject();
	int mStatus = mObj["status"].toInt();
	//mStatus = 0;
	if (mStatus == 1)
	{
		m_sLiveToken = mData["live_token"].toString();
		// �ɹ��Ժ��������
		m_iHeartCount = HEARTBEAT_FAIL_COUNT;
	}
	else if (mStatus == 0)
	{
		// ���������Ϣʧ�ܣ���ô���·������������½���
		if (mError["code"].toInt() == 0)
		{
			qDebug() << __FILE__ << __LINE__ << "������Ϣʧ�ܣ�";
			return;
		}
		qDebug() << __FILE__ << __LINE__ << "�رն�ʱ��2";
		m_HeartTimer->stop();
		RequestError(mError);
		return;
	}
}

// ����ʧ������
void LiveStatusManager::HeartBeatFailTimer()
{
	m_HeartFailTimer->stop();

	QString strUrl;
	if (m_mLessonType == d_ExclusiveLesson)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/events/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}


	QUrl mUrl = QUrl(strUrl);
	QNetworkRequest mRequest(mUrl);

	QByteArray append("live_token=");
	append += m_sLiveToken;
	append.append("&beat_step=");
	qInfo(append, __FILE__, __LINE__);
	append += QString::number(m_iBeatStep);
	append.append("&timestamp=");
	append += m_Time;

	QString strHeartBeat = "HearBeat:";
	strHeartBeat += append;
//	qDebug() << strHeartBeat;
	mRequest.setRawHeader("Remember-Token", m_sToken.toUtf8());
	qDebug() << __FILE__ << __LINE__ << m_sToken;
	m_HeartReply = manager.post(mRequest, append);
	connect(m_HeartReply, &QNetworkReply::finished, this, &LiveStatusManager::ReturnHeartBeat);
}

// ����ֱ����ʼ״̬(0:δֱ�� 1:ֱ���� 2:�ѹر�)
void LiveStatusManager::SendStartLiveHttpMsg(int iBoard, int iCamera, QString sLessonid, QString sToken, int mLessonType)
{
	m_lessonID = sLessonid;
	m_sToken = sToken;
	QString strUrl;
	// ��ȡ����������
	m_mLessonType = mLessonType;
	if (mLessonType == d_ExclusiveLesson)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/events/{lessons_id}/live_start";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/lessons/{lessons_id}/live_start";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	QByteArray append("board=");
	append += QString::number(iBoard);
	append.append("&camera=");
	qInfo(append, __FILE__, __LINE__);	// ��ӡ��־
	append += QString::number(iCamera);
	append.append("&t=");
	// ʱ���
	int mtTime = QDateTime::currentDateTime().toTime_t();
	append += QString::number(mtTime);

	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	qDebug() << __FILE__ << __LINE__ << m_sToken;
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
				// ���¿γ�״̬
				m_newParent->startLiveStream();
				m_newParent->SendRequestStatus("ֱ����");

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

// ֱֹͣ��������Ϣ
void LiveStatusManager::SendStopLiveHttpMsg(bool bConnect)
{
	qDebug() << "λ��3";
	StopTimer();
	QString strUrl;

	if (m_mLessonType == d_ExclusiveLesson)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/events/{lessons_id}/live_end";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/lessons/{lessons_id}/live_end";
		strUrl.replace("{lessons_id}", m_lessonID);
	}


	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QByteArray append;
	append.append("t=");
	// ʱ���
	int mtTime = QDateTime::currentDateTime().toTime_t();
	append += QString::number(mtTime);

	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	qDebug() << __FILE__ << __LINE__ << m_sToken;
	reply = manager.post(request,append);
	if (bConnect)
		connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::FinishStopLive);
}

void LiveStatusManager::FinishStopLive()
{
	QByteArray result = reply->readAll();
	int mNetErrorCode = reply->error();
	if (mNetErrorCode != QNetworkReply::NoError)
	{
		// ����5�κ����
		m_iStopLiveCount--;
		if (m_iStopLiveCount < 0)
		{
			m_StopFailTimer->stop();
			return;
		}

		m_StopFailTimer->start(300);
		m_StopFailTimer->stop();
		return;
	}
	QJsonDocument document(QJsonDocument::fromJson(result));
	QJsonObject obj = document.object();
	QJsonObject data = obj["data"].toObject();
	QJsonObject error = obj["error"].toObject();
	if (obj["status"].toInt() == 1)
	{
		if (m_newParent)
		{
			// ���¿γ�״̬
			m_newParent->SendRequestStatus("��ֱ��");

			m_iStopLiveCount = STOP_LIVE_FAIL_COUNT;
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
	else
	{
		
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
		qDebug() << __FILE__ << __LINE__ << "�رն�ʱ��3";
		m_TGetRtmpTimer->stop();
		m_HeartTimer->stop();
		m_HeartFailTimer->stop();
		m_SwitchFailTimer->stop();
		m_1v1HeartTimer->stop();
		m_1v1HeartFailTimer->stop();
	}
}

// ����ͷ״̬�л�
void LiveStatusManager::SendCameraSwitchMsg(int iBoard, int iCamera)
{
	m_SwitchFailTimer->stop();
	m_iBoardStatus = iBoard;
	m_iCameraStatus = iCamera;

	QString strUrl;
	if (m_mLessonType == d_ExclusiveLesson)
	{
		strUrl += m_homePage;
		
		strUrl += "/api/v1/live_studio/events/{lessons_id}/live_switch";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/lessons/{lessons_id}/live_switch";
		strUrl.replace("{lessons_id}", m_lessonID);
	}


	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	QByteArray append("board=");
	append += QString::number(iBoard);
	append.append("&camera=");
	append += QString::number(iCamera);
	qInfo(append, __FILE__, __LINE__);
	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	qDebug() << __FILE__ << __LINE__ << m_sToken;
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
	if (m_mLessonType == d_1V1Lesson)
	{
		SendCameraSwitchMsg(m_iBoardStatus, m_iCameraStatus);
	}
}

void LiveStatusManager::RequestError(QJsonObject& error, bool bTrue)
{
	QString strError;
	int mErrorCode = 0;
	mErrorCode = error["code"].toInt();
	qDebug() << __FILE__ << __LINE__ << "������룺"<<mErrorCode;
	if (m_newParent)
	{
		qDebug() << __FILE__ << __LINE__ << "�����쳣�Ͽ�";
		//�Ƚ�������߳�
		m_newParent->ErrorStop();
		StopTimer();
		m_newParent->EndDev();		
	}
	
	if (mErrorCode == 1002)
	{
		qDebug() << __FILE__ << __LINE__ << "�����û���¼��";
		strError = QString("��Ȩ����,�����µ�¼��");
/*
		if (m_newParent)
			m_newParent->ErrorStop();
		StopTimer();
		

		int iStatus = CMessageBox::showMessage(
			QString("����ʱ��"),
			QString(strError),
			QString("ȷ��"),
			QString());
		if (iStatus == 1 || iStatus == 0)
		{
			if (m_newParent)
				m_newParent->ReturnLogin();
		}
		return;
*/
	}
	else if (mErrorCode == 1003)
		strError = QString("û��Ȩ�޷��ʣ�");
	else if (mErrorCode == 1004)
		strError = QString("��Ȩʧ��,�����µ�¼��");
	else if (mErrorCode == 3001)
		strError = QString("��������,�����µ�¼��");
	else if (mErrorCode == 3002)
		strError = QString("���ݲ��Ϸ�,�����µ�¼��");
	else if (mErrorCode == 4001)
		strError = QString("�Ҳ�����Դ,�����µ�¼��");
	else if (mErrorCode == 9999)
		strError = QString("����������,�����µ�¼��");
	else
		strError = QString("δ֪����,�����µ�¼��");

	int iStatus = CMessageBox::showMessage(
		QString("����ʱ��"),
		QString(strError),
		QString("ȷ��"),
		QString());

	if (iStatus == 1 || iStatus == 0)
	{
		if (m_newParent)
		{	
			
			if (m_newParent->m_ScreenTip)
			{
				m_newParent->m_ScreenTip->close();
			}
			if (m_newParent->m_curChatRoom)
			{
				m_newParent->close();
			}
			m_newParent->ReturnLogin();
		}
			
	}
}

void LiveStatusManager::SetEnvironmental(bool bTyle, QString homePage)
{
	m_homePage = homePage;
	m_EnvironmentalTyle = bTyle;
}

/***********************************************************/
/*					����ֱ��							   */
/***********************************************************/
// ����1v1ֱ����ʼ״̬
void LiveStatusManager::SendStart1v1LiveHttpMsg(QString sLessonid,QString chatid, QString sToken)
{
	m_lessonID = sLessonid;
	m_sToken = sToken;
	QString strUrl;

	
	strUrl += m_homePage;
	strUrl += "/api/v1/live_studio/interactive_lessons/{lessons_id}/live_start";
	strUrl.replace("{lessons_id}", m_lessonID);
	


	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);

	QByteArray append("room_id=");
	append += chatid;

	append.append("&camera=");
	append += QString::number(m_PublicCameraStatus);

	append.append("&channel_id=");
	append += m_ChannelID;

	append.append("&t=");
	// ʱ���
	int mtTime = QDateTime::currentDateTime().toTime_t();
	append += QString::number(mtTime);

	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	qDebug() << __FILE__ << __LINE__ << m_sToken;
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
				// ���¿γ�״̬
				m_newParent->start1v1LiveStream();
				m_newParent->SendRequestStatus("ֱ����");

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

// ��ʼ����1��1����
void LiveStatusManager::Start1v1HeartBeat()
{
	m_1v1HeartTimer->start(m_iBeatStep * 1000);
}

// 1��1ֱ������
void LiveStatusManager::HeartBeat1v1Timer()
{
	QString strUrl;

	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl mUrl = QUrl(strUrl);
	QNetworkRequest request(mUrl);

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
	qDebug() << __FILE__ << __LINE__ << m_sToken;
	m_HeartReply = manager.post(request, append);
	connect(m_HeartReply, &QNetworkReply::finished, this, &LiveStatusManager::ReturnHeartBeat1v1);
}

// �������صĽ��
void LiveStatusManager::ReturnHeartBeat1v1()
{
	QByteArray mResult = m_HeartReply->readAll();
	int mNetErrorCode = m_HeartReply->error();
	// �����߼�Ϊ������״̬��������ѭ������
	if (mNetErrorCode != QNetworkReply::NoError)
	{
		// ����5�κ����
		m_iHeartCount--;
		if (m_iHeartCount < 0)
		{
			m_iHeartCount = HEARTBEAT_FAIL_COUNT;

		}
		m_1v1HeartFailTimer->start(300);
		return;
	}
	QJsonDocument document(QJsonDocument::fromJson(mResult));
	QJsonObject mObj = document.object();
	QJsonObject mData = mObj["data"].toObject();
	QJsonObject mError = mObj["error"].toObject();
	if (mObj["status"].toInt() == 1)
	{
		m_sLiveToken = mData["live_token"].toString();
		// �ɹ��Ժ��������
		m_iHeartCount = HEARTBEAT_FAIL_COUNT;
	}
	else if (mObj["status"].toInt() == 0)
	{
		RequestError(mError);
	}

}

// ����ʧ������
void LiveStatusManager::HeartBeatFailTimer1v1()
{
	m_1v1HeartFailTimer->stop();

	QString strUrl;
	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_lessons/{lessons_id}/heart_beat";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl mUrl = QUrl(strUrl);
	QNetworkRequest request(mUrl);

	QByteArray append("live_token=");
	append += m_sLiveToken;
	append.append("&beat_step=");
	qInfo(append, __FILE__, __LINE__);
	append += QString::number(m_iBeatStep);
	append.append("&timestamp=");
	append += m_Time;

	QString strHeartBeat = "HearBeat:";
	strHeartBeat += append;
	qDebug() << "ʧ��"<<strHeartBeat;
	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	qDebug() << __FILE__ << __LINE__ << m_sToken;
	reply = manager.post(request, append);
	connect(reply, &QNetworkReply::finished, this, &LiveStatusManager::ReturnHeartBeat1v1);
}

// ֱֹͣ��������Ϣ
void LiveStatusManager::SendStopLiveHttpMsg1v1(bool bConnect)
{
	StopTimer();
	QString strUrl;

	if (m_EnvironmentalTyle)
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_lessons/{lessons_id}/live_end";
		strUrl.replace("{lessons_id}", m_lessonID);
	}
	else
	{
		strUrl += m_homePage;
		strUrl += "/api/v1/live_studio/interactive_lessons/{lessons_id}/live_end";
		strUrl.replace("{lessons_id}", m_lessonID);
	}

	QUrl url = QUrl(strUrl);
	QNetworkRequest request(url);
	QByteArray append;
	append.append("t=");
	// ʱ���
	int mtTime = QDateTime::currentDateTime().toTime_t();
	append += QString::number(mtTime);


	request.setRawHeader("Remember-Token", m_sToken.toUtf8());
	qDebug() << __FILE__ << __LINE__ << m_sToken;
	reply = manager.post(request, "");
// 	if (bConnect)
// 	{
// 		if (m_newParent)
// 		{
// 			// ���¿γ�״̬
// 			m_newParent->SendRequestStatus("��ֱ��");
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
			// ���¿γ�״̬
			m_newParent->SendRequestStatus("��ֱ��");
			m_iStopLiveCount = STOP_LIVE_FAIL_COUNT;
		}
	}
	else if (obj["status"].toInt() == 0)
	{
		RequestError(error);
	}
}