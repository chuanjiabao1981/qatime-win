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
	QString					m_sLiveToken;			//心跳token
	QString					m_lessonID;				//课程ID
	QString					m_Time;					//直播心跳失败调用的时间戳

	int						m_iGetRtmpCount;		//获取推流地址失败重试次数
	QTimer*					m_TGetRtmpTimer;		//获取推流地址失败重试

	int						m_iBeatStep;			//心跳间隔
	int						m_iHeartCount;			//心跳失败重试次数
	QTimer*					m_HeartTimer;			//心跳定时器
	QTimer*					m_HeartFailTimer;		//心跳失败定时器

	int						m_iStopLiveCount;		//停止推流发送消息失败重试次数
	QTimer*					m_StopFailTimer;		//停止推流失败定时器

	QTimer*					m_SwitchFailTimer;		//切换状态失败定时器
	int						m_iBoardStatus;			//白板直播状态
	int						m_iCameraStatus;		//摄像头直播状态
private slots :
	void GetRtmpFailTimer();						//获取推流地址失败重试定时器信号槽
	void HeartBeatTimer();							//心跳定时器信号槽
	void HeartBeatFailTimer();						//心跳失败信号槽
	void StopLiveFailTimer();						//停止推流失败信号槽
	void SwitchFailTimer();							//切换状态失败信号槽

private:
	void ReturnRtmpAddressAndHeartBeat();			// 返回白板、摄像头、推流地址 及 心跳间隔
	void ReturnHeartBeat();							// 返回心跳
	void FinishStartLive();							// 返回开始直播请求的token
	void FinishStopLive();							// 返回结束直播请求的状态
	void FinishCameraSwitch();						// 返回摄像头切换结果
public:
	void setMainWindow(UIMainWindow* parent);									// 设置父窗口
	void GetRtmpAddressAndHeartBeat(QString lessonID, QString sToken);			// 获取白板、摄像头、推流地址 及 心跳间隔
	void SendStartLiveHttpMsg(int iBoard, int iCamera,QString sLessonid,
								QString sToken);								// 往服务器发送直播开始消息
	void SendStopLiveHttpMsg(bool bConnect = true);								// 往服务器发送直播停止消息
	void StartHeartBeat();														// 开始心跳
	void StopTimer();															// 停止所有计时器
	void SendCameraSwitchMsg(int iBoard, int iCamera);							// 发送摄像头切换状态消息
	void RequestError(QJsonObject& error, bool bTrue=true);						// 出现错误
};

#endif // LIVESTATUSMANAGER_H
