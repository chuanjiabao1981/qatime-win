#ifndef UIHELPWORD_H
#define UIHELPWORD_H

#include <QWidget>
#include "ui_UIHelpword.h"

#pragma execution_character_set("utf-8")

class UIHelpWord : public QWidget
{
	Q_OBJECT

public:
	UIHelpWord(QWidget *parent = 0);
	~UIHelpWord();

private:
	Ui::UIHelpWord ui;

private slots:
	void closeDialog();
	void Label_01Visible();
	void Label_02Visible();
	void Label_03Visible();
	void Label_04Visible();
	void Label_05Visible();
	void Label_06Visible();
	void Label_07Visible();
	void Label_08Visible();
	void Label_09Visible();
	void Label_10Visible();
	void Label_11Visible();
	void Label_12Visible();
	void Label_13Visible();
	void Label_14Visible();
	void Label_15Visible();
	void Label_16Visible();
	void Label_17Visible();
protected:
	bool nativeEvent(const QByteArray &eventType, void *message, long *result);
	void paintEvent(QPaintEvent *event);

private:
	void InitUI();
	void visibleAll();
};

#endif // UIHELPWORD_H
