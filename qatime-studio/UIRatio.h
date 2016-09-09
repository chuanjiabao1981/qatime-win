#ifndef UIRATIO_H
#define UIRATIO_H

#include <QWidget>
#include "ui_UIRatio.h"
#include <QButtonGroup>
#include <QVBoxLayout>
#include "UIMainwindow.h"
#include <QRadioButton>

class UIMainWindow;

class UIRatio : public QWidget
{
	Q_OBJECT

public:
	UIRatio(QWidget *parent = 0);
	~UIRatio();

private:
	Ui::UIRatio ui;
	UIMainWindow*	m_Parent;

	QButtonGroup*		m_pGroup;
	QVBoxLayout *		m_pVBox;

private slots:
	void onRadioClick(int id, bool bCheck);

public:
	void SetRatioInfo();
	void setVideoChange(UIMainWindow* Parent);
};

#endif // UIRATIO_H
