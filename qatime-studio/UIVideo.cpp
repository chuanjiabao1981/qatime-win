#include "UIVideo.h"
#include <QPainter>
#include "windows.h"
#include <iosfwd>
#include <sstream>
#include "define.h"

QMutex UIVideo::m_mutex;
ST_NLSS_VIDEO_SAMPLER UIVideo::m_SvideoSampler;
PicRegion UIVideo::capture_video_pic_;
#ifdef TEST
	#define _DEBUG
#else
#endif
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
{
	ui.setupUi(this);

	UIVideo::m_SvideoSampler.iWidth = 0;
	UIVideo::m_SvideoSampler.iHeight = 0;
	UIVideo::m_SvideoSampler.puaData = NULL;//一次性分配一个最大的控件，不断填充不同的数据
	UIVideo::m_SvideoSampler.iDataSize = 0;

	m_StartLiveTimer = new QTimer(this);

#ifdef STARTLS_ASYNC
	m_pWorker = new Worker();
	connect(this, SIGNAL(sig_StartLiveStream()), m_pWorker, SLOT(slot_StartLiveStream()));
	connect(m_pWorker, SIGNAL(sig_ResultReady(int)), this, SLOT(slot_FinishStartLiveStream(int)));
	connect(this, SIGNAL(sig_StopLiveStream()), m_pWorker, SLOT(slot_StopLiveStream()));
	connect(m_pWorker, SIGNAL(sig_StopResult(int)), this, SLOT(slot_FinishStopLiveStream(int)));
#endif

	//创建mediacapture类，失败抛出错	
// 	if (NLSS_OK != Nlss_Create(NULL, &m_hNlssService))
// 	{
// 		MessageBox(NULL, L"创建直播失败，请关闭程序重新启动", L"", MB_OK);
// 	}

	//默认采集音视频的麦克风以及摄像头设备，所以，先初始化ui控件时，将 音视频设备通通找出来
	//初始化音视频控件，利用到底层库
	EnumAvailableMediaDevices();

	SetMediaCapture(m_hNlssService);

	// 启动直播视频
	TCHAR szTempPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, szTempPath);
	lstrcat(szTempPath, L"\\win_video.exe");
	std::wstring wszCmdLine;
	std::wstringstream wstream;
 	HWND hwnd = (HWND)this->winId();
	wstream << L" -parenthwnd " << hwnd
		<< std::endl;
	wszCmdLine = wstream.str();

	STARTUPINFOW	si = { 0 };
	si.cb = sizeof(si);

	PROCESS_INFORMATION	pi;
	//查找关闭进程
	//	FindAndKillProcessByName(szExecFileName);

#ifdef TEST
	CreateProcess(szTempPath, (LPWSTR)wszCmdLine.c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	return;
#else
#endif

#ifdef _DEBUG
	CreateProcess(L"C:\\Users\\lenovo\\Downloads\\LiveVideo_Windows_Src_V1.0.0 (1)\\LiveVideo_Windows_Src_V1.0.0\\Demo_src\\nls_demo\\bin\\win_video.exe", (LPWSTR)wszCmdLine.c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
#else
	CreateProcess(szTempPath, (LPWSTR)wszCmdLine.c_str(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
#endif
}

UIVideo::~UIVideo()
{
// 	//销毁mediacapture类
// 	if (m_hNlssService != NULL)
// 	{
// 		if (m_bPreviewing)
// 		{
// 			Nlss_StopLiveStream(m_hNlssService);
// 		}
// 
// 		Nlss_Destroy(m_hNlssService);
// 		m_hNlssService = NULL;
// 	}
// 
// 	if (m_SvideoSampler.puaData != NULL)
// 	{
// 		delete[] m_SvideoSampler.puaData;
// 		m_SvideoSampler.puaData = NULL;
// 	}

	if (m_Parent)
		m_Parent = NULL;
}

void UIVideo::setPlugFlowUrl(QString url)
{
	m_strUrl = url;
}

void UIVideo::SetMediaCapture(_HNLSSERVICE hNlssService)
{
// 	PFN_NLSS_STATUS_NTY notify = UIVideo::OnLiveStreamStatusNty;
// 	Nlss_SetStatusCB(hNlssService, notify);
// 
// 	PFN_NLSS_VIDEOSAMPLER_CB frame_cb = UIVideo::SetVideoSampler;
// 	Nlss_SetVideoSamplerCB(m_hNlssService, frame_cb);
}

void UIVideo::SetVideoSampler(ST_NLSS_VIDEO_SAMPLER *pSampler)
{
	if (pSampler != NULL)
	{
		m_mutex.lock();

		if (m_SvideoSampler.puaData)
		{
			delete[]m_SvideoSampler.puaData;
			m_SvideoSampler.puaData = NULL;
		}

		m_SvideoSampler.puaData = new unsigned char[pSampler->iWidth * pSampler->iHeight * 4];

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
		qimage = QImage((uchar*)m_SvideoSampler.puaData, m_SvideoSampler.iWidth, m_SvideoSampler.iHeight, QImage::Format_RGB32);
		p.drawImage(rect(), qimage);
		m_mutex.unlock();
	}
}

bool UIVideo::InitMediaCapture()
{
// 	bool have_video_source = true;
// 	bool have_audio_source = true;
// 	ST_NLSS_PARAM stParam;
// 	Nlss_GetDefaultParam(m_hNlssService, &stParam);
// 
// 	switch (m_videoSourceType)
// 	{
// 	case EN_NLSS_VIDEOIN_FULLSCREEN:
// 		stParam.stVideoParam.iOutBitrate = 700000;
// 		stParam.stVideoParam.iOutFps = 10;
// 		break;
// 	case EN_NLSS_VIDEOIN_CAMERA:
// 		//获取视频参数
// 		if (m_pVideoDevices != NULL)
// 		{
// 			stParam.stVideoParam.u.stInCamera.paDevicePath = (char *)m_pVideoDevices[m_CurrentVideoIndex].paPath;
// 		}
// 		else
// 			have_video_source = false;
// 		stParam.stVideoParam.u.stInCamera.enLvl = m_videoQ;
// 		break;
// 	case EN_NLSS_VIDEOIN_RECTSCREEN:
// 		stParam.stVideoParam.u.stInRectScreen.iRectLeft = 100;
// 		stParam.stVideoParam.u.stInRectScreen.iRectRight = 500;
// 		stParam.stVideoParam.u.stInRectScreen.iRectTop = 100;
// 		stParam.stVideoParam.u.stInRectScreen.iRectBottom = 500;
// 		stParam.stVideoParam.iOutBitrate = 700000;
// 
// 		break;
// 	case EN_NLSS_VIDEOIN_APP:
// 		stParam.stVideoParam.iOutBitrate = 700000;
// 		stParam.stVideoParam.iOutFps = 10;
// 
// 		if (m_pAppWinds != NULL)
// 		{
// 			stParam.stVideoParam.u.stInApp.paAppPath = (char *)m_pAppWinds[m_iAppChangeIndex].paPath;
// 		}
// 		else
// 			have_video_source = false;
// 
// 		break;
// 	case EN_NLSS_VIDEOIN_NONE:
// 		have_video_source = false;
// 		break;
// 	default:
// 		break;
// 	}
// 	stParam.stVideoParam.enInType = m_videoSourceType;
// 	stParam.stAudioParam.iInSamplerate = 44100;// m_iAudioSample;
// 
// 	stParam.stAudioParam.paaudioDeviceName = "";
// 	switch (m_audioSourceType)
// 	{
// 	case EN_NLSS_AUDIOIN_MIC:
// 		if (m_pAudioDevices != NULL)
// 		{
// 			stParam.stAudioParam.paaudioDeviceName = m_pAudioDevices[m_CurrentMicIndex].paPath;
// 		}
// 		else
// 			have_audio_source = false;
// 		break;
// 
// 	case EN_NLSS_AUDIOIN_NONE:
// 		have_audio_source = false;
// 		break;
// 	default:
// 		break;
// 	}
// 
// 	stParam.stAudioParam.enInType = m_audioSourceType;
// 
// 	if (have_audio_source && have_video_source)
// 		stParam.enOutContent = EN_NLSS_OUTCONTENT_AV;//默认音视频设备都存在则推流音视频，当然，也可以设置成音频/视频，
// 	else if (have_audio_source && !have_video_source)
// 		stParam.enOutContent = EN_NLSS_OUTCONTENT_AUDIO;
// 	else if (have_video_source && !have_audio_source)
// 		stParam.enOutContent = EN_NLSS_OUTCONTENT_VIDEO;
// 	else if (!have_audio_source && !have_video_source)
// 	{
// 		MessageBox(NULL, L"没有视频源也没有音频源输入，推流参数初始化失败", L"答疑时间", MB_OK);
// 		return false;
// 	}
// 
// 	std::string strUrl = m_strUrl.toStdString();
// //	std::string strUrl = "rtmp://pa0a19f55.live.126.net/live/2794c854398f4d05934157e05e2fe419?wsSecret=cc547b3b2fe49e6b5da6e2aed293c7da&wsTime=1475212427";
// 	stParam.paOutUrl = new char[1024];
// 	memset(stParam.paOutUrl, 0, 1024);
// 	strcpy(stParam.paOutUrl, strUrl.c_str());
// 
// 	if (NLSS_OK != Nlss_InitParam(m_hNlssService, &stParam))
// 	{
// 		MessageBox(NULL, L"推流参数初始化失败", L"答疑时间", MB_OK);
// 		delete[]stParam.paOutUrl;
// 		return false;
// 	}
// 	m_bInited = true;
// 
// 	delete[]stParam.paOutUrl;
	return true;
}

void UIVideo::EnumAvailableMediaDevices()
{
	//默认设置为有桌面，以及部分桌面共享，以及不打开视频
// 	Nlss_GetFreeDevicesNum(&m_iVideoDeviceNum, &m_iAudioDeviceNum);
// 	if (m_iVideoDeviceNum <= 0)
// 	{
// 		MessageBox(NULL, L"请连接摄像头设备", L"答疑时间", MB_OK);
// 	}
// 	else
// 	{
// 		if (m_pVideoDevices)
// 		{
// 			delete[] m_pVideoDevices;
// 			m_pVideoDevices = NULL;
// 		}
// 
// 		m_pVideoDevices = new ST_NLSS_INDEVICE_INF[m_iVideoDeviceNum];
// 		for (int i = 0; i < m_iVideoDeviceNum; i++)
// 		{
// 			m_pVideoDevices[i].paPath = new char[1024];
// 			m_pVideoDevices[i].paFriendlyName = new char[1024];
// 		}
// 	}
// 
// 	if (m_iAudioDeviceNum <= 0)
// 	{
// 		MessageBox(NULL, L"请连接麦克风设备", L"答疑时间", MB_OK);
// 	}
// 	else
// 	{
// 		if (m_pAudioDevices)
// 		{
// 			delete[] m_pAudioDevices;
// 			m_pAudioDevices = NULL;
// 		}
// 
// 		m_pAudioDevices = new ST_NLSS_INDEVICE_INF[m_iAudioDeviceNum];
// 
// 		for (int i = 0; i < m_iAudioDeviceNum; i++)
// 		{
// 			m_pAudioDevices[i].paPath = new char[1024];
// 			m_pAudioDevices[i].paFriendlyName = new char[1024];
// 		}
// 	}
// 	// 获取视频和音频设备
// 	Nlss_GetFreeDeviceInf(m_pVideoDevices, m_pAudioDevices);
// 
// 	GetAvailableAppWindNum(&m_iAppWindNum);
// 	if (m_iAppWindNum <= 0)
// 	{
// //		ui.app_comboBox->addItem(QString("请选择其他视频源"));
// 	}
// 	else
// 	{
// 		if (m_pAppWinds)
// 		{
// 			delete[] m_pAppWinds;
// 			m_pAppWinds = NULL;
// 		}
// 
// 		m_pAppWinds = new ST_NLSS_INDEVICE_INF[m_iAppWindNum];
// 		for (int i = 0; i < m_iAppWindNum; i++)
// 		{
// 			m_pAppWinds[i].paPath = new char[1024];
// 			m_pAppWinds[i].paFriendlyName = new char[1024];
// 		}
// 	}
// 
// 	GetAvailableAppWind(m_pAppWinds);
}

void UIVideo::StartLiveVideo()
{
// 	if (m_hNlssService == NULL)
// 	{
// 		MessageBox(NULL, L"直播类为空，打开、关闭 预览 / 直播失败", L"答疑时间", MB_OK);
// 		return;
// 	}
// 
// 	if (m_bPreviewing)
// 	{
// 		Nlss_StopVideoPreview(m_hNlssService);
// 		Nlss_StopVideoCapture(m_hNlssService);
// 	}
// 
// 	Nlss_UninitParam(m_hNlssService);
// 
// 	if (!InitMediaCapture())
// 	{
// 		MessageBox(NULL, L"初始化参数失败", L"答疑时间", MB_OK);
// 		return;
// 	}
// 
// 	Nlss_SetVideoWaterMark(m_hNlssService, NULL);
// 	Nlss_SetVideoDisplayRatio(m_hNlssService, 0, 0);
// 
// 	if (NLSS_OK != Nlss_StartVideoCapture(m_hNlssService))
// 	{
// 		MessageBox(NULL, L"打开视频采集出错", L"答疑时间", MB_OK);
// 		return;
// 	}
// 
// 	if (NLSS_OK != Nlss_StartVideoPreview(m_hNlssService))
// 	{
// 		MessageBox(NULL, L"打开视频预览出错，具体错误信息请看返回值", L"答疑时间", MB_OK);
// 		return;
// 	}
// 
// 	if (m_strUrl.isEmpty())
// 	{
// 		MessageBox(NULL, L"推流地址为空，推流参数初始化失败", L"答疑时间", MB_OK);
// 		return;
// 	}
// 
//  	m_pWorker->SetMediaCapture(m_hNlssService);
//  	emit sig_StartLiveStream();
// 	m_bPreviewing = true;
 	m_bLiving = true;
}

void UIVideo::StopLiveVideo()
{
	m_bLiving = false;
// 	if (m_bLiving)
// 	{
// 		emit sig_StopLiveStream();
// 		m_bLiving = false;
// 	}
}

void UIVideo::slot_onRefreshTimeout()
{
	update();
}

void UIVideo::slot_onStartLiveTimeout()
{
	if (m_bStopLiveFinish)
	{
		m_StartLiveTimer->stop();
		m_pWorker->SetMediaCapture(m_hNlssService);
		emit sig_StartLiveStream();
	}
}

#ifdef STARTLS_ASYNC

void UIVideo::slot_FinishStartLiveStream(int iRet)
{
// 	if ((NLSS_RET)iRet != NLSS_OK)
// 	{
// 		MessageBox(NULL, L"打开直播出错，具体错误信息请看返回值", L"答疑时间", MB_OK);
// 	}
}

void UIVideo::slot_FinishStopLiveStream(int iRet)
{
//	m_Parent->setLiveBtnEnable(true);
	m_bStopLiveFinish = true;
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
//	Nlss_PauseVideoLiveStream(m_hNlssService);
}

void UIVideo::SetResumeVideo()
{
//	Nlss_ResumeVideoLiveStream(m_hNlssService);
}

void UIVideo::SetPauseAudio(HWND hwnd)
{
//	Nlss_PauseAudioLiveStream(m_hNlssService);
	if (IsWindow(hwnd))
	{
		::SendMessage(hwnd, MSG_DEVICE_AUDIO_CLOSE, 0, 0);
	}
}

void UIVideo::SetResumeAudio(HWND hwnd)
{
//	Nlss_ResumeAudioLiveStream(m_hNlssService);
	if (IsWindow(hwnd))
	{
		::SendMessage(hwnd, MSG_DEVICE_AUDIO_CLOSE, 1, 0);
	}
}

void UIVideo::ChangeLiveVideo()
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

void UIVideo::StopCaptureVideo()
{
//	Nlss_StopVideoCapture(m_hNlssService);
//	Nlss_StopVideoPreview(m_hNlssService);
}

void UIVideo::setLessonName(QString strLessonName)
{

}

void UIVideo::setPersonNum(int num)
{

}

void UIVideo::SetMainWnd(UIMainWindow* wnd)
{
	m_Parent = wnd;
}

void UIVideo::SetVideoWnd(HWND hWnd)
{
	m_VideoWnd = hWnd;
}

void UIVideo::SetChangeVideoAudio(HWND hwnd, QString path, bool bVideo)
{
	COPYDATASTRUCT sendData;
	char result[MAX_PATH];
	QByteArray chPath = path.toLatin1();

	if (bVideo)
		strcpy(result, chPath.data());
	else
		strcpy(result, path.toLocal8Bit().data());

	ZeroMemory(&sendData, sizeof(sendData));
	sendData.lpData = result;
	sendData.cbData = MAX_PATH;

	if (IsWindow(hwnd))
	{
		int iType = bVideo ? 0:1;
		::SendMessage(hwnd, WM_COPYDATA, iType, (LPARAM)&sendData);
	}
}