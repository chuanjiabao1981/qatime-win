#ifndef UIBULLETSET_H
#define UIBULLETSET_H

#include <QWidget>
#include "ui_UIBulletset.h"
#include "UIBulletscreen.h"

class UIBulletScreen;
class UIBulletSet : public QWidget
{
	Q_OBJECT

public:
	UIBulletSet(QWidget *parent = 0);
	~UIBulletSet();

private:
	Ui::UIBulletSet ui;
	UIBulletScreen*		m_parent;

protected:
	virtual void focusOutEvent(QFocusEvent *);
private slots :
	void Font1();
	void Font2();
	void Font3();
	void Font4();
	void Color1();
	void Color2();
	void Color3();
	void Color4();
	void closeDialog();
	void valueChange(int i);

public:
	void setMainWindow(UIBulletScreen* parent);
};

#endif // UIBULLETSET_H
