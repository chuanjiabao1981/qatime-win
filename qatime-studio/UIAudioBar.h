#ifndef UIAUDIOBAR_H
#define UIAUDIOBAR_H

#include <QWidget>
#include "ui_UIAudioBar.h"
#include "UIChatRoom.h"

class UIChatRoom;
class UIAudioBar : public QWidget
{
	Q_OBJECT

public:
	UIAudioBar(QWidget *parent = 0);
	~UIAudioBar();

private:
	Ui::UIAudioBar ui;
	UIChatRoom*			m_parent;
	QTimer*				m_timer;	// 秒数计时器

	int					m_iSec;		// 秒数
	bool				m_bSend;	// 是否发送

	QTimer*				m_FailTimer;// 超时5秒算失败
	bool				m_bCapturing;	//是否在录制中
private slots:
	void clickSend();									// 发送语音
	void clickCancel();									// 取消发送
	void slot_onTimeout();								// 秒数计时器
	void slot_onFailTimeout();							// 失败

public:
	void setMainWindow(UIChatRoom* parent);
	void CaptureAudio();								// 开始采集语音
	bool IsSend();										// 是否发送
	void MonitorFail();									// 监控是否失败
	bool IsCapturing();									// 是否在录音采集中 false：录音中 true：未录音
};

#endif // UIAUDIOBAR_H
