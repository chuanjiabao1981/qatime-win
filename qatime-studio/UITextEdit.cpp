#include "UITextEdit.h"
#include <QScrollBar>
#include <QTimer>
#include <QPainter>

UITextEdit::UITextEdit(QWidget *parent /*= 0*/)
	: QTextEdit(parent)
{
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
	connect(this->document(), SIGNAL(contentsChanged()), this, SLOT(textAreaChanged()));
}

UITextEdit::~UITextEdit()
{
}

void UITextEdit::textAreaChanged()
{
	QTextDocument *document = qobject_cast<QTextDocument*>(sender());
	document->adjustSize();
	if (document&&this)
	{
		int newheight = document->size().rheight();
		if (newheight <= this->height())
		{
			this->setFixedHeight(newheight);
		}

//		m_timer->start(50);
	}
}

void UITextEdit::timeout()
{
	if (isVisible())
	{
		m_timer->stop();
		delete m_timer;
		m_timer = NULL;
	}

	if (this->verticalScrollBar()->isVisible())
		this->setFixedHeight(height() + 40);
}

void UITextEdit::paintEvent(QPaintEvent *e)
{
	QTextEdit::paintEvent(e);

	if (this->verticalScrollBar()->isVisible())
		this->setFixedHeight(height() + 40);
}