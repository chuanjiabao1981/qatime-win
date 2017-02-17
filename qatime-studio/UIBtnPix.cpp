/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#include "UIBtnPix.h"
#include <QtGui/QPainter>
#include <QtGui/QEvent.h>

CBtnPix::CBtnPix(QWidget *parent /*= 0*/)
: QPushButton(parent)
, m_qsImage("")
{
}

CBtnPix::CBtnPix(QString url, QPixmap pixmap, QWidget *parent/* = 0*/)
: QPushButton(parent)
, m_qsImage("")
, m_nImageWidth(0)
, m_nImageHeight(0)
, m_pImgProcess(NULL)
, m_MsgID("")
, m_pFailBtn(NULL)
{
	pixmap = CenterRect(pixmap);
	m_qsImage = url;
	m_pixmap = pixmap;
	this->setIcon(QIcon(pixmap.scaled(QSize(120, 120))));
	this->setIconSize(QSize(120, 120));
	this->setFlat(true);

	connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked(bool)));

	m_pFailBtn = new QPushButton(this);
	m_pFailBtn->resize(120, 120);
	m_pFailBtn->setStyleSheet("QPushButton{background-color:rgba(255, 255, 255, 0);border-image:url(./images/alpha.png);}");
	connect(m_pFailBtn, SIGNAL(clicked(bool)), this, SLOT(onClickedFail(bool)));
}

CBtnPix::CBtnPix(QPixmap pixmap, QWidget *parent/* = 0*/)
	: QPushButton(parent)
{
	this->setIcon(QIcon(pixmap.scaled(QSize(24, 24))));
	this->setIconSize(QSize(24, 24));
	this->setFlat(true);
}

CBtnPix::~CBtnPix()
{
}

void CBtnPix::onClicked(bool bChecked)
{
	emit sig_idclicked(m_qsImage, m_pixmap, bChecked);
}

void CBtnPix::SetImgProcess(QPushButton* btn)
{
	m_pImgProcess = btn;
}

QPushButton* CBtnPix::GetImgProcess()
{
	if (m_pImgProcess)
		return m_pImgProcess;

	return NULL;
}

void CBtnPix::SetMsgID(QString msgid)
{
	m_MsgID = msgid;
}

QString CBtnPix::GetMsgID()
{
	return m_MsgID;
}

QString CBtnPix::GetUrl()
{
	return m_qsImage;
}

void CBtnPix::AddFailBtn()
{
	m_pFailBtn->setStyleSheet("QPushButton{background-color:rgba(255, 255, 255, 0);border-image:url(./images/retry.png);}");
}

void CBtnPix::DeleteFailBtn()
{
	m_pFailBtn->setStyleSheet("QPushButton{background-color:rgba(255, 255, 255, 0);border-image:url(./images/alpha.png);}}");
}

void CBtnPix::onClickedFail(bool bChecked)
{
	QString strSheet = m_pFailBtn->styleSheet();
	if (strSheet.indexOf("./images/alpha.png") > -1)
		emit sig_idclicked(m_qsImage, m_pixmap, bChecked); //正常点击
	else
		emit sig_faildclicked(this);					   //失败点击
}

// 中心裁切
QPixmap CBtnPix::CenterRect(QPixmap pix)
{
	QPixmap AfterPix;
	int h = pix.height();
	int w = pix.width();
	if (w>h)
	{
		int x = (w - h) / 2;
		AfterPix = pix.copy(x, 0, h, h);
	}
	else
	{
		int y = (h - w) / 2;
		AfterPix = pix.copy(0, y, w, w);
	}

	return AfterPix;
}