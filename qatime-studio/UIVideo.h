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
	bool							m_bPreviewing;		// 预览
	QString							m_strUrl;			// 推流地址
	bool							m_bInited;			// 初始化
	QString							m_errMsg;			// 错误信息
	EN_NLSS_VIDEOQUALITY_LVL		m_videoQ;			// 画面清晰度
	int								m_iAppChangeIndex;	// 应用路径

public:
	EN_NLSS_VIDEOIN_TYPE			m_videoSourceType;  // 视频源类型
	EN_NLSS_AUDIOIN_TYPE			m_audioSourceType;	// 音频源类型
	int								m_iVideoDeviceNum;	// 视频设备个数
	int								m_iAudioDeviceNum;	// 音频设备个数
	int								m_iAppWindNum;		// 其他应用个数
	ST_NLSS_INDEVICE_INF*			m_pVideoDevices;	// 视频设备
	ST_NLSS_INDEVICE_INF*			m_pAudioDevices;	// 音频设备
	ST_NLSS_INDEVICE_INF*			m_pAppWinds;		// 其他应用

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
	void StartLiveVideo();								// 开始直播
	void StopLiveVideo();								// 停止直播
	bool InitMediaCapture();							// 初始化直播参数
	void SetMediaCapture(_HNLSSERVICE hNlssService);														
	static void SetVideoSampler(ST_NLSS_VIDEO_SAMPLER *pSampler);
	static void OnLiveStreamStatusNty(EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode);
	void ChangeAppPath(int index);						// 改变应用
	bool IsCurrentPreview();							// 当前是否预览
	void SetPauseVideo();								// 暂停视频发送
	void SetResumeVideo();								// 恢复视频发送
	void SetPauseAudio();								// 暂停音频发送
	void SetResumeAudio();								// 恢复音频发送;
	void ChangeLiveVideo();								// 改变视频源
};

#endif // UIVideo_H
