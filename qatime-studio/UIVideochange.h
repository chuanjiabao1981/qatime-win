#ifndef UIVIDEOCHANGE_H
#define UIVIDEOCHANGE_H

#include <QWidget>
#include "ui_UIVideochange.h"
#include <QButtonGroup>
#include <QVBoxLayout>
#include "UIWindowSet.h"
#include <QRadioButton>

class UIWindowSet;

class UIVideoChange : public QWidget
{
	Q_OBJECT

public:
	UIVideoChange(QWidget *parent = 0);
	~UIVideoChange();

	int iCount;
private:
	Ui::UIVideoChange   ui;
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
	void setVideoChange(UIWindowSet* Parent);
	void setVideoParam(QString name, QString path);
};

#endif // UIVIDEOCHANGE_H
