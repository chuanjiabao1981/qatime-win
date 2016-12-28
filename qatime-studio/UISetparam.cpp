#include "UISetparam.h"
#include <QFile>
#include <windows.h>
#include <windowsx.h>

#define MAINWINDOW_X_MARGIN 6
#define MAINWINDOW_Y_MARGIN 6
#define MAINWINDOW_TITLE_HEIGHT 49

UISetParam::UISetParam(QWidget *parent)
	: QWidget(parent)
	, m_Parent(NULL)
{
	ui.setupUi(this);
	InitUI();

	ui.ratio_groupBox->setStyleSheet("border: 1px solid white;");
	ui.tirgger_groupBox->setStyleSheet("border: 1px solid white;");

	// ������˷���ʽ
	ui.bullet_checkBox->setStyleSheet("QCheckBox::indicator{width: 70px;height: 13px;}"
		"QCheckBox::indicator:unchecked{image: url(./images/closeBullet.png);}"
		"QCheckBox::indicator:checked{image: url(./images/openBullet.png);}");

	// ���崥��
	ui.all_radioButton->setStyleSheet("QRadioButton::indicator{width: 70px;height: 13px;}"
		"QRadioButton::indicator:unchecked{image: url(./images/allTirgger_nor.png);}"
		"QRadioButton::indicator:checked{image: url(./images/allTirgger_sel.png);}");
	connect(ui.all_radioButton, SIGNAL(clicked(bool)), this, SLOT(AllRidio(bool)));

	// ��ť����
	ui.btn_radioButton->setStyleSheet("QRadioButton::indicator{width: 70px;height: 13px;}"
		"QRadioButton::indicator:unchecked{image: url(./images/btnTirgger_nor.png);}"
		"QRadioButton::indicator:checked{image: url(./images/btnTirgger_sel.png);}");
	connect(ui.btn_radioButton, SIGNAL(clicked(bool)), this, SLOT(BtnRidio(bool)));

	connect(ui.close_toolButton, SIGNAL(clicked()), this, SLOT(CloseDialog()));
	connect(ui.bullet_checkBox, SIGNAL(stateChanged(int)), this, SLOT(BulletStatus(int)));

	ui.HD_radio->setChecked(true);
	ui.all_radioButton->setChecked(true);

	ui.delay_comboBox->addItem("0��", "0");
	ui.delay_comboBox->addItem("1��", "1");
	ui.delay_comboBox->addItem("2��", "2");
	ui.delay_comboBox->addItem("3��", "3");
	ui.delay_comboBox->setCurrentIndex(0);
}

UISetParam::~UISetParam()
{
	m_Parent = NULL;
}

void UISetParam::CloseDialog()
{
	setVisible(false);
}

void UISetParam::InitUI()
{
	QFont font;
	font.setPointSize(10);
	font.setFamily(("΢���ź�"));

	QFile file("styles/ComboBox.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = file.readAll();

	ui.Audio_comboBox->setStyleSheet(styleSheet);
	ui.Audio_comboBox->setMinimumHeight(22);
	ui.Audio_comboBox->setFont(font);
	connect(ui.Audio_comboBox, SIGNAL(activated(int)), this, SLOT(AudioChanged(int)));

	ui.Video_comboBox->setStyleSheet(styleSheet);
	ui.Video_comboBox->setMinimumHeight(22);
	ui.Video_comboBox->setFont(font);
	connect(ui.Video_comboBox, SIGNAL(activated(int)), this, SLOT(VideoChanged(int)));

	ui.delay_comboBox->setStyleSheet(styleSheet);
	ui.delay_comboBox->setMinimumHeight(22);
	ui.delay_comboBox->setFont(font);
	connect(ui.delay_comboBox, SIGNAL(activated(int)), this, SLOT(DelayChanged(int)));
}

void UISetParam::setAudioParam(QString name, QString path)
{
	ui.Audio_comboBox->addItem(name, path);
}

void UISetParam::setVideoParam(QString name, QString path)
{
	ui.Video_comboBox->addItem(name,path);
}

bool UISetParam::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	if ("windows_generic_MSG" == eventType)
	{
		PMSG pMsg = static_cast<PMSG>(message);
		switch (pMsg->message)
		{
		case WM_NCHITTEST:
		{
			int x = GET_X_LPARAM(pMsg->lParam) - this->frameGeometry().x();
			int y = GET_Y_LPARAM(pMsg->lParam) - this->frameGeometry().y();

			int xflag = (x <= MAINWINDOW_X_MARGIN) ? -1 : ((x < this->width() - MAINWINDOW_X_MARGIN) ? 0 : 1);
			int yflag = (y <= MAINWINDOW_Y_MARGIN) ? -1 : ((y < this->height() - MAINWINDOW_Y_MARGIN) ? 0 : 1);

			if (-1 == xflag && -1 == yflag)
			{
				*result = HTTOPLEFT;
			}
			else if (-1 == xflag && 0 == yflag)
			{
				*result = HTLEFT;
			}
			else if (-1 == xflag && 1 == yflag)
			{
				*result = HTBOTTOMLEFT;
			}
			else if (0 == xflag && -1 == yflag)
			{
				*result = HTTOP;
			}
			else if (0 == xflag && 0 == yflag)
			{
				*result = HTCLIENT;
			}
			else if (0 == xflag && 1 == yflag)
			{
				*result = HTBOTTOM;
			}
			else if (1 == xflag && -1 == yflag)
			{
				*result = HTTOPRIGHT;
			}
			else if (1 == xflag && 0 == yflag)
			{
				*result = HTRIGHT;
			}
			else if (1 == xflag && 1 == yflag)
			{
				*result = HTBOTTOMRIGHT;
			}
			if (0 == xflag && y > MAINWINDOW_Y_MARGIN && y <= MAINWINDOW_TITLE_HEIGHT)
			{
				if (ui.close_toolButton)
				{
					QRect Rect = ui.close_toolButton->geometry();
					if (x > Rect.left() && x < Rect.right() && y > Rect.top() && y < Rect.bottom())
					{
						return false;
					}
				}
				*result = HTCAPTION;
			}
			return true;
		}
		break;
		default:
			return false;
			break;
		}
	}
	return false;
}

void UISetParam::AudioChanged(int index)
{
	if (m_Parent)
		m_Parent->setAudioChangeIndex(index);
}

void UISetParam::VideoChanged(int index)
{
	if (m_Parent)
		m_Parent->setVideoChangeIndex(index);
}

void UISetParam::AllRidio(bool b)
{
	if (b && m_Parent)
	{
		m_Parent->setTriggerType(true);
	}
}

void UISetParam::BtnRidio(bool b)
{
	if (b && m_Parent)
	{
		m_Parent->setTriggerType(false);
	}
}

void UISetParam::DelayChanged(int index)
{
	if (m_Parent)
	{
		m_Parent->BulletDelay(index);
	}
}

void UISetParam::setParent(UIMainWindow* parent)
{
	m_Parent = parent;
}

void UISetParam::BulletStatus(int iStatus)
{
	if (m_Parent)
		m_Parent->SetBullet(iStatus);
}

void UISetParam::CloseBulletSet()
{
	ui.bullet_checkBox->setCheckState(Qt::CheckState::Unchecked);
}