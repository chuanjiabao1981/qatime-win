#ifndef UIVIEWIMAGE_H
#define UIVIEWIMAGE_H

#include <QWidget>
#include "ui_UIViewimage.h"

class UIViewImage : public QWidget
{
	Q_OBJECT

public:
	UIViewImage(QWidget *parent = 0);
	~UIViewImage();
private:
	Ui::UIViewImage ui;

public:
	void setImage(QString imgPath);
};

#endif // UIVIEWIMAGE_H
