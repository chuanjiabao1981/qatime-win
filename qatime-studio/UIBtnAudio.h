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
//---云信
#include "assert.h"
#include <string>
#include "nim_cpp_api.h"
#include "nim_cpp_client.h"
#include "nim_cpp_tool.h"
#include "nim_audio_cpp.h"
#pragma execution_character_set("utf-8")

class CBtnAudio : public QPushButton
{
	Q_OBJECT

public:
	CBtnAudio(std::string path, std::string sid, std::string msgid, QWidget *parent = 0, bool bRead = false); //bRead 默认为未读
	~CBtnAudio();

Q_SIGNALS:
	void sig_Audioclicked(std::string, std::string, std::string, bool);
	void sig_AudioLoadFail(nim::IMMessage);
	void sig_AudioLoadEnd(QPixmap*, QString, QString, QString, std::string, std::string, std::string, bool, bool, CBtnAudio*);
public Q_SLOTS:
	void onClicked(bool);
	void slot_onCountTimeout();		// 定时器动画
	void slot_onDownTimeout();		// 下载资源
	void slot_onDownEndTimeout();	// 下载完成

protected:

public:
	bool m_bRead;				// 语音已读未读
	std::string m_path;			// 语音路径
	std::string m_sid;			// 会话ID
	std::string m_msgid;		// 消息ID
	bool m_bIsPlay;				// 语音是否在播放
private:
	
	bool m_bDownEnd;			// 语音是否下载完成
	
	int  m_ImageCount;			// 播放图片的顺序

	QTimer*	m_Timer;			// 动画定时器
	QTimer* m_TimerDown;		// 下载语音定时器
	//add by zbc 20170705
	int			m_iCount;		// 循环60次 也就是3秒还下载不完，则重新下载
	nim::IMMessage m_msg;		// 消息结构
	bool m_LoadStatus;			// 下载状态
	CBtnAudio*	m_pLastAudio;	// 下一条语音

	//待插入信息
	QPixmap*	m_pixmap;		// 头像
	QString		m_name;			// 名字
	QString		m_time;			// 时间
	QString		m_text;			// 内容
	bool		m_bTeacher;		// 是否是老师
	QTimer* m_TimerDownEnd;		// 下载完成定时器

public:
	QString GetMsgID();							// 获取消息
	void stopPlay();							// 停止播放
	bool GetPlayStatus();						// 获取播放状态
	void setMsg(nim::IMMessage msg);			// add by zbc 20170705
	void LoadFail();							// 下载失败
	void LoadSuc();								// 下载成功
	void setLastAudio(CBtnAudio* pLastAudio);	// 保存下一个语音
	void setMsgAll(QPixmap* pixmap, QString name, QString time, QString text, std::string path, std::string sid, std::string msgid, bool bTeacher, bool bRead);
};

#endif // _LS_MEDIACAPTURE_UI_BUTTON_H_