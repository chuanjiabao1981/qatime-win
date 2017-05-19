#ifndef UINOTICEWND_H
#define UINOTICEWND_H

#include <QWidget>
#include "ui_UINoticewnd.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "UIWindowSet.h"
#include "UITextEdit.h"

class UIWindowSet;
#pragma execution_character_set("utf-8")
class UINoticeWnd : public QWidget
{
	Q_OBJECT

public:
	UINoticeWnd(QWidget *parent = 0);
	~UINoticeWnd();

	QWidget*			m_mainAllView;
	QVBoxLayout*		m_VerAll;
	QSpacerItem*		m_spacer;

	std::vector<QLabel*>	m_veclabel;
	std::vector<UITextEdit*>	m_vecEdit;
	UIWindowSet*		m_parent;
	QPushButton*		m_btn;
private:
	Ui::UINoticeWnd ui;

private slots:
	void clickShowNotice();
	void clickSendAnnounce();
	void clickCancel();

protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void focusOutEvent(QFocusEvent *);

public:
	void AddNotic(QString text, QString time,bool bInsert = false);
	void DeleteNotice();
	void style(QScrollArea *style);
	void setParentBtn(UIWindowSet* parent);
	void initDate();
};

#endif // UINOTICEWND_H
