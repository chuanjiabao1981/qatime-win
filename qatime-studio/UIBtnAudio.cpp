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

extern int		m_AutoAudioState;
extern bool		m_IsAudioPlaying;

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
	, m_pLastAudio(NULL)
{
	setCursor(Qt::PointingHandCursor);

	m_Timer = new QTimer(this);
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(slot_onCountTimeout()));

	m_TimerDown = new QTimer(this);
	connect(m_TimerDown, SIGNAL(timeout()), this, SLOT(slot_onDownTimeout()));
	m_TimerDown->start(50);

	connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked(bool)));

	//���ڵ�������ʾ�����ǣ������غ���ʾ�����Կ���ֱ��������������״̬ΪTrue
	m_bDownEnd = true;
}

CBtnAudio::~CBtnAudio()
{
	if (m_Timer)
	{
		m_Timer->stop();
		delete m_Timer;
		m_Timer = NULL;
	}

	if (m_TimerDown)
	{
		m_TimerDown->stop();
		delete m_TimerDown;
		m_TimerDown = NULL;
	}
}

void CBtnAudio::onClicked(bool bChecked)
{
	// ������״̬���ɵ��
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
	
	// ���û�����ֱ꣬�ӷ���
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
	m_IsAudioPlaying = true;
	//�����Ѷ��߼���ֻҪ�����Ϊ�Ѷ�
	m_bRead = true;
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

//�߼����˴�������Զ��������Ź��ܣ�ʹ�õķ�����ͨ������ʽ�ķ�ʽ������������������
//�������ų�ͻ��ͨ��m_IsAudioPlaying���ȫ�ֱ������������Ϊtrueʱ�������������ڲ����У�����Ϊfalseʱ������ǰû��������������
void CBtnAudio::stopPlay()
{
	if (m_bIsPlay)
	{
		m_bIsPlay = false;
		m_Timer->stop();
		setStyleSheet("QPushButton{border-image:url(./images/audio_2.png);}");
		
		if ((m_pLastAudio && !m_pLastAudio->m_bRead) && (m_AutoAudioState ==1))
		{
			emit m_pLastAudio->clicked();
		}
		if (m_pLastAudio == NULL)
		{
			m_IsAudioPlaying = false;
		}
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

void CBtnAudio::setLastAudio(CBtnAudio* pLastAudio)
{
	m_pLastAudio = pLastAudio;
}

void CBtnAudio::setMsgAll(QPixmap* pixmap, QString name, QString time, QString text, std::string path, std::string sid, std::string msgid, bool bTeacher, bool bRead)
{
	if (!pixmap)
		pixmap = &QPixmap("./images/teacherPhoto.png");

	m_pixmap = pixmap;
	m_name = name;
	m_time = time;
	m_text = text;
	m_path = path;
	m_sid = sid;
	m_msgid = msgid;
	m_bTeacher = bTeacher;
	m_bRead = bRead;

	m_TimerDownEnd = new QTimer(this);
	connect(m_TimerDownEnd, SIGNAL(timeout()), this, SLOT(slot_onDownEndTimeout()));
	m_TimerDownEnd->start(50);
}

void CBtnAudio::slot_onDownEndTimeout()
{
	// �������֮���ٲ�����Ϣ
	QString path = QString::fromStdString(m_path);
	QFile file(path);
	if (file.exists())
	{
		m_TimerDownEnd->stop();
		emit sig_AudioLoadEnd(m_pixmap,m_name,m_time,m_text,m_path,m_sid,m_msgid,m_bTeacher,m_bRead,this);
	}
}