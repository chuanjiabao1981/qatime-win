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
#include "UIWindowset.h"

#pragma execution_character_set("utf-8")

class UIMainWindow;
class UIWindowSet;
class LiveStatusManager : public QObject
{
	Q_OBJECT

public:
	LiveStatusManager(QObject *parent);
	~LiveStatusManager();


	QString					m_ChannelID;			//回放视频的ChannelID
private:
	QUrl url;
	QNetworkAccessManager	manager;
	QNetworkReply*			reply;
	UIMainWindow*			m_parent;
	UIWindowSet*			m_newParent;
	QString					m_sToken;				// token
	QString					m_sLiveToken;			// 心跳token
	QString					m_lessonID;				// 课程ID
	QString					m_Time;					// 直播心跳失败调用的时间戳
	

	int						m_iGetRtmpCount;		// 获取推流地址失败重试次数
	QTimer*					m_TGetRtmpTimer;		// 获取推流地址失败重试

	int						m_iBeatStep;			// 心跳间隔
	int						m_iHeartCount;			// 心跳失败重试次数
	QTimer*					m_HeartTimer;			// 心跳定时器
	QTimer*					m_HeartFailTimer;		// 心跳失败定时器

	int						m_iStopLiveCount;		// 停止推流发送消息失败重试次数
	QTimer*					m_StopFailTimer;		// 停止推流失败定时器

	QTimer*					m_SwitchFailTimer;		// 切换状态失败定时器
	int						m_iBoardStatus;			// 白板直播状态
	int						m_iCameraStatus;		// 摄像头直播状态
	bool					m_EnvironmentalTyle;	// 当前环境
	QString					m_homePage;

	// 互动
	QTimer*					m_1v1HeartTimer;		// 1v1心跳定时器
	QTimer*					m_1v1HeartFailTimer;	// 1v1心跳失败定时器
	int						m_mLessonType;			// 辅导班类型
private slots :
	void GetRtmpFailTimer();						// 获取推流地址失败重试定时器信号槽
	void HeartBeatTimer();							// 心跳定时器信号槽
	void HeartBeatFailTimer();						// 心跳失败信号槽
	void StopLiveFailTimer();						// 停止推流失败信号槽
	void SwitchFailTimer();							// 切换状态失败信号槽

	//互动直播
	void HeartBeat1v1Timer();						// 1v1心跳定时器信号槽
	void HeartBeatFailTimer1v1();					// 心跳失败信号槽
private:
	void ReturnRtmpAddressAndHeartBeat();			// 返回白板、摄像头、推流地址 及 心跳间隔
	void ReturnHeartBeat();							// 返回心跳
	void FinishStartLive();							// 返回开始直播请求的token
	void FinishStopLive();							// 返回结束直播请求的状态
	void FinishCameraSwitch();						// 返回摄像头切换结果
	//互动直播
	void Finish1v1StartLive();		
	void ReturnHeartBeat1v1();
	void FinishStopLive1v1();
public:
	// 直播
	void setMainWindow(UIWindowSet* parent);									// 设置父窗口
	void GetRtmpAddressAndHeartBeat(QString lessonID, QString sToken);			// 获取白板、摄像头、推流地址 及 心跳间隔
	void SendStartLiveHttpMsg(int iBoard, int iCamera,QString sLessonid,
								QString sToken, int mLessonType);								// 往服务器发送直播开始消息
	void SendStopLiveHttpMsg(bool bConnect = true);								// 往服务器发送直播停止消息
	void StartHeartBeat();														// 开始心跳
	void StopTimer();															// 停止所有计时器
	void SendCameraSwitchMsg(int iBoard, int iCamera);							// 发送摄像头切换状态消息
	void RequestError(QJsonObject& error, bool bTrue=true);						// 出现错误
	void SetEnvironmental(bool bTyle, QString homePage);

	// 一对一互动
	void SendStart1v1LiveHttpMsg(QString sLessonid,QString chatid, QString sToken);	// 往服务器发送1v1直播开始消息
	void Start1v1HeartBeat();
	void SendStopLiveHttpMsg1v1(bool bConnect = true);  						// 停止1对1发送消息
};

#endif // LIVESTATUSMANAGER_H
