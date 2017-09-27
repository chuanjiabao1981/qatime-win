#include "UIVideo.h"
#include <QPainter>
#include "windows.h"
#include <iosfwd>
#include <sstream>
#include "define.h"

_HNLSSCHILDSERVICE hChildFullVideoService1;

QMutex UIVideo::m_mutex;
ST_NLSS_VIDEO_SAMPLER UIVideo::m_SvideoSampler;
UIVideo* UIVideo::m_pThis = NULL;
#define  NLSS_720P_BITRATE 800000

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
	, m_bStopLiveFinish(true)
	, m_Parent(NULL)
	, m_NewParent(NULL)
	, m_pBkImage(NULL)
	, m_refreshTimer(NULL)
	, m_bPaint(true)
	, m_EnVideoQuality(EN_NLSS_VIDEOQUALITY_HIGH)
	, m_iBitRate(0)
	, m_iCompareCount(0)
{
	ui.setupUi(this);

	m_pThis = this;
	UIVideo::m_SvideoSampler.iWidth = 0;
	UIVideo::m_SvideoSampler.iHeight = 0;
	UIVideo::m_SvideoSampler.puaData = NULL;//一次性分配一个最大的控件，不断填充不同的数据
	UIVideo::m_SvideoSampler.iDataSize = 0;

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

	//默认采集音视频的麦克风以及摄像头设备，所以，先初始化ui控件时，将 音视频设备通通找出来
	//初始化音视频控件，利用到底层库
	EnumAvailableMediaDevices();

	SetMediaCapture(m_hNlssService);
	m_pWorker->SetMediaCapture(m_hNlssService);

	m_refreshTimer = new QTimer(this);
	m_refreshTimer->start(1000 / 25);
	connect(m_refreshTimer, SIGNAL(timeout()), this, SLOT(slot_onRefreshTimeout()));

	m_GetTimer = new QTimer(this);
	connect(m_GetTimer, SIGNAL(timeout()), this, SLOT(slots_time()));
}

UIVideo::~UIVideo()
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

void UIVideo::setPlugFlowUrl(QString url)
{
	m_strUrl = url;
}

void UIVideo::SetMediaCapture(_HNLSSERVICE hNlssService)
{
	m_hNlssService = hNlssService;
	PFN_NLSS_VIDEOSAMPLER_CB frame_cb = UIVideo::SetVideoSampler;
	Nlss_SetVideoSamplerCB(m_hNlssService, frame_cb);

	PFN_NLSS_STATUS_NTY notify = UIVideo::OnLiveStreamStatusNty;
	Nlss_SetStatusCB(m_hNlssService, notify);
}

void UIVideo::SetVideoSampler(void *pNlssChildID, ST_NLSS_VIDEO_SAMPLER *pSampler)
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

void UIVideo::OnLiveStreamStatusNty(EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode)
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
#define   WIDTHBYTES(bits)         (((bits)   +   31)   /   32   *   4) 
#ifndef HDIB 
#define HDIB HANDLE 
#endif
void SuoFangImage(int mWidth, int mHeight)
{
	int fx = 1;
	int NewWidth = int(mWidth*fx + 0.5);
	int NewHeight = int(mHeight*fx + 0.5);
	int NewLineBytes = WIDTHBYTES(NewWidth * 8);
	HDIB hNewDIB = (HDIB)::GlobalAlloc(GHND, 40 + 4 * 256 + NewLineBytes*NewHeight);
	if (hNewDIB == NULL)
	{
		return;
	}
	LPBYTE lpDIBnew = (LPBYTE)::GlobalLock((HGLOBAL)hNewDIB);
	//memcpy(lpDIBnew, lpDIB, 1064);
}

void UIVideo::paintEvent(QPaintEvent *)
{
	if (this->isVisible() && m_bPaint)
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

bool UIVideo::InitMediaCapture()
{
	char* pAudioPath;
	if (m_pAudioDevices != NULL)
		pAudioPath = (char*)m_pAudioDevices[m_CurrentMicIndex].paPath;
	else
		pAudioPath = "";
	ST_NLSS_PARAM stParam;
	Nlss_GetDefaultParam(m_hNlssService, &stParam);
	
	SetVideoOutParam(&stParam.stVideoParam, m_EnVideoQuality, true);
	SetAudioParam(&stParam.stAudioParam, pAudioPath, EN_NLSS_AUDIOIN_MIC);
	stParam.enOutContent = EN_NLSS_OUTCONTENT_AV;
	initLiveStream(m_hNlssService, &stParam, (char*)m_strUrl.toStdString().c_str());

	ST_NLSS_VIDEOIN_PARAM stChildVInParam;
	SetVideoInParam(&stChildVInParam, EN_NLSS_VIDEOIN_FULLSCREEN, "", EN_NLSS_VIDEOQUALITY_MIDDLE);
	Nlss_Start(m_hNlssService);
	Nlss_StartVideoPreview(m_hNlssService);

	hChildFullVideoService1 = Nlss_ChildVideoOpen(m_hNlssService, &stChildVInParam);
	Nlss_ChildVideoSetBackLayer(hChildFullVideoService1);
	Nlss_ChildVideoStartCapture(hChildFullVideoService1);
	return true;
}

//获取建议的输出目标码率供开发者使用，请关注
int UIVideo::getOutBitrate(int iWidth, int iHeight, int iFps)
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

void UIVideo::EnumAvailableMediaDevices()
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

void UIVideo::InitDeviceParam()
{
	if ((m_pVideoDevices != NULL) && (m_iVideoDeviceNum>0))
	{

		for (int i = 0; i < m_iVideoDeviceNum; i++)
		{
			m_NewParent->addVideoDevices(QString::fromUtf8(m_pVideoDevices[i].paFriendlyName), QString::fromUtf8(m_pVideoDevices[i].paPath));
		}
	}


	if ((m_pAudioDevices != NULL) && (m_iAudioDeviceNum>0))
	{

		for (int i = 0; i < m_iAudioDeviceNum; i++)
		{
			m_NewParent->addAudioDevices(QString::fromLocal8Bit(m_pAudioDevices[i].paFriendlyName),QString::fromLocal8Bit(m_pAudioDevices[i].paPath));

		}
	}
}

void UIVideo::StartLiveVideo()
{
	if (m_hNlssService == NULL)
	{
		MessageBox(NULL, L"直播类为空，打开、关闭 预览 / 直播失败", L"答疑时间", MB_OK);
		return;
	}

	if (m_bPreviewing)
	{
		if (hChildFullVideoService1)
		{
			Nlss_ChildVideoStopCapture(hChildFullVideoService1);
			Nlss_ChildVideoClose(hChildFullVideoService1);
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

	ST_NLSS_VIDEO_WATER_PARAM stFilterParam;
	stFilterParam.pucFilePath = "WaterMark.png";
	stFilterParam.uiStartx = 10;
	stFilterParam.uiStarty = 10;
	Nlss_SetVideoWaterMark(m_hNlssService, &stFilterParam);
	
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
	m_bPaint = false;
 	m_pWorker->SetMediaCapture(m_hNlssService);
 	emit sig_StartLiveStream();
	m_bLiving = true;
}

void UIVideo::StopLiveVideo()
{
	if (m_bLiving)
	{
		m_GetTimer->stop();	
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
}

#ifdef STARTLS_ASYNC

// 开始直播完成
void UIVideo::slot_FinishStartLiveStream(int iRet)
{
	m_bPaint = true;
	if ((NLSS_RET)iRet != NLSS_OK)
	{
		m_NewParent->UpatateLiveStatus(this, false);
		m_bLiving = false;
		qDebug() << __FILE__ << __LINE__ << "白板打开直播出错，具体错误信息请看返回值";
	}
	else
	{
		m_NewParent->UpatateLiveStatus(this, true);
		InitFailParam();
		qDebug() <<__FILE__<<__LINE__ <<"白板直播成功";
	}
}

// 结束直播完成
void UIVideo::slot_FinishStopLiveStream(int iRet)
{
	// 结束推流完成
	if (iRet == 0)
	{
		m_NewParent->setLiveBtnEnable(true);
		m_NewParent->StopSuccess(this);
	}
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

void UIVideo::SetPauseAudio(HWND hwnd)
{
	Nlss_PauseAudioLiveStream(m_hNlssService);
}

void UIVideo::SetResumeAudio(HWND hwnd)
{
	Nlss_ResumeAudioLiveStream(m_hNlssService);
}

void UIVideo::ChangeLiveVideo()
{
	m_bPreviewing = true;
}

void UIVideo::StopCaptureVideo()
{
	m_bPreviewing = false;
	Nlss_ChildVideoStopCapture(hChildFullVideoService1);
	Nlss_ChildVideoClose(hChildFullVideoService1);
	hChildFullVideoService1 = NULL;
	emit sig_StopCapture();
}

void UIVideo::setLessonName(QString strLessonName)
{

}

void UIVideo::SetMainWnd(UIWindowSet* parent)
{
	m_NewParent = parent;
}

void UIVideo::SetVideoWnd(HWND hWnd)
{
	m_VideoWnd = hWnd;
}

void UIVideo::SetChangeAudio(int index)
{
	m_CurrentMicIndex = index;
}

void UIVideo::setBkImage(QString qsImage)
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

void UIVideo::slot_livestreamErrorHappened()
{
	qDebug() << "ErrorStopLive 4";
	m_NewParent->ErrorStopLive(this);
}

void UIVideo::setRatio(int iType)
{
	if (iType==0)
		m_EnVideoQuality = EN_NLSS_VIDEOQUALITY_HIGH;
	else
		m_EnVideoQuality = EN_NLSS_VIDEOQUALITY_SUPER;
}

void UIVideo::mousePressEvent(QMouseEvent *event)
{
	setFocus();
}

void UIVideo::slots_time()
{
	ST_NLSS_STATS stats;
	Nlss_GetStaticInfo(m_hNlssService, &stats);

	int iBitRate = stats.uiVSendBitRate;
	// 如果上次和这次的码率相同，则m_iCompareCount计数加1
	if (m_iBitRate == iBitRate)
	{
		m_iCompareCount++;
		qDebug() << __FILE__ << __LINE__ << "码率：" << iBitRate << "次数" << m_iCompareCount;
	}
	else
		m_iCompareCount = 0;
		
	m_iBitRate = iBitRate;

	// 如果连续12相同，则判断推流失败
	if (m_iCompareCount == 12)
	{
		if (m_NewParent)
			m_NewParent->ErrorStopLive(this);
		qDebug() << __FILE__ << __LINE__ << "自身判断失败！";
	}
}

void UIVideo::InitFailParam()
{
	m_GetTimer->start(1000);
	m_iBitRate = 0;
	m_iCompareCount = 0;
}

void UIVideo::DeleteThread()
{
	delete m_pWorker;
}