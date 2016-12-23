#ifndef UISCREENTIP_H
#define UISCREENTIP_H

#include <QWidget>
#include "ui_UIScreentip.h"

class UIScreenTip : public QWidget
{
	Q_OBJECT

public:
	UIScreenTip(QWidget *parent = 0);
	~UIScreenTip();

private:
	Ui::UIScreenTip ui;

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private slots :
	void CloseDialog();
	void TipStatus(int);

public:
	bool IsTip();
	void setErrorTip(QString error);
};

#endif // UISCREENTIP_H
