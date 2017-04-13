/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#ifndef _UI_WORKTHREAD_H_
#define _UI_WORKTHREAD_H_

#include <QLabel>
#include <Qthread>
#include <QMap>

// º”‘ÿÕº∆¨◊®”√
class WorkerPic :public QObject
{
	Q_OBJECT
public:
	WorkerPic();
	~WorkerPic();
	void SetUrl(QLabel* pic, QString url);
	
	QMap<QLabel*, QString>	m_mapUrl;

	bool					m_bClose;

Q_SIGNALS:
	void sig_ResultReady(QLabel*,QPixmap*);
protected Q_SLOTS :
	void slot_StartLoading();
	void slot_Close();

private:
	QThread		workerThread;
	QString		m_url;
	QLabel*		m_pic;

};
#endif//_LS_MEDIACAPTURE_UI_WORKTHREAD_H_

