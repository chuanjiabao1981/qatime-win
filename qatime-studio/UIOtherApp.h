#ifndef UIOtherApp_H
#define UIOtherApp_H

#include <QDialog>
#include <QWidget>
#include "ui_UIOtherApp.h"
#include "nlss_type.h"

#pragma execution_character_set("utf-8")

struct CaptureWindowInfo {
	HWND id;
	std::wstring title;
};
typedef std::vector<CaptureWindowInfo> CaptureWindowInfoList;

class UIOtherApp : public QDialog
{
	Q_OBJECT

public:
	UIOtherApp(QWidget *parent = 0);
	~UIOtherApp();

private:
	Ui::UIOtherApp ui;
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
	bool GetCaptureWindowList(CaptureWindowInfoList* windows);
};

#endif // UIOtherApp_H
