#ifndef RANGECAPTURE_H
#define RANGECAPTURE_H

#include <QWidget>
#include "ui_Rangecapture.h"
#include "UIRangewnd.h"
#include "UIMainwindow.h"

class UIRangeWnd;
class UIMainWindow;

class RangeCapture : public QWidget
{
	Q_OBJECT

public:
	RangeCapture(QWidget *parent = 0);
	~RangeCapture();

private:
	Ui::RangeCapture ui;

	QTimer*							m_Timer;			
	UIRangeWnd*						m_RangeWnd;

	int								m_ScreenWidth;
	int								m_ScreenHeight;

	HWND							m_VideoWnd;
	bool							m_bMaxDialog;
	UIMainWindow*					m_parent;

	int								m_RangeLeft;
	int								m_RangeTop;
	int								m_RangeWidth;
	int								m_RangeHeight;
private slots :
	void RectDialog();								// …Ë÷√Œª÷√
	void FullDialog();
protected:
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result); // ÃÌº”caption
	virtual void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent *);

public:
	void setMainWindow(UIMainWindow* parent);
	void setShowOrHide(bool bShow);
	void setVideoWnd(HWND hWnd);
	void ChangeRange(QString sRc);
	void ToFullDialog(bool bCheck=false);
	void ToRectDialog();
	bool IsMax();
	void SetCurRect();
};

#endif // RANGECAPTURE_H
