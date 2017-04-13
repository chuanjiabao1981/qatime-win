#include "UIPerson.h"

UIPerson::UIPerson(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

UIPerson::~UIPerson()
{

}

void UIPerson::AddPersonInfo(personListBuddy* buddy, QString chatid)//QPixmap pix, QString name, QString id, QString chatID, Qt::CheckState state
{
	QFont font;
	font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
	font.setPixelSize(13);

	if (buddy)
	{
		m_personBuddy = buddy;

		QPixmap* pix = NULL;
		pix = (QPixmap*)buddy->head->pixmap();
		QPixmap pixscaled = pix->scaled(30, 30);
		QString name = buddy->name->text();
		m_id = buddy->m_ID;
		m_chatID = chatid;
		Qt::CheckState state = buddy->button->checkState();

		ui.pic_label->setPixmap(pixscaled);
		ui.name_label->setFont(font);
		ui.name_label->setText(name);

		// 加省略号
		int fontSize = ui.name_label->fontMetrics().width(name);//获取之前设置的字符串的像素大小
		if (fontSize >= ui.name_label->width()) //与label自身相比较
		{
			QString  tmpStr = ui.name_label->fontMetrics().elidedText(name, Qt::ElideRight, ui.name_label->width());
			ui.name_label->setText(tmpStr);
			ui.name_label->setToolTip(name);
		}

		ui.checkBox->setCheckState(state);
		ui.checkBox->setFont(font);
		connect(ui.checkBox, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
	}
}

void UIPerson::stateChanged(int i)
{
	if (i == 0)			// 取消禁言
	{
		m_personBuddy->button->setCheckState(Qt::Unchecked);
		emit sig_change(false, m_id, m_chatID);
	}
	else if (i == 2)	// 禁言
	{
		m_personBuddy->button->setCheckable(Qt::Checked);
		emit sig_change(true, m_id, m_chatID);
	}
}