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
	std::vector<CBtnPix*>	m_vecImgProcess;	//������е�ͼƬ�ؼ�

	std::vector<CBtnAudio*> m_vecAudio;			//���е�audio��ť
private:
	bool isDigitStr(QString str);
	void style(QScrollArea *style);
public:
	void InsertChat(QPixmap* head, QString name, QString time, QString text, bool bTeacher=true);  // bTeacher ����������ɫ
	void InsertAudioChat(QPixmap* head, QString name, QString time, QString text, 
		std::string path, std::string sid, std::string msgid, bool bTeacher = true);  // ������������
	void InsertNotice(QString text);												// ��Ϣ֪ͨ
	void InsertPic(QPixmap* head, QString name, QString time, QString URL, QString sMsgID="", bool bTeacher = true);	// ����ͼƬ
	void InsertEmoji(QPixmap* head, QString name, QString time, QString text, bool bTeacher=true);		// �������
	void ParseFace(AnimatedTextBrowserA* Anim, QString qContect);					// ��������
	QString BuildFaceToUrl(QString qFace);											// ��������url
	void stringToHtml(QString &str, QColor crl);									// QStringתhtlm����ɫ
	void ScrollDown();																// ��������ײ�
	void PicProcess(double iProcess);												// ����ͼƬ����
	void SendStatus(QString msgid);													// ����״̬
	void setMainWindow(UIChatRoom* parent);											// ���ø�����
	void ClearAll();																// ���
	void DelaySrcoll();
	void stopAudio(char* msgid);													// ֹͣ��������
};

#endif // UITALK_H