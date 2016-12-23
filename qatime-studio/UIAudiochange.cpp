#include "UIAudiochange.h"

UIAudioChange::UIAudioChange(QWidget *parent)
	: QWidget(parent)
	, m_Parent(NULL)
	, m_pAudioGroup(NULL)
	, iCount(0)
{
	ui.setupUi(this);
	m_pVBox = new QVBoxLayout;
	m_pAudioGroup = new QButtonGroup;
	connect(m_pAudioGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onRadioClick(QAbstractButton*)));
}

UIAudioChange::~UIAudioChange()
{
	if (m_Parent)
	{
		m_Parent = NULL;
	}
}

void UIAudioChange::setAudioChange(UIMainWindow* Parent)
{
	m_Parent = Parent;
}


void UIAudioChange::onRadioClick(QAbstractButton *btn)
{
	QRadioButton* radio = (QRadioButton*)btn;
	QString strpath = radio->accessibleDescription();
//	m_Parent->setAudioChangeIndex(strpath);
	hide();
}

void UIAudioChange::SetAudioInfo(int iAudioNum, QString strName, QString path)
{
	if (iAudioNum == 0)
		return;

	int height = iAudioNum * 30;
	ui.audio_groupBox->setMinimumHeight(height);

	QRadioButton* radio = new QRadioButton();
	radio->setText(strName);
	radio->setAccessibleDescription(path);
	m_pVBox->addWidget(radio);
	m_pAudioGroup->addButton(radio, iCount);

	if (iCount == 0)
		radio->setChecked(true);

	ui.audio_groupBox->setLayout(m_pVBox);

	iCount++;
}