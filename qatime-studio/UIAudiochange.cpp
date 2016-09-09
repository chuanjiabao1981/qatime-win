#include "UIAudiochange.h"

UIAudioChange::UIAudioChange(QWidget *parent)
	: QWidget(parent)
	, m_Parent(NULL)
	, m_pAudioGroup(NULL)
{
	ui.setupUi(this);
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

void UIAudioChange::SetAudioInfo(int iAudioNum, ST_NLSS_INDEVICE_INF* AudioDevices)
{
	if (iAudioNum == 0)
		return;
	
	int height = iAudioNum * 30;
	ui.audio_groupBox->setMinimumHeight(height);

	m_pVBox = new QVBoxLayout;
	m_pAudioGroup = new QButtonGroup;
	for (int i = 0; i < iAudioNum; i++)
	{
		QRadioButton* radio = new QRadioButton();
		radio->setText(QString::fromLocal8Bit(AudioDevices[i].paFriendlyName));
		radio->setAccessibleDescription(QString::number(i));
		m_pVBox->addWidget(radio);
		m_pAudioGroup->addButton(radio, i);

		if (i == 0 )
			radio->setChecked(true);
	}
	connect(m_pAudioGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(onRadioClick(int, bool)));
	ui.audio_groupBox->setLayout(m_pVBox);
}

void UIAudioChange::onRadioClick(int id, bool bCheck)
{	
	m_Parent->setAudioChangeIndex(m_pAudioGroup->checkedId());
	hide();
}