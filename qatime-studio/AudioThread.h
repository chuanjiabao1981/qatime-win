#ifndef _LS_AUDIOTHREAD_H
#define _LS_AUDIOTHREAD_H


#include <Qthread>

class AudioThread :public QObject
{
	Q_OBJECT
public:
	AudioThread();
	~AudioThread();
	


private:
	QThread workerThread;
	

};
#endif//_LS_MEDIACAPTURE_UI_WORKTHREAD_H_

