#ifndef UIVideoRecord_H
#define UIVideoRecord_H

#include <QWidget>
#include "nlss_type.h"
#include "nlss_api.h"
#include "nlss_define.h"
#include <QTimer>
#include "UIWorkThread.h"
#include "UIMainwindow.h"
#include "UIWindowset.h"

#define STARTLS_ASYNC
#pragma execution_character_set("utf-8")

class UIMainWindow;
class UIWindowSet;
class UIVideoRecord : public QWidget
{
	Q_OBJECT

public:
	UIVideoRecord(QWidget *parent = 0);
	~UIVideoRecord();

private:
	_HNLSSERVICE					m_hNlssService;
	bool							m_bLiving;			// ֱ��
	bool							m_bPreviewing;		// Ԥ��
	QString							m_strUrl;			// ������ַ
	bool							m_bInited;			// ��ʼ��
	QString							m_errMsg;			// ������Ϣ
	int								m_iAppChangeIndex;	// Ӧ��·��
	HWND							m_VideoWnd;			// ��Ƶ���ھ��
	QTimer*							m_StartLiveTimer;	// �ӳ�1������

public:
	EN_NLSS_VIDEOIN_TYPE			m_videoSourceType;  // ��ƵԴ����
	EN_NLSS_AUDIOIN_TYPE			m_audioSourceType;	// ��ƵԴ����
	int								m_iVideoDeviceNum;	// ��Ƶ�豸����
	int								m_iAudioDeviceNum;	// ��Ƶ�豸����
	int								m_iAppWindNum;		// ����Ӧ�ø���
	ST_NLSS_INDEVICE_INF*			m_pVideoDevices;	// ��Ƶ�豸
	ST_NLSS_INDEVICE_INF*			m_pAudioDevices;	// ��Ƶ�豸
	ST_NLSS_INDEVICE_INF*			m_pAppWinds;		// ����Ӧ��
	EN_NLSS_VIDEOQUALITY_LVL		m_videoQ;			// ����������
	int								m_CurrentMicIndex;	// ��ǰ��˷�����
	int								m_CurrentVideoIndex;// ��ǰ����ͷ����
	bool							m_bStopLiveFinish;	// ֱֹͣ����

	UIWindowSet*					m_NewParent;		// ��������
	static UIVideoRecord*					m_pThis;
#ifdef STARTLS_ASYNC
	Worker* m_pWorker;
#endif

Q_SIGNALS:
	void sig_changeLiveStatus(bool bTrue);
	void sig_livestreamErrorHappened();
	void sig_StopCapture();
#ifdef STARTLS_ASYNC
	void sig_StartLiveStream();
	void sig_StopLiveStream();
#endif

private slots:
	void slot_livestreamErrorHappened();				// ���ִ���

#ifdef STARTLS_ASYNC
	void slot_FinishStartLiveStream(int);
	void slot_FinishStopLiveStream(int);
#endif

public:
	void StartLiveVideo();								// ��ʼֱ��
	void StopLiveVideo();								// ֱֹͣ��
	bool InitMediaCapture();							// ��ʼ��ֱ������
	void SetMediaCapture(_HNLSSERVICE hNlssService);														
	static void SetVideoSampler(void *pNlssChildID, ST_NLSS_VIDEO_SAMPLER *pSampler);
	static void OnLiveStreamStatusNty(EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode);
	void ChangeAppPath(int index);						// �ı�Ӧ��
	bool IsCurrentPreview();							// ��ǰ�Ƿ�Ԥ��
	bool IsCurrentLiving();								// ��ǰ�Ƿ�ֱ��
	void SetPauseVideo();								// ��ͣ��Ƶ����
	void SetResumeVideo();								// �ָ���Ƶ����
	void SetPauseAudio(HWND hwnd);						// ��ͣ��Ƶ����
	void SetResumeAudio(HWND hwnd);						// �ָ���Ƶ����;
	void ChangeLiveVideo();								// �ı���ƵԴ
	void StopCaptureVideo();							// ֹͣ�ɼ�
	void setPlugFlowUrl(QString url);					// ��������url
	void EnumAvailableMediaDevices();					// ö���豸
	void SetMainWnd(UIWindowSet* parent);
	void SetVideoWnd(HWND hWnd);
	void SetChangeAudio(int index);
	void InitDeviceParam();								// ��ʼ���豸����
	int  getOutBitrate(int iWidth, int iHeight, int iFps);

};

#endif // UIVideoRecord_H