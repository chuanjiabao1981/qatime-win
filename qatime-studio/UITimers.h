#ifndef UITIMERS_H
#define UITIMERS_H

#include <QWidget>
#include "ui_UITimers.h"
#include "UIMenuTool.h"
#pragma execution_character_set("utf-8")
class UIMenuTool;

class UITimers : public QWidget
{
	Q_OBJECT

public:
	UITimers(QWidget *parent = 0);
	~UITimers();
	void setLivePushBtnText(bool bStart);
	void setTimeLabelVisible(bool b);
	void setTimeLabelText(QString text);
	void setParent(UIMenuTool* p);
public slots:	
	void slot_startOrStopLiveStream();
signals:
	void emit_startOrStopLiveStream();
private:
	Ui::UITimers ui;
	UIMenuTool* m_Parent;
};

#endif // UITIMERS_H
