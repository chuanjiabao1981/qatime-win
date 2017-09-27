#ifndef UIHOMEWORK_H
#define UIHOMEWORK_H

#include <QWidget>
#include "ui_UIHomework.h"
#include "UIMessageBox.h"
#include "QDesktopServices"
#include "QUrl"


#pragma execution_character_set("utf-8")

class UIHomework : public QWidget
{
	Q_OBJECT

public:
	UIHomework(QWidget *parent = 0);
	~UIHomework();

	QString m_URL;
	void ShowPage(QString mContent, QString mSignText, QString mURL);

private:
	Ui::UIHomework ui;

protected:
	void mousePressEvent(QMouseEvent *event);
	
};

#endif // UIHOMEWORK_H
