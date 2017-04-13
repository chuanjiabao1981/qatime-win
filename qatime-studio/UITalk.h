#ifndef UITALK_H
#define UITALK_H

#include <QWidget>
#include "ui_UITalk.h"
#include <QScrollArea>
#include "UIBtnPix.h"
#include "UIBtnAudio.h"
#include "animatedtextbrowserA.h"
#include "UIChatRoom.h"

#pragma execution_character_set("utf-8")

class UIChatRoom;
class AnimatedTextBrowserA;
class UITalk : public QWidget
{
	Q_OBJECT

public:
	UITalk(QWidget *parent = 0);
	~UITalk();

protected Q_SLOTS:
	void slot_btnclicked(QString, QPixmap, bool);
	void slot_faildclicked(CBtnPix*);
	void slot_scrollDown();
	void slot_Audioclicked(std::string, std::string, std::string, bool);

private:
	Ui::UITalk ui;
	QSpacerItem*		m_spacer;
	QScrollArea*		m_view;
	QWidget*			m_mainView;
	QVBoxLayout*		m_Ver;
	QVBoxLayout*		m_VerAll;
	UIChatRoom*			m_parent;
	std::vector<CBtnPix*>	m_vecImgProcess;	//存放所有的图片控件

	std::vector<CBtnAudio*> m_vecAudio;			//所有的audio按钮
private:
	bool isDigitStr(QString str);
	void style(QScrollArea *style);
public:
	void InsertChat(QPixmap* head, QString name, QString time, QString text, bool bTeacher=true);  // bTeacher 区分名字颜色
	void InsertAudioChat(QPixmap* head, QString name, QString time, QString text, 
		std::string path, std::string sid, std::string msgid, bool bTeacher = true);  // 插入语音聊天
	void InsertNotice(QString text);												// 消息通知
	void InsertPic(QPixmap* head, QString name, QString time, QString URL, QString sMsgID="", bool bTeacher = true);	// 插入图片
	void InsertPicUrl(QPixmap* head, QString name, QString time, QString URL, QString sMsgID = "", bool bTeacher = true);	// 插入图片
	void InsertEmoji(QPixmap* head, QString name, QString time, QString text, bool bTeacher=true);		// 插入表情
	void ParseFace(AnimatedTextBrowserA* Anim, QString qContect);					// 解析表情
	QString BuildFaceToUrl(QString qFace);											// 创建表情url
	void stringToHtml(QString &str, QColor crl);									// QString转htlm带颜色
	void ScrollDown();																// 滚动到最底部
	void PicProcess(double iProcess);												// 发送图片进度
	void SendStatus(QString msgid);													// 发送状态
	void setMainWindow(UIChatRoom* parent);											// 设置父窗口
	void ClearAll();																// 清空
	void DelaySrcoll();
	void stopAudio(char* msgid);													// 停止语音播放
};

#endif // UITALK_H
