#ifndef UIVIDEOCHANGE_H
#define UIVIDEOCHANGE_H

#include <QWidget>
#include "ui_UIVideochange.h"
#include <QButtonGroup>
#include <QVBoxLayout>
#include "UIMainwindow.h"
#include <QRadioButton>

class UIMainWindow;

class UIVideoChange : public QWidget
{
	Q_OBJECT

public:
	UIVideoChange(QWidget *parent = 0);
	~UIVideoChange();

	int iCount;
private:
	Ui::UIVideoChange   ui;
	UIMainWindow*		m_Parent;

	QButtonGroup*		m_pVideoGroup;
	QVBoxLayout *		m_pVBox;

private slots:
	void onRadioClick(QAbstractButton *btn);

public:
	void setVideoChange(UIMainWindow* Parent);
	void SetVideoInfo(int iAudioNum, QString strName, QString path);
};

#endif // UIVIDEOCHANGE_H
