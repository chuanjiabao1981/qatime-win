#ifndef UIMENUTOOL_H
#define UIMENUTOOL_H

#include <QWidget>
#include "ui_UIMenuTool.h"

class UIMenuTool : public QWidget
{
	Q_OBJECT

public:
	UIMenuTool(QWidget *parent = 0);
	~UIMenuTool();

	void setLivePushBtnText(QString text);
	QPoint getGeometry();
	void setMenuCheckState(Qt::CheckState state);
	void setVideoCheckState(Qt::CheckState state);
	bool getisChecked();
	void setVideoEnabled(bool bEnable);
	void setFullScreenCheck(bool bEnable);
	bool getFullScreenIsChecked();
	void setFullScreenEnabled(bool bEnable);
	void setAppEnabled(bool bEnable);
	void setLessonEnabled(bool bEnable);
	void setTimeLabelText(QString text);
	void setTimeLabelVisible(bool b);
	void setRatioText(QString text);
signals:
	void emit_startOrStopLiveStream();					// ��ʼֱ��
	void emit_VideoStatus(int iStatus);					// ��Ƶ״̬��ֱ������ͣ�������Ŀ��ƣ�
	void emit_AudioStatus(int iStatus);					// ����״̬��ֱ������ͣ�������Ŀ��ƣ�
	void emit_FullScreenStatus(int iStatus);			// �л�ȫ����ƵԴ
	void emit_OtherApp(int i);							// ����APPӦ��
	void emit_clickChangeAudio(int);
	void emit_clickChangeVideo(int);					// ��������ͷѡ���
	void emit_clickChangeRatio();						// �����ֱ��ʿ�
	void emit_clickLessonList();						// �����γ̱�
public slots:
	void slot_startOrStopLiveStream();					// ��ʼֱ��
	void VideoStatus(int iStatus);						// ��Ƶ״̬��ֱ������ͣ�������Ŀ��ƣ�
	void AudioStatus(int iStatus);						// ����״̬��ֱ������ͣ�������Ŀ��ƣ�
	void FullScreenStatus(int iStatus);					// �л�ȫ����ƵԴ
	void OtherApp(int i);								// ����APPӦ��
	void clickChangeAudio(int i);
	void clickChangeVideo(int i);							// ��������ͷѡ���
	void clickChangeRatio();							// �����ֱ��ʿ�
	void clickLessonList();								// �����γ̱�
private:
	Ui::UIMenuTool ui;
};

#endif // UIMENUTOOL_H
