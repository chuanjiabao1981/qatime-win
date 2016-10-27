#include "UISidescreen.h"

UISideScreen::UISideScreen(QWidget *parent)
	: QWidget(parent)
	, m_parent(NULL)
{
	ui.setupUi(this);

	connect(ui.swichScreen_pushBtn, SIGNAL(clicked()), this, SLOT(slot_swichScreen()));
	connect(ui.pack_pushBtn, SIGNAL(clicked()), this, SLOT(slot_packScreen()));
}

UISideScreen::~UISideScreen()
{

}

void UISideScreen::SetMainWindow(UIMainWindow* parent)
{
	m_parent = parent;
}

void UISideScreen::slot_packScreen()
{
	if (ui.swichScreen_pushBtn->isHidden())
		ui.swichScreen_pushBtn->setHidden(false);
	else
		ui.swichScreen_pushBtn->setHidden(true);

	emit emit_HideSideScreen();
}

void UISideScreen::slot_swichScreen()
{
	emit emit_SwichScreen();
}

void UISideScreen::ChangeTip(QString Change)
{
	ui.tip_label->setText(Change);
}