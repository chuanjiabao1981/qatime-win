#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include "ui_colorpicker.h"

class ColorPicker : public QWidget
{
	Q_OBJECT

public:
	ColorPicker(QWidget *parent = 0);
	~ColorPicker();

	enum EnumColor
	{
		color_black,
		color_red,
		color_yellow,
		color_green,
		color_blue,
		color_purple
	};

	void setCurColor(const QColor &curColor);

signals:
	void selectColor(int);

private slots:
	void on_pushButton_black_clicked();
	void on_pushButton_yellow_clicked();
	void on_pushButton_purple_clicked();
	void on_pushButton_green_clicked();
	void on_pushButton_blue_clicked();
	void on_pushButton_red_clicked();

protected:
	void focusOutEvent(QFocusEvent* e);

private:
	Ui::ColorPicker ui;

	QColor mCurColor;
	EnumColor mColor;
};

#endif // COLORPICKER_H
