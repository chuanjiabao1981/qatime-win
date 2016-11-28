#ifndef RANGECAPTURE_H
#define RANGECAPTURE_H

#include <QWidget>
#include "ui_Rangecapture.h"

class RangeCapture : public QWidget
{
	Q_OBJECT

public:
	RangeCapture(QWidget *parent = 0);
	~RangeCapture();

private:
	Ui::RangeCapture ui;

	QTimer*							m_Timer;			

private slots :
	void setPos();								// …Ë÷√Œª÷√

protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // ÃÌº”caption
	virtual void paintEvent(QPaintEvent *event);
};

#endif // RANGECAPTURE_H
