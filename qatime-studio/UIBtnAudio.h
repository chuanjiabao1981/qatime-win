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
//---����
#include "assert.h"
#include <string>
#include "nim_cpp_api.h"
#include "nim_cpp_client.h"
#include "nim_cpp_tool.h"
#include "nim_audio_cpp.h"
#pragma execution_character_set("utf-8")

class CBtnAudio : public QPushButton
{
	Q_OBJECT

public:
	CBtnAudio(std::string path, std::string sid, std::string msgid, QWidget *parent = 0, bool bRead = false); //bRead Ĭ��Ϊδ��
	~CBtnAudio();

Q_SIGNALS:
	void sig_Audioclicked(std::string, std::string, std::string, bool);
	void sig_AudioLoadFail(nim::IMMessage);
	void sig_AudioLoadEnd(QPixmap*, QString, QString, QString, std::string, std::string, std::string, bool, bool, CBtnAudio*);
public Q_SLOTS:
	void onClicked(bool);
	void slot_onCountTimeout();		// ��ʱ������
	void slot_onDownTimeout();		// ������Դ
	void slot_onDownEndTimeout();	// �������

protected:

public:
	bool m_bRead;				// �����Ѷ�δ��
	std::string m_path;			// ����·��
	std::string m_sid;			// �ỰID
	std::string m_msgid;		// ��ϢID
	bool m_bIsPlay;				// �����Ƿ��ڲ���
private:
	
	bool m_bDownEnd;			// �����Ƿ��������
	
	int  m_ImageCount;			// ����ͼƬ��˳��

	QTimer*	m_Timer;			// ������ʱ��
	QTimer* m_TimerDown;		// ����������ʱ��
	//add by zbc 20170705
	int			m_iCount;		// ѭ��60�� Ҳ����3�뻹���ز��꣬����������
	nim::IMMessage m_msg;		// ��Ϣ�ṹ
	bool m_LoadStatus;			// ����״̬
	CBtnAudio*	m_pLastAudio;	// ��һ������

	//��������Ϣ
	QPixmap*	m_pixmap;		// ͷ��
	QString		m_name;			// ����
	QString		m_time;			// ʱ��
	QString		m_text;			// ����
	bool		m_bTeacher;		// �Ƿ�����ʦ
	QTimer* m_TimerDownEnd;		// ������ɶ�ʱ��

public:
	QString GetMsgID();							// ��ȡ��Ϣ
	void stopPlay();							// ֹͣ����
	bool GetPlayStatus();						// ��ȡ����״̬
	void setMsg(nim::IMMessage msg);			// add by zbc 20170705
	void LoadFail();							// ����ʧ��
	void LoadSuc();								// ���سɹ�
	void setLastAudio(CBtnAudio* pLastAudio);	// ������һ������
	void setMsgAll(QPixmap* pixmap, QString name, QString time, QString text, std::string path, std::string sid, std::string msgid, bool bTeacher, bool bRead);
};

#endif // _LS_MEDIACAPTURE_UI_BUTTON_H_