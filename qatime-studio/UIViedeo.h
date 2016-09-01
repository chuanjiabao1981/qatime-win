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
	EN_NLSS_VIDEOIN_TYPE			m_videoSourceType;  // 视频源类型
	EN_NLSS_AUDIOIN_TYPE			m_audioSourceType;	// 音频源类型
	bool							m_bPreviewing;		// 预览
	QString							m_strUrl;			// 推流地址
	int								m_iVideoDeviceNum;	// 视频设备个数
	int								m_iAudioDeviceNum;	// 音频设备个数
	ST_NLSS_INDEVICE_INF*			m_pVideoDevices;	// 视频设备
	ST_NLSS_INDEVICE_INF*			m_pAudioDevices;	// 音频设备
	bool							m_bInited;			// 初始化
	QString							m_errMsg;			// 错误信息

	EN_NLSS_VIDEOQUALITY_LVL		m_videoQ;			// 画面清晰度
#ifdef STARTLS_ASYNC
	Worker* m_pWorker;
#endif

protected:
	virtual void paintEvent(QPaintEvent *);

private:
	void EnumAvailableMediaDevices();					// 枚举设备
	void InitPlugFlowUrl();								// 初始化推流url

Q_SIGNALS:
	void sig_changeLiveStatus(bool bTrue);
#ifdef STARTLS_ASYNC
	void sig_StartLiveStream();
	void sig_StopLiveStream();
#endif

private slots:
	void slot_onRefreshTimeout();						// 刷新界面
#ifdef STARTLS_ASYNC
	void slot_FinishStartLiveStream(int);
	void slot_FinishStopLiveStream(int);
#endif

public:
	void LiveVideo();									// 开始直播
	bool InitMediaCapture();							// 初始化直播参数
	void SetMediaCapture(_HNLSSERVICE hNlssService);														
	static void SetVideoSampler(ST_NLSS_VIDEO_SAMPLER *pSampler);
	static void OnLiveStreamStatusNty(EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode);

};

#endif // UIVIEDEO_H
