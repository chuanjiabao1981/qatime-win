#include "UICamera.h"
#include <QPainter>
#include "windows.h"
#include <iosfwd>
#include <sstream>
#include "define.h"
#include <QImage>

QMutex UICamera::m_mutex;
ST_NLSS_VIDEO_SAMPLER UICamera::m_SvideoSampler;
UICamera*	UICamera::m_pThis=NULL;
#ifdef TEST
	#define _DEBUG
#else
#endif
UICamera::UICamera(QWidget *parent)
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
	, m_bStopLiveFinish(true)
	, m_Parent(NULL)
	, m_pBkImage(NULL)
	, m_refreshTimer(NULL)
{
	ui.setupUi(this);
	m_pThis = this;

	UICamera::m_SvideoSampler.iWidth = 0;
	UICamera::m_SvideoSampler.iHeight = 0;
	UICamera::m_SvideoSampler.puaData = NULL;//一次性分配一个最大的控件，不断填充不同的数据
	UICamera::m_SvideoSampler.iDataSize = 0;

	m_StartLiveTimer = new QTimer(this);

	connect(this, SIGNAL(sig_livestreamErrorHappened()), this, SLOT(slot_livestreamErrorHappened()));
#ifdef STARTLS_ASYNC
	m_pWorker = new Worker();
	connect(this, SIGNAL(sig_StartLiveStream()), m_pWorker, SLOT(slot_StartLiveStream()));
	connect(m_pWorker, SIGNAL(sig_ResultReady(int)), this, SLOT(slot_FinishStartLiveStream(int)));
	connect(this, SIGNAL(sig_StopLiveStream()), m_pWorker, SLOT(slot_StopLiveStream()));
	connect(m_pWorker, SIGNAL(sig_StopResult(int)), this, SLOT(slot_FinishStopLiveStream(int)));
#endif

	//创建mediacapture类，失败抛出错	
	if (NLSS_OK != Nlss_Create(NULL, &m_hNlssService))
	{
		MessageBox(NULL, L"创建直播失败，请关闭程序重新启动", L"", MB_OK);
	}

	//默认采集音视频的麦克风以及摄像头设备，所以，先初始化ui控件时，将 音视频设备通通找出来
	//初始化音视频控件，利用到底层库
	EnumAvailableMediaDevices();

	SetMediaCapture(m_hNlssService);

	m_refreshTimer = new QTimer(this);
	m_refreshTimer->start(1000 / 25);
	connect(m_refreshTimer, SIGNAL(timeout()), this, SLOT(slot_onRefreshTimeout()));

//	StartLiveVideo();
}

UICamera::~UICamera()
{
	//销毁mediacapture类
	if (m_hNlssService != NULL)
	{
		if (!m_bLiving)
		{
			Nlss_StopLiveStream(m_hNlssService);
			Nlss_Destroy(m_hNlssService);
			m_hNlssService = NULL;
		}
	}

	if (m_SvideoSampler.puaData != NULL)
	{
		delete[] m_SvideoSampler.puaData;
		m_SvideoSampler.puaData = NULL;
	}

	if (m_Parent)
		m_Parent = NULL;

	if (m_pBkImage)
	{
		delete m_pBkImage;
		m_pBkImage = NULL;
	}
}

void UICamera::setPlugFlowUrl(QString url)
{
	m_strUrl = url;
}

void UICamera::SetMediaCapture(_HNLSSERVICE hNlssService)
{
	m_hNlssService = hNlssService;
	PFN_NLSS_VIDEOSAMPLER_CB frame_cb = UICamera::SetVideoSampler;
	Nlss_SetVideoSamplerCB(m_hNlssService, frame_cb);

	PFN_NLSS_STATUS_NTY notify = UICamera::OnLiveStreamStatusNty;
	Nlss_SetStatusCB(m_hNlssService, notify);
}

void UICamera::SetVideoSampler(ST_NLSS_VIDEO_SAMPLER *pSampler)
{
	if (pSampler != NULL)
	{
		m_mutex.lock();
		if (pSampler->iWidth * pSampler->iHeight > m_SvideoSampler.iWidth * m_SvideoSampler.iHeight)
		{
			if (m_SvideoSampler.puaData)
				delete[]m_SvideoSampler.puaData;
			m_SvideoSampler.puaData = new unsigned char[pSampler->iWidth * pSampler->iHeight * 4];
		}

		m_SvideoSampler.iWidth = pSampler->iWidth;
		m_SvideoSampler.iHeight = pSampler->iHeight;
		m_SvideoSampler.iDataSize = pSampler->iDataSize;
		memcpy(m_SvideoSampler.puaData, pSampler->puaData, pSampler->iDataSize);

		m_mutex.unlock();
	}
}

void UICamera::OnLiveStreamStatusNty(EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode)
{
	if (m_pThis == NULL)
	{
		return;
	}

	if (enStatus == EN_NLSS_STATUS_START)
	{
	}
	else if (enStatus == EN_NLSS_STATUS_ERR)
	{
		m_pThis->emit sig_livestreamErrorHappened();
	}
}

void UICamera::paintEvent(QPaintEvent *)
{
	if (this->isVisible())
	{
		QPainter p(this);
		p.setPen(QColor(0x8099be));

		if (m_SvideoSampler.iDataSize > 0)
		{
			QImage qimage;
			m_mutex.lock();
			qimage = QImage((uchar*)m_SvideoSampler.puaData, m_SvideoSampler.iWidth, m_SvideoSampler.iHeight, QImage::Format_RGB32);
			p.drawImage(rect(), qimage);
			m_mutex.unlock();
		}
		else
		{
			if (!m_bLiving)
			{
				if (NULL != m_pBkImage)
					p.drawPixmap(QPoint(0, 0), *m_pBkImage);
			}

		}
	}
}

bool UICamera::InitMediaCapture()
{
	bool have_video_source = true;
	bool have_audio_source = true;
	ST_NLSS_PARAM stParam;
	Nlss_GetDefaultParam(m_hNlssService, &stParam);

	m_videoSourceType = EN_NLSS_VIDEOIN_CAMERA;// ;

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
		stParam.stVideoParam.iOutBitrate = 300000;

		break;
	case EN_NLSS_VIDEOIN_APP:
		stParam.stVideoParam.iOutBitrate = 300000;
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

	if (have_video_source)
		stParam.enOutContent = EN_NLSS_OUTCONTENT_VIDEO;
	else if (!have_audio_source && !have_video_source)
	{
		qDebug() << "没有视频源也没有音频源输入";
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

void UICamera::EnumAvailableMediaDevices()
{
	//默认设置为有桌面，以及部分桌面共享，以及不打开视频
	Nlss_GetFreeDevicesNum(&m_iVideoDeviceNum, &m_iAudioDeviceNum);
	if (m_iVideoDeviceNum <= 0)
	{
		qDebug() << "没有摄像头设备";
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
		qDebug() << "没有麦克风设备";
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
	// 获取视频和音频设备
	Nlss_GetFreeDeviceInf(m_pVideoDevices, m_pAudioDevices);
}

void UICamera::StartLiveVideo()
{
	if (m_hNlssService == NULL)
	{
		MessageBox(NULL, L"直播类为空，打开、关闭 预览 / 直播失败", L"答疑时间", MB_OK);
		return;
	}

	Nlss_UninitParam(m_hNlssService);
	if (!InitMediaCapture())
	{
		MessageBox(NULL, L"初始化参数失败", L"答疑时间", MB_OK);
		return;
	}

	if (m_bPreviewing)
	{
		Nlss_StopVideoPreview(m_hNlssService);
		Nlss_StopVideoCapture(m_hNlssService);
	}

 	Nlss_SetVideoWaterMark(m_hNlssService, NULL);
 	Nlss_SetVideoDisplayRatio(m_hNlssService, 0, 0);

	if (NLSS_OK != Nlss_StartVideoCapture(m_hNlssService))
	{
		qDebug() << "打开视频采集出错";
		return;
	}

	if (NLSS_OK != Nlss_StartVideoPreview(m_hNlssService))
	{
		qDebug() << "打开视频预览出错";
		return;
	}

	if (!m_bPreviewing)
	{
		m_bPreviewing = true;
		return;
	}
	if (m_strUrl.isEmpty())
	{
		MessageBox(NULL, L"推流地址为空，推流参数初始化失败", L"答疑时间", MB_OK);
		return;
	}
	m_pWorker->SetMediaCapture(m_hNlssService);
	emit sig_StartLiveStream();
	m_bLiving = true;
}

void UICamera::StopLiveVideo()
{
	if (m_bLiving)
	{
		emit sig_StopLiveStream();
		m_bLiving = false;
	}
}

void UICamera::slot_onRefreshTimeout()
{
	update();
}

void UICamera::slot_onStartLiveTimeout()
{
	if (m_bStopLiveFinish)
	{
		m_StartLiveTimer->stop();
		m_pWorker->SetMediaCapture(m_hNlssService);
		emit sig_StartLiveStream();
	}
}

#ifdef STARTLS_ASYNC

void UICamera::slot_FinishStartLiveStream(int iRet)
{
	if ((NLSS_RET)iRet != NLSS_OK)
	{
		m_Parent->UpatateLiveStatus(this,false);
	}
	else
	{
		m_Parent->UpatateLiveStatus(this,true);
	}

	m_Parent->setCameraEnable();
}

void UICamera::slot_FinishStopLiveStream(int iRet)
{
	if (iRet == 0)
	{
		m_bStopLiveFinish = true;
		m_Parent->setCameraEnable();
		m_Parent->StopSuccess(this);
	}
}
#endif

void UICamera::ChangeAppPath(int index)
{
	m_iAppChangeIndex = index;
}

bool UICamera::IsCurrentPreview()
{
	return m_bPreviewing;
}

bool UICamera::IsCurrentLiving()
{
	return m_bLiving;
}

void UICamera::SetPauseVideo()
{
	Nlss_PauseVideoLiveStream(m_hNlssService);
}

void UICamera::SetResumeVideo()
{
	Nlss_ResumeVideoLiveStream(m_hNlssService);
}

void UICamera::SetPauseAudio(HWND hwnd)
{
	Nlss_PauseAudioLiveStream(m_hNlssService);
}

void UICamera::SetResumeAudio(HWND hwnd)
{
	Nlss_ResumeAudioLiveStream(m_hNlssService);
}

void UICamera::ChangeLiveVideo()
{
// 	if (m_hNlssService == NULL)
// 	{
// 		MessageBox(NULL, L"直播类为空，打开、关闭 预览 / 直播失败", L"答疑时间", MB_OK);
// 		return;
// 	}
// 
// 	if (m_bLiving)
// 	{
// 		m_bStopLiveFinish = false;
// 		emit sig_StopLiveStream();
// 
// 		Nlss_UninitParam(m_hNlssService);
// 
// 		Nlss_StopVideoPreview(m_hNlssService);
//  		Nlss_StopVideoCapture(m_hNlssService);
// 
// 		if (!InitMediaCapture())
// 		{
// 			MessageBox(NULL, L"初始化参数失败", L"答疑时间", MB_OK);
// 			return;
// 		}
// 
// 		Nlss_SetVideoWaterMark(m_hNlssService, NULL);
// 
// 		Nlss_SetVideoDisplayRatio(m_hNlssService, 0, 0);
// 
// 		if (NLSS_OK != Nlss_StartVideoCapture(m_hNlssService))
// 		{
// 			MessageBox(NULL, L"打开视频采集出错", L"答疑时间", MB_OK);
// 			return;
// 		}
// 
// 		if (NLSS_OK != Nlss_StartVideoPreview(m_hNlssService))
// 		{
// 			MessageBox(NULL, L"打开视频预览出错，具体错误信息请看返回值", L"答疑时间", MB_OK);
// 			return;
// 		}
// 
// 		if (m_bStopLiveFinish)// 彻底结束直播流,才可进行直播
// 		{
// 			m_pWorker->SetMediaCapture(m_hNlssService);
// 			emit sig_StartLiveStream();
// 		}
// 		else
// 		{
// 			m_StartLiveTimer->start(1000);
// 			connect(m_StartLiveTimer, SIGNAL(timeout()), this, SLOT(slot_onStartLiveTimeout()));
// 		}
//  		
// 	}
// 	else
// 	{
// 		if (m_bPreviewing)
// 		{
// 			Nlss_StopVideoPreview(m_hNlssService);
// 			Nlss_StopVideoCapture(m_hNlssService);
// 		}
// 
// 		Nlss_UninitParam(m_hNlssService);
// 		
// 		if (!InitMediaCapture())
// 		{
// 			MessageBox(NULL, L"初始化参数失败", L"答疑时间", MB_OK);
// 			return;
// 		}
// 
// 		Nlss_SetVideoWaterMark(m_hNlssService, NULL);
// 
// 		Nlss_SetVideoDisplayRatio(m_hNlssService, 0, 0);
// 
// 		if (NLSS_OK != Nlss_StartVideoCapture(m_hNlssService))
// 		{
// 			MessageBox(NULL, L"打开视频采集出错", L"答疑时间", MB_OK);
// 			return;
// 		}
// 
// 		if (NLSS_OK != Nlss_StartVideoPreview(m_hNlssService))
// 		{
// 			MessageBox(NULL, L"打开视频预览出错，具体错误信息请看返回值", L"答疑时间", MB_OK);
// 			return;
// 		}
// 	}
	m_bPreviewing = true;
}

void UICamera::StopCaptureVideo()
{
	m_bPreviewing = false;
	Nlss_StopVideoCapture(m_hNlssService);
	Nlss_StopVideoPreview(m_hNlssService);
}

void UICamera::setLessonName(QString strLessonName)
{

}

void UICamera::setPersonNum(int num)
{

}

void UICamera::SetMainWnd(UIWindowSet* wnd)
{
	m_Parent = wnd;
}

void UICamera::SetVideoWnd(HWND hWnd)
{
	m_VideoWnd = hWnd;
}

void UICamera::SetChangeVideo(int index)
{
	m_CurrentVideoIndex = index;
	refurbish();
}

void UICamera::resizeEvent(QResizeEvent *e)
{
// 	if (m_BoadWnd)
// 		m_BoadWnd->resize(e->size().width(),e->size().height());
}

void UICamera::setBkImage(QString qsImage)
{
	if (!qsImage.isEmpty())
	{
		if (NULL == m_pBkImage)
		{
			m_pBkImage = new QPixmap(qsImage);
		}
		else
		{
			m_pBkImage->load(qsImage);
		}
	}
}

void UICamera::slot_livestreamErrorHappened()
{
	m_Parent->ErrorStopLive(this);
}

void UICamera::refurbish()
{
	if (m_bLiving)
		return;
	
	if (m_hNlssService == NULL)
	{
		MessageBox(NULL, L"直播类为空，打开、关闭 预览 / 直播失败", L"答疑时间", MB_OK);
		return;
	}

	Nlss_UninitParam(m_hNlssService);
	if (!InitMediaCapture())
	{
		MessageBox(NULL, L"初始化参数失败", L"答疑时间", MB_OK);
		return;
	}

	if (m_bPreviewing)
	{
		Nlss_StopVideoPreview(m_hNlssService);
		Nlss_StopVideoCapture(m_hNlssService);
	}

	Nlss_SetVideoWaterMark(m_hNlssService, NULL);
	Nlss_SetVideoDisplayRatio(m_hNlssService, 0, 0);

	if (NLSS_OK != Nlss_StartVideoCapture(m_hNlssService))
	{
		qDebug() << "打开视频采集出错";
		return;
	}

	if (NLSS_OK != Nlss_StartVideoPreview(m_hNlssService))
	{
		qDebug() << "打开视频预览出错";
		return;
	}

	if (!m_bPreviewing)
	{
		m_bPreviewing = true;
		return;
	}
}