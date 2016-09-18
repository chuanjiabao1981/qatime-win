#ifndef UIMAINWINDOW_H
#define UIMAINWINDOW_H

#include <QWidget>
#include "ui_uimainwindow.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "user.h"
#include "UIOtherApp.h"
#include "UIVideo.h"
#include <windows.h>
#include "UIAuxiliaryPanel.h"
#include "UIAudiochange.h"
#include "UIVideochange.h"
#include "UIRatio.h"

#include "UIChatRoom.h"

#include "loginwindow.h"


#pragma execution_character_set("utf-8")
#define STARTLS_ASYNC

class LoginWindow;
class UIAuxiliaryPanel;
class UIAudioChange;
class UIVideoChange;
class UIRatio;

class UIMainWindow : public QWidget
{
	Q_OBJECT

public:
	UIMainWindow(QWidget *parent = 0);
	~UIMainWindow();

private:
	Ui::UIMainWindow				ui;
	QString							mRemeberToken;
	QNetworkAccessManager			manager;
	QNetworkReply*					reply;


	UIChatRoom*						m_charRoom;			//������


	LoginWindow*					m_LoginWindow;

	UIAuxiliaryPanel*				m_AuxiliaryPanel;	// ��ʦ�γ����
	UIOtherApp*					    m_OtherAppInfo;		// ����Ӧ����Ϣ����
	UIVideo*						m_VideoInfo;		// ֱ������
	QString							m_teacherID;		// ��ʦID
	QTimer*							m_CountTimer;		// ��ʱ��
	QTimer*							m_HeartTimer;		// ����
	INT64							m_iTimerCount;		// ��ʱ������
	bool							m_bOtherApp;		// ��������Ӧ��

	UIAudioChange*					m_AudioChangeInfo;	// ��˷�ѡ�񴰿�
	UIVideoChange*					m_VideoChangeInfo;	// ����ͷѡ�񴰿�
	UIRatio*						m_RatioChangeInfo;	// �ֱ��ʴ���


private slots :
	void MinDialog();									// ��С���Ի���
	void MaxDialog();									// ��󻯶Ի���
	void CloseDialog();									// �رնԻ���
	void Expansion();									// �������
	void slot_startOrStopLiveStream();					// ��ʼֱ��
	void VideoStatus(int iStatus);						// ��Ƶ״̬��ֱ������ͣ�������Ŀ��ƣ�
	void AudioStatus(int iStatus);						// ����״̬��ֱ������ͣ�������Ŀ��ƣ�
	void FullScreenStatus(int iStatus);					// �л�ȫ����ƵԴ
	void slot_onCountTimeout();							// ��ʱ�� �ı�ֱ��ʱ��
	void slot_onHeartTimeout();							// 5��һ�Σ���������
	void OtherApp(int i);								// ����APPӦ��
	void clickChangeAudio(int);
	void clickChangeVideo(int);							// ��������ͷѡ���
	void clickChangeRatio();							// �����ֱ��ʿ�

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // ���caption
	virtual void resizeEvent(QResizeEvent *);			// ���ô���Բ��
	virtual void paintEvent(QPaintEvent *event);
	virtual void closeEvent(QCloseEvent *e);
	virtual void focusInEvent(QFocusEvent *e);

public:
	void setTeacherInfo(QJsonObject& data);					// ������ʦ��Ϣ
	void setRemeberToken(const QString &name);				// ����Token
	void AuxiliaryRequestFinished();						// ������http����
	void LessonRequestFinished();							// �γ�http����
	void EnumAvailableMediaDevices();						// ö���豸
	void SendStartLiveHttpMsg();							// ������������ֱ����ʼ��Ϣ
	void SendStopLiveHttpMsg();								// ������������ֱ��ֹͣ��Ϣ
	void SendHeartBeatHttpMsg();							// ������������ֱ��������Ϣ��5����һ�Σ�
	void ShowAuxiliary();									// ��ʾ������
	void InitAudioList();
	void InitVideoList();
	void setAudioChangeIndex(int index);					//  �ı���˷�
	void setVideoChangeIndex(int index);					//  �ı���Ƶͷ
	void setRatioChangeIndex(int index);					//  �ı�ֱ���


	void HideOtherUI( QWidget* self=NULL);						//  ������������
	void returnClick();
	void setLoginWindow(LoginWindow* parent);

};

#endif // UIMAINWINDOW_H
