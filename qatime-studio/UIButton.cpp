/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#include "UIButton.h"
#include <QtGui/QPainter>
#include <QtGui/QEvent.h>

CButton::CButton(QWidget *parent /*= 0*/)
: QPushButton(parent)
, m_pImage(NULL)
, m_qsImage("")
, m_eStatus(BS_NORMAL)
, m_bMouseDown(false)
, m_nImageWidth(0)
, m_nImageHeight(0)
, m_nId(0)
, m_nState(0)
{
	m_pImage = new QPixmap();
}

CButton::CButton(int nId, QWidget *parent/* = 0*/)
: QPushButton(parent)
, m_pImage(NULL)
, m_qsImage("")
, m_eStatus(BS_NORMAL)
, m_bMouseDown(false)
, m_nImageWidth(0)
, m_nImageHeight(0)
, m_nId(nId)
, m_nState(0)
{
	m_pImage = new QPixmap();
	connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked(bool)));
}

CButton::~CButton()
{
	if (m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}

bool CButton::HintBtn(QPoint& pos)
{
	return hitButton(pos);
}

void CButton::setImage(QString qsImage, int nState /*= 4*/)
{
	if (m_qsImage != qsImage)
	{
		m_qsImage = qsImage;
		if (NULL == m_pImage)
		{
			m_pImage = new QPixmap();
		}
		m_pImage->load(m_qsImage);
		QSize s = m_pImage->size();
		m_nImageWidth = s.width() / nState;
		m_nImageHeight = s.height();
		m_nState = nState;

		if (s.height() ==0 && s.width()==0)
		{
			setText(qsImage);
			if (qsImage == "È·¶¨")
				setStyleSheet("QPushButton{background:url(./images/chatBtnColor.png); color:rgb(255,255,255);}");
			else
				setStyleSheet("QPushButton{background:url(./images/AuxiliaryPanelBack.png);}");
			return;
		}

		setFixedSize(m_nImageWidth, s.height());
	}
}

void CButton::onClicked(bool bChecked)
{
	emit sig_idclicked(m_nId, bChecked);
}

void CButton::mousePressEvent(QMouseEvent *e)
{
	if (Qt::LeftButton == e->button())
	{
		m_eStatus = BS_DOWN;
		m_bMouseDown = true;
		update();
	}
	QPushButton::mousePressEvent(e);
}

void CButton::mouseReleaseEvent(QMouseEvent *e)
{
	if (Qt::LeftButton != e->button())
	{
		QPushButton::mouseReleaseEvent(e);
		return;
	}

	m_eStatus = BS_HOVER;
	m_bMouseDown = false;

	QPushButton::mouseReleaseEvent(e);
}

void CButton::mouseMoveEvent(QMouseEvent *e)
{
	m_eStatus = m_bMouseDown ? BS_DOWN : BS_HOVER;
	update();
	QPushButton::mouseMoveEvent(e);
}

void CButton::enterEvent(QEvent *e)
{
	m_eStatus = BS_HOVER;
	QPushButton::enterEvent(e);
	update();
}

void CButton::leaveEvent(QEvent *e)
{
	m_eStatus = BS_NORMAL;
	m_bMouseDown = false;
	QPushButton::leaveEvent(e);
	update();
}

void CButton::paintEvent(QPaintEvent *e)
{
	if (m_pImage && m_nImageWidth && m_nImageHeight)
	{
		QPainter p(this);
		int i = static_cast<int>(getImageIndex());
		p.drawPixmap(0, 0, *m_pImage, i*m_nImageWidth, 0, m_nImageWidth, m_nImageHeight);
	}
	else
	{
		QPushButton::paintEvent(e);
	}
}

void CButton::changeEvent(QEvent *e)
{
	QPushButton::changeEvent(e);
	if (QEvent::EnabledChange == e->type())
	{
		m_eStatus = isEnabled() ? BS_NORMAL : BS_DISABLED;
		update();
	}
}

CButton::EButtonStatus CButton::getButtonStatus() const
{
	return m_eStatus;
}
CButton::EButtonImageIndex CButton::getImageIndex() const
{
	
	EButtonImageIndex index = BI_NORMAL;
	switch (m_eStatus)
	{
	case CButton::BS_NORMAL:
		index = BI_NORMAL;
		break;
	case CButton::BS_HOVER:
		index = (m_nState != 1) ? BI_HOVER : BI_NORMAL;
		break;
	case CButton::BS_DOWN:
		index = (m_nState == 4 || m_nState == 3) ? BI_DOWN : BI_NORMAL;
		break;
	case CButton::BS_DISABLED:
		index = (m_nState == 4) ? BI_DISABLED : BI_NORMAL;
		break;
	default:
		break;
	}
	return index;
}