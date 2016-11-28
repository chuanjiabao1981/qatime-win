#ifndef UIPERSONNUM_H
#define UIPERSONNUM_H

#include <QWidget>
#include "ui_UIPersonnum.h"

class UIPersonNum : public QWidget
{
	Q_OBJECT

public:
	UIPersonNum(QWidget *parent = 0);
	~UIPersonNum();

private:
	Ui::UIPersonNum ui;

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
};

#endif // UIPERSONNUM_H
