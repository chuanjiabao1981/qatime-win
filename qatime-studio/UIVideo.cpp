#include "UIVideo.h"
#include <QPainter>
#include "windows.h"

QMutex UIVideo::m_mutex;
ST_NLSS_VIDEO_SAMPLER UIVideo::m_SvideoSampler;

UIVideo::UIVideo(QWidget *parent)
	: QWidget(parent)
	, m_videoSourceType(EN_NLSS_VIDEOIN_NONE)
	, m_audioSourceType(EN_NLSS_AUDIOIN_MIC)
	, m_bPreviewing(false)
	, m_hNlssService(NULL)
	, m_videoQ(EN_NLSS_VIDEOQUALITY_MIDDLE)
	, m_iAppWindNum(0)
	, m_iAudioDeviceNum(0)
	, m_iVideoDeviceNum(0)
	, m_iAppChangeIndex(0)
	, m_pVideoDevices(NULL)
	, m_pAudioDevices(NULL)
	, m_pAppWinds(NULL)
	, m_bLiving(false)
	, m_CurrentMicIndex(0)
	, m_CurrentVideoIndex(0)
{
	ui.setupUi(this);

	UIVideo::m_SvideoSampler.iWidth = 0;
	UIVideo::m_SvideoSampler.iHeight = 0;
	UIVideo::m_SvideoSampler.puaData = NULL;//һ���Է���һ�����Ŀؼ���������䲻ͬ������
	UIVideo::m_SvideoSampler.iDataSize = 0;

	m_StartLiveTimer = new QTimer(this);
	m_refreshTimer = new QTimer(this);
	m_refreshTimer->start(1000 / 12);
	connect(m_refreshTimer, SIGNAL(timeout()), this, SLOT(slot_onRefreshTimeout()));

#ifdef STARTLS_ASYNC
	m_pWorker = new Worker();
	connect(this, SIGNAL(sig_StartLiveStream()), m_pWorker, SLOT(slot_StartLiveStream()));
	connect(m_pWorker, SIGNAL(sig_ResultReady(int)), this, SLOT(slot_FinishStartLiveStream(int)));
	connect(this, SIGNAL(sig_StopLiveStream()), m_pWorker, SLOT(slot_StopLiveStream()));
	connect(m_pWorker, SIGNAL(sig_StopResult(int)), this, SLOT(slot_FinishStopLiveStream(int)));
#endif

	//Ĭ�ϲɼ�����Ƶ����˷��Լ�����ͷ�豸�����ԣ��ȳ�ʼ��ui�ؼ�ʱ���� ����Ƶ�豸ͨͨ�ҳ���
	//��ʼ������Ƶ�ؼ������õ��ײ��
	EnumAvailableMediaDevices();

	//����mediacapture�࣬ʧ���׳���	
	if (NLSS_OK != Nlss_Create(NULL, &m_hNlssService))
	{
		MessageBox(NULL, L"����ֱ��ʧ�ܣ���رճ�����������", L"", MB_OK);
	}

	SetMediaCapture(m_hNlssService);
}

UIVideo::~UIVideo()
{
	//����mediacapture��
	if (m_hNlssService != NULL)
	{
		if (m_bPreviewing)
		{
			Nlss_StopLiveStream(m_hNlssService);
		}

		Nlss_Destroy(m_hNlssService);
		m_hNlssService = NULL;
	}

	if (m_SvideoSampler.puaData != NULL)
	{
		delete[] m_SvideoSampler.puaData;
		m_SvideoSampler.puaData = NULL;
	}

	//ɾ����ʱ��
	if (m_refreshTimer)
	{
		if (m_refreshTimer->isActive())
			m_refreshTimer->stop();
		delete m_refreshTimer;
		m_refreshTimer = NULL;
	}
}

void UIVideo::setPlugFlowUrl(QString url)
{
	m_strUrl = url;
}

void UIVideo::SetMediaCapture(_HNLSSERVICE hNlssService)
{
	PFN_NLSS_STATUS_NTY notify = UIVideo::OnLiveStreamStatusNty;
	Nlss_SetStatusCB(hNlssService, notify);

	PFN_NLSS_VIDEOSAMPLER_CB frame_cb = UIVideo::SetVideoSampler;
	Nlss_SetVideoSamplerCB(m_hNlssService, frame_cb);
}

void UIVideo::SetVideoSampler(ST_NLSS_VIDEO_SAMPLER *pSampler)
{
	if (pSampler != NULL)
	{
		m_mutex.lock();
		if (pSampler->iWidth * pSampler->iHeight > m_SvideoSampler.iWidth * m_SvideoSampler.iHeight)
		{
			if (m_SvideoSampler.puaData)
			{
				delete[]m_SvideoSampler.puaData;
				m_SvideoSampler.puaData = NULL;
			}
				
			m_SvideoSampler.puaData = new unsigned char[pSampler->iWidth * pSampler->iHeight * 4];
		}

		m_SvideoSampler.iWidth = pSampler->iWidth;
		m_SvideoSampler.iHeight = pSampler->iHeight;
		m_SvideoSampler.iDataSize = pSampler->iDataSize;
		memcpy(m_SvideoSampler.puaData, pSampler->puaData, pSampler->iDataSize);

		m_mutex.unlock();
	}
}

void UIVideo::OnLiveStreamStatusNty(EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode)
{
	// 	if (m_pThis == NULL)
	// 	{
	// 		return;
	// 	}
	// 
	// 	if (enStatus == EN_NLSS_STATUS_START)
	// 	{
	// 		m_pThis->emit sig_livestreamSucessed();
	// 	}
	// 	else if (enStatus == EN_NLSS_STATUS_ERR)
	// 	{
	// 		m_pThis->emit sig_livestreamErrorHappened();
	// 	}
}

void UIVideo::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	p.setPen(QColor(0x8099be));

	if (m_SvideoSampler.iDataSize > 0)
	{
		QImage qimage;
		m_mutex.lock();
		qimage = QImage((uchar*)m_SvideoSampler.puaData, m_SvideoSampler.iWidth, m_SvideoSampler.iHeight, QImage::Format_ARGB32);
		p.drawImage(rect(), qimage);
		m_mutex.unlock();
	}
}

bool UIVideo::InitMediaCapture()
{
	bool have_video_source = true;
	bool have_audio_source = true;
	ST_NLSS_PARAM stParam;
	Nlss_GetDefaultParam(m_hNlssService, &stParam);

	switch (m_videoSourceType)
	{
	case EN_NLSS_VIDEOIN_FULLSCREEN:
		stParam.stVideoParam.iOutBitrate = 700000;
		stParam.stVideoParam.iOutFps = 10;
		break;
	case EN_NLSS_VIDEOIN_CAMERA:
		//��ȡ��Ƶ����
		if (m_pVideoDevices != NULL)
		{
			stParam.stVideoParam.u.stInCamera.paDevicePath = (char *)m_pVideoDevices[m_CurrentVideoIndex].paPath;
		}
		else
			have_video_source = false;
		stParam.stVideoParam.u.stInCamera.enLvl = m_videoQ;
		break;
	case EN_NLSS_VIDEOIN_RECTSCREEN:
		stParam.stVideoParam.u.stInRectScreen.iRectLeft = 100;
		stParam.stVideoParam.u.stInRectScreen.iRectRight = 500;
		stParam.stVideoParam.u.stInRectScreen.iRectTop = 100;
		stParam.stVideoParam.u.stInRectScreen.iRectBottom = 500;
		stParam.stVideoParam.iOutBitrate = 700000;

		break;
	case EN_NLSS_VIDEOIN_APP:
		stParam.stVideoParam.iOutBitrate = 700000;
		stParam.stVideoParam.iOutFps = 10;

		if (m_pAppWinds != NULL)
		{
			stParam.stVideoParam.u.stInApp.paAppPath = (char *)m_pAppWinds[m_iAppChangeIndex].paPath;
		}
		else
			have_video_source = false;

		break;
	case EN_NLSS_VIDEOIN_NONE:
		have_video_source = false;
		break;
	default:
		break;
	}
	stParam.stVideoParam.enInType = m_videoSourceType;
	stParam.stAudioParam.iInSamplerate = 44100;// m_iAudioSample;

	stParam.stAudioParam.paaudioDeviceName = "";
	switch (m_audioSourceType)
	{
	case EN_NLSS_AUDIOIN_MIC:
		if (m_pAudioDevices != NULL)
		{
			stParam.stAudioParam.paaudioDeviceName = m_pAudioDevices[m_CurrentMicIndex].paPath;
		}
		else
			have_audio_source = false;
		break;

	case EN_NLSS_AUDIOIN_NONE:
		have_audio_source = false;
		break;
	default:
		break;
	}

	stParam.stAudioParam.enInType = m_audioSourceType;

	if (have_audio_source && have_video_source)
		stParam.enOutContent = EN_NLSS_OUTCONTENT_AV;//Ĭ������Ƶ�豸����������������Ƶ����Ȼ��Ҳ�������ó���Ƶ/��Ƶ��
	else if (have_audio_source && !have_video_source)
		stParam.enOutContent = EN_NLSS_OUTCONTENT_AUDIO;
	else if (have_video_source && !have_audio_source)
		stParam.enOutContent = EN_NLSS_OUTCONTENT_VIDEO;
	else if (!have_audio_source && !have_video_source)
	{
		MessageBox(NULL, L"û����ƵԴҲû����ƵԴ���룬����������ʼ��ʧ��", L"����ʱ��", MB_OK);
		return false;
	}

	std::string strUrl = m_strUrl.toStdString();
	stParam.paOutUrl = new char[1024];
	memset(stParam.paOutUrl, 0, 1024);
	strcpy(stParam.paOutUrl, strUrl.c_str());

	if (NLSS_OK != Nlss_InitParam(m_hNlssService, &stParam))
	{
		MessageBox(NULL, L"����������ʼ��ʧ��", L"����ʱ��", MB_OK);
		delete[]stParam.paOutUrl;
		return false;
	}
	m_bInited = true;

	delete[]stParam.paOutUrl;
	return true;
}

void UIVideo::EnumAvailableMediaDevices()
{
	//Ĭ������Ϊ�����棬�Լ��������湲���Լ�������Ƶ
	Nlss_GetFreeDevicesNum(&m_iVideoDeviceNum, &m_iAudioDeviceNum);
	if (m_iVideoDeviceNum <= 0)
	{
		MessageBox(NULL, L"����������ͷ�豸", L"����ʱ��", MB_OK);
	}
	else
	{
		if (m_pVideoDevices)
		{
			delete[] m_pVideoDevices;
			m_pVideoDevices = NULL;
		}

		m_pVideoDevices = new ST_NLSS_INDEVICE_INF[m_iVideoDeviceNum];
		for (int i = 0; i < m_iVideoDeviceNum; i++)
		{
			m_pVideoDevices[i].paPath = new char[1024];
			m_pVideoDevices[i].paFriendlyName = new char[1024];
		}
	}

	if (m_iAudioDeviceNum <= 0)
	{
		MessageBox(NULL, L"��������˷��豸", L"����ʱ��", MB_OK);
	}
	else
	{
		if (m_pAudioDevices)
		{
			delete[] m_pAudioDevices;
			m_pAudioDevices = NULL;
		}

		m_pAudioDevices = new ST_NLSS_INDEVICE_INF[m_iAudioDeviceNum];

		for (int i = 0; i < m_iAudioDeviceNum; i++)
		{
			m_pAudioDevices[i].paPath = new char[1024];
			m_pAudioDevices[i].paFriendlyName = new char[1024];
		}
	}
	// ��ȡ��Ƶ����Ƶ�豸
	Nlss_GetFreeDeviceInf(m_pVideoDevices, m_pAudioDevices);

	GetAvailableAppWindNum(&m_iAppWindNum);
	if (m_iAppWindNum <= 0)
	{
//		ui.app_comboBox->addItem(QString("��ѡ��������ƵԴ"));
	}
	else
	{
		if (m_pAppWinds)
		{
			delete[] m_pAppWinds;
			m_pAppWinds = NULL;
		}

		m_pAppWinds = new ST_NLSS_INDEVICE_INF[m_iAppWindNum];
		for (int i = 0; i < m_iAppWindNum; i++)
		{
			m_pAppWinds[i].paPath = new char[1024];
			m_pAppWinds[i].paFriendlyName = new char[1024];
		}
	}

	GetAvailableAppWind(m_pAppWinds);
}

void UIVideo::StartLiveVideo()
{
	if (m_hNlssService == NULL)
	{
		MessageBox(NULL, L"ֱ����Ϊ�գ��򿪡��ر� Ԥ�� / ֱ��ʧ��", L"����ʱ��", MB_OK);
		return;
	}

	if (m_bPreviewing)
	{
		Nlss_StopVideoPreview(m_hNlssService);
		Nlss_StopVideoCapture(m_hNlssService);
	}

	Nlss_UninitParam(m_hNlssService);

	if (!InitMediaCapture())
	{
		MessageBox(NULL, L"��ʼ������ʧ��", L"����ʱ��", MB_OK);
		return;
	}

	Nlss_SetVideoWaterMark(m_hNlssService, NULL);
	Nlss_SetVideoDisplayRatio(m_hNlssService, 0, 0);

	if (NLSS_OK != Nlss_StartVideoCapture(m_hNlssService))
	{
		MessageBox(NULL, L"����Ƶ�ɼ�����", L"����ʱ��", MB_OK);
		return;
	}

	if (NLSS_OK != Nlss_StartVideoPreview(m_hNlssService))
	{
		MessageBox(NULL, L"����ƵԤ���������������Ϣ�뿴����ֵ", L"����ʱ��", MB_OK);
		return;
	}

	if (m_strUrl.isEmpty())
	{
		MessageBox(NULL, L"������ַΪ�գ�����������ʼ��ʧ��", L"����ʱ��", MB_OK);
		return;
	}

// 		m_StartLiveTimer->start(1000);
// 		connect(m_StartLiveTimer, SIGNAL(timeout()), this, SLOT(slot_onStartLiveTimeout()));
 	m_pWorker->SetMediaCapture(m_hNlssService);
 	emit sig_StartLiveStream();
	m_bPreviewing = true;
	m_bLiving = true;
}

void UIVideo::StopLiveVideo()
{
	if (m_bLiving)
	{
		emit sig_StopLiveStream();
		m_bLiving = false;
	}
}

void UIVideo::slot_onRefreshTimeout()
{
	update();
}

void UIVideo::slot_onStartLiveTimeout()
{
	m_StartLiveTimer->stop();
	m_pWorker->SetMediaCapture(m_hNlssService);
	emit sig_StartLiveStream();
}

#ifdef STARTLS_ASYNC

void UIVideo::slot_FinishStartLiveStream(int iRet)
{
	if ((NLSS_RET)iRet != NLSS_OK)
	{
		MessageBox(NULL, L"��ֱ���������������Ϣ�뿴����ֵ", L"����ʱ��", MB_OK);
	}
}

void UIVideo::slot_FinishStopLiveStream(int iRet)
{
	emit sig_changeLiveStatus(false);
	return;
}
#endif

void UIVideo::ChangeAppPath(int index)
{
	m_iAppChangeIndex = index;
}

bool UIVideo::IsCurrentPreview()
{
	return m_bPreviewing;
}

bool UIVideo::IsCurrentLiving()
{
	return m_bLiving;
}

void UIVideo::SetPauseVideo()
{
	Nlss_PauseVideoLiveStream(m_hNlssService);
}

void UIVideo::SetResumeVideo()
{
	Nlss_ResumeVideoLiveStream(m_hNlssService);
}

void UIVideo::SetPauseAudio()
{
	Nlss_PauseAudioLiveStream(m_hNlssService);
}

void UIVideo::SetResumeAudio()
{
	Nlss_ResumeAudioLiveStream(m_hNlssService);
}

void UIVideo::ChangeLiveVideo()
{
	if (m_hNlssService == NULL)
	{
		MessageBox(NULL, L"ֱ����Ϊ�գ��򿪡��ر� Ԥ�� / ֱ��ʧ��", L"����ʱ��", MB_OK);
		return;
	}

	if (m_bLiving)
	{
		emit sig_StopLiveStream();

		Nlss_UninitParam(m_hNlssService);

 		Nlss_StopVideoCapture(m_hNlssService);
 		Nlss_StopVideoPreview(m_hNlssService);

		if (!InitMediaCapture())
		{
			MessageBox(NULL, L"��ʼ������ʧ��", L"����ʱ��", MB_OK);
			return;
		}

		Nlss_SetVideoWaterMark(m_hNlssService, NULL);

		Nlss_SetVideoDisplayRatio(m_hNlssService, 0, 0);

		if (NLSS_OK != Nlss_StartVideoCapture(m_hNlssService))
		{
			MessageBox(NULL, L"����Ƶ�ɼ�����", L"����ʱ��", MB_OK);
			return;
		}

		if (NLSS_OK != Nlss_StartVideoPreview(m_hNlssService))
		{
			MessageBox(NULL, L"����ƵԤ���������������Ϣ�뿴����ֵ", L"����ʱ��", MB_OK);
			return;
		}

 		m_pWorker->SetMediaCapture(m_hNlssService);
 		emit sig_StartLiveStream();
	}
	else
	{
		if (m_bPreviewing)
		{
			Nlss_StopVideoPreview(m_hNlssService);
			Nlss_StopVideoCapture(m_hNlssService);
		}

		Nlss_UninitParam(m_hNlssService);
		
		if (!InitMediaCapture())
		{
			MessageBox(NULL, L"��ʼ������ʧ��", L"����ʱ��", MB_OK);
			return;
		}

		Nlss_SetVideoWaterMark(m_hNlssService, NULL);

		Nlss_SetVideoDisplayRatio(m_hNlssService, 0, 0);

		if (NLSS_OK != Nlss_StartVideoCapture(m_hNlssService))
		{
			MessageBox(NULL, L"����Ƶ�ɼ�����", L"����ʱ��", MB_OK);
			return;
		}

		if (NLSS_OK != Nlss_StartVideoPreview(m_hNlssService))
		{
			MessageBox(NULL, L"����ƵԤ���������������Ϣ�뿴����ֵ", L"����ʱ��", MB_OK);
			return;
		}
	}
	m_bPreviewing = true;
}

void UIVideo::StopCaptureVideo()
{
	Nlss_StopVideoCapture(m_hNlssService);
	Nlss_StopVideoPreview(m_hNlssService);
}

void UIVideo::setLessonName(QString strLessonName)
{
	//[��ǰֱ���γ�]
	ui.lessonName_label->setText(strLessonName);
}