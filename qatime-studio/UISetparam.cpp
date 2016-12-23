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

	connect(ui.close_toolButton, SIGNAL(clicked()), this, SLOT(CloseDialog()));

	ui.HD_radio->setChecked(true);
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
	font.setFamily(("Î¢ÈíÑÅºÚ"));

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

void UISetParam::setParent(UIMainWindow* parent)
{
	m_Parent = parent;
}