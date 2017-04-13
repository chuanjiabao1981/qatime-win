#ifndef UIBULLETPARAM_H
#define UIBULLETPARAM_H

#include <QWidget>
#include "ui_UIBulletparam.h"
#include "UIWindowset.h"

class UIWindowSet;
class UIBulletParam : public QWidget
{
	Q_OBJECT

public:
	UIBulletParam(QWidget *parent = 0);
	~UIBulletParam();

private:
	Ui::UIBulletParam ui;
	UIWindowSet*			m_Parent;

	// 鼠标按下拖动的操作
	QPoint								m_startPos;
	QPoint								m_clickPos;
	QPoint								m_WndCurPos;
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual bool eventFilter(QObject *target, QEvent *event);
	virtual void focusOutEvent(QFocusEvent *);

private slots :
	void DelayChanged(int);
	void AllRidio(bool b);
	void BtnRidio(bool b);
	void CloseDialog();

public:
	void setParent(UIWindowSet* parent);

};

#endif // UIBULLETPARAM_H
