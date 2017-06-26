#ifndef DEFDATETIMEEDIT_H
#define DEFDATETIMEEDIT_H

#include <QDateTimeEdit>
#include "DefineCalendar.h"

class DefDateTimeEdit : public QDateTimeEdit
{
	Q_OBJECT

public:
	DefDateTimeEdit(QWidget *parent);
	~DefDateTimeEdit();
	void setMyStytle();

signals:
	void sig_CalendarClick(QDate);
protected slots:
	void getDateTime(const QDateTime &dateTime);
	void CalendarClick(QDate);
private:
	DefineCalendar *m_DefCalendar;
	
};

#endif // DEFDATETIMEEDIT_H
