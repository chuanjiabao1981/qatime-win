#ifndef UIPERSON_H
#define UIPERSON_H

#include <QWidget>
#include "ui_UIPerson.h"
#include "UIPersonwnd.h"

class UIPerson : public QWidget
{
	Q_OBJECT

public:
	UIPerson(QWidget *parent = 0);
	~UIPerson();

private:
	Ui::UIPerson ui;
	QString				m_id;
	QString				m_chatID;
	personListBuddy*	m_personBuddy;

signals:
	void sig_change(bool,QString,QString);

private slots:
	void stateChanged(int);

public:
	void AddPersonInfo(personListBuddy* buddy, QString chatid);
};

#endif // UIPERSON_H
