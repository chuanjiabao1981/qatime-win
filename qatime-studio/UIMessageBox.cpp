/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#include <qt_windows.h>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QBitmap>

#include "UIButton.h"
#include "UIMessageBox.h"


#define MESSAGEBOX_FIXED_HEIGHT 150
#define MESSAGEBOX_FIXED_WIDTH  300
#define MESSAGEBOX_FIXED_TITLE_HEIGHT 128

class CSpliter : public QWidget
{
public:
	CSpliter(QWidget *pWidget)
		: QWidget(pWidget)
	{

	}

protected:
	virtual void paintEvent(QPaintEvent *)
	{
		QPainter p(this);

		QRect rt = rect();
		p.setPen(Qt::NoPen);
		QBrush b(QColor(0xababab));
		p.setBrush(b);
		p.drawRect(rt.left(), rt.top(), rt.width(), rt.height());
	};
};


CMessageBox::~CMessageBox()
{
	//É¾³ý¶¨Ê±Æ÷
	if (m_hideTimers)
	{
		if (m_hideTimers->isActive())
			m_hideTimers->stop();
		delete m_hideTimers;
		m_hideTimers = NULL;
	}
}

int CMessageBox::showMessage(
	 const QString & title, const QString & text,
	const QString& btn0,
	const QString& btn1,
	QWidget *parent,
	bool bTip)
{
	CMessageBox m(title, text, btn0, btn1, parent,bTip);
	SetWindowPos((HWND)m.winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	return m.exec();
}

CMessageBox::CMessageBox(const QString& title,const QString & text,
	const QString& btn0,
	const QString& btn1,
	QWidget *parent,
	bool bTip)
	: QDialog(parent, Qt::Dialog | Qt::Popup | Qt::FramelessWindowHint)
	, m_pImage(NULL)
	, m_pInnerText(NULL)
	, m_pBtnClose(NULL)
	, m_qsImage(title)
	, m_qsText(text)
	, m_hideTimers(NULL)
{
	setFixedSize(MESSAGEBOX_FIXED_WIDTH, MESSAGEBOX_FIXED_HEIGHT);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_DeleteOnClose, false);
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
	setModal(true);
	
	if (!btn0.isEmpty())
	{
		CButton* pBtn = new CButton(1,this);		
		if (pBtn)
		{
			pBtn->setImage(btn0,1);
			m_lBtns.push_back(pBtn);
			connect(pBtn, SIGNAL(sig_idclicked(int, bool)), this, SLOT(slot_btnclicked(int, bool)));
		}
	}

	if (!btn1.isEmpty())
	{
		CButton* pBtn = new CButton(2,this);
		if (pBtn)
		{
			pBtn->setImage(btn1,1);
			m_lBtns.push_back(pBtn);
			connect(pBtn, SIGNAL(sig_idclicked(int, bool)), this, SLOT(slot_btnclicked(int, bool)));
		}
	}

	/*if (!btn2.isEmpty())
	{
		CButton* pBtn = new CButton(3, this);
		if (pBtn)
		{
			pBtn->setImage(btn2);
			m_lBtns.push_back(pBtn);
			connect(pBtn, SIGNAL(sig_idclicked(int, bool)), this, SLOT(slot_btnclicked(int, bool)));
		}
	}*/

	initUI();

	connect(m_pBtnClose, SIGNAL(clicked(bool)), this, SLOT(slot_titleClose(bool)));

	if (bTip)
	{
		m_hideTimers = new QTimer;
		connect(m_hideTimers, SIGNAL(timeout()), this, SLOT(slot_onCountTimeout()));
		m_hideTimers->start(1500);
	}
}

void CMessageBox::initUI()
{
	
	QFont font,font1;
	font.setPixelSize(14);
	font.setFamily("Î¢ÈíÑÅºÚ");

	font1.setPixelSize(12);
	font1.setFamily("Î¢ÈíÑÅºÚ");


	QPalette pe,pe1;
	pe.setColor(QPalette::WindowText, QColor(0x3f3f3f));
	pe1.setColor(QPalette::WindowText, QColor(0x3f3f3f));

	QVBoxLayout* pLayout = new QVBoxLayout;
	if (pLayout)
	{
		pLayout->setMargin(0);
		pLayout->setSpacing(0);
		pLayout->addSpacing(4);
		QHBoxLayout* pTopLayout = new QHBoxLayout;
		if (pTopLayout)
		{
			m_pLabelIcon = new QLabel(this);
			if (m_pLabelIcon)
			{
//				m_pLabelIcon->setPixmap(QPixmap("./images/logo.png"));
				pTopLayout->addWidget(m_pLabelIcon, 0, Qt::AlignLeft);

			}
			pTopLayout->addSpacing(2);
			m_pImage = new QLabel(this);
			if (m_pImage)
			{
				m_pImage->setFont(font);
				m_pImage->setPalette(pe);
				m_pImage->setText(m_qsImage);
				pTopLayout->addWidget(m_pImage, 0, Qt::AlignLeft);
			}
			pTopLayout->addStretch(3);
			m_pBtnClose = new CButton(this);
			if (m_pBtnClose)
			{
				m_pBtnClose->setImage(QString("./images/close.png"), 1);
				pTopLayout->addWidget(m_pBtnClose, 0, Qt::AlignRight);
			}
			pTopLayout->addSpacing(10);
			pLayout->addLayout(pTopLayout);
		}

		pLayout->addSpacing(10);
		CSpliter* pSpliter = new CSpliter(this);
		if (pSpliter)
		{
			pSpliter->setFixedHeight(1);
			pLayout->addWidget(pSpliter);
		}

		QHBoxLayout * pMiddleLayout = new QHBoxLayout;
		if (pMiddleLayout)
		{
			pMiddleLayout->setMargin(0);
			pMiddleLayout->setSpacing(0);

			pMiddleLayout->addSpacing(29);
			m_pInnerText = new QLabel();
			if (m_pInnerText)
			{
				

				if (m_qsText.isEmpty())

				{
					m_pInnerText->setPixmap(QString("./images/tuxing.png"));
				
				}
				else
				{
					
					m_pInnerText->setFont(font);

					QPalette pe1;
					pe.setColor(QPalette::WindowText, QColor(0x3f3f3f));
					m_pInnerText->setPalette(pe);

					m_pInnerText->setWordWrap(true);
					m_pInnerText->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter | Qt::AlignJustify | Qt::AlignAbsolute);
					m_pInnerText->setText(m_qsText);
				}

				pMiddleLayout->addWidget(m_pInnerText);
			}


			pMiddleLayout->addSpacing(52);

			pLayout->addLayout(pMiddleLayout);
		}


		pLayout->addSpacing(10);

		QHBoxLayout * pBottomLayout = new QHBoxLayout;
		if (pBottomLayout)
		{
			pBottomLayout->setMargin(0);
			pBottomLayout->setSpacing(0);
			pBottomLayout->addStretch(3);
			for (QList<CButton*>::iterator iter = m_lBtns.begin(); iter != m_lBtns.end(); ++iter)
			{
				if (*iter)
				{
					pBottomLayout->addWidget((*iter), 0, Qt::AlignCenter);
					pBottomLayout->addSpacing(4);
				}
			}

			pLayout->addLayout(pBottomLayout);
		}
//		pLayout->addSpacing(20);

	}
	setLayout(pLayout);
}

void CMessageBox::slot_titleClose(bool /*bChecked*/)
{
	done(0);
}

void CMessageBox::slot_btnclicked(int nId, bool /*bChecked*/)
{
	done(nId);
}

void CMessageBox::slot_onCountTimeout()
{
	done(0);
}

bool CMessageBox::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	if ("windows_generic_MSG" == eventType)
	{
		PMSG pMsg = static_cast<PMSG>(message);
		switch (pMsg->message)
		{
		case WM_NCHITTEST:
		{
			int y = GET_Y_LPARAM(pMsg->lParam) - this->frameGeometry().y();

			if (y < MESSAGEBOX_FIXED_TITLE_HEIGHT)
			{
				QPoint pt = mapFromGlobal(QPoint(GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam)));
				if (!childAt(pt))
				{
					*result = HTCAPTION;
					return true;
				}
			}

			return false;
		}
			break;
		default:
			return false;
			break;

		}
	}
	return false;
}

void CMessageBox::paintEvent(QPaintEvent *)
{
	
	QPainter p(this);
	QRect rt = rect();
	QPen pen(QColor(0xababab), 0);
	p.setPen(pen);
	QBrush b(QColor(0xffffff));
	p.setBrush(b);
	p.drawRect(rt.left(), rt.top(), rt.width() - 1, rt.height() - 1);
}

void CMessageBox::closeEvent(QCloseEvent *)
{
	reject();
}