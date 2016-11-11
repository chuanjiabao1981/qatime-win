#ifndef UIVideo_H
#define UIVideo_H

#include <QWidget>
#include "ui_UIVideo.h"
#include "nlss_type.h"
#include "nlss_api.h"
#include <QTimer>
#include <QMutex>
#include "UIWorkThread.h"
#include "UIMainwindow.h"
#include <stdint.h>
#include "WhiteBoard/boardwindow.h"

#define STARTLS_ASYNC
#pragma execution_character_set("utf-8")

struct PicRegion //һ����ɫ�����������������ڲ�������
{
	PicRegion()
	{
		pdata_ = NULL;
		//subtype_ = nim::kNIMVideoSubTypeARGB;
		size_max_ = 0;
		size_ = 0;
	}

	~PicRegion()
	{
		Clear();
	}
	void Clear()
	{
		if (pdata_)
		{
			delete[] pdata_;
			pdata_ = NULL;
		}
		size_max_ = 0;
		size_ = 0;
	}
	int ResetData(uint64_t time, const char* data, int size, unsigned int width, unsigned int height/*, nim::NIMVideoSubType subtype*/)
	{
		if (size > size_max_)
		{
			if (pdata_)
			{
				delete[] pdata_;
				pdata_ = NULL;
			}
			pdata_ = new char[size];
			size_max_ = size;
		}
		width_ = width;
		height_ = height;
		timestamp_ = time;
		//subtype_ = subtype;
		size_ = size;
		memcpy(pdata_, data, size);
		return size;
	}

	//nim::NIMVideoSubType subtype_;
	char*		pdata_;         //��ɫ�����׵�ַ
	int			size_max_;
	int			size_;
	long        width_;         //���ؿ��
	long        height_;        //���ظ߶�
	uint64_t	timestamp_;     //ʱ��������룩
};

enum FrameType
{
	Ft_I420 = 0,
	Ft_ARGB,
	Ft_ARGB_r,
};

class UIMainWindow;
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
	static  QMutex					m_mutex;
	bool							m_bLiving;			// ֱ��
	bool							m_bPreviewing;		// Ԥ��
	QString							m_strUrl;			// ������ַ
	bool							m_bInited;			// ��ʼ��
	QString							m_errMsg;			// ������Ϣ
	int								m_iAppChangeIndex;	// Ӧ��·��
	HWND							m_VideoWnd;			// ��Ƶ���ھ��
	QTimer*							m_StartLiveTimer;	// �ӳ�1������
	BoardWindow*					m_BoadWnd;			// �װ�

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

	UIMainWindow*					m_Parent;			// ������
	static PicRegion				capture_video_pic_;
#ifdef STARTLS_ASYNC
	Worker* m_pWorker;
#endif

protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void resizeEvent(QResizeEvent *e);

Q_SIGNALS:
	void sig_changeLiveStatus(bool bTrue);
#ifdef STARTLS_ASYNC
	void sig_StartLiveStream();
	void sig_StopLiveStream();
#endif

private slots:
	void slot_onRefreshTimeout();						// ˢ�½���
	void slot_onStartLiveTimeout();						// ��ʼֱ��

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
	bool IsCurrentLiving();								// ��ǰ�Ƿ�ֱ��
	void SetPauseVideo();								// ��ͣ��Ƶ����
	void SetResumeVideo();								// �ָ���Ƶ����
	void SetPauseAudio(HWND hwnd);						// ��ͣ��Ƶ����
	void SetResumeAudio(HWND hwnd);						// �ָ���Ƶ����;
	void ChangeLiveVideo();								// �ı���ƵԴ
	void StopCaptureVideo();							// ֹͣ�ɼ�
	void setPlugFlowUrl(QString url);					// ��������url
	void setLessonName(QString strLessonName);
	void EnumAvailableMediaDevices();					// ö���豸
	void setPersonNum(int num);
	void sendCoutom(ST_NLSS_VIDEO_SAMPLER	m_SvideoSampler);
	void SetMainWnd(UIMainWindow* wnd);
	void SetVideoWnd(HWND hWnd);
	void SetChangeVideoAudio(HWND hwnd, QString path, bool bVideo=true);
	void SetWhiteBoard();
	void SetWhiteBoardHidden(bool bHide);
	bool IsWhiteBoardVisible();
};

#endif // UIVideo_H
