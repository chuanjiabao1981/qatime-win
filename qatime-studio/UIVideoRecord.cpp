#include "UIVideoRecord.h"
#include <QPainter>
#include "windows.h"
#include <iosfwd>
#include <sstream>
#include "define.h"

_HNLSSCHILDSERVICE hChildRecordVideoService1;

UIVideoRecord* UIVideoRecord::m_pThis = NULL;
#define  NLSS_720P_BITRATE 800000

UIVideoRecord::UIVideoRecord(QWidget *parent)
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
	, m_NewParent(NULL)
{
	m_pThis = this;
	m_StartLiveTimer = new QTimer(this);

	connect(this, SIGNAL(sig_livestreamErrorHappened()), this, SLOT(slot_livestreamErrorHappened()));
#ifdef STARTLS_ASYNC
	m_pWorker = new Worker();
	connect(this, SIGNAL(sig_StartLiveStream()), m_pWorker, SLOT(slot_StartLiveStream()));
	connect(m_pWorker, SIGNAL(sig_ResultReady(int)), this, SLOT(slot_FinishStartLiveStream(int)));
	connect(this, SIGNAL(sig_StopLiveStream()), m_pWorker, SLOT(slot_StopLiveStream()));
	connect(m_pWorker, SIGNAL(sig_StopResult(int)), this, SLOT(slot_FinishStopLiveStream(int)));
	connect(this, SIGNAL(sig_StopCapture()), m_pWorker, SLOT(slot_StopCapture()));
#endif

	//创建mediacapture类，失败抛出错	
	
	if (NLSS_OK != Nlss_Create(NULL,NULL, &m_hNlssService))
	{
		MessageBox(NULL, L"创建直播失败，请关闭程序重新启动", L"", MB_OK);
	}

	//默认采集音视频的麦克风以及摄像头设备，所以，先初始化ui控件时，将 音视频设备通通找出来
	//初始化音视频控件，利用到底层库
	EnumAvailableMediaDevices();

	SetMediaCapture(m_hNlssService);
	m_pWorker->SetMediaCapture(m_hNlssService);

//	StartLiveVideo();
}

UIVideoRecord::~UIVideoRecord()
{
	//销毁mediacapture类
	if (m_hNlssService != NULL)
	{
		if (m_bPreviewing)
		{
			Nlss_StopLiveStream(m_hNlssService);
		}

		Nlss_Destroy(m_hNlssService);
		m_hNlssService = NULL;
	}
}

void UIVideoRecord::setPlugFlowUrl(QString url)
{
	m_strUrl = url;
}

void UIVideoRecord::SetMediaCapture(_HNLSSERVICE hNlssService)
{
	m_hNlssService = hNlssService;
	PFN_NLSS_VIDEOSAMPLER_CB frame_cb = UIVideoRecord::SetVideoSampler;
	Nlss_SetVideoSamplerCB(m_hNlssService, frame_cb);

	PFN_NLSS_STATUS_NTY notify = UIVideoRecord::OnLiveStreamStatusNty;
	Nlss_SetStatusCB(m_hNlssService, notify);
}

void UIVideoRecord::SetVideoSampler(void *pNlssChildID, ST_NLSS_VIDEO_SAMPLER *pSampler)
{
	if (pSampler != NULL)
	{

	}
}

void UIVideoRecord::OnLiveStreamStatusNty(EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode)
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
		qDebug() << "直播过程中出现错误";
		m_pThis->emit sig_livestreamErrorHappened();
	}
}

bool UIVideoRecord::InitMediaCapture()
{
	char* pAudioPath;
	if (m_pAudioDevices != NULL)
		pAudioPath = (char*)m_pAudioDevices[m_CurrentMicIndex].paPath;
	else
		pAudioPath = "";
	ST_NLSS_PARAM stParam;
	Nlss_GetDefaultParam(m_hNlssService, &stParam);
	SetVideoOutParam(&stParam.stVideoParam, EN_NLSS_VIDEOQUALITY_HIGH, true);
	SetAudioParam(&stParam.stAudioParam, pAudioPath, EN_NLSS_AUDIOIN_MIC);
	stParam.enOutContent = EN_NLSS_OUTCONTENT_AV;
	initLiveStream(m_hNlssService, &stParam, (char*)m_strUrl.toStdString().c_str());

	ST_NLSS_VIDEOIN_PARAM stChildVInParam;
	SetVideoInParam(&stChildVInParam, EN_NLSS_VIDEOIN_FULLSCREEN, "", EN_NLSS_VIDEOQUALITY_MIDDLE);
	Nlss_Start(m_hNlssService);
	Nlss_StartVideoPreview(m_hNlssService);

	hChildRecordVideoService1 = Nlss_ChildVideoOpen(m_hNlssService, &stChildVInParam);
	Nlss_ChildVideoSetBackLayer(hChildRecordVideoService1);
	Nlss_ChildVideoStartCapture(hChildRecordVideoService1);
	return true;
}

//获取建议的输出目标码率供开发者使用，请关注
int UIVideoRecord::getOutBitrate(int iWidth, int iHeight, int iFps)
{
	if (iWidth == 0 || iHeight == 0 || iFps == 0)
	{
		return  NLSS_720P_BITRATE; //1280*720 15fps ，设置的码率
	}

	int iOutBitrate = NLSS_720P_BITRATE / 1280 * iWidth;
	iOutBitrate = iOutBitrate / 720 * iHeight;
	if (iFps >= 15)
	{
		iOutBitrate = iOutBitrate / 15 * iFps;
	}

	return iOutBitrate;
}

void UIVideoRecord::EnumAvailableMediaDevices()
{
	//默认设置为有桌面，以及部分桌面共享，以及不打开视频
	Nlss_GetFreeDevicesNum(&m_iVideoDeviceNum, &m_iAudioDeviceNum);
	qDebug() << "摄像头个数"<<QString::number(m_iVideoDeviceNum);
	qDebug() << "麦克风个数" << QString::number(m_iAudioDeviceNum);
	
	if (m_iVideoDeviceNum <= 0)
	{
		MessageBox(NULL, L"请连接摄像头设备", L"答疑时间", MB_OK);
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
		MessageBox(NULL, L"请连接麦克风设备", L"答疑时间", MB_OK);
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

void UIVideoRecord::InitDeviceParam()
{
	if ((m_pVideoDevices != NULL) && (m_iVideoDeviceNum>0))
	{

		for (int i = 0; i < m_iVideoDeviceNum; i++)
		{
//			m_NewParent->addVideoDevices(QString::fromUtf8(m_pVideoDevices[i].paFriendlyName), QString::fromUtf8(m_pVideoDevices[i].paPath));
		}
	}


	if ((m_pAudioDevices != NULL) && (m_iAudioDeviceNum>0))
	{

		for (int i = 0; i < m_iAudioDeviceNum; i++)
		{
//			m_NewParent->addAudioDevices(QString::fromLocal8Bit(m_pAudioDevices[i].paFriendlyName),QString::fromLocal8Bit(m_pAudioDevices[i].paPath));

		}
	}
}

void UIVideoRecord::StartLiveVideo()
{
	if (m_hNlssService == NULL)
	{
		MessageBox(NULL, L"直播类为空，打开、关闭 预览 / 直播失败", L"答疑时间", MB_OK);
		return;
	}

// 	if (m_bPreviewing)
// 	{
		if (hChildRecordVideoService1)
		{
			Nlss_ChildVideoStopCapture(hChildRecordVideoService1);
			Nlss_ChildVideoClose(hChildRecordVideoService1);
		}
		Nlss_StopVideoPreview(m_hNlssService);
		Nlss_Stop(m_hNlssService);
//	}

	Nlss_UninitParam(m_hNlssService);
	if (!InitMediaCapture())
	{
		MessageBox(NULL, L"初始化参数失败", L"答疑时间", MB_OK);
		return;
	}

	ST_NLSS_VIDEO_WATER_PARAM stFilterParam;
	stFilterParam.pucFilePath = "WaterMark.png";
	stFilterParam.uiStartx = 10;
	stFilterParam.uiStarty = 10;
	Nlss_SetVideoWaterMark(m_hNlssService, &stFilterParam);
	
// 	if (!m_bPreviewing)
// 	{
// 		m_bPreviewing = true;
// 		return;
// 	}
	if (m_strUrl.isEmpty())
	{
		MessageBox(NULL, L"推流地址为空，推流参数初始化失败", L"答疑时间", MB_OK);
		return;
	}
	
 	m_pWorker->SetMediaCapture(m_hNlssService);
 	emit sig_StartLiveStream();
	m_bLiving = true;
}

void UIVideoRecord::StopLiveVideo()
{
	if (m_bLiving)
	{
		emit sig_StopLiveStream();
		m_bLiving = false;
	}
}

#ifdef STARTLS_ASYNC

// 开始直播完成
void UIVideoRecord::slot_FinishStartLiveStream(int iRet)
{
	if ((NLSS_RET)iRet != NLSS_OK)
	{
//		m_NewParent->UpatateLiveStatus(this, false);
		m_bLiving = false;
		qDebug() << "白板打开直播出错，具体错误信息请看返回值";
	}
	else
	{
//		m_NewParent->UpatateLiveStatus(this, true);
		qDebug() << "白板直播成功";
	}
}

// 结束直播完成
void UIVideoRecord::slot_FinishStopLiveStream(int iRet)
{
	// 结束推流完成
	if (iRet == 0)
	{
//		m_NewParent->setLiveBtnEnable(true);
//		m_NewParent->StopSuccess(this);
	}
}
#endif

void UIVideoRecord::ChangeAppPath(int index)
{
	m_iAppChangeIndex = index;
}

bool UIVideoRecord::IsCurrentPreview()
{
	return m_bPreviewing;
}

bool UIVideoRecord::IsCurrentLiving()
{
	return m_bLiving;
}

void UIVideoRecord::SetPauseVideo()
{
	Nlss_PauseVideoLiveStream(m_hNlssService);
}

void UIVideoRecord::SetResumeVideo()
{
	Nlss_ResumeVideoLiveStream(m_hNlssService);
}

void UIVideoRecord::SetPauseAudio(HWND hwnd)
{
	Nlss_PauseAudioLiveStream(m_hNlssService);
}

void UIVideoRecord::SetResumeAudio(HWND hwnd)
{
	Nlss_ResumeAudioLiveStream(m_hNlssService);
}

void UIVideoRecord::ChangeLiveVideo()
{
	m_bPreviewing = true;
}

void UIVideoRecord::StopCaptureVideo()
{
	m_bPreviewing = false;
	Nlss_ChildVideoStopCapture(hChildRecordVideoService1);
	Nlss_ChildVideoClose(hChildRecordVideoService1);
	hChildRecordVideoService1 = NULL;
	emit sig_StopCapture();
}

void UIVideoRecord::SetMainWnd(UIWindowSet* parent)
{
	m_NewParent = parent;
}

void UIVideoRecord::SetVideoWnd(HWND hWnd)
{
	m_VideoWnd = hWnd;
}

void UIVideoRecord::SetChangeAudio(int index)
{
	m_CurrentMicIndex = index;
}

void UIVideoRecord::slot_livestreamErrorHappened()
{
//	m_NewParent->ErrorStopLive(this);
}