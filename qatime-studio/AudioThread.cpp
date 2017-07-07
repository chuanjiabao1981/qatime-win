#include "AudioThread.h"

AudioThread::AudioThread() {

	workerThread.start();
	this->moveToThread(&workerThread);
	
}

AudioThread::~AudioThread()
{
	
}