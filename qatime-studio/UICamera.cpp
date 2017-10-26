#include "UICamera.h"
#include <QPainter>
#include "windows.h"
#include <iosfwd>
#include <sstream>
#include "define.h"
#include <QImage>

_HNLSSCHILDSERVICE hChildVideoService1;

QMutex UICamera::m_mutex;
ST_NLSS_VIDEO_SAMPLER UICamera::m_SvideoSampler;
UICamera*	UICamera::m_pThis=NULL;
#define  NLSS_720P_BITRATE 800000

ST_NLSS_INDEVICE_INF *m_pVideoDevices; 
ST_NLSS_INDEVICE_INF *m_pAudioDevices;

int GetOutBitrate(int iWidth, int iHeight, int iFps)
{
	if (iWidth == 0 || iHeight == 0 || iFps == 0)
	{
		return  NLSS_720P_BITRATE; //1280*720 20fps,设置的码率
	}

	int iOutBitrate = NLSS_720P_BITRATE * iWidth / 1280;
	iOutBitrate = iOutBitrate * iHeight / 720;
	if (iFps >= 15)
	{
		iOutBitrate = iOutBitrate * iFps / 20;
	}
	else if (iFps < 15)
	{
		iOutBitrate = iOutBitrate * 15 / 20;
	}

	return iOutBitrate;
}

bool SetVideoOutParam(ST_NLSS_VIDEOOUT_PARAM *pstVideoParam, EN_NLSS_VIDEOQUALITY_LVL enVideoQ, bool bWideScreen)
{
	pstVideoParam->enOutCodec = EN_NLSS_VIDEOOUT_CODEC_X264;
	pstVideoParam->bHardEncode = false;
	pstVideoParam->iOutFps = 18;

	switch (enVideoQ)
	{
	case EN_NLSS_VIDEOQUALITY_SUPER:
		pstVideoParam->iOutWidth = 1920;
		pstVideoParam->iOutHeight = 1080;
		pstVideoParam->iOutFps = 13;
		break;
	case EN_NLSS_VIDEOQUALITY_HIGH:
		pstVideoParam->iOutWidth = 1280;
		pstVideoParam->iOutHeight = 720;
		pstVideoParam->iOutFps = 13;
		break;
	case EN_NLSS_VIDEOQUALITY_MIDDLE:
		pstVideoParam->iOutWidth = 640;
		pstVideoParam->iOutHeight = 480;
		pstVideoParam->iOutFps = 15;
		break;
	case EN_NLSS_VIDEOQUALITY_LOW:
		pstVideoParam->iOutWidth = 320;
		pstVideoParam->iOutHeight = 240;
		break;
	default:
		return false;
		break;
	}

	if (bWideScreen)
	{
		pstVideoParam->iOutWidth = pstVideoParam->iOutWidth / 4 * 4;
		pstVideoParam->iOutHeight = pstVideoParam->iOutWidth * 9 / 16;
		pstVideoParam->iOutHeight = pstVideoParam->iOutHeight / 4 * 4;
	}
	pstVideoParam->iOutBitrate = GetOutBitrate(pstVideoParam->iOutWidth, pstVideoParam->iOutHeight, pstVideoParam->iOutFps);
	return true;
}

bool SetVideoInParam(ST_NLSS_VIDEOIN_PARAM *pstVideoParam, EN_NLSS_VIDEOIN_TYPE mVideoSourceType, char *pVideoPath, EN_NLSS_VIDEOQUALITY_LVL  enLvl)
{
	pstVideoParam->enInType = mVideoSourceType;
	switch (mVideoSourceType)
	{
	case EN_NLSS_VIDEOIN_FULLSCREEN:
	{
		pstVideoParam->iCaptureFps = 10;
	}
	break;
	case EN_NLSS_VIDEOIN_CAMERA:
		//获取视频参数
		pstVideoParam->iCaptureFps = 15;
		if (m_pVideoDevices != NULL)
		{
			pstVideoParam->u.stInCamera.paDevicePath = pVideoPath;
		}
		else
		{
			return false;
		}
		pstVideoParam->u.stInCamera.enLvl = enLvl;
		break;
	case EN_NLSS_VIDEOIN_RECTSCREEN:
		pstVideoParam->u.stInRectScreen.iRectLeft = 100;
		pstVideoParam->u.stInRectScreen.iRectRight = 500;
		pstVideoParam->u.stInRectScreen.iRectTop = 100;
		pstVideoParam->u.stInRectScreen.iRectBottom = 500;
		pstVideoParam->iCaptureFps = 20;

		break;
	case EN_NLSS_VIDEOIN_APP:
	{
		pstVideoParam->iCaptureFps = 20;
		pstVideoParam->u.stInApp.paAppPath = (char *)pVideoPath;

	}

	break;
	case EN_NLSS_VIDEOIN_PNG:
		pstVideoParam->u.stInPng.paPngPath = "logo.png";
		pstVideoParam->iCaptureFps = 20;
		break;
	case EN_NLSS_VIDEOIN_NONE:
		return false;
		break;
	default:
		return false;
		break;
	}

	return true;
}

void SetAudioParam(ST_NLSS_AUDIO_PARAM *pstAudioParam, char *pAudioPath, EN_NLSS_AUDIOIN_TYPE enAudioType)
{
	pstAudioParam->stIn.iInSamplerate = 44100;
	pstAudioParam->stIn.enInType = enAudioType;
	pstAudioParam->stIn.paaudioDeviceName = pAudioPath;
}

bool initLiveStream(_HNLSSERVICE hNLSService, ST_NLSS_PARAM *pstParam, char *paOutUrl)
{
	pstParam->paOutUrl = new char[1024];
	memset(pstParam->paOutUrl, 0, 1024);
	strcpy(pstParam->paOutUrl, paOutUrl);

	if (NLSS_OK != Nlss_InitParam(hNLSService, pstParam))
	{
		delete[]pstParam->paOutUrl;
		return false;
	}
	delete[]pstParam->paOutUrl;
	return true;
}

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

	m_iVideoOutWidth = 1280;
	m_iVideoOutWidthOrigin = 1280;
	m_iVideoOutHeight = 720;
	m_iVideoOutHeightOrigin = 720;

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
	connect(this, SIGNAL(sig_StopLiveStream()), m_pWorker, SLOT(slot_StopLiveStream()), Qt::DirectConnection);
	
	connect(m_pWorker, SIGNAL(sig_StopResult(int)), this, SLOT(slot_FinishStopLiveStream(int)));
	connect(this, SIGNAL(sig_StopCapture()), m_pWorker, SLOT(slot_StopCapture()), Qt::DirectConnection); 
#endif

	//创建mediacapture类，失败抛出错	
	if (NLSS_OK != Nlss_Create(NULL,NULL, &m_hNlssService))
	{
		MessageBox(NULL, L"创建直播失败，请关闭程序重新启动", L"", MB_OK);
	}

	m_pWorker->SetMediaCapture(m_hNlssService);

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

void UICamera::SetVideoSampler(void *pNlssChildID, ST_NLSS_VIDEO_SAMPLER *pSampler)
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
	char* pVideoPath;
	if (m_pVideoDevices)
		pVideoPath = (char *)m_pVideoDevices[m_CurrentVideoIndex].paPath;
	else
		pVideoPath = "";
	ST_NLSS_PARAM stParam;
	Nlss_GetDefaultParam(m_hNlssService, &stParam);
	SetVideoOutParam(&stParam.stVideoParam, EN_NLSS_VIDEOQUALITY_MIDDLE, true);
	SetAudioParam(&stParam.stAudioParam, "", EN_NLSS_AUDIOIN_MIC);

	stParam.enOutContent = EN_NLSS_OUTCONTENT_VIDEO;
	initLiveStream(m_hNlssService, &stParam, (char*)m_strUrl.toStdString().c_str());

	ST_NLSS_VIDEOIN_PARAM stChildVInParam;
	SetVideoInParam(&stChildVInParam, EN_NLSS_VIDEOIN_CAMERA, pVideoPath, EN_NLSS_VIDEOQUALITY_MIDDLE);
	hChildVideoService1 = Nlss_ChildVideoOpen(m_hNlssService, &stChildVInParam);
	return true;
}

bool UICamera::GetVideoOutParam(ST_NLSS_VIDEOOUT_PARAM *pstVideoParam)
{
	pstVideoParam->enOutCodec = EN_NLSS_VIDEOOUT_CODEC_X264;
	pstVideoParam->bHardEncode = false;
	pstVideoParam->iOutFps = 18;

	switch (m_videoQ)
	{
	case EN_NLSS_VIDEOQUALITY_SUPER:
		pstVideoParam->iOutWidth = 1920;
		pstVideoParam->iOutHeight = 1080;
		break;
	case EN_NLSS_VIDEOQUALITY_HIGH:
		pstVideoParam->iOutWidth = 1280;
		pstVideoParam->iOutHeight = 720;
		break;
	case EN_NLSS_VIDEOQUALITY_MIDDLE:
		pstVideoParam->iOutWidth = 640;
		pstVideoParam->iOutHeight = 480;
		break;
	case EN_NLSS_VIDEOQUALITY_LOW:
		pstVideoParam->iOutWidth = 320;
		pstVideoParam->iOutHeight = 240;
		break;
	default:
		return false;
		break;
	}

	m_iVideoOutHeight = pstVideoParam->iOutHeight;
	m_iVideoOutWidth = pstVideoParam->iOutWidth;
	m_iVideoOutWidthStreach = (float)m_iVideoOutWidth / m_iVideoOutWidthOrigin;
	m_iVideoOutHeigthStreach = (float)m_iVideoOutHeight / m_iVideoOutHeightOrigin;
	m_iVideoOutWidthOrigin = m_iVideoOutWidth;
	m_iVideoOutHeightOrigin = m_iVideoOutHeight;
	pstVideoParam->iOutBitrate = GetOutBitrate(pstVideoParam->iOutWidth, pstVideoParam->iOutHeight, pstVideoParam->iOutFps);
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
	Nlss_GetFreeDeviceInf(m_pVideoDevices,10, m_pAudioDevices,10);
}

void UICamera::StartLiveVideo()
{
	if (m_hNlssService == NULL)
	{
		MessageBox(NULL, L"直播类为空，打开、关闭 预览 / 直播失败", L"答疑时间", MB_OK);
		return;
	}

  	if (m_bPreviewing)
  	{
		if (hChildVideoService1 != NULL)
		{
			Nlss_ChildVideoStopCapture(hChildVideoService1);
			Nlss_ChildVideoClose(hChildVideoService1);
		}
		Nlss_StopVideoPreview(m_hNlssService);
		Nlss_Stop(m_hNlssService);
 	}
	Nlss_UninitParam(m_hNlssService);
	if (!InitMediaCapture())
	{
		MessageBox(NULL, L"初始化参数失败", L"答疑时间", MB_OK);
		return;
	}

	Nlss_Start(m_hNlssService);
	Nlss_ChildVideoSetBackLayer(hChildVideoService1);
	Nlss_ChildVideoStartCapture(hChildVideoService1);
	Nlss_StartVideoPreview(m_hNlssService);

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
	
	emit sig_StartLiveStream();
	m_bLiving = true;
}

bool UICamera::GetVideoInParam(ST_NLSS_VIDEOIN_PARAM *VideoInParam, EN_NLSS_VIDEOIN_TYPE mVideoSourceType, std::string&m_AccID)
{
	VideoInParam->enInType = mVideoSourceType;
	bool bVideoExisten = false;
	switch (mVideoSourceType)
	{
	case EN_NLSS_VIDEOIN_FULLSCREEN:

		VideoInParam->iCaptureFps = 10;
		m_AccID = "video name is fullscreen";
		break;

	case EN_NLSS_VIDEOIN_CAMERA:

		//获取视频参数
		VideoInParam->iCaptureFps = 15;

		if (m_pVideoDevices != NULL)
		{
			VideoInParam->u.stInCamera.paDevicePath = (char*)m_pVideoDevices[m_CurrentVideoIndex].paPath;
		}

		else
		{
			return false;
		}

		VideoInParam->u.stInCamera.enLvl = EN_NLSS_VIDEOQUALITY_MIDDLE;//摄像头的输入分辨率设置成同一个
		break;

	case EN_NLSS_VIDEOIN_NONE:
		return false;
		break;

	default:
		return false;
		break;
	}

	return true;
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
	Nlss_ChildVideoStopCapture(hChildVideoService1);
	Nlss_ChildVideoClose(hChildVideoService1);
	hChildVideoService1 = NULL;
	emit sig_StopCapture();
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

	if (m_bPreviewing)
	{
		if (hChildVideoService1 != NULL)
		{
			Nlss_ChildVideoStopCapture(hChildVideoService1);
			Nlss_ChildVideoClose(hChildVideoService1);
		}
		Nlss_StopVideoPreview(m_hNlssService);
		Nlss_Stop(m_hNlssService);
	}
	Nlss_UninitParam(m_hNlssService);
	if (!InitMediaCapture())
	{
		MessageBox(NULL, L"初始化参数失败", L"答疑时间", MB_OK);
		return;
	}

	Nlss_Start(m_hNlssService);
	Nlss_ChildVideoSetBackLayer(hChildVideoService1);
	Nlss_ChildVideoStartCapture(hChildVideoService1);
	Nlss_StartVideoPreview(m_hNlssService);
}

void UICamera::DeleteThread()
{
	delete m_pWorker;
}