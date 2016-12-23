#ifndef UISETPARAM_H
#define UISETPARAM_H

#include <QWidget>
#include "ui_UISetparam.h"
#include "UIMainwindow.h"

class UIMainWindow;

class UISetParam : public QWidget
{
	Q_OBJECT

public:
	UISetParam(QWidget *parent = 0);
	~UISetParam();

private:
	Ui::UISetParam ui;
	UIMainWindow*			m_Parent;

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private slots :
	void CloseDialog();									// �رնԻ���
	void AudioChanged(int);
	void VideoChanged(int);								

private:
	void InitUI();										// ��ʼ����ʽ

public:
	void setAudioParam(QString name, QString path);		// ��˷����
	void setVideoParam(QString name, QString path);		// ����ͷ����
	void setParent(UIMainWindow* parent);
};

#endif // UISETPARAM_H
