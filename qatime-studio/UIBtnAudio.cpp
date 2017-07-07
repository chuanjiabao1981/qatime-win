/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#include "UIBtnAudio.h"
#include "windows.h"
#include <QFile>

#define LOOP_COUNT 100
#define LOOP_FAIL_COUNT 160
CBtnAudio::CBtnAudio(std::string path, std::string sid, std::string msgid, QWidget *parent /*= 0*/, bool bRead)
	: QPushButton(parent)
	, m_path(path)
	, m_sid(sid)
	, m_msgid(msgid)
	, m_Timer(NULL)
	, m_TimerDown(NULL)
	, m_bIsPlay(false)
	, m_ImageCount(0)
	, m_bDownEnd(false)
	, m_bRead(bRead)
, m_iCount(0)
, m_LoadStatus(true)
{
	setCursor(Qt::PointingHandCursor);

	m_Timer = new QTimer(this);
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(slot_onCountTimeout()));

	m_TimerDown = new QTimer(this);
	connect(m_TimerDown, SIGNAL(timeout()), this, SLOT(slot_onDownTimeout()));
	m_TimerDown->start(50);

	connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked(bool)));
}

CBtnAudio::~CBtnAudio()
{
}

void CBtnAudio::onClicked(bool bChecked)
{
	// 加载中状态不可点击
	QString strPicName = styleSheet();
	if (strPicName.indexOf("audio_loading") > 0)
		return;
	
	if (!m_LoadStatus)
	{
		emit sig_AudioLoadFail(m_msg);
		setStyleSheet("QPushButton{border-image:url(./images/audio_loading.png);}");
		m_TimerDown->start(50);
		return;
	}
	
	// 如果没下载完，直接返回
	if (!m_bDownEnd)
		return;
	
	if (m_bIsPlay)
	{
		m_bIsPlay = false;
		m_Timer->stop();
		setStyleSheet("QPushButton{border-image:url(./images/audio_2.png);}");
	}
	else
	{
		m_bIsPlay = true;
		m_Timer->start(300);
	}

	emit sig_Audioclicked(m_path, m_sid, m_msgid, m_bIsPlay);
}

QString CBtnAudio::GetMsgID()
{
	return QString::fromStdString(m_msgid);
}


void CBtnAudio::slot_onCountTimeout()
{
	if (m_ImageCount == 3)
	{
		m_ImageCount = 0;
	}

	QString strImage = QString::number(m_ImageCount);
	QString strSTyle = "QPushButton{border-image:url(./images/audio_{number}.png);}";
	strSTyle.replace("{number}", strImage);
	setStyleSheet(strSTyle);

	m_ImageCount++;
}

void CBtnAudio::slot_onDownTimeout()
{
	m_iCount++;
	QString path = QString::fromStdString(m_path);
	QFile file(path);
	if (file.exists())
	{
		m_TimerDown->stop();
		m_bDownEnd = true;
		m_LoadStatus = true;
		if (m_bRead)
			setStyleSheet("QPushButton{border-image:url(./images/audio_2.png);}");
		else
			setStyleSheet("QPushButton{border-image:url(./images/audio_noread.png);}");
	}
}

void CBtnAudio::stopPlay()
{
	if (m_bIsPlay)
	{
		m_bIsPlay = false;
		m_Timer->stop();
		setStyleSheet("QPushButton{border-image:url(./images/audio_2.png);}");
	}
}

bool CBtnAudio::GetPlayStatus()
{
	return m_bIsPlay;
}

void CBtnAudio::setMsg(nim::IMMessage msg)
{
	m_msg = msg;
}

void CBtnAudio::LoadFail()
{
	m_iCount++;
	emit sig_AudioLoadFail(m_msg);

	if (m_iCount == 20)
	{
		m_TimerDown->stop();
		m_LoadStatus = false;
		setStyleSheet("QPushButton{border-image:url(./images/audio_fail.png);}");
	}
}

void CBtnAudio::LoadSuc()
{

}