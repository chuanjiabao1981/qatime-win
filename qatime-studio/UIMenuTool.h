#ifndef UIMENUTOOL_H
#define UIMENUTOOL_H

#include <QWidget>
#include "ui_UIMenuTool.h"
#include "UITimers.h"

class UITimers;
class UIMenuTool : public QWidget
{
	Q_OBJECT

public:
	UIMenuTool(QWidget *parent = 0);
	~UIMenuTool();

	void setLivePushBtnText(bool bStart);
	QPoint getGeometry();
	void setMenuCheckState(Qt::CheckState state);
	void setVideoCheckState(Qt::CheckState state);
	bool getisChecked();
	void setVideoEnabled(bool bEnable);
	void setFullScreenCheck(Qt::CheckState status);
	bool getFullScreenIsChecked();
	void setFullScreenEnabled(bool bEnable);
	void setAppEnabled(bool bEnable);
	void setLessonEnabled(bool bEnable);
	void setTimeLabelText(QString text);
	void setTimeLabelVisible(bool b);
	void setRatioText(QString text);
	void moveLiveBtn();
	void InitMoveLiveBtn();
	void setFullEnabled();
signals:
	void emit_startOrStopLiveStream();					// 开始直播
	void emit_VideoStatus(int iStatus);					// 视频状态（直播中暂停、继续的控制）
	void emit_AudioStatus(int iStatus);					// 声音状态（直播中暂停、继续的控制）
	void emit_FullScreenStatus(int iStatus);			// 切换全屏视频源
	void emit_OtherApp(int i);							// 其它APP应用
	void emit_clickChangeAudio(int);
	void emit_clickChangeVideo(int);					// 弹出摄像头选择框
	void emit_clickChangeRatio();						// 弹出分辨率框
	void emit_clickLessonList();						// 弹出课程表
	void emit_FulSereen(int b);							// 隐藏右侧聊天区
public slots:
	void slot_startOrStopLiveStream();					// 开始直播
	void VideoStatus(int iStatus);						// 视频状态（直播中暂停、继续的控制）
	void AudioStatus(int iStatus);						// 声音状态（直播中暂停、继续的控制）
	void FullScreenStatus(int iStatus);					// 切换全屏视频源
	void OtherApp(int i);								// 其它APP应用
	void clickChangeAudio(int i);
	void clickChangeVideo(int i);						// 弹出摄像头选择框
	void clickChangeRatio();							// 弹出分辨率框
	void clickLessonList();								// 弹出课程表
	void slot_fulSereen(int b);								// 隐藏聊天窗口
private:
	Ui::UIMenuTool ui;
	UITimers *						m_timers;			//  开始直播的时间和按钮
};

#endif // UIMENUTOOL_H
