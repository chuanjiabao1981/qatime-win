#ifndef UIABOUTVERSION_H
#define UIABOUTVERSION_H

#include <QWidget>
#include "ui_UIAboutversion.h"

class UIAboutVersion : public QWidget
{
	Q_OBJECT

public:
	UIAboutVersion(QWidget *parent = 0);
	~UIAboutVersion();

private:
	Ui::UIAboutVersion ui;

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
	virtual void paintEvent(QPaintEvent *event);

public:
	void setVersion(QString version);

private slots:
	void clickDialog();
};

#endif // UIABOUTVERSION_H
