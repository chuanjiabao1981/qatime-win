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

private:
	Ui::UIVideoChange   ui;
	UIMainWindow*		m_Parent;

	QButtonGroup*		m_pVideoGroup;
	QVBoxLayout *		m_pVBox;

private slots:
	void onRadioClick(int id, bool bCheck);

public:
	void SetVideoInfo(int iAudioNum, ST_NLSS_INDEVICE_INF* AudioDevices);
	void setVideoChange(UIMainWindow* Parent);
};

#endif // UIVIDEOCHANGE_H
