#ifndef UIOtherApp_H
#define UIOtherApp_H

#include <QDialog>
#include <QWidget>
#include "ui_UIOtherApp.h"
#include "nlss_type.h"

#pragma execution_character_set("utf-8")

class UIOtherApp : public QDialog
{
	Q_OBJECT

public:
	UIOtherApp(QWidget *parent = 0);
	~UIOtherApp();

private:
	Ui::UIOtherApp ui;
	bool bFirstInit;
	int m_CurrentIndex;

protected:
	virtual void resizeEvent(QResizeEvent *);
	virtual void closeEvent(QCloseEvent *);

private slots :
	void CurrentIndex();
	void OK();
	void CANCEL();

public:
	void setAppInfo(ST_NLSS_INDEVICE_INF* m_pAppWinds, int iAppCount);
	void initAppPath();
	int  getCurrentIndex();
	bool IsInit();
};

#endif // UIOtherApp_H
