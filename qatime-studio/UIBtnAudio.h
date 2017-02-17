/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#ifndef _LS_UI_BUTTON_AUDIO_H_
#define _LS_UI_BUTTON_AUDIO_H_
#include <QtWidgets/QPushButton>
#include <QTimer>
#pragma execution_character_set("utf-8")

class CBtnAudio : public QPushButton
{
	Q_OBJECT

public:
	CBtnAudio(std::string path, std::string sid, std::string msgid, QWidget *parent = 0, bool bRead = false); //bRead 默认为未读
	~CBtnAudio();

Q_SIGNALS:
	void sig_Audioclicked(std::string, std::string, std::string, bool);

public Q_SLOTS:
	void onClicked(bool);
	void slot_onCountTimeout();		// 定时器动画
	void slot_onDownTimeout();		// 下载资源

protected:

private:
	std::string m_path;			// 语音路径
	std::string m_sid;			// 会话ID
	std::string m_msgid;		// 消息ID
	bool m_bIsPlay;				// 语音是否在播放
	bool m_bDownEnd;			// 语音是否下载完成
	bool m_bRead;				// 语音已读未读
	int  m_ImageCount;			// 播放图片的顺序

	QTimer*	m_Timer;			// 动画定时器
	QTimer* m_TimerDown;		// 下载语音定时器

public:
	QString GetMsgID();							// 获取消息
	void stopPlay();							// 停止播放
	bool GetPlayStatus();						// 获取播放状态

};

#endif // _LS_MEDIACAPTURE_UI_BUTTON_H_