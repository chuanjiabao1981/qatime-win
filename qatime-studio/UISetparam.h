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
	void CloseDialog();									// 关闭对话框
	void AudioChanged(int);
	void VideoChanged(int);								

private:
	void InitUI();										// 初始化样式

public:
	void setAudioParam(QString name, QString path);		// 麦克风参数
	void setVideoParam(QString name, QString path);		// 摄像头参数
	void setParent(UIMainWindow* parent);
};

#endif // UISETPARAM_H
