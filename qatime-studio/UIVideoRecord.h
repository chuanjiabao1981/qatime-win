#ifndef UIVideoRecord_H
#define UIVideoRecord_H

#include <QWidget>
#include "nlss_type.h"
#include "nlss_api.h"
#include "nlss_define.h"
#include <QTimer>
#include "UIWorkThread.h"
//#include "UIMainwindow.h"
#include "UIWindowset.h"

#define STARTLS_ASYNC
#pragma execution_character_set("utf-8")

//class UIMainWindow;
class UIWindowSet;
class UIVideoRecord : public QWidget
{
	Q_OBJECT

public:
	UIVideoRecord(QWidget *parent = 0);
	~UIVideoRecord();

private:
	_HNLSSERVICE					m_hNlssService;
	bool							m_bLiving;			// 直播
	bool							m_bPreviewing;		// 预览
	QString							m_strUrl;			// 推流地址
	bool							m_bInited;			// 初始化
	QString							m_errMsg;			// 错误信息
	int								m_iAppChangeIndex;	// 应用路径
	HWND							m_VideoWnd;			// 视频窗口句柄
	QTimer*							m_StartLiveTimer;	// 延迟1秒推流

public:
	EN_NLSS_VIDEOIN_TYPE			m_videoSourceType;  // 视频源类型
	EN_NLSS_AUDIOIN_TYPE			m_audioSourceType;	// 音频源类型
	int								m_iVideoDeviceNum;	// 视频设备个数
	int								m_iAudioDeviceNum;	// 音频设备个数
	int								m_iAppWindNum;		// 其他应用个数
	ST_NLSS_INDEVICE_INF*			m_pVideoDevices;	// 视频设备
	ST_NLSS_INDEVICE_INF*			m_pAudioDevices;	// 音频设备
	ST_NLSS_INDEVICE_INF*			m_pAppWinds;		// 其他应用
	EN_NLSS_VIDEOQUALITY_LVL		m_videoQ;			// 画面清晰度
	int								m_CurrentMicIndex;	// 当前麦克风索引
	int								m_CurrentVideoIndex;// 当前摄像头索引
	bool							m_bStopLiveFinish;	// 停止直播流

	UIWindowSet*					m_NewParent;		// 新主窗口
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
	void slot_livestreamErrorHappened();				// 出现错误

#ifdef STARTLS_ASYNC
	void slot_FinishStartLiveStream(int);
	void slot_FinishStopLiveStream(int);
#endif

public:
	void StartLiveVideo();								// 开始直播
	void StopLiveVideo();								// 停止直播
	bool InitMediaCapture();							// 初始化直播参数
	void SetMediaCapture(_HNLSSERVICE hNlssService);														
	static void SetVideoSampler(void *pNlssChildID, ST_NLSS_VIDEO_SAMPLER *pSampler);
	static void OnLiveStreamStatusNty(EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode);
	void ChangeAppPath(int index);						// 改变应用
	bool IsCurrentPreview();							// 当前是否预览
	bool IsCurrentLiving();								// 当前是否直播
	void SetPauseVideo();								// 暂停视频发送
	void SetResumeVideo();								// 恢复视频发送
	void SetPauseAudio(HWND hwnd);						// 暂停音频发送
	void SetResumeAudio(HWND hwnd);						// 恢复音频发送;
	void ChangeLiveVideo();								// 改变视频源
	void StopCaptureVideo();							// 停止采集
	void setPlugFlowUrl(QString url);					// 设置推流url
	void EnumAvailableMediaDevices();					// 枚举设备
	void SetMainWnd(UIWindowSet* parent);
	void SetVideoWnd(HWND hWnd);
	void SetChangeAudio(int index);
	void InitDeviceParam();								// 初始化设备参数
	int  getOutBitrate(int iWidth, int iHeight, int iFps);

};

#endif // UIVideoRecord_H
