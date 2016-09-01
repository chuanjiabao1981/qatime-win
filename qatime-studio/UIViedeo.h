#ifndef UIVIEDEO_H
#define UIVIEDEO_H

#include <QWidget>
#include "ui_UIViedeo.h"
#include "nlss_type.h"
#include "nlss_api.h"
#include <QTimer>
#include <QMutex>
#include "UIWorkThread.h"

#define STARTLS_ASYNC

class UIViedeo : public QWidget
{
	Q_OBJECT

public:
	UIViedeo(QWidget *parent = 0);
	~UIViedeo();

private:
	Ui::UIViedeo ui;

	_HNLSSERVICE					m_hNlssService;
	static ST_NLSS_VIDEO_SAMPLER	m_SvideoSampler;
	QTimer*							m_refreshTimer;
	static  QMutex					m_mutex;
	EN_NLSS_VIDEOIN_TYPE			m_videoSourceType;  // ��ƵԴ����
	EN_NLSS_AUDIOIN_TYPE			m_audioSourceType;	// ��ƵԴ����
	bool							m_bPreviewing;		// Ԥ��
	QString							m_strUrl;			// ������ַ
	int								m_iVideoDeviceNum;	// ��Ƶ�豸����
	int								m_iAudioDeviceNum;	// ��Ƶ�豸����
	ST_NLSS_INDEVICE_INF*			m_pVideoDevices;	// ��Ƶ�豸
	ST_NLSS_INDEVICE_INF*			m_pAudioDevices;	// ��Ƶ�豸
	bool							m_bInited;			// ��ʼ��
	QString							m_errMsg;			// ������Ϣ

	EN_NLSS_VIDEOQUALITY_LVL		m_videoQ;			// ����������
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
	void LiveVideo();									// ��ʼֱ��
	bool InitMediaCapture();							// ��ʼ��ֱ������
	void SetMediaCapture(_HNLSSERVICE hNlssService);														
	static void SetVideoSampler(ST_NLSS_VIDEO_SAMPLER *pSampler);
	static void OnLiveStreamStatusNty(EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode);

};

#endif // UIVIEDEO_H
