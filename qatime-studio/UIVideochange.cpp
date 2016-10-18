#include "UIVideochange.h"

UIVideoChange::UIVideoChange(QWidget *parent)
	: QWidget(parent)
	, m_Parent(NULL)
	, iCount(0)
{
	ui.setupUi(this);
	m_pVBox = new QVBoxLayout;
	m_pVideoGroup = new QButtonGroup;
	connect(m_pVideoGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onRadioClick(QAbstractButton*)));
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

void UIVideoChange::onRadioClick(QAbstractButton *btn)
{
	QRadioButton* radio = (QRadioButton*)btn;
	QString strpath = radio->accessibleDescription();
	m_Parent->setVideoChangeIndex(strpath);
	hide();
}

void UIVideoChange::SetVideoInfo(int iAudioNum, QString strName,QString path)
{
	if (iAudioNum == 0)
		return;

	int height = iAudioNum * 30;
	ui.video_groupBox->setMinimumHeight(height);
	
	QRadioButton* radio = new QRadioButton();
	radio->setText(strName);
	radio->setAccessibleDescription(path);
	m_pVBox->addWidget(radio);
	m_pVideoGroup->addButton(radio, iCount);

	if (iCount == 0)
		radio->setChecked(true);
	
	ui.video_groupBox->setLayout(m_pVBox);

	iCount++;
}