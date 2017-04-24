#ifndef UIPERSONWND_H
#define UIPERSONWND_H

#include <QWidget>
#include "ui_UIPersonwnd.h"
#include "personListBuddy.h"
#include <QPushButton>
#include "UIWindowSet.h"
#include "UIPerson.h"

class UIWindowSet;
class UIPerson;
#pragma execution_character_set("utf-8")

class UIPersonWnd : public QWidget
{
	Q_OBJECT

public:
	UIPersonWnd(QWidget *parent = 0);
	~UIPersonWnd();

private:
	Ui::UIPersonWnd ui;
	QSpacerItem*				m_spacer;

	std::vector<QLabel*>		m_vecLayout;		// 头像和名字
	std::vector<QCheckBox*>		m_vecLayoutCheck;	// 禁言按钮
	QPushButton*				m_btn;
	UIWindowSet*				m_parent;
	std::vector<UIPerson*>	m_vecPerson;		// 人员列表

signals:
	void sig_stateChange(bool, QString, QString);
private slots:
	void slot_change(bool, QString, QString);
	void slot_allChange(bool,QString);
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void focusOutEvent(QFocusEvent *);

public:
	void AddPerson(std::vector<personListBuddy*> vecBuddy, QString chatID);
	void AddAllMute(QString chatid, bool bAllMute);
	void DeletePerson();
	void style(QScrollArea *style);
	void setParentBtn(UIWindowSet* parent);
};

#endif // UIPERSONWND_H
