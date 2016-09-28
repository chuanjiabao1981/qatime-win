#include "UIRatio.h"

UIRatio::UIRatio(QWidget *parent)
	: QWidget(parent)
	, m_pGroup(NULL)
	, m_pVBox(NULL)
	, m_Parent(NULL)
{
	ui.setupUi(this);
	SetRatioInfo();
}

UIRatio::~UIRatio()
{
	if (m_Parent)
	{
		m_Parent = NULL;
	}
}

void UIRatio::setVideoChange(UIMainWindow* Parent)
{
	m_Parent = Parent;
}

void UIRatio::SetRatioInfo()
{
	m_pVBox = new QVBoxLayout;
	m_pGroup = new QButtonGroup;

	QRadioButton* radio = new QRadioButton();
	radio->setText("标清  适合网络一般的用户使用");
	m_pVBox->addWidget(radio);
	m_pGroup->addButton(radio, 0);
	radio->setChecked(true);
		
	QRadioButton* radio1 = new QRadioButton();
	radio1->setText("高清  适合宽带较高的用户使用");
	m_pVBox->addWidget(radio1);
	m_pGroup->addButton(radio1, 1);
	
	connect(m_pGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(onRadioClick(int, bool)));
	ui.Ratio_groupBox->setLayout(m_pVBox);
}

void UIRatio::onRadioClick(int id, bool bCheck)
{
	m_Parent->setRatioChangeIndex(m_pGroup->checkedId());
	hide();
}