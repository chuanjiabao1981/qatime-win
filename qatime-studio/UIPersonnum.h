#ifndef UIPERSONNUM_H
#define UIPERSONNUM_H

#include <QWidget>
#include "ui_UIPersonnum.h"
#include "UIPersonwnd.h"

class UIPersonWnd;
class UIPersonNum : public QWidget
{
	Q_OBJECT

public:
	UIPersonNum(QWidget *parent = 0);
	~UIPersonNum();

	QString			m_id;
private:
	Ui::UIPersonNum ui;

private slots:
	void stateChanged(int);

public:
	void AddPersonInfo(QPixmap pix, QString name, QString id, Qt::CheckState state);
};

#endif // UIPERSONNUM_H
