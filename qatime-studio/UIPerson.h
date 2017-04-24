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
public:
	personListBuddy*	m_personBuddy;

signals:
	void sig_change(bool,QString,QString);
	void sig_allChange(bool , QString);

private slots:
	void stateChanged(int);
	void stateAllChanged(int);

public:
	void AddPersonInfo(personListBuddy* buddy, QString chatid);
	void setMuteState(bool bAllMute);
};

#endif // UIPERSON_H
