/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#include "UIWorkThread.h"

Worker::Worker() {

	workerThread.start();
	this->moveToThread(&workerThread);
    m_hNlssService = NULL;
}

Worker::~Worker()
{
    m_hNlssService = NULL;
	
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
        iRet = NLSS_OK;
    emit sig_StopResult((int)iRet);
}