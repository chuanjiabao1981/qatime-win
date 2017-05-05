#ifndef UIAudioChange1v1_H
#define UIAudioChange1v1_H

#include <QWidget>
#include "ui_UIAudioChange1v1.h"
#include "nlss_type.h"
#include "UIWindowSet.h"

class UIWindowSet;

class UIAudioChange1v1 : public QWidget
{
	Q_OBJECT

public:
	UIAudioChange1v1(QWidget *parent = 0);
	~UIAudioChange1v1();

private:
	Ui::UIAudioChange1v1 ui;
	UIWindowSet*		m_Parent;

	// 鼠标按下拖动的操作
	QPoint								m_startPos;
	QPoint								m_clickPos;
	QPoint								m_WndCurPos;							
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual bool eventFilter(QObject *target, QEvent *event);
	virtual void focusOutEvent(QFocusEvent *);

private slots:
	void AudioChanged(int index);
	void clickClose();
	void valueChanged(int);

public:
	void setSliderStyle();
	void setAudioParam(QString strName, QString path);
	void setAudioChange(UIWindowSet* Parent);
	QString GetCurPath();
};

#endif // UIAudioChange1v1_H
