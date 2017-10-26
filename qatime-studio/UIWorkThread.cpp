/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#include "UIWorkThread.h"
#include "QDebug.h"

Worker::Worker() {

	workerThread.start();
	this->moveToThread(&workerThread);
    m_hNlssService = NULL;
}

Worker::~Worker()
{
    m_hNlssService = NULL;
	// 请求停止
	workerThread.requestInterruption();
	workerThread.quit();
	workerThread.wait();
}
void Worker::SetMediaCapture(_HNLSSERVICE hNlssService)
{
    m_hNlssService = hNlssService;
}

void Worker::slot_StartLiveStream()
{
	NLSS_RET iRet = 0;
    if (m_hNlssService != NULL)
	{
        iRet = Nlss_StartLiveStream(m_hNlssService);
		/*
		// 获取网易云视频库版本号
		char **pm_Version = new char *[20];
		Nlss_GetSDKVersion(pm_Version);
		delete pm_Version;
		*/
	}
	else
		iRet = NLSS_ERR;
    emit sig_ResultReady((int)(iRet));
}

void Worker::slot_StopLiveStream()
{
    NLSS_RET iRet = NLSS_OK;
    if (m_hNlssService != NULL)
    {
        Nlss_StopLiveStream(m_hNlssService);
    }
	else
	{
		iRet = NLSS_OK;
	}
        
    emit sig_StopResult((int)iRet);
}

void Worker::slot_StopCapture()
{
	if (m_hNlssService != NULL)
	{
		Nlss_StopVideoPreview(m_hNlssService);
		Nlss_Stop(m_hNlssService);
	}
}