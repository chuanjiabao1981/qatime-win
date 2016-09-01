#include "UIViedeo.h"
#include <QPainter>
#include "windows.h"

QMutex UIViedeo::m_mutex;
ST_NLSS_VIDEO_SAMPLER UIViedeo::m_SvideoSampler;

UIViedeo::UIViedeo(QWidget *parent)
	: QWidget(parent)
	, m_videoSourceType(EN_NLSS_VIDEOIN_CAMERA)
	, m_audioSourceType(EN_NLSS_AUDIOIN_SYS)
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
{
	ui.setupUi(this);

	UIViedeo::m_SvideoSampler.iWidth = 0;
	UIViedeo::m_SvideoSampler.iHeight = 0;
	UIViedeo::m_SvideoSampler.puaData = NULL;//一次性分配一个最大的控件，不断填充不同的数据
	UIViedeo::m_SvideoSampler.iDataSize = 0;

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

	//默认采集音视频的麦克风以及摄像头设备，所以，先初始化ui控件时，将 音视频设备通通找出来
	//初始化音视频控件，利用到底层库
	EnumAvailableMediaDevices();

	// 初始化推流URL
	InitPlugFlowUrl();

	//创建mediacapture类，失败抛出错	
	if (NLSS_OK != Nlss_Create(NULL, &m_hNlssService))
	{
		MessageBox(NULL, L"创建直播失败，请关闭程序重新启动", L"", MB_OK);
	}

	SetMediaCapture(m_hNlssService);
}

UIViedeo::~UIViedeo()
{
	if (m_SvideoSampler.puaData != NULL)
	{
		delete[] m_SvideoSampler.puaData;
		m_SvideoSampler.puaData = NULL;
	}

	//删除定时器
	if (m_refreshTimer)
	{
		if (m_refreshTimer->isActive())
			m_refreshTimer->stop();
		delete m_refreshTimer;
		m_refreshTimer = NULL;
	}

	//销毁mediacapture类
	if (m_hNlssService != NULL)
	{
		Nlss_Destroy(m_hNlssService);
		m_hNlssService = NULL;
	}
}

void UIViedeo::InitPlugFlowUrl()
{
#ifdef _DEBUG
	m_strUrl = "rtmp://pa0a19f55.live.126.net/live/ae753e52ec6741fbb94ed4c0aea672c6?wsSecret=cacb5b393123f706e6f7b6e6a8291259&wsTime=1472695026";
#else
	// 待编写，需要从服务端获取
#endif
}

void UIViedeo::SetMediaCapture(_HNLSSERVICE hNlssService)
{
	PFN_NLSS_STATUS_NTY notify = UIViedeo::OnLiveStreamStatusNty;
	Nlss_SetStatusCB(hNlssService, notify);

	PFN_NLSS_VIDEOSAMPLER_CB frame_cb = UIViedeo::SetVideoSampler;
	Nlss_SetVideoSamplerCB(m_hNlssService, frame_cb);
}

void UIViedeo::SetVideoSampler(ST_NLSS_VIDEO_SAMPLER *pSampler)
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

void UIViedeo::OnLiveStreamStatusNty(EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode)
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

void UIViedeo::paintEvent(QPaintEvent *)
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

bool UIViedeo::InitMediaCapture()
{
	bool have_video_source = true;
	bool have_audio_source = true;
	ST_NLSS_PARAM stParam;
	Nlss_GetDefaultParam(m_hNlssService, &stParam);

	switch (m_videoSourceType)
	{
	case EN_NLSS_VIDEOIN_FULLSCREEN:
		stParam.stVideoParam.iOutBitrate = 1500000;
		stParam.stVideoParam.iOutFps = 10;
		break;
	case EN_NLSS_VIDEOIN_CAMERA:
		//获取视频参数
		if (m_pVideoDevices != NULL)
		{
			stParam.stVideoParam.u.stInCamera.paDevicePath = (char *)m_pVideoDevices[0].paPath;
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
		stParam.stVideoParam.iOutBitrate = 300000;

		break;
	case EN_NLSS_VIDEOIN_APP:
		stParam.stVideoParam.iOutBitrate = 1000000;
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
	stParam.stAudioParam.iInSamplerate = 4410;// m_iAudioSample;

	stParam.stAudioParam.paaudioDeviceName = "";
	switch (m_audioSourceType)
	{
	case EN_NLSS_AUDIOIN_MIC:
		if (m_pAudioDevices != NULL)
		{
			stParam.stAudioParam.paaudioDeviceName = m_pAudioDevices[0].paPath;

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
		stParam.enOutContent = EN_NLSS_OUTCONTENT_AV;//默认音视频设备都存在则推流音视频，当然，也可以设置成音频/视频，
	else if (have_audio_source && !have_video_source)
		stParam.enOutContent = EN_NLSS_OUTCONTENT_AUDIO;
	else if (have_video_source && !have_audio_source)
		stParam.enOutContent = EN_NLSS_OUTCONTENT_VIDEO;
	else if (!have_audio_source && !have_video_source)
	{
		MessageBox(NULL, L"没有视频源也没有音频源输入，推流参数初始化失败", L"答疑时间", MB_OK);
		return false;
	}

	std::string strUrl = m_strUrl.toStdString();
	stParam.paOutUrl = new char[1024];
	memset(stParam.paOutUrl, 0, 1024);
	strcpy(stParam.paOutUrl, strUrl.c_str());

	if (NLSS_OK != Nlss_InitParam(m_hNlssService, &stParam))
	{
		MessageBox(NULL, L"推流参数初始化失败", L"答疑时间", MB_OK);
		delete[]stParam.paOutUrl;
		return false;
	}
	m_bInited = true;

	delete[]stParam.paOutUrl;
	return true;
}

void UIViedeo::EnumAvailableMediaDevices()
{
	//默认设置为有桌面，以及部分桌面共享，以及不打开视频
	Nlss_GetFreeDevicesNum(&m_iVideoDeviceNum, &m_iAudioDeviceNum);
	if (m_iVideoDeviceNum <= 0)
	{
		MessageBox(NULL, L"请连接摄像头设备", L"答疑时间", MB_OK);
	}
	else
	{
		m_pVideoDevices = new ST_NLSS_INDEVICE_INF[m_iVideoDeviceNum];
		for (int i = 0; i < m_iVideoDeviceNum; i++)
		{
			m_pVideoDevices[i].paPath = new char[1024];
			m_pVideoDevices[i].paFriendlyName = new char[1024];
		}
	}

	if (m_iAudioDeviceNum <= 0)
	{
		MessageBox(NULL, L"请连接麦克风设备", L"答疑时间", MB_OK);
	}
	else
	{
		m_pAudioDevices = new ST_NLSS_INDEVICE_INF[m_iAudioDeviceNum];

		for (int i = 0; i < m_iAudioDeviceNum; i++)
		{
			m_pAudioDevices[i].paPath = new char[1024];
			m_pAudioDevices[i].paFriendlyName = new char[1024];
		}
	}
	// 获取视频和音频设备
	Nlss_GetFreeDeviceInf(m_pVideoDevices, m_pAudioDevices);

	GetAvailableAppWindNum(&m_iAppWindNum);
	if (m_iAppWindNum <= 0)
	{
//		ui.app_comboBox->addItem(QString("请选择其他视频源"));
	}
	else
	{
		m_pAppWinds = new ST_NLSS_INDEVICE_INF[m_iAppWindNum];
		for (int i = 0; i < m_iAppWindNum; i++)
		{
			m_pAppWinds[i].paPath = new char[1024];
			m_pAppWinds[i].paFriendlyName = new char[1024];
		}
	}

	GetAvailableAppWind(m_pAppWinds);

}

void UIViedeo::StartLiveVideo()
{
	if (m_hNlssService == NULL)
	{
		MessageBox(NULL, L"直播类为空，打开、关闭 预览 / 直播失败", L"答疑时间", MB_OK);
		return;
	}

	if (!m_bPreviewing)//不在preview状态，先打开preview，再点击，这个button，才进入直播
	{

		Nlss_UninitParam(m_hNlssService);


		if (!InitMediaCapture())
		{
			MessageBox(NULL, L"初始化参数失败", L"答疑时间", MB_OK);
		}

		Nlss_SetVideoWaterMark(m_hNlssService, NULL);

		Nlss_SetVideoDisplayRatio(m_hNlssService, 0, 0);

		if (NLSS_OK != Nlss_StartVideoCapture(m_hNlssService))
		{
			MessageBox(NULL, L"打开视频采集出错", L"答疑时间", MB_OK);
		}

		if (NLSS_OK != Nlss_StartVideoPreview(m_hNlssService))
		{
			MessageBox(NULL, L"打开视频预览出错，具体错误信息请看返回值", L"答疑时间", MB_OK);
		}

		if (m_strUrl.isEmpty())
		{
			MessageBox(NULL, L"推流地址为空，推流参数初始化失败", L"答疑时间", MB_OK);
		}

// 		m_pWorker->SetMediaCapture(m_hNlssService);
// 		emit sig_StartLiveStream();
		Nlss_StartLiveStream(m_hNlssService);
		m_bPreviewing = true;
		return;
	}
	//可以写在非ui线程，毕竟startLivestream中的socket 连接需要一定的时间，耗时2s左右
// #ifdef STARTLS_ASYNC
// 	m_pWorker->SetMediaCapture(m_hNlssService);
// 	emit sig_StartLiveStream();
// #endif
// 	return;
}

void UIViedeo::StopLiveVideo()
{
	if (m_bPreviewing)
	{
		m_bPreviewing = false;

//		emit sig_StopLiveStream();
		Nlss_StopLiveStream(m_hNlssService);

		if (m_hNlssService != NULL)
		{
			Nlss_StopVideoPreview(m_hNlssService);
			Nlss_StopVideoCapture(m_hNlssService);
		}
	}
}

void UIViedeo::slot_onRefreshTimeout()
{
	update();
}

#ifdef STARTLS_ASYNC

void UIViedeo::slot_FinishStartLiveStream(int iRet)
{
	if ((NLSS_RET)iRet != NLSS_OK)
	{
		MessageBox(NULL, L"打开直播出错，具体错误信息请看返回值", L"答疑时间", MB_OK);
	}
}

void UIViedeo::slot_FinishStopLiveStream(int iRet)
{
	emit sig_changeLiveStatus(false);
	//	m_bLiving = false;
	return;
}
#endif

void UIViedeo::ChangeAppPath(int index)
{
	m_iAppChangeIndex = index;

	StopLiveVideo();
	StartLiveVideo();
}

bool UIViedeo::IsCurrentPreview()
{
	return m_bPreviewing;
}