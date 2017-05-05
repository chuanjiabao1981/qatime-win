#ifndef UIAPPWND_H
#define UIAPPWND_H

#include <QWidget>
#include "ui_UIAppWnd.h"
#include "UIBtnApp.h"

class UIAppWnd : public QWidget
{
	Q_OBJECT

public:
	UIAppWnd(QWidget *parent = 0);
	~UIAppWnd();

private:
	Ui::UIAppWnd ui;

	QSpacerItem*				m_spacer;
	std::vector<CBtnApp*>		m_vecBtnApp;
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
	void style(QScrollArea *style);

signals:
	void sig_selectWnd(HWND);
private slots:
	void slot_click(HWND);		
	void clickCancel();
	void clickOK();
	void clickClose();
public:
	void AddAppWnd(QString title, HWND id);
	void deleteWnd();
};

#endif // UIAPPWND_H
