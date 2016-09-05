#ifndef UIVideo_H
#define UIVideo_H

#include <QWidget>
#include "ui_UIVideo.h"
#include "nlss_type.h"
#include "nlss_api.h"
#include <QTimer>
#include <QMutex>
#include "UIWorkThread.h"

#define STARTLS_ASYNC

class UIVideo : public QWidget
{
	Q_OBJECT

public:
	UIVideo(QWidget *parent = 0);
	~UIVideo();

private:
	Ui::UIVideo ui;

	_HNLSSERVICE					m_hNlssService;
	static ST_NLSS_VIDEO_SAMPLER	m_SvideoSampler;
	QTimer*							m_refreshTimer;
	static  QMutex					m_mutex;
	bool							m_bPreviewing;		// Ԥ��
	QString							m_strUrl;			// ������ַ
	bool							m_bInited;			// ��ʼ��
	QString							m_errMsg;			// ������Ϣ
	EN_NLSS_VIDEOQUALITY_LVL		m_videoQ;			// ����������
	int								m_iAppChangeIndex;	// Ӧ��·��

public:
	EN_NLSS_VIDEOIN_TYPE			m_videoSourceType;  // ��ƵԴ����
	EN_NLSS_AUDIOIN_TYPE			m_audioSourceType;	// ��ƵԴ����
	int								m_iVideoDeviceNum;	// ��Ƶ�豸����
	int								m_iAudioDeviceNum;	// ��Ƶ�豸����
	int								m_iAppWindNum;		// ����Ӧ�ø���
	ST_NLSS_INDEVICE_INF*			m_pVideoDevices;	// ��Ƶ�豸
	ST_NLSS_INDEVICE_INF*			m_pAudioDevices;	// ��Ƶ�豸
	ST_NLSS_INDEVICE_INF*			m_pAppWinds;		// ����Ӧ��

#ifdef STARTLS_ASYNC
	Worker* m_pWorker;
#endif

protected:
	virtual void paintEvent(QPaintEvent *);

private:
	void EnumAvailableMediaDevices();					// ö���豸
	void InitPlugFlowUrl();								// ��ʼ������url

Q_SIGNALS:
	void sig_changeLiveStatus(bool bTrue);
#ifdef STARTLS_ASYNC
	void sig_StartLiveStream();
	void sig_StopLiveStream();
#endif

private slots:
	void slot_onRefreshTimeout();						// ˢ�½���
#ifdef STARTLS_ASYNC
	void slot_FinishStartLiveStream(int);
	void slot_FinishStopLiveStream(int);
#endif

public:
	void StartLiveVideo();								// ��ʼֱ��
	void StopLiveVideo();								// ֱֹͣ��
	bool InitMediaCapture();							// ��ʼ��ֱ������
	void SetMediaCapture(_HNLSSERVICE hNlssService);														
	static void SetVideoSampler(ST_NLSS_VIDEO_SAMPLER *pSampler);
	static void OnLiveStreamStatusNty(EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode);
	void ChangeAppPath(int index);						// �ı�Ӧ��
	bool IsCurrentPreview();							// ��ǰ�Ƿ�Ԥ��
	void SetPauseVideo();								// ��ͣ��Ƶ����
	void SetResumeVideo();								// �ָ���Ƶ����
	void SetPauseAudio();								// ��ͣ��Ƶ����
	void SetResumeAudio();								// �ָ���Ƶ����;
	void ChangeLiveVideo();								// �ı���ƵԴ
};

#endif // UIVideo_H
