#ifndef UIWHITEBOARDTOOL_H
#define UIWHITEBOARDTOOL_H

#include <QWidget>
#include "ui_UIWhiteBoardTool.h"
#include "colorpicker.h"

class UIWhiteBoardTool : public QWidget
{
	Q_OBJECT

public:
	UIWhiteBoardTool(QWidget *parent = 0);
	~UIWhiteBoardTool();

private:
	Ui::UIWhiteBoardTool ui;

	ColorPicker*	mColorPicker;
signals:
	void selectColor(QColor&);
	void returnClick();
	void deleteClick();
	void laserClick();
	void drawClick();
private:
	void initColorPicker();
private slots:
	void setPenColor(int);
	void colorClicked();
	void returnClicked();
	void deleteClicked();
	void laserClicked();
	void drawClicked();
protected:
	void paintEvent(QPaintEvent *event);
};

#endif // UIWHITEBOARDTOOL_H
