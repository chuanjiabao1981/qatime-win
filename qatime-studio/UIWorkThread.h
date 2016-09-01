/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#ifndef _LS_MEDIACAPTURE_UI_WORKTHREAD_H_
#define _LS_MEDIACAPTURE_UI_WORKTHREAD_H_


#include "nlss_api.h"
#include <Qthread>

class Worker :public QObject
{
	Q_OBJECT
public:
	Worker();
	~Worker();
	void SetMediaCapture(_HNLSSERVICE hNlssService);


Q_SIGNALS:
	void sig_ResultReady(int);
    void sig_StopResult(int);
	protected Q_SLOTS :
	void slot_StartLiveStream();
    void slot_StopLiveStream();

private:
	QThread workerThread;
    _HNLSSERVICE m_hNlssService;

};
#endif//_LS_MEDIACAPTURE_UI_WORKTHREAD_H_

