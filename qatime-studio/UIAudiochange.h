#ifndef UIAUDIOCHANGE_H
#define UIAUDIOCHANGE_H

#include <QWidget>
#include "ui_UIAudiochange.h"
#include "nlss_type.h"
#include <QButtonGroup>
#include <QVBoxLayout>
#include "UIMainwindow.h"
#include <QRadioButton>

class UIMainWindow;

class UIAudioChange : public QWidget
{
	Q_OBJECT

public:
	UIAudioChange(QWidget *parent = 0);
	~UIAudioChange();

private:
	Ui::UIAudioChange ui;
	UIMainWindow*		m_Parent;

public:
	QButtonGroup*	m_pAudioGroup;
	QVBoxLayout *   m_pVBox;

private slots:
	void onRadioClick(int id, bool bCheck);

public:
	void SetAudioInfo(int iAudioNum, ST_NLSS_INDEVICE_INF* AudioDevices);
	void setAudioChange(UIMainWindow* Parent);
};

#endif // UIAUDIOCHANGE_H