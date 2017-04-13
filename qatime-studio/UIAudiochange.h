#ifndef UIAUDIOCHANGE_H
#define UIAUDIOCHANGE_H

#include <QWidget>
#include "ui_UIAudiochange.h"
#include "nlss_type.h"
#include "UIWindowSet.h"

class UIWindowSet;

class UIAudioChange : public QWidget
{
	Q_OBJECT

public:
	UIAudioChange(QWidget *parent = 0);
	~UIAudioChange();

private:
	Ui::UIAudioChange ui;
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

public:
	void setAudioParam(QString strName, QString path);
	void setAudioChange(UIWindowSet* Parent);
};

#endif // UIAUDIOCHANGE_H
