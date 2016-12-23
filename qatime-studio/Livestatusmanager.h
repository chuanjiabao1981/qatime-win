#ifndef LIVESTATUSMANAGER_H
#define LIVESTATUSMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include "uimainwindow.h"

#pragma execution_character_set("utf-8")

class UIMainWindow;
class LiveStatusManager : public QObject
{
	Q_OBJECT

public:
	LiveStatusManager(QObject *parent);
	~LiveStatusManager();

private:
	QUrl url;
	QNetworkAccessManager	manager;
	QNetworkReply*			reply;
	UIMainWindow*			m_parent;
	QString					m_sToken;				//token
	QString					m_sLiveToken;			//����token
	QString					m_lessonID;				//�γ�ID
	QString					m_Time;					//ֱ������ʧ�ܵ��õ�ʱ���

	int						m_iGetRtmpCount;		//��ȡ������ַʧ�����Դ���
	QTimer*					m_TGetRtmpTimer;		//��ȡ������ַʧ������

	int						m_iBeatStep;			//�������
	int						m_iHeartCount;			//����ʧ�����Դ���
	QTimer*					m_HeartTimer;			//������ʱ��
	QTimer*					m_HeartFailTimer;		//����ʧ�ܶ�ʱ��

	int						m_iStopLiveCount;		//ֹͣ����������Ϣʧ�����Դ���
	QTimer*					m_StopFailTimer;		//ֹͣ����ʧ�ܶ�ʱ��

	QTimer*					m_SwitchFailTimer;		//�л�״̬ʧ�ܶ�ʱ��
	int						m_iBoardStatus;			//�װ�ֱ��״̬
	int						m_iCameraStatus;		//����ͷֱ��״̬
private slots :
	void GetRtmpFailTimer();						//��ȡ������ַʧ�����Զ�ʱ���źŲ�
	void HeartBeatTimer();							//������ʱ���źŲ�
	void HeartBeatFailTimer();						//����ʧ���źŲ�
	void StopLiveFailTimer();						//ֹͣ����ʧ���źŲ�
	void SwitchFailTimer();							//�л�״̬ʧ���źŲ�

private:
	void ReturnRtmpAddressAndHeartBeat();			// ���ذװ塢����ͷ��������ַ �� �������
	void ReturnHeartBeat();							// ��������
	void FinishStartLive();							// ���ؿ�ʼֱ�������token
	void FinishStopLive();							// ���ؽ���ֱ�������״̬
	void FinishCameraSwitch();						// ��������ͷ�л����
public:
	void setMainWindow(UIMainWindow* parent);									// ���ø�����
	void GetRtmpAddressAndHeartBeat(QString lessonID, QString sToken);			// ��ȡ�װ塢����ͷ��������ַ �� �������
	void SendStartLiveHttpMsg(int iBoard, int iCamera,QString sLessonid,
								QString sToken);								// ������������ֱ����ʼ��Ϣ
	void SendStopLiveHttpMsg(bool bConnect = true);								// ������������ֱ��ֹͣ��Ϣ
	void StartHeartBeat();														// ��ʼ����
	void StopTimer();															// ֹͣ���м�ʱ��
	void SendCameraSwitchMsg(int iBoard, int iCamera);							// ��������ͷ�л�״̬��Ϣ
	void RequestError(QJsonObject& error, bool bTrue=true);						// ���ִ���
};

#endif // LIVESTATUSMANAGER_H
