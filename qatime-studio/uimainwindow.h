#ifndef UIMAINWINDOW_H
#define UIMAINWINDOW_H

#include <QWidget>
#include "ui_uimainwindow.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "user.h"
#include "UIAuxiliary.h"
#include "UILesson.h"
#include "UIVideo.h"
#include <windows.h>
#include "UIAuxiliaryPanel.h"

#pragma execution_character_set("utf-8")
#define STARTLS_ASYNC

class UIMainWindow : public QWidget
{
	Q_OBJECT

public:
	UIMainWindow(QWidget *parent = 0);
	~UIMainWindow();

private:
	Ui::UIMainWindow				ui;
	QString							mRemeberToken;
	QNetworkAccessManager			manager;
	QNetworkReply*					reply;
	QPoint							last;

	UIAuxiliary*					m_AuxiliaryInfo;	// ��������Ϣ����
	UILesson*						m_LessonInfo;		// �γ���Ϣ����
	UIVideo*						m_VideoInfo;		// ֱ������
	QString							m_teacherID;		// ��ʦID
	QTimer*							m_CountTimer;		// ��ʱ��
	QTimer*							m_HeartTimer;		// ����
	INT64							m_iTimerCount;		// ��ʱ������

	UIAuxiliaryPanel*				m_AuxiliaryPanel;	// ��ʦ���

private slots :
	void MinDialog();									// ��С���Ի���
	void MaxDialog();									// ��󻯶Ի���
	void CloseDialog();									// �رնԻ���
	void Expansion();									// �������
	void slot_startOrStopLiveStream();					// ��ʼֱ��
	void VideoSourceChange(int index);					// ��ƵԴ�仯
	void VideoAppChange(int index);						// APPӦ�÷����仯
	void VideoStatus(int iStatus);						// ��Ƶ״̬��ֱ������ͣ�������Ŀ��ƣ�
	void AudioStatus(int iStatus);						// ����״̬��ֱ������ͣ�������Ŀ��ƣ�
	void slot_onCountTimeout();							// ��ʱ�� �ı�ֱ��ʱ��
	void slot_onHeartTimeout();							// 5��һ�Σ���������

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // ��ӱ���caption
	virtual void resizeEvent(QResizeEvent *);			// ���ô���Բ��
	virtual void paintEvent(QPaintEvent *event);

private:
	void SetSourceAppPath();							// ����ƵԴѡ������Ӧ��ʱ������APP·������

public:
	void setTeacherInfo(QJsonObject& data);					// ������ʦ��Ϣ
	void setRemeberToken(const QString &name);				// ����Token
	void AuxiliaryRequestFinished();						// ������http����
	void LessonRequestFinished();							// �γ�http����
	void EnumAvailableMediaDevices();						// ö���豸
	void SendStartLiveHttpMsg();							// ������������ֱ����ʼ��Ϣ
	void SendStopLiveHttpMsg();								// ������������ֱ��ֹͣ��Ϣ
	void SendHeartBeatHttpMsg();							// ������������ֱ��������Ϣ��5����һ�Σ�
	void ShowAuxiliary();									// ��ʾ������
};

#endif // UIMAINWINDOW_H
