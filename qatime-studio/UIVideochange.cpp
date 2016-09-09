#include "UIVideochange.h"

UIVideoChange::UIVideoChange(QWidget *parent)
	: QWidget(parent)
	, m_Parent(NULL)
{
	ui.setupUi(this);
}

UIVideoChange::~UIVideoChange()
{
	if (m_Parent)
	{
		m_Parent = NULL;
	}
}

void UIVideoChange::setVideoChange(UIMainWindow* Parent)
{
	m_Parent = Parent;
}

void UIVideoChange::SetVideoInfo(int iAudioNum, ST_NLSS_INDEVICE_INF* AudioDevices)
{
	if (iAudioNum == 0)
		return;

	int height = iAudioNum * 30;
	ui.video_groupBox->setMinimumHeight(height);

	m_pVBox = new QVBoxLayout;
	m_pVideoGroup = new QButtonGroup;
	for (int i = 0; i < iAudioNum; i++)
	{
		QRadioButton* radio = new QRadioButton();
		radio->setText(QString::fromLocal8Bit(AudioDevices[i].paFriendlyName));
		radio->setAccessibleDescription(QString::number(i));
		m_pVBox->addWidget(radio);
		m_pVideoGroup->addButton(radio, i);

		if (i == 0)
			radio->setChecked(true);
	}
	connect(m_pVideoGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(onRadioClick(int, bool)));
	ui.video_groupBox->setLayout(m_pVBox);
}

void UIVideoChange::onRadioClick(int id, bool bCheck)
{
	m_Parent->setVideoChangeIndex(m_pVideoGroup->checkedId());
	hide();
}