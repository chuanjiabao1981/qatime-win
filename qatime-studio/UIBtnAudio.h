/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#ifndef _LS_UI_BUTTON_AUDIO_H_
#define _LS_UI_BUTTON_AUDIO_H_
#include <QtWidgets/QPushButton>
#include <QTimer>
#pragma execution_character_set("utf-8")

class CBtnAudio : public QPushButton
{
	Q_OBJECT

public:
	CBtnAudio(std::string path, std::string sid, std::string msgid, QWidget *parent = 0, bool bRead = false); //bRead Ĭ��Ϊδ��
	~CBtnAudio();

Q_SIGNALS:
	void sig_Audioclicked(std::string, std::string, std::string, bool);

public Q_SLOTS:
	void onClicked(bool);
	void slot_onCountTimeout();		// ��ʱ������
	void slot_onDownTimeout();		// ������Դ

protected:

private:
	std::string m_path;			// ����·��
	std::string m_sid;			// �ỰID
	std::string m_msgid;		// ��ϢID
	bool m_bIsPlay;				// �����Ƿ��ڲ���
	bool m_bDownEnd;			// �����Ƿ��������
	bool m_bRead;				// �����Ѷ�δ��
	int  m_ImageCount;			// ����ͼƬ��˳��

	QTimer*	m_Timer;			// ������ʱ��
	QTimer* m_TimerDown;		// ����������ʱ��

public:
	QString GetMsgID();							// ��ȡ��Ϣ
	void stopPlay();							// ֹͣ����
	bool GetPlayStatus();						// ��ȡ����״̬

};

#endif // _LS_MEDIACAPTURE_UI_BUTTON_H_