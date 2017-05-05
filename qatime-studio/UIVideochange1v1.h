#ifndef UIVideoChange1v1_H
#define UIVideoChange1v1_H

#include <QWidget>
#include "ui_UIVideoChange1v1.h"
#include <QButtonGroup>
#include <QVBoxLayout>
#include "UIWindowSet.h"
#include <QRadioButton>

class UIWindowSet;

class UIVideoChange1v1 : public QWidget
{
	Q_OBJECT

public:
	UIVideoChange1v1(QWidget *parent = 0);
	~UIVideoChange1v1();

	int iCount;
private:
	Ui::UIVideoChange1v1   ui;
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
	QString GetCurPath();
};

#endif // UIVideoChange1v1_H
